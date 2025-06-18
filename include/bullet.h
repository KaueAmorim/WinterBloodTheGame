#ifndef BULLET_H
#define BULLET_H

#include <allegro5/allegro5.h>
#include <stdbool.h>

// Enum para identificar o dono do projétil
typedef enum {
    OWNER_PLAYER,
    OWNER_ENEMY
} BulletOwner;

typedef struct Bullet {
    float x, y;
    float vel_x;
    bool ativo;
    int direcao;
    BulletOwner owner;
} Bullet;

void bullet_fire(Bullet *b, float start_x, float start_y, int direction, float speed, BulletOwner owner);
void bullet_update(Bullet *b, float camera_x);
void bullet_draw(Bullet *b, ALLEGRO_BITMAP *sprite, float camera_x, float camera_y);

#endif