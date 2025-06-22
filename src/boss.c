#include "boss.h"
#include "config.h"
#include "player.h"
#include "bullet.h"
#include <stdio.h>
#include <math.h>

void boss_init(struct Boss *boss) {
    boss->x = 0;
    boss->y = 0;
    boss->vel_x = 0;
    boss->vel_y = 0;
    boss->hp = 0;
    boss->direcao = 0;
    boss->estado = BOSS_MOVING;
    boss->ativo = 0;
    boss->sprite_move = NULL;
    boss->sprite_tiro = NULL;
    boss->anim_atual = NULL;
    boss->anim_moving = NULL;
    boss->anim_tiro = NULL;
    boss->frame_largura = 0;
    boss->frame_altura = 0;
    boss->cooldown_tiro = 0.0f;
    boss->tiro_ativo = 0;
    boss->hitbox_largura = BOSS_HITBOX_LARGURA;
    boss->hitbox_altura = BOSS_HITBOX_ALTURA;
    boss->hitbox_offset_x = BOSS_HITBOX_OFFSET_X;
    boss->hitbox_offset_y = BOSS_HITBOX_OFFSET_Y;
}

void boss_spawn(struct Boss *boss, float x, float y, ALLEGRO_BITMAP *sprite, ALLEGRO_BITMAP *fx_sprite) {
    if (boss->ativo) return;

    boss->ativo = 1;
    boss->x = x;
    boss->y = y;
    boss->hp = 50;
    boss->direcao = -1;
    boss->estado = BOSS_MOVING;

    boss->sprite_move = sprite;
    boss->sprite_tiro = fx_sprite;

    boss->anim_moving = animation_create(NUM_FRAMES_BOSS_MOVING, 5.0f);
    boss->anim_tiro = animation_create(NUM_FRAMES_FX_TIRO_CHEFE, 30.0f);

    boss->frame_largura = al_get_bitmap_width(sprite) / NUM_FRAMES_BOSS_MOVING;
    boss->frame_altura = al_get_bitmap_height(sprite);
    boss->anim_atual = boss->anim_moving;
    boss->cooldown_tiro = 2.0f;
    boss->tiro_ativo = 0;
}

void boss_update(struct Boss *boss, struct Player *p, struct Bullet bullets[], int max_bullets) {
    if (!boss->ativo) return;

    // Se o efeito de tiro estiver ativo, atualiza sua animação
    if (boss->tiro_ativo) {
        animation_update(boss->anim_tiro);
        if (boss->anim_tiro->frame_atual >= boss->anim_tiro->num_frames - 1) {
            boss->tiro_ativo = 0;
        }
    }

    animation_update(boss->anim_atual);

    // Define a direção e a velocidade para seguir o jogador
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

    // Lógica de tiro baseada em cooldown
    boss->cooldown_tiro -= 1.0 / 60.0;
    if (boss->cooldown_tiro <= 0) {
        // Chefe atira 3 projéteis de uma vez
        int slots_livres[3];
        int slots_encontrados = 0;
        for (int i = 0; i < max_bullets && slots_encontrados < 3; i++) {
            if (!bullets[i].ativo) {
                slots_livres[slots_encontrados] = i;
                slots_encontrados++;
            }
        }

        if (slots_encontrados == 3) {

            boss->tiro_ativo = 1;
            animation_reset(boss->anim_tiro);
            
            float start_x;
            if(boss->direcao == 1){
                start_x = boss->x + (BOSS_OFFSET_TIRO_X * ESCALA);
            } 
            else {
                start_x = boss->x + ((boss->frame_largura - BOSS_OFFSET_TIRO_X) * ESCALA);
            }

            // Dispara os 3 projéteis
            bullet_fire(&bullets[slots_livres[0]], start_x, boss->y + (BOSS_OFFSET_TIRO_Y1 * ESCALA), boss->direcao, VELOCIDADE_PROJETIL, ENEMY);
            bullet_fire(&bullets[slots_livres[1]], start_x, boss->y + (BOSS_OFFSET_TIRO_Y2 * ESCALA), boss->direcao, VELOCIDADE_PROJETIL, ENEMY);
            bullet_fire(&bullets[slots_livres[2]], start_x, boss->y + (BOSS_OFFSET_TIRO_Y3 * ESCALA), boss->direcao, VELOCIDADE_PROJETIL, ENEMY);

            // Reinicia o cooldown
            boss->cooldown_tiro = 3.5f;
        }
    }

    // Atualiza a posição X do chefe com base na velocidade
    boss->x += boss->vel_x;
}

void boss_draw(struct Boss *boss, float camera_x, float camera_y) {
    if (!boss->ativo) return;

    // Desenha o corpo do chefe normalmente
    if (boss->sprite_move && boss->anim_atual) {
        float sx = boss->anim_atual->frame_atual * boss->frame_largura;
        int flags = (boss->direcao == -1) ? ALLEGRO_FLIP_HORIZONTAL : 0;
        al_draw_scaled_bitmap(boss->sprite_move, sx, 0, boss->frame_largura, boss->frame_altura, boss->x - camera_x, boss->y - camera_y, boss->frame_largura * ESCALA, boss->frame_altura * ESCALA, flags);
    }

    // Desenha o efeito de tiro por cima do chefe, se estiver ativo
    if (boss->tiro_ativo && boss->sprite_tiro) {

        // Seleciona o Frame da Animação do Efeito
        float fx_w = 50; // Largura do frame do efeito de tiro
        float fx_h = 48; // Altura do frame do efeito de tiro
        float sx = boss->anim_tiro->frame_atual * fx_w;

        // Posição da ponta da arma
        float fx_x = boss->x + (boss->direcao == 1 ? BOSS_OFFSET_TIRO_X * ESCALA : (boss->frame_largura - BOSS_OFFSET_TIRO_X) * ESCALA);
        float fx_y = boss->y + (BOSS_OFFSET_TIRO_Y2 * ESCALA);

        // Centraliza o efeito na ponta da arma
        fx_x -= (fx_w * ESCALA) / 2;
        fx_y -= ((fx_h * ESCALA) / 2) - 10;
        
        int flags = (boss->direcao == 1) ? ALLEGRO_FLIP_HORIZONTAL : 0;
        al_draw_scaled_bitmap(boss->sprite_tiro, sx, 0, fx_w, fx_h, fx_x - camera_x, fx_y - camera_y, fx_w * ESCALA, fx_h * ESCALA, flags);
    }
}

void boss_destroy_animations(struct Boss *boss) {
    animation_destroy(boss->anim_moving);
    animation_destroy(boss->anim_tiro);
}