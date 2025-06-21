#ifndef ANIMATION_H
#define ANIMATION_H

#include <allegro5/allegro5.h>

struct Animation {
    int frame_atual;    // O índice do frame atual a ser desenhado.
    int num_frames;     // O número total de frames que esta animação possui.
    double tempo_frame; // Um acumulador de tempo, funciona como um cronômetro para saber quando trocar de frame.
    float fps;          // A velocidade da animação, em "frames por segundo".
};

// Cria uma nova animação.
struct Animation* animation_create(int num_frames, float fps);

// Libera a memória alocada para a animação.
void animation_destroy(struct Animation *anim);

// Atualiza a animação, incrementando o frame atual baseado no tempo acumulado.
void animation_update(struct Animation *anim);

// Reseta a animação para o primeiro frame e zera o tempo acumulado.
void animation_reset(struct Animation *anim);

#endif
