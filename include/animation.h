#ifndef ANIMATION_H
#define ANIMATION_H

#include <allegro5/allegro5.h>

typedef struct {
    int frame_atual;
    int num_frames;
    double tempo_frame; // Acumulador de tempo
    float fps;         // Velocidade da animação
} Animation;

Animation* animation_create(int num_frames, float fps);
void animation_destroy(Animation *anim);
void animation_update(Animation *anim);
void animation_reset(Animation *anim);

#endif
