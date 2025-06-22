#include "player.h"

struct Player* player_create(float x, float y) {
    
    struct Player *p;

    if(!(p = malloc(sizeof(struct Player)))){
        return NULL;
    }

    p->x = x;
    p->y = y;
    p->vel_x = 0;
    p->vel_y = 0;
    p->hp = 5;
    p->direcao = 1;
    p->no_chao = true;
    p->estado = PARADO;
    p->controles = controls_create();

    if (!p->controles) {
        player_destroy(p);
        return NULL;
    }

    p->anim_parado = animation_create(NUM_FRAMES_PARADO, FPS_ANIMACAO);
    p->anim_correndo = animation_create(NUM_FRAMES_CORRENDO, FPS_ANIMACAO);
    p->anim_pulando = animation_create(NUM_FRAMES_PULANDO, FPS_ANIMACAO);
    p->anim_atirando = animation_create(NUM_FRAMES_ATIRANDO, FPS_ANIMACAO * 2);
    p->anim_agachado = animation_create(NUM_FRAMES_AGACHADO, FPS_ANIMACAO);
    p->anim_agachado_atirando = animation_create(NUM_FRAMES_AGACHADO_ATIRANDO, FPS_ANIMACAO * 2);

    p->anim_atual = p->anim_parado;

    p->folha_sprite_parado = al_load_bitmap("assets/Idle.png");
    p->folha_sprite_correndo = al_load_bitmap("assets/Run.png");
    p->folha_sprite_atirando = al_load_bitmap("assets/Shot.png");
    p->folha_sprite_pulando = al_load_bitmap("assets/Jump.png");
    p->folha_sprite_agachado = al_load_bitmap("assets/Crouch.png");
    p->folha_sprite_agachado_atirando = al_load_bitmap("assets/CrouchShot.png");

    if (!p->folha_sprite_parado || !p->folha_sprite_correndo || !p->folha_sprite_atirando || !p->folha_sprite_pulando || !p->folha_sprite_agachado || !p->folha_sprite_agachado_atirando) {
        printf("Falha ao carregar um dos sprites do jogador.\n");
        player_destroy(p);
        return NULL;
    }

    p->frame_largura = 128;
    p->frame_altura = 128;
    p->tempo_estado_tiro = 0.0f;
    p->cooldown_tiro = 0.0f;
    p->hitbox_largura = HITBOX_NORMAL_LARGURA;
    p->hitbox_altura = HITBOX_NORMAL_ALTURA;
    p->hitbox_offset_x = HITBOX_NORMAL_OFFSET_X;
    p->hitbox_offset_y = HITBOX_NORMAL_OFFSET_Y;

    return p;
}

void player_destroy(struct Player *p) {
    if (p) {
        animation_destroy(p->anim_parado);
        animation_destroy(p->anim_correndo);
        animation_destroy(p->anim_pulando);
        animation_destroy(p->anim_atirando);
        animation_destroy(p->anim_agachado);
        animation_destroy(p->anim_agachado_atirando);

        al_destroy_bitmap(p->folha_sprite_parado);
        al_destroy_bitmap(p->folha_sprite_correndo);
        al_destroy_bitmap(p->folha_sprite_atirando);
        al_destroy_bitmap(p->folha_sprite_pulando);
        al_destroy_bitmap(p->folha_sprite_agachado);
        al_destroy_bitmap(p->folha_sprite_agachado_atirando);

        controls_destroy(p->controles);
        free(p);
    }
}

void player_update(struct Player *p) {
    
    enum EstadoJogador estado_anterior = p->estado;

    // Se o jogador está no meio da animação de tiro, ela tem prioridade
    if (p->estado == ATIRANDO || p->estado == AGACHADO_ATIRANDO) {
        p->tempo_estado_tiro -= 1.0 / 60.0;
        if (p->tempo_estado_tiro <= 0) {
            if (p->controles->baixo) {
                p->estado = AGACHADO;
            }
            else if (p->controles->esquerda || p->controles->direita) {
                p->estado = CORRENDO;
            }
            else {
                p->estado = PARADO;
            }
        }
    }
    else if (!p->no_chao) {
        p->estado = PULANDO;
        p->anim_atual = p->anim_pulando;
    }
    else if (p->controles->baixo) {
        p->estado = AGACHADO;
        p->vel_x = 0; // Impede o jogador de se mover enquanto está agachado
        p->anim_atual = p->anim_agachado;
    }
    else {
        if (p->controles->esquerda) {
            p->vel_x = -VELOCIDADE_JOGADOR;
            p->direcao = -1;
            p->estado = CORRENDO;
            p->anim_atual = p->anim_correndo;
        } else if (p->controles->direita) {
            p->vel_x = VELOCIDADE_JOGADOR;
            p->direcao = 1;
            p->estado = CORRENDO;
            p->anim_atual = p->anim_correndo;
        } else {
            p->vel_x = 0;
            p->estado = PARADO;
            p->anim_atual = p->anim_parado;
        }
    }

    // Lógica de Pulo
    if (p->controles->cima && p->no_chao) {
        p->vel_y = FORCA_PULO;
        p->no_chao = false;
    }

    // Física e Colisão com o Mundo
    p->vel_y += GRAVIDADE;
    p->x += p->vel_x;
    p->y += p->vel_y;

    if (p->y >= FLOOR_Y) {
        p->y = FLOOR_Y;
        p->vel_y = 0;
        p->no_chao = true;
    }

    // Impede o jogador de sair pela esquerda da tela (borda x = -100)
    if (p->x < -100) {
        p->x = -100;
    }

    // Se o estado mudou, reseta a nova animação para começar do frame 0
    if (p->estado != estado_anterior) {
        animation_reset(p->anim_atual);
    }
    
    animation_update(p->anim_atual);

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

void player_draw(struct Player *p, float camera_x, float camera_y) {
    if (!p) return;

    ALLEGRO_BITMAP *folha_atual = NULL;
    switch(p->estado) {
        case ATIRANDO: folha_atual = p->folha_sprite_atirando; break;
        case CORRENDO: folha_atual = p->folha_sprite_correndo; break;
        case PULANDO:  folha_atual = p->folha_sprite_pulando;  break;
        case AGACHADO: folha_atual = p->folha_sprite_agachado; break;
        case AGACHADO_ATIRANDO: folha_atual = p->folha_sprite_agachado_atirando; break;
        case PARADO:   folha_atual = p->folha_sprite_parado;   break;
    }
    
    if (folha_atual && p->anim_atual) {
        float sx = p->anim_atual->frame_atual * p->frame_largura;
        int flags = (p->direcao == -1) ? ALLEGRO_FLIP_HORIZONTAL : 0;
        al_draw_scaled_bitmap(folha_atual, sx, 0, p->frame_largura, p->frame_altura, p->x - camera_x, p->y - camera_y, p->frame_largura * ESCALA, p->frame_altura * ESCALA, flags);
    }
}

void player_fire(struct Player *p, struct Bullet bullets[], int max_bullets) {
    
    if (p->controles->tiro && p->cooldown_tiro <= 0) {
        for (int i = 0; i < max_bullets; i++) {
            if (!bullets[i].ativo) {
                
                float offset_x = 0;
                float offset_y = 0;

                // Escolhe o offset da "ponta da arma" baseado no estado do jogador
                switch(p->estado) {
                    case PARADO:   
                        offset_x = OFFSET_TIRO_PARADO_X;   
                        offset_y = OFFSET_TIRO_PARADO_Y;   
                        break;
                    case CORRENDO: 
                        offset_x = OFFSET_TIRO_CORRENDO_X; 
                        offset_y = OFFSET_TIRO_CORRENDO_Y; 
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

                // Calcula a posição inicial do projétil, invertendo o X se necessário
                if (p->direcao == 1) {
                    start_x = p->x + (offset_x * ESCALA);
                } 
                else {
                    start_x = p->x + ((p->frame_largura - offset_x) * ESCALA);
                }
                start_y = p->y + (offset_y * ESCALA);
                
                bullet_fire(&bullets[i], start_x, start_y, p->direcao, VELOCIDADE_PROJETIL, PLAYER);
                
                // Reinicia o cooldown do tiro
                p->cooldown_tiro = 0.3f; 

                if (p->estado == AGACHADO || p->estado == AGACHADO_ATIRANDO) {
                    p->estado = AGACHADO_ATIRANDO;
                    p->anim_atual = p->anim_agachado_atirando;
                } 
                else {
                    p->estado = ATIRANDO;
                    p->anim_atual = p->anim_atirando;
                }
                
                // Reseta a nova animação de tiro para que ela comece do frame 0
                animation_reset(p->anim_atual);
                
                p->tempo_estado_tiro = 0.25f;

                break;
            }
        }
    }
}