#ifndef BULLET_H
#define BULLET_H

#include <allegro5/allegro5.h>
#include <stdbool.h>

typedef struct Bullet {
    float x, y;
    float vel_x;
    bool ativo;
    int direcao;
} Bullet;

void bullet_fire(Bullet *b, float start_x, float start_y, int direction);
void bullet_update(Bullet *b, float camera_x);
// A função de desenho agora recebe um único sprite, não um array
void bullet_draw(Bullet *b, ALLEGRO_BITMAP *sprite, float camera_x, float camera_y);

#endif