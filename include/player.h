#ifndef PLAYER_H
#define PLAYER_H

#include <allegro5/allegro5.h>
#include <stdbool.h>
#include <stdio.h>
#include "config.h"
#include "bullet.h"
#include "controls.h"
#include "animation.h"

typedef enum { PARADO, CORRENDO, ATIRANDO, PULANDO, AGACHADO, AGACHADO_ATIRANDO } EstadoJogador;

struct Player {
    float x, y, vel_x, vel_y;
    int hp;
    int direcao;
    EstadoJogador estado;
    bool no_chao;

    // TADs para Lógica
    struct Controls *controles;
    struct Animation *anim_atual;

    // Armazenamento das Animações
    struct Animation *anim_parado, *anim_correndo, *anim_pulando, *anim_atirando, *anim_agachado, *anim_agachado_atirando;

    // Armazenamento dos Sprites
    ALLEGRO_BITMAP *folha_sprite_parado, *folha_sprite_correndo, *folha_sprite_atirando, *folha_sprite_pulando, *folha_sprite_agachado, *folha_sprite_agachado_atirando;

    int frame_largura, frame_altura;
    float tempo_estado_tiro;
    float cooldown_tiro;

    // Hitbox Ativa
    float hitbox_largura, hitbox_altura, hitbox_offset_x, hitbox_offset_y;
};

struct Player* player_create(float x, float y);
void player_destroy(struct Player *p);
void player_update(struct Player *p);
void player_draw(struct Player *p, float camera_x, float camera_y);
void player_fire(struct Player *p, struct Bullet bullets[], int max_bullets);

#endif