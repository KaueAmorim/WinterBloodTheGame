#include "enemy.h"
#include "config.h"

// Inicializa o array de inimigos, marcando todos como inativos
void enemy_init(Enemy inimigos[], int max_inimigos) {
    for (int i = 0; i < max_inimigos; i++) {
        inimigos[i].ativo = false;
        inimigos[i].anim_parado = NULL;
        inimigos[i].anim_atirando = NULL;
        inimigos[i].anim_morrendo = NULL;
    }
}

void enemy_destroy_animations(Enemy inimigos[], int max_inimigos) {
    for (int i = 0; i < max_inimigos; i++) {
        animation_destroy(inimigos[i].anim_parado);
        animation_destroy(inimigos[i].anim_atirando);
        animation_destroy(inimigos[i].anim_morrendo);
    }
}

// "Spawna" um inimigo em uma posição específica
void enemy_spawn(Enemy inimigos[], int max_inimigos, const EnemyConfig *config, float x, float y) {
    for (int i = 0; i < max_inimigos; i++) {
        if (!inimigos[i].ativo) {
            Enemy *e = &inimigos[i];
            e->ativo = true;
            e->x = x;
            e->y = FLOOR_Y;
            e->direcao = -1;
            e->estado = INIMIGO_PARADO;
            e->frame_largura = 128;
            e->frame_altura = 128;

            // --- COPIA OS DADOS DA "RECEITA" ---
            e->tipo = config->tipo;
            e->hp = config->hp;
            e->folha_sprite_parado = config->folha_sprite_parado;
            e->folha_sprite_atirando = config->folha_sprite_atirando;
            e->folha_sprite_morrendo = config->folha_sprite_morrendo;
            // ------------------------------------

            // Cria os objetos de animação com os dados da receita
            e->anim_parado = animation_create(config->num_frames_parado, FPS_ANIMACAO / 2.0);
            e->anim_atirando = animation_create(config->num_frames_atirando, FPS_ANIMACAO);
            e->anim_morrendo = animation_create(config->num_frames_morrendo, FPS_ANIMACAO);
            
            if (!e->anim_parado || !e->anim_atirando || !e->anim_morrendo) {
                printf("ERRO: Falha ao criar animação do inimigo.\n");
                e->ativo = false;
                return;
            }

            e->anim_atual = e->anim_parado;
            e->cooldown_tiro = 1.5f;
            break;
        }
    }
}

void enemy_update(Enemy *e, Player *p, Bullet bullets[], int max_bullets) {
    if (!e->ativo || !e->anim_atual) return; // Trava de segurança

    // A lógica de animação agora é sempre chamada, o TAD cuida do resto
    animation_update(e->anim_atual);

    // --- Máquina de Estados do Inimigo ---
    switch(e->estado) {
        case INIMIGO_MORRENDO:
            // Se a animação de morte terminou, desativa o inimigo
            if (e->anim_atual->frame_atual >= e->anim_atual->num_frames - 1) {
                e->ativo = false;
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
                    // --- NOVA LÓGICA DE DECISÃO DE ATAQUE ---
                    int chance = rand() % 100; // Gera um número de 0a 99

                    // 30% de chance de dar um tiro duplo
                    if (chance < 30) {
                        int slots_livres[2];
                        int slots_encontrados = 0;
                        // Procura por DOIS slots de projéteis livres
                        for (int i = 0; i < max_bullets && slots_encontrados < 2; i++) {
                            if (!bullets[i].ativo) {
                                slots_livres[slots_encontrados++] = i;
                            }
                        }
                    
                        // Se encontrou dois slots, dispara o tiro duplo
                        if (slots_encontrados == 2) {
                            float start_x;
                            if(e->direcao == 1) start_x = e->x + (OFFSET_TIRO_INIMIGO_X * ESCALA);
                            else start_x = e->x + ((e->frame_largura - OFFSET_TIRO_INIMIGO_X) * ESCALA);

                            // Dispara dois projéteis em alturas diferentes
                            bullet_fire(&bullets[slots_livres[0]], start_x, e->y + (OFFSET_TIRO_DUPLO_Y1 * ESCALA), e->direcao, OWNER_ENEMY);
                            bullet_fire(&bullets[slots_livres[1]], start_x, e->y + (OFFSET_TIRO_DUPLO_Y2 * ESCALA), e->direcao, OWNER_ENEMY);

                            // Ativa o estado de tiro e um cooldown maior
                            e->estado = INIMIGO_ATIRANDO;
                            e->anim_atual = e->anim_atirando;
                            animation_reset(e->anim_atual);
                            e->cooldown_tiro = 3.0f; 
                        }
                    }
                    else {
                        // Procura por UM slot de projétil livre
                        for (int i = 0; i < max_bullets; i++) {
                            if (!bullets[i].ativo) {
                                float start_x;
                                if(e->direcao == 1) start_x = e->x + (OFFSET_TIRO_INIMIGO_X * ESCALA);
                                else start_x = e->x + ((e->frame_largura - OFFSET_TIRO_INIMIGO_X) * ESCALA);

                                float start_y = e->y + (OFFSET_TIRO_INIMIGO_Y * ESCALA);
                            
                                bullet_fire(&bullets[i], start_x, start_y, e->direcao, OWNER_ENEMY);
                            
                                e->estado = INIMIGO_ATIRANDO;
                                e->anim_atual = e->anim_atirando;
                                animation_reset(e->anim_atual);
                            
                                e->cooldown_tiro = 2.0f; // Cooldown normal
                                break;
                            }
                        }
                    }
                }
            }
            break;
    }
}

// Desenha todos os inimigos ativos
void enemy_draw(Enemy inimigos[], int max_inimigos, float camera_x, float camera_y) {
    for (int i = 0; i < max_inimigos; i++) {
        if (inimigos[i].ativo) {
            Enemy *e = &inimigos[i];
            
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