#include "bullet.h"
#include "config.h" 

// Ao atirar, damos ao projétil um tempo de vida
void bullet_fire(Bullet *b, float start_x, float start_y, int direction, float speed, BulletOwner owner) {
    b->ativo = true;
    b->x = start_x;
    b->y = start_y;
    b->vel_x = speed * direction;
    b->direcao = direction;
    b->owner = owner;
}

// A função agora recebe camera_x para saber onde está a tela
void bullet_update(Bullet *b, float camera_x) {
    if (b->ativo) {
        // 1. Move o projétil normalmente
        b->x += b->vel_x;
        
        // 2. Verifica se a posição do projétil está fora da tela
        // Adicionamos uma margem de segurança para garantir que ele já sumiu
        float margem = 100.0f; 

        if (b->x < camera_x - margem || b->x > camera_x + LARGURA_TELA + margem) {
            b->ativo = false; // Desativa o projétil
        }
    }
}
void bullet_draw(Bullet *b, ALLEGRO_BITMAP *sprite, float camera_x, float camera_y) {
    if (b->ativo) {
        // Verifica a direção e define a flag de inversão se necessário
        int flags = (b->direcao == -1) ? ALLEGRO_FLIP_HORIZONTAL : 0;

        // Passa a flag para a função de desenho
        al_draw_bitmap(sprite, b->x - camera_x, b->y - camera_y, flags);
    }
}