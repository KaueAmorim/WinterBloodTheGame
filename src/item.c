#include "item.h"
#include "config.h"
#include <stdio.h>

void item_init(struct Item itens[], int max_itens) {
    for (int i = 0; i < max_itens; i++) {
        itens[i].ativo = 0;
        itens[i].x = 0;
        itens[i].y = 0;
        itens[i].tipo = VODKA;
        itens[i].sprite = NULL;
    }
}

void item_spawn(struct Item itens[], int max_itens, enum ItemType tipo, ALLEGRO_BITMAP *sprite, float x, float y) {
    for (int i = 0; i < max_itens; i++) {
        if (!itens[i].ativo) {
            struct Item *item = &itens[i];
            item->ativo = 1;
            item->x = x;
            item->y = y;
            item->tipo = tipo;
            item->sprite = sprite;
            break;
        }
    }
}

void item_draw(struct Item itens[], int max_itens, float camera_x, float camera_y) {
    for (int i = 0; i < max_itens; i++) {
        if (itens[i].ativo) {
            struct Item *item = &itens[i];
            al_draw_scaled_bitmap(item->sprite, 0, 0, al_get_bitmap_width(item->sprite), al_get_bitmap_height(item->sprite), item->x - camera_x, item->y - camera_y, 96, 96, 0);
        }
    }
}