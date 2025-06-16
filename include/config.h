#ifndef CONFIG_H
#define CONFIG_H

typedef enum {
    MENU,
    JOGANDO,
    FIM_DE_JOGO,
    SAIR
} GameState;

// --- GERAL ---
#define LARGURA_TELA 1000
#define ALTURA_TELA 650
#define LARGURA_MUNDO 1156

// --- JOGADOR ---
// A constante de escala agora vive aqui
#define ESCALA 2.0f 
#define ALTURA_JOGADOR (128 * ESCALA)
#define FLOOR_Y ((ALTURA_TELA - ALTURA_JOGADOR) - 20)
#define VELOCIDADE_JOGADOR 5.0
#define FORCA_PULO -13.0
#define GRAVIDADE 0.38
#define FPS_ANIMACAO 10.0

// --- JOGADOR - ANIMAÇÕES ---
#define NUM_FRAMES_PARADO 7
#define NUM_FRAMES_CORRENDO 12
#define NUM_FRAMES_PULANDO 10
#define NUM_FRAMES_ATIRANDO 4
#define NUM_FRAMES_AGACHADO 1
#define NUM_FRAMES_AGACHADO_ATIRANDO 4

#define NUM_FRAMES_INIMIGO_SOLDADO 6

// Hitbox para os estados normais (Parado, Correndo, Pulando)
#define HITBOX_NORMAL_LARGURA 10
#define HITBOX_NORMAL_ALTURA 65
#define HITBOX_NORMAL_OFFSET_X 55 
#define HITBOX_NORMAL_OFFSET_Y 60 

// Hitbox para o estado AGACHADO (menor e mais baixa)
#define HITBOX_AGACHADO_LARGURA 10
#define HITBOX_AGACHADO_ALTURA 50
#define HITBOX_AGACHADO_OFFSET_X 55
#define HITBOX_AGACHADO_OFFSET_Y 80
// ---------------------------------

// --- PROJÉTEIS ---
#define MAX_BULLETS 20
#define VELOCIDADE_PROJETIL 10.0f
#define LARGURA_BULLET 24
#define ALTURA_BULLET 10

#define OFFSET_TIRO_PARADO_X 95
#define OFFSET_TIRO_PARADO_Y 75

#define OFFSET_TIRO_CORRENDO_X 90
#define OFFSET_TIRO_CORRENDO_Y 75

#define OFFSET_TIRO_ATIRANDO_X 98
#define OFFSET_TIRO_ATIRANDO_Y 75

#define OFFSET_TIRO_PULANDO_X 100
#define OFFSET_TIRO_PULANDO_Y 75

#define OFFSET_TIRO_AGACHADO_X 90
#define OFFSET_TIRO_AGACHADO_Y 90

#define MAX_INIMIGOS 10

// --- ADICIONE ESTE BLOCO PARA A HITBOX DO INIMIGO ---
#define HITBOX_INIMIGO_LARGURA 10  // Largura da caixa (chute inicial)
#define HITBOX_INIMIGO_ALTURA 70 // Altura da caixa (chute inicial)
#define HITBOX_INIMIGO_OFFSET_X 75 // (128 - 45) / 2 
#define HITBOX_INIMIGO_OFFSET_Y 60 // (128 - 110)
// ----------------------------------------------------

#endif
