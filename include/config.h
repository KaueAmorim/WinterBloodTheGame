#ifndef CONFIG_H 
#define CONFIG_H

// Enum que controla o fluxo principal do jogo (ex: em qual tela estamos).
enum GameState{ 
    MENU, 
    JOGANDO, 
    FIM_DE_JOGO, 
    SAIR 
};

// --- CONFIGURAÇÕES GERAIS DA TELA E DO MUNDO ---
#define LARGURA_TELA 1000       // Largura da janela do jogo em pixels.
#define ALTURA_TELA 650         // Altura da janela do jogo em pixels.
#define LARGURA_MUNDO 1156      // Largura total do cenário da fase.

// --- CONFIGURAÇÕES DO JOGADOR ---
#define ESCALA 2.0f                 // Fator de escala para os sprites. 2.0f = dobro do tamanho.
#define ALTURA_JOGADOR_VISUAL (128 * ESCALA) // Altura visual final do jogador para cálculos de física.
#define FLOOR_Y ((ALTURA_TELA - ALTURA_JOGADOR_VISUAL) - 20) // Coordenada Y do "chão", com uma margem de 20px.

// Física do Jogador
#define VELOCIDADE_JOGADOR 5.0f     // Velocidade de movimento horizontal.
#define FORCA_PULO -13.0f           // Força inicial do pulo (valor negativo para ir para cima).
#define GRAVIDADE 0.38f             // Aceleração constante para baixo que afeta o pulo.
#define FPS_ANIMACAO 10.0f          // Velocidade padrão das animações (em frames por segundo).

// Animações do Jogador (Contagem de Frames)
#define NUM_FRAMES_PARADO 7
#define NUM_FRAMES_CORRENDO 12
#define NUM_FRAMES_PULANDO 10
#define NUM_FRAMES_ATIRANDO 4
#define NUM_FRAMES_AGACHADO 1
#define NUM_FRAMES_AGACHADO_ATIRANDO 4

// Hitbox do Jogador (Caixa de Colisão)
// Hitbox para os estados normais (em pé, correndo, pulando)
#define HITBOX_NORMAL_LARGURA 10
#define HITBOX_NORMAL_ALTURA 65
#define HITBOX_NORMAL_OFFSET_X 55
#define HITBOX_NORMAL_OFFSET_Y 65
// Hitbox para o estado agachado
#define HITBOX_AGACHADO_LARGURA 10
#define HITBOX_AGACHADO_ALTURA 50
#define HITBOX_AGACHADO_OFFSET_X 55
#define HITBOX_AGACHADO_OFFSET_Y 80

// --- CONFIGURAÇÕES DOS PROJÉTEIS ---
#define MAX_BULLETS 20                    // Número máximo de projéteis na tela ao mesmo tempo (object pool).
#define VELOCIDADE_PROJETIL 12.0f         // Velocidade padrão dos projéteis.
#define VELOCIDADE_PROJETIL_RAPIDO 18.0f  // Velocidade especial para o tiro rápido do inimigo.
#define LARGURA_BULLET 24                 // Largura da hitbox do projétil.
#define ALTURA_BULLET 10                  // Altura da hitbox do projétil.

// Offsets de Tiro do Jogador (de onde o tiro sai)
#define OFFSET_TIRO_PARADO_X 95
#define OFFSET_TIRO_PARADO_Y 75
#define OFFSET_TIRO_CORRENDO_X 90
#define OFFSET_TIRO_CORRENDO_Y 75
#define OFFSET_TIRO_PULANDO_X 100
#define OFFSET_TIRO_PULANDO_Y 75
#define OFFSET_TIRO_AGACHADO_X 90
#define OFFSET_TIRO_AGACHADO_Y 90

// --- CONFIGURAÇÕES DOS INIMIGOS ---
#define MAX_INIMIGOS 10              // Número máximo de inimigos no array.

// Hitbox do Inimigo Padrão
#define HITBOX_INIMIGO_LARGURA 10
#define HITBOX_INIMIGO_ALTURA 70
#define HITBOX_INIMIGO_OFFSET_X 75
#define HITBOX_INIMIGO_OFFSET_Y 60

// Offsets de Tiro do Inimigo
#define OFFSET_TIRO_INIMIGO_X 90
#define OFFSET_TIRO_INIMIGO_Y 70
#define OFFSET_TIRO_DUPLO_Y1 65   // Posição do tiro de cima no ataque duplo.
#define OFFSET_TIRO_DUPLO_Y2 85   // Posição do tiro de baixo no ataque duplo.

// --- CONFIGURAÇÕES DO CHEFE (BOSS) ---
#define NUM_FRAMES_BOSS_MOVING 4
#define NUM_FRAMES_FX_TIRO_CHEFE 7
#define VELOCIDADE_BOSS 2.0f

// Hitbox do Chefe
#define BOSS_HITBOX_LARGURA 120
#define BOSS_HITBOX_ALTURA 150
#define BOSS_HITBOX_OFFSET_X 36
#define BOSS_HITBOX_OFFSET_Y 21

// Offsets de Tiro do Chefe (para o ataque triplo)
#define BOSS_OFFSET_TIRO_X 260
#define BOSS_OFFSET_TIRO_Y1 0
#define BOSS_OFFSET_TIRO_Y2 90
#define BOSS_OFFSET_TIRO_Y3 160 

// --- CONFIGURAÇÕES DOS ITENS ---
#define MAX_ITENS 10

// Tamanho e Posição da HITBOX do item
#define HITBOX_ITEM_LARGURA 60
#define HITBOX_ITEM_ALTURA 48
#define HITBOX_ITEM_OFFSET_X 8
#define HITBOX_ITEM_OFFSET_Y 24

#endif