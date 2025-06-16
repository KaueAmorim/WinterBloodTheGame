#include "player.h"
#include "config.h"

Player* player_create(float x, float y) {
    Player *p = (Player*) calloc(1, sizeof(Player));
    if (!p) return NULL;

    p->x = x;
    p->y = y;
    p->direcao = 1;
    p->estado = PARADO;
    p->frame_largura = 128;
    p->frame_altura = 128;
    p->no_chao = true;
    p->hp = 5;

    // Usa as constantes do config.h para configurar os frames
    p->num_frames_parado = NUM_FRAMES_PARADO;
    p->num_frames_correndo = NUM_FRAMES_CORRENDO;
    p->num_frames_atirando = NUM_FRAMES_ATIRANDO;
    p->num_frames_pulando = NUM_FRAMES_PULANDO;
    p->num_frames_agachado = NUM_FRAMES_AGACHADO;
    p->num_frames_agachado_atirando = NUM_FRAMES_AGACHADO_ATIRANDO;

    // Carrega todas as folhas de sprite necessárias
    p->folha_sprite_parado = al_load_bitmap("assets/Idle.png");
    p->folha_sprite_correndo = al_load_bitmap("assets/Run.png");
    p->folha_sprite_atirando = al_load_bitmap("assets/Shot.png");
    p->folha_sprite_pulando = al_load_bitmap("assets/Jump.png");
    p->folha_sprite_agachado = al_load_bitmap("assets/Crouch.png");
    p->folha_sprite_agachado_atirando = al_load_bitmap("assets/CrouchShot.png");

    // Verifica se TODAS foram carregadas com sucesso
    if (!p->folha_sprite_parado || !p->folha_sprite_correndo || !p->folha_sprite_atirando || !p->folha_sprite_pulando || !p->folha_sprite_agachado || !p->folha_sprite_agachado_atirando) {
        printf("Falha ao carregar um dos sprites do jogador.\n");
        player_destroy(p);
        return NULL;
    }
    return p;
}

void player_destroy(Player *p) {
    if (p) {
        al_destroy_bitmap(p->folha_sprite_parado);
        al_destroy_bitmap(p->folha_sprite_correndo);
        al_destroy_bitmap(p->folha_sprite_atirando);
        al_destroy_bitmap(p->folha_sprite_pulando);
        al_destroy_bitmap(p->folha_sprite_agachado);
        al_destroy_bitmap(p->folha_sprite_agachado_atirando);
        free(p);
    }
}

void player_handle_input(Player *p, ALLEGRO_EVENT *evento) {
    if (evento->type == ALLEGRO_EVENT_KEY_DOWN) {
        switch (evento->keyboard.keycode) {
            case ALLEGRO_KEY_UP:    p->tecla_cima = true; break;
            case ALLEGRO_KEY_LEFT:  p->tecla_esquerda = true; break;
            case ALLEGRO_KEY_RIGHT: p->tecla_direita = true; break;
            case ALLEGRO_KEY_SPACE: p->tecla_tiro = true; break;
            case ALLEGRO_KEY_DOWN:  p->tecla_baixo = true; break;
        }
    } 
    else if (evento->type == ALLEGRO_EVENT_KEY_UP) {
        switch (evento->keyboard.keycode) {
            case ALLEGRO_KEY_UP:    p->tecla_cima = false; break;
            case ALLEGRO_KEY_LEFT:  p->tecla_esquerda = false; break;
            case ALLEGRO_KEY_RIGHT: p->tecla_direita = false; break;
            case ALLEGRO_KEY_SPACE: p->tecla_tiro = false; break;
            case ALLEGRO_KEY_DOWN:  p->tecla_baixo = false; break;
        }
    }
}

void player_update(Player *p) {
    EstadoJogador estado_anterior = p->estado;

    // Se o jogador está no meio da animação de tiro, ela tem prioridade
    if (p->estado == ATIRANDO || p->estado == AGACHADO_ATIRANDO) {
        p->tempo_estado_tiro -= 1.0 / 60.0;
        if (p->tempo_estado_tiro <= 0) {
            // Quando o tiro acaba, se a tecla baixo AINDA estiver pressionada, volta para agachado
            if (p->tecla_baixo) {
                 p->estado = AGACHADO;
            } else { // Senão, volta para parado
                 p->estado = PARADO;
            }
        }
    }
    else if (!p->no_chao) {
        p->estado = PULANDO;
    }
    else if (p->tecla_baixo) { 
        p->estado = AGACHADO;
        p->vel_x = 0; // Impede o jogador de se mover enquanto está agachado
    }
    else { 
        if (p->tecla_esquerda) {
            p->vel_x = -VELOCIDADE_JOGADOR;
            p->direcao = -1;
            p->estado = CORRENDO;
        } else if (p->tecla_direita) {
            p->vel_x = VELOCIDADE_JOGADOR;
            p->direcao = 1;
            p->estado = CORRENDO;
        } else {
            p->vel_x = 0;
            p->estado = PARADO;
        }
    }

    if (p->tecla_cima && p->no_chao) {
        p->vel_y = FORCA_PULO;
        p->no_chao = false;
    }

    // --- Física e Colisão ---
    p->vel_y += GRAVIDADE;
    p->x += p->vel_x;
    p->y += p->vel_y;

    if (p->y >= FLOOR_Y) {
        p->y = FLOOR_Y;
        p->vel_y = 0;
        p->no_chao = true;
    }

    // Impede o jogador de sair pela esquerda da tela (borda x = 0)
    if (p->x < -100) {
        p->x = -100;
    }

    if (p->y >= FLOOR_Y) {
        p->y = FLOOR_Y;
        p->vel_y = 0;
        p->no_chao = true;
    }

    // --- Animação ---
    if (p->estado != estado_anterior) {
        p->frame_atual = 0;
        p->tempo_frame = 0;
    }
    p->tempo_frame += 1.0 / 60.0;
    if (p->tempo_frame >= 1.0 / FPS_ANIMACAO) {
        p->tempo_frame = 0;
        p->frame_atual++;

        int total_frames_animacao = 0;
        if (p->estado == PARADO) total_frames_animacao = p->num_frames_parado;
        else if (p->estado == CORRENDO) total_frames_animacao = p->num_frames_correndo;
        else if (p->estado == ATIRANDO) total_frames_animacao = p->num_frames_atirando;
        else if (p->estado == PULANDO) total_frames_animacao = p->num_frames_pulando;
        else if (p->estado == AGACHADO) total_frames_animacao = p->num_frames_agachado;
        else if (p->estado == AGACHADO_ATIRANDO) total_frames_animacao = p->num_frames_agachado_atirando;

        if (p->frame_atual >= total_frames_animacao) {
            if (p->estado == ATIRANDO) p->frame_atual = total_frames_animacao - 1;
            else p->frame_atual = 0;
        }
    }

    // --- Cooldown de Tiro ---
    if (p->cooldown_tiro > 0) {
        p->cooldown_tiro -= 1.0 / 60.0;
    }

    if(p->estado == AGACHADO || p->estado == AGACHADO_ATIRANDO) {
        p->hitbox_largura = HITBOX_AGACHADO_LARGURA;
        p->hitbox_altura = HITBOX_AGACHADO_ALTURA;
        p->hitbox_offset_x = HITBOX_AGACHADO_OFFSET_X;
        p->hitbox_offset_y = HITBOX_AGACHADO_OFFSET_Y;
    } 
    else {
        p->hitbox_largura = HITBOX_NORMAL_LARGURA;
        p->hitbox_altura = HITBOX_NORMAL_ALTURA;
        p->hitbox_offset_x = HITBOX_NORMAL_OFFSET_X;
        p->hitbox_offset_y = HITBOX_NORMAL_OFFSET_Y;
    }
}

void player_draw(Player *p, float camera_x, float camera_y) {
    if (!p) return;

    ALLEGRO_BITMAP *folha_atual = NULL;
    switch(p->estado) {
        case ATIRANDO: folha_atual = p->folha_sprite_atirando; break;
        case CORRENDO: folha_atual = p->folha_sprite_correndo; break;
        case PULANDO:  folha_atual = p->folha_sprite_pulando;  break;
        case PARADO:   folha_atual = p->folha_sprite_parado;   break;
        case AGACHADO: folha_atual = p->folha_sprite_agachado; break;
        case AGACHADO_ATIRANDO: folha_atual = p->folha_sprite_agachado_atirando; break;
    }
    
    if (folha_atual) {
        int flags = (p->direcao == -1) ? ALLEGRO_FLIP_HORIZONTAL : 0;
        float sx = p->frame_atual * p->frame_largura;
        float sy = 0;
        float dw = p->frame_largura * ESCALA;
        float dh = p->frame_altura * ESCALA;
        al_draw_scaled_bitmap(folha_atual, sx, sy, p->frame_largura, p->frame_altura, p->x - camera_x, p->y - camera_y, dw, dh, flags);
    }
}

void player_fire(Player *p, Bullet bullets[], int max_bullets) {
    if (p->tecla_tiro && p->cooldown_tiro <= 0) {
        for (int i = 0; i < max_bullets; i++) {
            if (!bullets[i].ativo) {
                
                float offset_x = 0;
                float offset_y = 0;

                // --- SWITCH CORRIGIDO E COMPLETO ---
                switch(p->estado) {
                    case PARADO:
                        offset_x = OFFSET_TIRO_PARADO_X;
                        offset_y = OFFSET_TIRO_PARADO_Y;
                        break;
                    case CORRENDO:
                        offset_x = OFFSET_TIRO_CORRENDO_X;
                        offset_y = OFFSET_TIRO_CORRENDO_Y;
                        break;
                    case ATIRANDO:
                        offset_x = OFFSET_TIRO_ATIRANDO_X;
                        offset_y = OFFSET_TIRO_ATIRANDO_Y;
                        break;
                    case PULANDO:
                        offset_x = OFFSET_TIRO_PULANDO_X;
                        offset_y = OFFSET_TIRO_PULANDO_Y;
                        break;
                    case AGACHADO:
                        offset_x = OFFSET_TIRO_AGACHADO_X;
                        offset_y = OFFSET_TIRO_AGACHADO_Y;
                        break;
                }

                float start_x, start_y;

                if (p->direcao == 1) { // Direita
                    start_x = p->x + (offset_x * ESCALA);
                } else { // Esquerda
                    start_x = p->x + ((p->frame_largura - offset_x) * ESCALA);
                }
                start_y = p->y + (offset_y * ESCALA);
                
                // Dispara o projétil
                bullet_fire(&bullets[i], start_x, start_y, p->direcao);
                
                // Define o estado de tiro correto
                if (p->estado == AGACHADO || p->estado == AGACHADO_ATIRANDO) {
                    p->estado = AGACHADO_ATIRANDO;
                } else {
                    p->estado = ATIRANDO;
                }

                // Reinicia os timers e frames
                p->cooldown_tiro = 0.3f; 
                p->tempo_estado_tiro = 0.25f;
                p->frame_atual = 0;
                break; 
            }
        }
    }
}