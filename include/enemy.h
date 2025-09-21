#ifndef ENEMY_H
#define ENEMY_H

#include <allegro5/allegro5.h>
#include <math.h>
#include "player.h"
#include "animation.h"
#include "config.h"

// Enum para os diferentes tipos de inimigos no jogo.
enum EnemyType {
    SOLDADO_ESPINGARDA,
    SOLDADO_ESCUDO
};

// Estrutura de configuração que funciona como uma "receita" para criar um tipo de inimigo.
struct EnemyConfig {
    enum EnemyType tipo;
    int hp;
    int num_frames_parado;
    int num_frames_atirando;
    int num_frames_morrendo;
    ALLEGRO_BITMAP *folha_sprite_parado;
    ALLEGRO_BITMAP *folha_sprite_atirando;
    ALLEGRO_BITMAP *folha_sprite_morrendo;
};

// Enum para os estados do inimigo
enum EnemyState {
    INIMIGO_PARADO,
    INIMIGO_ATIRANDO,
    INIMIGO_MORRENDO
};

struct Enemy {
    float x, y;
    int hp;
    unsigned int ativo;
    enum EnemyType tipo;
    enum EnemyState estado;

    ALLEGRO_BITMAP *folha_sprite_parado;
    ALLEGRO_BITMAP *folha_sprite_atirando;
    ALLEGRO_BITMAP *folha_sprite_morrendo;

    struct Animation *anim_parado;
    struct Animation *anim_atirando;
    struct Animation *anim_morrendo;
    struct Animation *anim_atual;

    int frame_largura, frame_altura;

    int direcao;
    float cooldown_tiro;
    float tempo_estado_tiro;
};

// Inicializa o array de inimigos, marcando todos como inativos.
void enemy_init(struct Enemy inimigos[], int max_inimigos);

// Cria (spawna) um inimigo em uma posição, usando uma "receita" (config).
void enemy_spawn(struct Enemy inimigos[], int max_inimigos, const struct EnemyConfig *config, float x, float y);

// Atualiza a lógica (IA, estado, animação) de um único inimigo.
void enemy_update(struct Enemy *e, struct Player *p, struct Bullet bullets[], int max_bullets);

// Desenha todos os inimigos ativos na tela.
void enemy_draw(struct Enemy inimigos[], int max_inimigos, float camera_x, float camera_y);

// Libera a memória de todos os objetos de animação de todos os inimigos.
void enemy_destroy_animations(struct Enemy inimigos[], int max_inimigos);

#endif