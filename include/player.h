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

typedef struct {
    float x, y, vel_x, vel_y;
    int hp;
    int direcao;
    EstadoJogador estado;

    Animation *anim_parado;
    Animation *anim_correndo;
    Animation *anim_pulando;
    Animation *anim_atirando;
    Animation *anim_agachado;
    Animation *anim_agachado_atirando;
    Animation *anim_atual;
    
    // Ponteiros para TODAS as folhas de sprite
    ALLEGRO_BITMAP *folha_sprite_parado;
    ALLEGRO_BITMAP *folha_sprite_correndo;
    ALLEGRO_BITMAP *folha_sprite_atirando;
    ALLEGRO_BITMAP *folha_sprite_pulando;
    ALLEGRO_BITMAP *folha_sprite_agachado;
    ALLEGRO_BITMAP *folha_sprite_agachado_atirando;

    int frame_largura, frame_altura;

    Controls *controles;
    float cooldown_tiro;
    float tempo_estado_tiro;
    bool no_chao;

    // Hitbox Ativa (as dimensões atuais da colisão)
    float hitbox_largura;
    float hitbox_altura;
    float hitbox_offset_x;
    float hitbox_offset_y;
} Player;

// Protótipos
Player* player_create(float x, float y);
void player_destroy(Player *p);
void player_update(Player *p);
void player_draw(Player *p, float camera_x, float camera_y);
void player_fire(Player *p, struct Bullet bullets[], int max_bullets);

#endif