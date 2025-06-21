#include "boss.h"
#include "config.h"
#include "player.h"
#include "bullet.h"
#include <stdio.h>
#include <math.h>

void boss_init(Boss *boss) {
    boss->ativo = false;
    boss->anim_moving = NULL;
    boss->anim_fx_tiro = NULL;
}

void boss_destroy(Boss *boss) {
    animation_destroy(boss->anim_moving);
    animation_destroy(boss->anim_fx_tiro);
}

void boss_spawn(Boss *boss, float x, ALLEGRO_BITMAP *sprite, ALLEGRO_BITMAP *fx_sprite) {
    if (boss->ativo) return;

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
    boss->sprite_fx_tiro = fx_sprite;

    boss->anim_moving = animation_create(NUM_FRAMES_BOSS_MOVING, 5.0f);
    boss->anim_fx_tiro = animation_create(NUM_FRAMES_FX_TIRO_CHEFE, FPS_FX_TIRO_CHEFE);
    boss->anim_atual = boss->anim_moving;
    boss->cooldown_tiro = 2.0f;
    boss->fx_tiro_ativo = false;
}

void boss_update(Boss *boss, struct Player *p, struct Bullet bullets[], int max_bullets) {
    if (!boss->ativo) return;

    // Se o efeito de tiro estiver ativo, atualiza sua animação
    if (boss->fx_tiro_ativo) {
        animation_update(boss->anim_fx_tiro);
        // Se a animação do efeito terminou, desativa
        if (boss->anim_fx_tiro->frame_atual >= boss->anim_fx_tiro->num_frames - 1) {
            boss->fx_tiro_ativo = false;
        }
    }

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

            boss->fx_tiro_ativo = true;
            animation_reset(boss->anim_fx_tiro);
            float start_x;
            if(boss->direcao == 1) start_x = boss->x + (BOSS_OFFSET_TIRO_X * ESCALA);
            else start_x = boss->x + ((boss->frame_largura - BOSS_OFFSET_TIRO_X) * ESCALA);

            // Dispara os 3 projéteis
            bullet_fire(&bullets[slots_livres[0]], start_x, boss->y + (BOSS_OFFSET_TIRO_Y1 * ESCALA), boss->direcao, VELOCIDADE_PROJETIL, ENEMY);
            bullet_fire(&bullets[slots_livres[1]], start_x, boss->y + (BOSS_OFFSET_TIRO_Y2 * ESCALA), boss->direcao, VELOCIDADE_PROJETIL, ENEMY);
            bullet_fire(&bullets[slots_livres[2]], start_x, boss->y + (BOSS_OFFSET_TIRO_Y3 * ESCALA), boss->direcao, VELOCIDADE_PROJETIL, ENEMY);

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

    // 1. Desenha o corpo do chefe normalmente
    if (boss->sprite_sheet && boss->anim_atual) {
        float sx = boss->anim_atual->frame_atual * boss->frame_largura;
        int flags = (boss->direcao == -1) ? ALLEGRO_FLIP_HORIZONTAL : 0;
        al_draw_scaled_bitmap(boss->sprite_sheet, sx, 0, boss->frame_largura, boss->frame_altura, boss->x - camera_x, boss->y - camera_y, boss->frame_largura * ESCALA, boss->frame_altura * ESCALA, flags);
    }

    // 2. DESENHA O EFEITO DE TIRO POR CIMA, SE ESTIVER ATIVO (COM AS DIMENSÕES CORRIGIDAS)
    if (boss->fx_tiro_ativo && boss->sprite_fx_tiro) {
        // Usa as constantes de config.h para o tamanho exato do frame
        float fx_w = FX_TIRO_CHEFE_FRAME_LARGURA;
        float fx_h = FX_TIRO_CHEFE_FRAME_ALTURA;
        float sx = boss->anim_fx_tiro->frame_atual * fx_w;

        // Posição da ponta da arma
        float fx_x = boss->x + (boss->direcao == 1 ? BOSS_OFFSET_TIRO_X * ESCALA : (boss->frame_largura - BOSS_OFFSET_TIRO_X) * ESCALA);
        float fx_y = boss->y + (BOSS_OFFSET_TIRO_Y2 * ESCALA);

        // Centraliza o efeito na ponta da arma
        fx_x -= (fx_w * ESCALA) / 2;
        fx_y -= ((fx_h * ESCALA) / 2) - 10;
        
        int flags = (boss->direcao == 1) ? ALLEGRO_FLIP_HORIZONTAL : 0;
        al_draw_scaled_bitmap(boss->sprite_fx_tiro, sx, 0, fx_w, fx_h, fx_x - camera_x, fx_y - camera_y, fx_w * ESCALA, fx_h * ESCALA, flags);
    }
}