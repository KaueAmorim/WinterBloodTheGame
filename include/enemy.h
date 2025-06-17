#ifndef ENEMY_H
#define ENEMY_H

#include <allegro5/allegro5.h>
#include <stdbool.h>
#include <math.h>
#include "player.h"
#include "animation.h"

// Enum para os estados do inimigo
typedef enum {
    INIMIGO_PARADO,
    INIMIGO_ATIRANDO
} EnemyState;

// Um enum para os tipos de inimigos, para o futuro
typedef enum {
    SOLDADO_NORMAL
} EnemyType;

typedef struct {
    float x, y;
    int hp;
    bool ativo;
    EnemyType tipo;
    EnemyState estado;

    // Sprites
    ALLEGRO_BITMAP *folha_sprite_parado;
    ALLEGRO_BITMAP *folha_sprite_atirando;

    // Animações
    Animation *anim_parado;
    Animation *anim_atirando;
    Animation *anim_atual;

    int frame_largura, frame_altura;

    int direcao;
    float cooldown_tiro;
    float tempo_estado_tiro;
} Enemy;

void enemy_init(Enemy inimigos[], int max_inimigos);
void enemy_spawn(Enemy inimigos[], int max_inimigos, ALLEGRO_BITMAP *sprite_parado, ALLEGRO_BITMAP *sprite_atirando, float x, float y);
void enemy_update(Enemy *e, Player *p, Bullet bullets[], int max_bullets);
void enemy_draw(Enemy inimigos[], int max_inimigos, float camera_x, float camera_y);
void enemy_destroy_animations(Enemy inimigos[], int max_inimigos);

#endif
