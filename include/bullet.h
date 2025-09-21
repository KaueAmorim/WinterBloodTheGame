#ifndef BULLET_H
#define BULLET_H

#include <allegro5/allegro5.h>
#include "config.h" 

// Enum para identificar o "dono" do projétil.
enum BulletOwner {
    PLAYER,
    ENEMY
};

struct Bullet {
    float x, y;
    float vel_x;
    unsigned int ativo;
    int direcao;
    enum BulletOwner owner;
};

// Ativa e configura um projétil para ser disparado.
void bullet_fire(struct Bullet *b, float start_x, float start_y, int direction, float speed, enum BulletOwner owner);

// Atualiza a posição do projétil e verifica se ele saiu da tela.
void bullet_update(struct Bullet *b, float camera_x);

// Desenha um único projétil na tela, se ele estiver ativo.
void bullet_draw(struct Bullet *b, ALLEGRO_BITMAP *sprite, float camera_x, float camera_y);

#endif