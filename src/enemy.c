#include "enemy.h"

void enemy_init(struct Enemy inimigos[], int max_inimigos) {
    for (int i = 0; i < max_inimigos; i++) {
        inimigos[i].x = 0;
        inimigos[i].y = 0;
        inimigos[i].hp = 0;
        inimigos[i].ativo = 0;
        inimigos[i].tipo = SOLDADO_ESPINGARDA;
        inimigos[i].estado = INIMIGO_PARADO;
        inimigos[i].folha_sprite_parado = NULL;
        inimigos[i].folha_sprite_atirando = NULL;
        inimigos[i].folha_sprite_morrendo = NULL;
        inimigos[i].anim_parado = NULL;
        inimigos[i].anim_atirando = NULL;
        inimigos[i].anim_morrendo = NULL;
        inimigos[i].anim_atual = NULL;
        inimigos[i].frame_largura = 0;
        inimigos[i].frame_altura = 0;
        inimigos[i].direcao = 1;
        inimigos[i].cooldown_tiro = 0;
        inimigos[i].tempo_estado_tiro = 0;
    }
}

void enemy_spawn(struct Enemy inimigos[], int max_inimigos, const struct EnemyConfig *config, float x, float y) {
    for (int i = 0; i < max_inimigos; i++) {
        if (!inimigos[i].ativo) {
            struct Enemy *e = &inimigos[i];
            
            e->ativo = 1;
            e->x = x;
            e->y = y;
            e->direcao = -1;
            e->estado = INIMIGO_PARADO;
            e->frame_largura = 128;
            e->frame_altura = 128;

            e->tipo = config->tipo;
            e->hp = config->hp;
            e->folha_sprite_parado = config->folha_sprite_parado;
            e->folha_sprite_atirando = config->folha_sprite_atirando;
            e->folha_sprite_morrendo = config->folha_sprite_morrendo;

            e->anim_parado = animation_create(config->num_frames_parado, FPS_ANIMACAO);
            e->anim_atirando = animation_create(config->num_frames_atirando, FPS_ANIMACAO);
            e->anim_morrendo = animation_create(config->num_frames_morrendo, FPS_ANIMACAO);
            
            if (!e->anim_parado || !e->anim_atirando || !e->anim_morrendo) {
                printf("ERRO: Falha ao criar animação do inimigo.\n");
                e->ativo = 0;
                return;
            }

            e->anim_atual = e->anim_parado;
            e->cooldown_tiro = 1.5f;
            break;
        }
    }
}

void enemy_update(struct Enemy *e, struct Player *p, struct Bullet bullets[], int max_bullets) {
    
    if (!e->ativo || !e->anim_atual) return;

    animation_update(e->anim_atual);

    switch(e->estado) {
        case INIMIGO_MORRENDO:
            // Se a animação de morte terminou, desativa o inimigo
            if (e->anim_atual->frame_atual >= e->anim_atual->num_frames - 1) {
                e->ativo = 0;
            }
            break;

        case INIMIGO_ATIRANDO:
            // Se a animação de tiro terminou, volta ao estado parado
            if (e->anim_atual->frame_atual >= e->anim_atirando->num_frames - 1) {
                e->estado = INIMIGO_PARADO;
                e->anim_atual = e->anim_parado;
                animation_reset(e->anim_atual);
            }
            break;

        case INIMIGO_PARADO:
            // IA de virar e atirar
            if (p->x < e->x) e->direcao = -1;
            else e->direcao = 1;

            if (e->cooldown_tiro > 0) {
                e->cooldown_tiro -= 1.0 / 60.0;
            } 
            else {
                float distancia = fabs(p->x - e->x);
                if (distancia < LARGURA_TELA) {
                    switch(e->tipo) {
                        case SOLDADO_ESCUDO:
                            // Lógica do tiro único e rápido
                            for (int i = 0; i < max_bullets; i++) {
                                if (!bullets[i].ativo) {
                                    
                                    float start_x, start_y;
                                    
                                    if(e->direcao == 1) {
                                        // Se o inimigo está virado para a direita, o tiro sai da direita
                                        start_x = e->x + (OFFSET_TIRO_INIMIGO_X * ESCALA);
                                    } 
                                    else {
                                        // Se o inimigo está virado para a esquerda, o tiro sai da esquerda
                                        start_x = e->x + ((e->frame_largura - OFFSET_TIRO_INIMIGO_X) * ESCALA);
                                    }

                                    start_y = e->y + (OFFSET_TIRO_INIMIGO_Y * ESCALA);
                                
                                    // Dispara com a velocidade rápida
                                    bullet_fire(&bullets[i], start_x, start_y, e->direcao, VELOCIDADE_PROJETIL_RAPIDO, ENEMY);
                                
                                    e->estado = INIMIGO_ATIRANDO;
                                    e->anim_atual = e->anim_atirando;
                                    animation_reset(e->anim_atual);
                                    e->cooldown_tiro = 2.5f; // Cooldown um pouco maior
                                    break;
                                }
                            }
                            break;
                        case SOLDADO_ESPINGARDA:
                            // Lógica do tiro duplo
                            int slots_livres[2];
                            int slots_encontrados = 0;
                            for (int i = 0; i < max_bullets && slots_encontrados < 2; i++) {
                                if (!bullets[i].ativo) {
                                    slots_livres[slots_encontrados] = i;
                                    slots_encontrados++;
                                }
                            }

                            if (slots_encontrados == 2) {
                                
                                float start_x;
                                
                                if(e->direcao == 1) {
                                    // Se o inimigo está virado para a direita, o tiro sai da direita
                                    start_x = e->x + (OFFSET_TIRO_INIMIGO_X * ESCALA);
                                } 
                                else {
                                    // Se o inimigo está virado para a esquerda, o tiro sai da esquerda
                                    start_x = e->x + ((e->frame_largura - OFFSET_TIRO_INIMIGO_X) * ESCALA);
                                }

                                // Dispara dois projéteis com velocidade normal
                                bullet_fire(&bullets[slots_livres[0]], start_x, e->y + (OFFSET_TIRO_DUPLO_Y1 * ESCALA), e->direcao, VELOCIDADE_PROJETIL, ENEMY);
                                bullet_fire(&bullets[slots_livres[1]], start_x, e->y + (OFFSET_TIRO_DUPLO_Y2 * ESCALA), e->direcao, VELOCIDADE_PROJETIL, ENEMY);

                                e->estado = INIMIGO_ATIRANDO;
                                e->anim_atual = e->anim_atirando;
                                animation_reset(e->anim_atual);
                                e->cooldown_tiro = 3.0f;
                            }
                            break;
                    
                    }
                }
            }
            break;
    }
}

void enemy_draw(struct Enemy inimigos[], int max_inimigos, float camera_x, float camera_y) {
    for (int i = 0; i < max_inimigos; i++) {
        if (inimigos[i].ativo) {
            struct Enemy *e = &inimigos[i];

            ALLEGRO_BITMAP *folha_atual = NULL;
            switch(e->estado) {
                case INIMIGO_PARADO:   folha_atual = e->folha_sprite_parado;   break;
                case INIMIGO_ATIRANDO: folha_atual = e->folha_sprite_atirando; break;
                case INIMIGO_MORRENDO: folha_atual = e->folha_sprite_morrendo; break;
            }
            
            if (folha_atual && e->anim_atual) {
                float sx = e->anim_atual->frame_atual * e->frame_largura;
                int flags = (e->direcao == -1) ? ALLEGRO_FLIP_HORIZONTAL : 0;
                al_draw_scaled_bitmap(folha_atual, sx, 0, e->frame_largura, e->frame_altura, e->x - camera_x, e->y - camera_y, e->frame_largura * ESCALA, e->frame_altura * ESCALA, flags);
            }
        }
    }
}

void enemy_destroy_animations(struct Enemy inimigos[], int max_inimigos) {
    for (int i = 0; i < max_inimigos; i++) {
        animation_destroy(inimigos[i].anim_parado);
        animation_destroy(inimigos[i].anim_atirando);
        animation_destroy(inimigos[i].anim_morrendo);
    }
}