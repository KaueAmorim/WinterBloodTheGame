#include "controls.h"

struct Controls* controls_create() {

    struct Controls *c;

    if(!(c = malloc(sizeof(struct Controls)))){
        return NULL;
    }

    c->cima = 0;
    c->baixo = 0;
    c->esquerda = 0;
    c->direita = 0;
    c->tiro = 0;

    return c;
}

void controls_destroy(struct Controls *c) {
    free(c);
}

void controls_handle_input(struct Controls *c, ALLEGRO_EVENT *evento) {
    if (evento->type == ALLEGRO_EVENT_KEY_DOWN) {
        switch (evento->keyboard.keycode) {
            case ALLEGRO_KEY_UP:    c->cima = 1; break;
            case ALLEGRO_KEY_LEFT:  c->esquerda = 1; break;
            case ALLEGRO_KEY_RIGHT: c->direita = 1; break;
            case ALLEGRO_KEY_DOWN:  c->baixo = 1; break;
            case ALLEGRO_KEY_SPACE: c->tiro = 1; break;
        }
    } 
    else if (evento->type == ALLEGRO_EVENT_KEY_UP) {
        switch (evento->keyboard.keycode) {
            case ALLEGRO_KEY_UP:    c->cima = 0; break;
            case ALLEGRO_KEY_LEFT:  c->esquerda = 0; break;
            case ALLEGRO_KEY_RIGHT: c->direita = 0; break;
            case ALLEGRO_KEY_DOWN:  c->baixo = 0; break;
            case ALLEGRO_KEY_SPACE: c->tiro = 0; break;
        }
    }
}

void controls_reset(struct Controls *c) {
    if (!c) return;

    c->cima = 0;
    c->baixo = 0;
    c->esquerda = 0;
    c->direita = 0;
    c->tiro = 0;
}