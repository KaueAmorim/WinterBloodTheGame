#ifndef BOSS_H
#define BOSS_H

#include <allegro5/allegro5.h>
#include "animation.h"
#include "player.h"
#include "bullet.h"
#include "config.h"
#include <math.h>

// Enum para os estados do chefe. 
enum BossState{
    BOSS_MOVING
};

struct Boss {
    float x, y;
    float vel_x, vel_y;
    int hp;
    int direcao;
    enum BossState estado;
    unsigned int ativo;

    ALLEGRO_BITMAP *sprite_move;
    ALLEGRO_BITMAP *sprite_tiro;

    struct Animation *anim_atual;
    struct Animation *anim_moving;
    struct Animation *anim_tiro;

    int frame_largura, frame_altura;
    float cooldown_tiro;
    unsigned int tiro_ativo;   // Flag que indica se a animação do efeito de tiro está tocando.

    int hitbox_largura;
    int hitbox_altura;
    int hitbox_offset_x;
    int hitbox_offset_y;
};

// Inicializa a estrutura do chefe.
void boss_init(struct Boss *boss);

// Cria o chefe na posição especificada.
void boss_spawn(struct Boss *boss, float x, float y, ALLEGRO_BITMAP *sprite, ALLEGRO_BITMAP *fx_sprite);

// Atualiza a lógica do chefe (IA de movimento e tiro) a cada quadro.
void boss_update(struct Boss *boss, struct Player *p, struct Bullet bullets[], int max_bullets);

// Desenha o chefe e seu efeito de tiro na tela.
void boss_draw(struct Boss *boss, float camera_x, float camera_y);

// Libera a memória de todos os objetos de animação do chefe.
void boss_destroy_animations(struct Boss *boss);

#endif