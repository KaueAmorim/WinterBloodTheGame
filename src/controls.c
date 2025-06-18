#include "controls.h"
#include <stdlib.h>

Controls* controls_create() {
    // Usamos calloc para garantir que todos os bools começam como 'false' (0)
    Controls *c = (Controls*) calloc(1, sizeof(Controls));
    return c;
}

void controls_destroy(Controls *c) {
    free(c);
}

// Esta função contém a lógica que ANTES estava em player_handle_input
void controls_handle_input(Controls *c, ALLEGRO_EVENT *evento) {
    if (evento->type == ALLEGRO_EVENT_KEY_DOWN) {
        switch (evento->keyboard.keycode) {
            case ALLEGRO_KEY_UP:    c->cima = true; break;
            case ALLEGRO_KEY_LEFT:  c->esquerda = true; break;
            case ALLEGRO_KEY_RIGHT: c->direita = true; break;
            case ALLEGRO_KEY_SPACE: c->tiro = true; break;
            case ALLEGRO_KEY_DOWN:  c->baixo = true; break;
        }
    } 
    else if (evento->type == ALLEGRO_EVENT_KEY_UP) {
        switch (evento->keyboard.keycode) {
            case ALLEGRO_KEY_UP:    c->cima = false; break;
            case ALLEGRO_KEY_LEFT:  c->esquerda = false; break;
            case ALLEGRO_KEY_RIGHT: c->direita = false; break;
            case ALLEGRO_KEY_SPACE: c->tiro = false; break;
            case ALLEGRO_KEY_DOWN:  c->baixo = false; break;
        }
    }
}

// Reseta todos os estados de tecla para 'false'
void controls_reset(Controls *c) {
    if (!c) return;

    c->cima = false;
    c->baixo = false;
    c->esquerda = false;
    c->direita = false;
    c->tiro = false;
}