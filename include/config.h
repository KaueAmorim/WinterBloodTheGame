#ifndef CONFIG_H
#define CONFIG_H

typedef enum { MENU, JOGANDO, FIM_DE_JOGO, SAIR } GameState;

// --- GERAL ---
#define LARGURA_TELA 1000
#define ALTURA_TELA 650
#define LARGURA_MUNDO 1156 // Ou o tamanho do seu cenário
#define LARGURA_CORACAO 50 // Largura desejada para o coração na tela
#define ALTURA_CORACAO 50

// --- ITENS ---
#define MAX_ITENS 10

// Tamanho VISUAL do item (como ele aparece na tela)
#define LARGURA_VISUAL_ITEM (48 * ESCALA) // Um pouco maior na tela
#define ALTURA_VISUAL_ITEM (48 * ESCALA)

// Tamanho e Posição da HITBOX do item (para a colisão)
#define HITBOX_ITEM_LARGURA (30 * ESCALA)
#define HITBOX_ITEM_ALTURA (24 * ESCALA)
#define HITBOX_ITEM_OFFSET_X (4 * ESCALA) 
#define HITBOX_ITEM_OFFSET_Y (12 * ESCALA)
// -------------

// --- JOGADOR ---
#define ESCALA 2.0f 
#define ALTURA_JOGADOR_VISUAL (128 * ESCALA)
#define FLOOR_Y ((ALTURA_TELA - ALTURA_JOGADOR_VISUAL) - 20)
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

// --- JOGADOR - HITBOX ---
#define HITBOX_NORMAL_LARGURA 10
#define HITBOX_NORMAL_ALTURA 65
#define HITBOX_NORMAL_OFFSET_X 55 
#define HITBOX_NORMAL_OFFSET_Y 65
#define HITBOX_AGACHADO_LARGURA 10
#define HITBOX_AGACHADO_ALTURA 50
#define HITBOX_AGACHADO_OFFSET_X 55
#define HITBOX_AGACHADO_OFFSET_Y 80

// --- PROJÉTEIS ---
#define MAX_BULLETS 20
#define VELOCIDADE_PROJETIL 12.0f
#define VELOCIDADE_PROJETIL_RAPIDO 18.0f
#define LARGURA_BULLET 24
#define ALTURA_BULLET 10

#define OFFSET_TIRO_PARADO_X 95
#define OFFSET_TIRO_PARADO_Y 75
#define OFFSET_TIRO_CORRENDO_X 90
#define OFFSET_TIRO_CORRENDO_Y 75
#define OFFSET_TIRO_PULANDO_X 100
#define OFFSET_TIRO_PULANDO_Y 75
#define OFFSET_TIRO_AGACHADO_X 90
#define OFFSET_TIRO_AGACHADO_Y 90

// --- INIMIGOS ---
#define MAX_INIMIGOS 7
#define NUM_FRAMES_INIMIGO_SOLDADO 6
#define NUM_FRAMES_INIMIGO_ATIRANDO 7
#define NUM_FRAMES_INIMIGO_MORRENDO 6

#define HITBOX_INIMIGO_LARGURA 10
#define HITBOX_INIMIGO_ALTURA 70
#define HITBOX_INIMIGO_OFFSET_X 75
#define HITBOX_INIMIGO_OFFSET_Y 60

#define OFFSET_TIRO_INIMIGO_X 90
#define OFFSET_TIRO_INIMIGO_Y 70
#define OFFSET_TIRO_DUPLO_Y1 65 // Posição do tiro de cima
#define OFFSET_TIRO_DUPLO_Y2 85 // Posição do tiro de baixo

// --- CHEFE (BOSS) ---
#define NUM_FRAMES_BOSS_MOVING 4 // Based on your image
#define VELOCIDADE_BOSS 2.0f // Adjust as needed
#define BOSS_HITBOX_LARGURA (120 * ESCALA) // Adjust as needed
#define BOSS_HITBOX_ALTURA (150 * ESCALA) // Adjust as needed
#define BOSS_HITBOX_OFFSET_X (36 * ESCALA) // Adjust based on sprite
#define BOSS_HITBOX_OFFSET_Y (21 * ESCALA) // Adjust based on sprite
#define BOSS_OFFSET_TIRO_X 140 // Adjust based on your sprite
#define BOSS_OFFSET_TIRO_Y1 60  // Adjust based on your sprite
#define BOSS_OFFSET_TIRO_Y2 90  // Adjust based on your sprite
#define BOSS_OFFSET_TIRO_Y3 120 // Adjust based on your sprite

#endif