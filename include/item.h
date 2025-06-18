#ifndef ITEM_H
#define ITEM_H

#include <allegro5/allegro5.h>
#include <stdbool.h>

typedef enum {
    VODKA
} ItemType;

typedef struct {
    bool ativo;
    float x, y;
    ItemType tipo;
    ALLEGRO_BITMAP *sprite;
} Item;

void item_init(Item itens[], int max_itens);

void item_spawn(Item itens[], int max_itens, ItemType tipo, ALLEGRO_BITMAP *sprite, float x, float y);
void item_draw(Item itens[], int max_itens, float camera_x, float camera_y);

#endif