#ifndef ENEMY_H
#define ENEMY_H

#include <allegro5/allegro5.h>
#include <stdbool.h>
#include <math.h>
#include "player.h"
#include "animation.h"
// Um enum para os tipos de inimigos, para o futuro
typedef enum {
    SOLDADO_ESPINGARDA,
    SOLDADO_ESCUDO
} EnemyType;

// --- A "RECEITA" PARA CADA TIPO DE INIMIGO ---
typedef struct {
    EnemyType tipo;
    int hp;
    int num_frames_parado;
    int num_frames_atirando;
    int num_frames_morrendo;
    ALLEGRO_BITMAP *folha_sprite_parado;
    ALLEGRO_BITMAP *folha_sprite_atirando;
    ALLEGRO_BITMAP *folha_sprite_morrendo;
} EnemyConfig;

// Enum para os estados do inimigo
typedef enum {
    INIMIGO_PARADO,
    INIMIGO_ATIRANDO,
    INIMIGO_MORRENDO
} EnemyState;


typedef struct {
    float x, y;
    int hp;
    bool ativo;
    EnemyType tipo;
    EnemyState estado;

    // Sprites
    ALLEGRO_BITMAP *folha_sprite_parado;
    ALLEGRO_BITMAP *folha_sprite_atirando;
    ALLEGRO_BITMAP *folha_sprite_morrendo;

    // Animações
    struct Animation *anim_parado;
    struct Animation *anim_atirando;
    struct Animation *anim_morrendo;
    struct Animation *anim_atual;

    int frame_largura, frame_altura;

    int direcao;
    float cooldown_tiro;
    float tempo_estado_tiro;
} Enemy;

void enemy_init(Enemy inimigos[], int max_inimigos);
void enemy_spawn(Enemy inimigos[], int max_inimigos, const EnemyConfig *config, float x, float y);
void enemy_update(Enemy *e, struct Player *p, struct Bullet bullets[], int max_bullets);
void enemy_draw(Enemy inimigos[], int max_inimigos, float camera_x, float camera_y);
void enemy_destroy_animations(Enemy inimigos[], int max_inimigos);

#endif
