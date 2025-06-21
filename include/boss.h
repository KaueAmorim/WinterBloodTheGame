#ifndef BOSS_H
#define BOSS_H

#include <allegro5/allegro5.h>
#include <stdbool.h>
#include "animation.h"
#include "player.h"
#include "bullet.h"

typedef enum {
    BOSS_MOVING // The boss will primarily be in a moving state
} BossState;

typedef struct {
    bool ativo;
    float x, y;
    float vel_x, vel_y;
    int hp;
    int direcao;
    BossState estado;

    ALLEGRO_BITMAP *sprite_sheet;
    ALLEGRO_BITMAP *sprite_fx_tiro;

    Animation *anim_moving;
    Animation *anim_atual;
    Animation *anim_fx_tiro;

    int frame_largura, frame_altura;
    float cooldown_tiro;

    // Hitbox
    float hitbox_largura, hitbox_altura, hitbox_offset_x, hitbox_offset_y;
    bool fx_tiro_ativo;
} Boss;

void boss_init(Boss *boss);
void boss_spawn(Boss *boss, float x, ALLEGRO_BITMAP *sprite, ALLEGRO_BITMAP *fx_sprite);
void boss_update(Boss *boss, Player *p, Bullet bullets[], int max_bullets);
void boss_draw(Boss *boss, float camera_x, float camera_y);
void boss_destroy(Boss *boss);

#endif