#ifndef ITEM_H
#define ITEM_H

#include <allegro5/allegro5.h>

enum ItemType {
    VODKA
};

struct Item {
    unsigned int ativo;
    float x, y;
    enum ItemType tipo;
    ALLEGRO_BITMAP *sprite;
};

// Inicializa o array de itens, marcando todos como inativos.
void item_init(struct Item itens[], int max_itens);

// Cria (spawna) um novo item em uma posição específica do mundo.
void item_spawn(struct Item itens[], int max_itens, enum ItemType tipo, ALLEGRO_BITMAP *sprite, float x, float y);

// Desenha todos os itens ativos na tela.
void item_draw(struct Item itens[], int max_itens, float camera_x, float camera_y);

#endif