#include "enemy.h"
#include "config.h"

// Inicializa o array de inimigos, marcando todos como inativos
void enemy_init(Enemy inimigos[], int max_inimigos) {
    for (int i = 0; i < max_inimigos; i++) {
        inimigos[i].ativo = false;
    }
}

// "Spawna" um inimigo em uma posição específica
void enemy_spawn(Enemy inimigos[], int max_inimigos, ALLEGRO_BITMAP *sprite, float x, float y) {
    for (int i = 0; i < max_inimigos; i++) {
        if (!inimigos[i].ativo) {
            Enemy *e = &inimigos[i];
            e->ativo = true;
            e->x = x;
            e->y = y;
            e->hp = 3; // Inimigo morre com 3 tiros
            e->direcao = -1; // Começa olhando para a esquerda
            e->tipo = SOLDADO_NORMAL;
            e->folha_sprite = sprite;
            e->frame_largura = 128;
            e->frame_altura = 128;
            e->num_frames = NUM_FRAMES_INIMIGO_SOLDADO;
            e->frame_atual = 0;
            e->tempo_frame = 0;
            break; // Sai do loop após encontrar um espaço vago
        }
    }
}

void enemy_update(Enemy *e, Player *p, Bullet bullets[], int max_bullets) {
    if (!e->ativo) return;

    // --- IA (Inteligência Artificial) ---
    // Vira o inimigo para olhar na direção do jogador
    if (p->x < e->x) {
        e->direcao = -1;
    } else {
        e->direcao = 1;
    }

    // Lógica de tiro do inimigo
    if (e->cooldown_tiro > 0) {
        e->cooldown_tiro -= 1.0 / 60.0;
    } else {
        // Atira se o jogador estiver a uma distância razoável
        float distancia = fabs(p->x - e->x); // 'fabs' é o valor absoluto para floats
        if (distancia < LARGURA_TELA / 1.5) { // Se o jogador está "quase" na tela
            // Encontra um projétil livre para o inimigo usar
            for (int i = 0; i < max_bullets; i++) {
                if (!bullets[i].ativo) {
                    // Posição da "arma" do inimigo (ajuste em config.h se necessário)
                    float start_x = e->x + (e->direcao == 1 ? e->frame_largura * ESCALA : 0);
                    float start_y = e->y + (e->frame_altura * ESCALA / 2.0);
                    
                    bullet_fire(&bullets[i], start_x, start_y, e->direcao);
                    e->cooldown_tiro = 2.0f; // Inimigo atira a cada 2 segundos
                    break;
                }
            }
        }
    }

    // Animação do inimigo (continua a mesma)
    e->tempo_frame += 1.0 / 60.0;
    if (e->tempo_frame >= 1.0 / (FPS_ANIMACAO / 2.0)) {
        e->tempo_frame = 0;
        e->frame_atual = (e->frame_atual + 1) % e->num_frames;
    }
}

// Desenha todos os inimigos ativos
void enemy_draw(Enemy inimigos[], int max_inimigos, float camera_x) {
    for (int i = 0; i < max_inimigos; i++) {
        if (inimigos[i].ativo) {
            Enemy *e = &inimigos[i];
            float sx = e->frame_atual * e->frame_largura;
            float sy = 0;
            float dw = e->frame_largura * ESCALA;
            float dh = e->frame_altura * ESCALA;
            int flags = (e->direcao == -1) ? ALLEGRO_FLIP_HORIZONTAL : 0;
            
            al_draw_scaled_bitmap(e->folha_sprite, sx, sy, e->frame_largura, e->frame_altura, e->x - camera_x, e->y, dw, dh, flags);
        }
    }
}
