#include "animation.h"
#include <stdlib.h>

struct Animation* animation_create(int num_frames, float fps) {
    
    struct Animation *anim;

    if(!(anim = malloc(sizeof(struct Animation)))){
        return NULL;
    }
    
    anim->frame_atual = 0;
    anim->num_frames = num_frames;
    anim->tempo_frame = 0;
    anim->fps = fps;

    return anim;
}

void animation_destroy(struct Animation *anim) {
    free(anim);
}

// Atualiza o frame baseado na passagem do tempo
void animation_update(struct Animation *anim) {
    if (!anim) return;

    anim->tempo_frame += 1.0 / 60.0; // Assumindo 60FPS no jogo
    if (anim->tempo_frame >= 1.0 / anim->fps) {
        anim->tempo_frame = 0;
        anim->frame_atual++;
        if (anim->frame_atual >= anim->num_frames) {
            anim->frame_atual = 0; // Volta ao início
        }
    }
}

// Reseta a animação para o começo
void animation_reset(struct Animation *anim) {
    if (anim) {
        anim->frame_atual = 0;
        anim->tempo_frame = 0;
    }
}
