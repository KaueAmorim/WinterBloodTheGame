#include "enemy.h"
#include "config.h"

// Inicializa o array de inimigos, marcando todos como inativos
void enemy_init(Enemy inimigos[], int max_inimigos) {
    for (int i = 0; i < max_inimigos; i++) {
        inimigos[i].ativo = false;
        inimigos[i].anim_parado = NULL;
        inimigos[i].anim_atirando = NULL;
    }
}

void enemy_destroy_animations(Enemy inimigos[], int max_inimigos) {
    for (int i = 0; i < max_inimigos; i++) {
        animation_destroy(inimigos[i].anim_parado);
        animation_destroy(inimigos[i].anim_atirando);
    }
}

// "Spawna" um inimigo em uma posição específica
void enemy_spawn(Enemy inimigos[], int max_inimigos, ALLEGRO_BITMAP *sprite_parado, ALLEGRO_BITMAP *sprite_atirando, float x, float y) {
    for (int i = 0; i < max_inimigos; i++) {
        if (!inimigos[i].ativo) {
            Enemy *e = &inimigos[i];
            e->ativo = true;
            e->x = x;
            e->y = FLOOR_Y; // Garante que o inimigo fique no chão
            e->hp = 3;
            e->direcao = -1;
            e->tipo = SOLDADO_NORMAL;
            e->estado = INIMIGO_PARADO;
            e->frame_largura = 128;
            e->frame_altura = 128;

            e->folha_sprite_parado = sprite_parado;
            e->folha_sprite_atirando = sprite_atirando;

            // Cria os objetos de animação
            e->anim_parado = animation_create(NUM_FRAMES_INIMIGO_SOLDADO, FPS_ANIMACAO / 2.0);
            e->anim_atirando = animation_create(NUM_FRAMES_INIMIGO_ATIRANDO, FPS_ANIMACAO);
            e->anim_atual = e->anim_parado; // Animação inicial
            break;
        }
    }
}
void enemy_update(Enemy *e, Player *p, Bullet bullets[], int max_bullets) {
    if (!e->ativo) return;

    // --- Máquina de Estados do Inimigo ---
    if (e->estado == INIMIGO_ATIRANDO) {
        // Se a animação de tiro terminou (verificando o frame_atual)
        if (e->anim_atual->frame_atual >= e->anim_atirando->num_frames - 1) {
             e->estado = INIMIGO_PARADO;
             e->anim_atual = e->anim_parado;
             animation_reset(e->anim_atual);
        }
    } else { // Se o estado for INIMIGO_PARADO
        if (p->x < e->x) e->direcao = -1;
        else e->direcao = 1;

        if (e->cooldown_tiro > 0) {
            e->cooldown_tiro -= 1.0 / 60.0;
        } else {
            float distancia = fabs(p->x - e->x);
            if (distancia < LARGURA_TELA / 1.5) {
                for (int i = 0; i < max_bullets; i++) {
                    if (!bullets[i].ativo) {
                        float start_x = e->x + (e->direcao == 1 ? 90 * ESCALA : 38 * ESCALA);
                        float start_y = e->y + (70 * ESCALA);
                        bullet_fire(&bullets[i], start_x, start_y, e->direcao, OWNER_ENEMY);
                        
                        // MUDA PARA O ESTADO E ANIMAÇÃO DE TIRO
                        e->estado = INIMIGO_ATIRANDO;
                        e->anim_atual = e->anim_atirando;
                        animation_reset(e->anim_atual);
                        
                        e->cooldown_tiro = 2.0f;
                        break;
                    }
                }
            }
        }
    }

    // A atualização da animação é sempre chamada, de forma simples e limpa
    animation_update(e->anim_atual);
}

// Desenha todos os inimigos ativos
void enemy_draw(Enemy inimigos[], int max_inimigos, float camera_x, float camera_y) {
    for (int i = 0; i < max_inimigos; i++) {
        if (inimigos[i].ativo) {
            Enemy *e = &inimigos[i];
            ALLEGRO_BITMAP *folha_atual = (e->estado == INIMIGO_PARADO) ? e->folha_sprite_parado : e->folha_sprite_atirando;

            if (folha_atual) {
                float sx = e->anim_atual->frame_atual * e->frame_largura;
                int flags = (e->direcao == -1) ? ALLEGRO_FLIP_HORIZONTAL : 0;
                al_draw_scaled_bitmap(folha_atual, sx, 0, e->frame_largura, e->frame_altura, e->x - camera_x, e->y, e->frame_largura * ESCALA, e->frame_altura * ESCALA, flags);
            }
        }
    }
}