#include "item.h"
#include "config.h"
#include <stdio.h>
#include <allegro5/allegro_primitives.h>

void item_init(Item itens[], int max_itens) {
    for (int i = 0; i < max_itens; i++) {
        itens[i].ativo = false;
    }
}

void item_spawn(Item itens[], int max_itens, ItemType tipo, ALLEGRO_BITMAP *sprite, float x, float y) {
    for (int i = 0; i < max_itens; i++) {
        if (!itens[i].ativo) {
            Item *item = &itens[i];
            item->ativo = true;
            item->x = x;
            item->y = y;
            item->tipo = tipo;
            item->sprite = sprite;
            break;
        }
    }
}

void item_draw(Item itens[], int max_itens, float camera_x, float camera_y) {
    for (int i = 0; i < max_itens; i++) {
        if (itens[i].ativo) {
            Item *item = &itens[i];
            
            // Usa as constantes de TAMANHO VISUAL para o desenho
            al_draw_scaled_bitmap(item->sprite, 0, 0, al_get_bitmap_width(item->sprite), al_get_bitmap_height(item->sprite), item->x - camera_x, item->y - camera_y, LARGURA_VISUAL_ITEM, ALTURA_VISUAL_ITEM, 0);
        }
    }
}