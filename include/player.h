#ifndef PLAYER_H
#define PLAYER_H

#include <allegro5/allegro5.h>
#include <stdbool.h>
#include <stdio.h>
#include "config.h"
#include "bullet.h"


typedef enum { PARADO, CORRENDO, ATIRANDO, PULANDO, AGACHADO, AGACHADO_ATIRANDO } EstadoJogador;

typedef struct {
    float x, y, vel_x, vel_y;
    int hp;
    int direcao;
    EstadoJogador estado;
    int frame_atual;
    double tempo_frame;

    // Ponteiros para TODAS as folhas de sprite
    ALLEGRO_BITMAP *folha_sprite_parado;
    ALLEGRO_BITMAP *folha_sprite_correndo;
    ALLEGRO_BITMAP *folha_sprite_atirando;
    ALLEGRO_BITMAP *folha_sprite_pulando;
    ALLEGRO_BITMAP *folha_sprite_agachado;
    ALLEGRO_BITMAP *folha_sprite_agachado_atirando;

    int frame_largura, frame_altura;

    // Contagem de frames para TODAS as animações
    int num_frames_parado, num_frames_correndo, num_frames_atirando, num_frames_pulando, num_frames_agachado, num_frames_agachado_atirando;
    
    // Timer para controlar a duração da animação de tiro
    float tempo_estado_tiro;

    bool tecla_cima, tecla_esquerda, tecla_direita, tecla_tiro, tecla_baixo;
    float cooldown_tiro;
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
void player_handle_input(Player *p, ALLEGRO_EVENT *evento);
void player_fire(Player *p, struct Bullet bullets[], int max_bullets);

#endif