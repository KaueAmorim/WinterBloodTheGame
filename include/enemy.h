#ifndef ENEMY_H
#define ENEMY_H

#include <allegro5/allegro5.h>
#include <stdbool.h>
#include <math.h>
#include "player.h"


// Um enum para os tipos de inimigos, para o futuro
typedef enum {
    SOLDADO_NORMAL
} EnemyType;

typedef struct {
    float x, y;
    int hp;
    bool ativo;
    EnemyType tipo;

    // Animação
    ALLEGRO_BITMAP *folha_sprite;
    int frame_largura, frame_altura;
    int num_frames, frame_atual;
    double tempo_frame;
    int direcao;
    float cooldown_tiro;

} Enemy;

// Protótipos das nossas funções
void enemy_init(Enemy inimigos[], int max_inimigos);
void enemy_spawn(Enemy inimigos[], int max_inimigos, ALLEGRO_BITMAP *sprite, float x, float y);
void enemy_update(Enemy *e, Player *p, Bullet bullets[], int max_bullets);
void enemy_draw(Enemy inimigos[], int max_inimigos, float camera_x);

#endif
