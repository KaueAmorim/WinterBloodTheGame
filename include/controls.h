#ifndef CONTROLS_H
#define CONTROLS_H

#include <allegro5/allegro5.h>

struct Controls {
    unsigned int cima;
    unsigned int baixo;
    unsigned int esquerda;
    unsigned int direita;
    unsigned int tiro;
};

// Cria uma nova estrutura de controles.
struct Controls* controls_create();

// Libera a memória alocada para os controles.
void controls_destroy(struct Controls *c);

// Processa os eventos de entrada do teclado e atualiza os estados de controle.
void controls_handle_input(struct Controls *c, ALLEGRO_EVENT *evento);

// Reseta todos os estados de controle para 'false'.
void controls_reset(struct Controls *c);

#endif
