#ifndef PLAYER_H
#define PLAYER_H

#include <allegro5/allegro5.h>
#include <stdio.h>
#include "config.h"
#include "bullet.h"
#include "controls.h"
#include "animation.h"

// Enum para todos os estados possíveis do jogador.
enum EstadoJogador { 
    PARADO, 
    CORRENDO, 
    ATIRANDO, 
    PULANDO, 
    AGACHADO, 
    AGACHADO_ATIRANDO 
};

struct Player {
    float x, y;
    float vel_x, vel_y;
    int hp;
    int direcao;           // Direção para a qual o jogador está virado (-1 para esquerda, 1 para direita).
    unsigned int no_chao;  // Flag para verificar se o jogador está no chão (para controlar o pulo).

    enum EstadoJogador estado;
    struct Controls *controles;

    // Armazenamento das Animações
    struct Animation *anim_atual;
    struct Animation *anim_parado;
    struct Animation *anim_correndo;
    struct Animation *anim_pulando;
    struct Animation *anim_atirando;
    struct Animation *anim_agachado;
    struct Animation *anim_agachado_atirando;

    // Armazenamento dos Sprites
    ALLEGRO_BITMAP *folha_sprite_parado; 
    ALLEGRO_BITMAP *folha_sprite_correndo;
    ALLEGRO_BITMAP *folha_sprite_atirando;
    ALLEGRO_BITMAP *folha_sprite_pulando;
    ALLEGRO_BITMAP *folha_sprite_agachado; 
    ALLEGRO_BITMAP *folha_sprite_agachado_atirando;

    int frame_largura, frame_altura; // Dimensões de um único frame do sprite sheet.
    float tempo_estado_tiro;         // Cronômetro para controlar a duração da animação de tiro.
    float cooldown_tiro;             // Cronômetro para controlar o intervalo entre os disparos.

    // Guarda as dimensões e a posição relativa da caixa de colisão atual.
    int hitbox_largura;
    int hitbox_altura;
    int hitbox_offset_x;
    int hitbox_offset_y;
};

// Cria um novo jogador na posição (x, y).
struct Player* player_create(float x, float y);

// Libera toda a memória alocada para o jogador (bitmaps, animações, controles).
void player_destroy(struct Player *p);

// Atualiza o estado do jogador, processando controles e física.
void player_update(struct Player *p);

// Desenha o jogador na tela, considerando a posição da câmera.
void player_draw(struct Player *p, float camera_x, float camera_y);

// Verifica se o jogador deve atirar e, se sim, "spawna" um projétil.
void player_fire(struct Player *p, struct Bullet bullets[], int max_bullets);

#endif