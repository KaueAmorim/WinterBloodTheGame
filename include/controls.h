#ifndef CONTROLS_H
#define CONTROLS_H

#include <allegro5/allegro5.h>
#include <stdbool.h>

typedef struct {
    bool cima;
    bool baixo;
    bool esquerda;
    bool direita;
    bool tiro;
} Controls;

Controls* controls_create();
void controls_destroy(Controls *c);
void controls_handle_input(Controls *c, ALLEGRO_EVENT *evento);

#endif
