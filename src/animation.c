#include "animation.h"
#include <stdlib.h>

Animation* animation_create(int num_frames, float fps) {
    Animation *anim = (Animation*) calloc(1, sizeof(Animation));
    if (!anim) return NULL;

    anim->num_frames = num_frames;
    anim->fps = fps;
    return anim;
}

void animation_destroy(Animation *anim) {
    free(anim);
}

// Reseta a animação para o começo
void animation_reset(Animation *anim) {
    if (anim) {
        anim->frame_atual = 0;
        anim->tempo_frame = 0;
    }
}

// O coração do TAD: atualiza o frame baseado na passagem do tempo
void animation_update(Animation *anim) {
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
