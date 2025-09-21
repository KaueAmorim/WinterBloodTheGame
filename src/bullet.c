#include "bullet.h"

void bullet_fire(struct Bullet *b, float start_x, float start_y, int direction, float speed, enum BulletOwner owner) {
    b->x = start_x;
    b->y = start_y;
    b->vel_x = speed * direction;
    b->ativo = 1;
    b->direcao = direction;
    b->owner = owner;
}

void bullet_update(struct Bullet *b, float camera_x) {
    if (b->ativo) {
        
        // Física do Movimento
        b->x += b->vel_x;
        
        // Lógica de "Limpeza"
        float margem = 100.0f; 

        // Verificação de Limites
        if (b->x < camera_x - margem || b->x > camera_x + LARGURA_TELA + margem) {
            b->ativo = 0;
        }
    }
}

void bullet_draw(struct Bullet *b, ALLEGRO_BITMAP *sprite, float camera_x, float camera_y) {
    if (b->ativo) {
        
        // Lógica de Inversão (Espelhamento)
        int flags = (b->direcao == -1) ? ALLEGRO_FLIP_HORIZONTAL : 0;

        al_draw_bitmap(sprite, b->x - camera_x, b->y - camera_y, flags);
    }
}