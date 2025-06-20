#include "boss.h"
#include "config.h"
#include "player.h"
#include "bullet.h"
#include <stdio.h>
#include <math.h>

void boss_init(Boss *boss) {
    boss->ativo = false;
    boss->anim_moving = NULL;
}

void boss_destroy(Boss *boss) {
    animation_destroy(boss->anim_moving);
}

void boss_spawn(Boss *boss, float x, ALLEGRO_BITMAP *sprite) {
    if (boss->ativo) return;

    printf("CHEFE APARECEU!\\n");
    fflush(stdout);

    boss->ativo = true;
    boss->x = x;
    boss->y = FLOOR_Y - 100;
    boss->vel_x = 0;
    boss->vel_y = 0;
    boss->hp = 50;
    boss->direcao = -1;
    boss->estado = BOSS_MOVING;
    boss->frame_largura = al_get_bitmap_width(sprite) / NUM_FRAMES_BOSS_MOVING; // Calculate frame width
    boss->frame_altura = al_get_bitmap_height(sprite);

    boss->hitbox_largura = BOSS_HITBOX_LARGURA;
    boss->hitbox_altura = BOSS_HITBOX_ALTURA;
    boss->hitbox_offset_x = BOSS_HITBOX_OFFSET_X;
    boss->hitbox_offset_y = BOSS_HITBOX_OFFSET_Y;

    boss->sprite_sheet = sprite;

    boss->anim_moving = animation_create(NUM_FRAMES_BOSS_MOVING, 5.0f);
    boss->anim_atual = boss->anim_moving;
    boss->cooldown_tiro = 2.0f; // Initial cooldown
}

void boss_update(Boss *boss, Player *p, Bullet bullets[], int max_bullets) {
    if (!boss->ativo) return;

    // A animação de movimento está sempre tocando
    animation_update(boss->anim_atual);

    // --- IA DE PERSEGUIÇÃO E TIRO ---

    // 1. Define a direção e a velocidade para seguir o jogador
    // Adicionamos uma "zona morta" para o chefe não ficar "tremendo" quando estiver perto
    if (p->x < boss->x - 10) { 
        boss->direcao = -1;
        boss->vel_x = -VELOCIDADE_BOSS;
    } else if (p->x > boss->x + 10) {
        boss->direcao = 1;
        boss->vel_x = VELOCIDADE_BOSS;
    } else {
        // Para de se mover se estiver alinhado com o jogador
        boss->vel_x = 0;
    }

    // 2. Lógica de tiro baseada em cooldown
    boss->cooldown_tiro -= 1.0 / 60.0;
    if (boss->cooldown_tiro <= 0) {
        // Chefe atira 3 projéteis de uma vez
        int slots_livres[3];
        int slots_encontrados = 0;
        for (int i = 0; i < max_bullets && slots_encontrados < 3; i++) {
            if (!bullets[i].ativo) {
                slots_livres[slots_encontrados++] = i;
            }
        }

        if (slots_encontrados == 3) {
            float start_x;
            if(boss->direcao == 1) start_x = boss->x + (BOSS_OFFSET_TIRO_X * ESCALA);
            else start_x = boss->x + ((boss->frame_largura - BOSS_OFFSET_TIRO_X) * ESCALA);

            // Dispara os 3 projéteis
            bullet_fire(&bullets[slots_livres[0]], start_x, boss->y + (BOSS_OFFSET_TIRO_Y1 * ESCALA), boss->direcao, VELOCIDADE_PROJETIL, OWNER_ENEMY);
            bullet_fire(&bullets[slots_livres[1]], start_x, boss->y + (BOSS_OFFSET_TIRO_Y2 * ESCALA), boss->direcao, VELOCIDADE_PROJETIL, OWNER_ENEMY);
            bullet_fire(&bullets[slots_livres[2]], start_x, boss->y + (BOSS_OFFSET_TIRO_Y3 * ESCALA), boss->direcao, VELOCIDADE_PROJETIL, OWNER_ENEMY);

            // Reinicia o cooldown
            boss->cooldown_tiro = 3.5f;
        }
    }

    // --- FÍSICA DO CHEFE ---

    // 3. Atualiza a posição X do chefe com base na velocidade
    boss->x += boss->vel_x;
}

void boss_draw(Boss *boss, float camera_x, float camera_y) {
    if (!boss->ativo) return;

    if (boss->sprite_sheet && boss->anim_atual) {
        float sx = boss->anim_atual->frame_atual * boss->frame_largura;
        int flags = (boss->direcao == -1) ? ALLEGRO_FLIP_HORIZONTAL : 0;
        al_draw_scaled_bitmap(boss->sprite_sheet, sx, 0, boss->frame_largura, boss->frame_altura, boss->x - camera_x, boss->y - camera_y, boss->frame_largura * ESCALA, boss->frame_altura * ESCALA, flags);
    }
}