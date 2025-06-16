#include <stdio.h>
#include <stdbool.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#include "config.h" 
#include "player.h"
#include "bullet.h"
#include "enemy.h"

// Função que verifica se dois retângulos (r1 e r2) estão colidindo
bool check_collision(float r1x, float r1y, float r1w, float r1h, float r2x, float r2y, float r2w, float r2h) {
    if (r1x + r1w >= r2x && // Borda direita de r1 >= Borda esquerda de r2
        r1x <= r2x + r2w && // Borda esquerda de r1 <= Borda direita de r2
        r1y + r1h >= r2y && // Borda de baixo de r1 >= Borda de cima de r2
        r1y <= r2y + r2h) { // Borda de cima de r1 <= Borda de baixo de r2
        return true;
    }
    return false;
}

// --- NOVA FUNÇÃO PARA RESETAR O JOGO ---
void resetar_jogo(Player *p, Enemy inimigos[], Bullet bullets[], float *camera_x) {
    printf("Resetando o jogo...\n");

    // Reseta o jogador
    p->hp = 5; // Vida cheia
    p->x = 100;
    p->y = FLOOR_Y;
    p->vel_x = 0;
    p->vel_y = 0;
    p->estado = PARADO;

    // Reseta a câmera
    *camera_x = 0;

    // "Mata" todos os projéteis ativos
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].ativo = false;
    }

    // "Mata" todos os inimigos atuais e os respawna nas posições originais
    enemy_init(inimigos, MAX_INIMIGOS);
    enemy_spawn(inimigos, MAX_INIMIGOS, al_load_bitmap("assets/enemy1.png"), 600, FLOOR_Y);
    enemy_spawn(inimigos, MAX_INIMIGOS, al_load_bitmap("assets/enemy1.png"), 900, FLOOR_Y);
    // Adicione mais spawns aqui para chegar aos 6 inimigos do requisito
}


void inicializar_allegro() {
    al_init();
    al_init_image_addon();
    al_install_keyboard();
    al_install_mouse();
    al_init_font_addon(); // Inicializa o addon de fontes
    al_init_ttf_addon();  // Inicializa o addon para ler arquivos .ttf
    al_init_primitives_addon(); // Inicializa o addon de primitivas
}

int main() {
    inicializar_allegro();

    // --- Variáveis ---
    ALLEGRO_DISPLAY *janela = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_FONT *fonte = NULL;
    ALLEGRO_BITMAP *cenario = NULL;
    ALLEGRO_BITMAP *bullet_sprite = NULL;
    Bullet bullets[MAX_BULLETS];
    Player *jogador = NULL;
    Enemy inimigos[MAX_INIMIGOS];
    ALLEGRO_BITMAP *enemy_sprite = NULL;
    
    float camera_x = 0, camera_y = 0;
    int rodando = 1;

    // --- Variáveis de Estado do Jogo ---
    GameState estado_atual = MENU;
    int opcao_menu_selecionada = 0; // 0 = Iniciar, 1 = Sair

    // --- Inicialização ---
    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    fila_eventos = al_create_event_queue();
    timer = al_create_timer(1.0 / 60.0);
    fonte = al_load_font("assets/PressStart2P-Regular.ttf", 50, 0); 
    cenario = al_load_bitmap("assets/cenario.webp");
    bullet_sprite = al_load_bitmap("assets/bullet.png");
    jogador = player_create(100, FLOOR_Y);
    enemy_sprite = al_load_bitmap("assets/enemy1_idle.png");

    if (!janela || !fila_eventos || !timer || !fonte || !cenario || !bullet_sprite || !jogador || !enemy_sprite) {
        printf("ERRO: Falha em um dos componentes de inicialização.\n");
        return -1;
    }

    for (int i = 0; i < MAX_BULLETS; i++) { bullets[i].ativo = false; }

    enemy_init(inimigos, MAX_INIMIGOS); // <-- ADICIONE - Prepara o array de inimigos
    // Spawna alguns inimigos para teste
    enemy_spawn(inimigos, MAX_INIMIGOS, enemy_sprite, 600, FLOOR_Y);
    enemy_spawn(inimigos, MAX_INIMIGOS, enemy_sprite, 900, FLOOR_Y);

    al_set_window_title(janela, "Run 'n Gun");
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);
    al_register_event_source(fila_eventos, al_get_mouse_event_source());

    al_start_timer(timer);

    // --- Loop Principal com Máquina de Estados ---
    while (rodando) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);

        // --- Eventos Globais ---
        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            estado_atual = SAIR; // Em vez de fechar direto, muda para o estado de sair
        }

        // --- Lógica de Estados ---
        switch (estado_atual) {
            case MENU:
                { // Chaves para criar um escopo local para as variáveis
                    int altura_linha = al_get_font_line_height(fonte);
                    int margem_entre_opcoes = 10; // Espaço extra entre as opções

                    float y_opcao1_inicio = ALTURA_TELA / 2.0f;
                    float y_opcao1_fim = y_opcao1_inicio + altura_linha;

                    float y_opcao2_inicio = y_opcao1_fim + margem_entre_opcoes;
                    float y_opcao2_fim = y_opcao2_inicio + altura_linha;

                    // Lógica de Hover (passar o mouse por cima)
                    if (evento.type == ALLEGRO_EVENT_MOUSE_AXES) {
                        int mouse_y = evento.mouse.y;
                        if (mouse_y >= y_opcao1_inicio && mouse_y < y_opcao1_fim) opcao_menu_selecionada = 0;
                        else if (mouse_y >= y_opcao2_inicio && mouse_y < y_opcao2_fim) opcao_menu_selecionada = 1;
                    }
                    
                    // Lógica de Clique do Mouse
                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && evento.mouse.button == 1) {
                        if (opcao_menu_selecionada == 0) estado_atual = JOGANDO;
                        else if (opcao_menu_selecionada == 1) estado_atual = SAIR;
                    }

                    // Lógica do Teclado
                    if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
                        switch (evento.keyboard.keycode) {
                            case ALLEGRO_KEY_UP:   opcao_menu_selecionada = 0; break;
                            case ALLEGRO_KEY_DOWN: opcao_menu_selecionada = 1; break;
                            case ALLEGRO_KEY_ENTER:
                                if (opcao_menu_selecionada == 0) estado_atual = JOGANDO;
                                else if (opcao_menu_selecionada == 1) estado_atual = SAIR;
                                break;
                        }
                    }
                }
                break; // Fim do case MENU
            case JOGANDO:
                // Passa os eventos de teclado para o jogador apenas se estiver jogando
                player_handle_input(jogador, &evento);
                break; // Fim do case JOGANDO
            case FIM_DE_JOGO:
                if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
                    // Agora a tecla Enter reinicia o jogo
                    if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                        resetar_jogo(jogador, inimigos, bullets, &camera_x);
                        estado_atual = JOGANDO;
                    }
                    // E a tecla ESC vai para o menu
                    if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                        estado_atual = MENU;
                    }
                }
                break;
            case SAIR:
                rodando = 0; // Marca para sair do loop principal
                break;
        }

        // --- Lógica de Atualização e Desenho (baseada em timer) ---
        if (evento.type == ALLEGRO_EVENT_TIMER) {
            
            al_clear_to_color(al_map_rgb(0, 0, 0));

            switch (estado_atual) {
                case MENU:
                    int altura_linha = al_get_font_line_height(fonte);
                    int margem_entre_opcoes = 10;
                        
                    ALLEGRO_COLOR cor_selecionada = al_map_rgb(255, 255, 0); // Amarelo
                    ALLEGRO_COLOR cor_padrao = al_map_rgb(255, 255, 255);   // Branco

                    // Posições Y calculadas dinamicamente
                    float y_titulo = ALTURA_TELA / 2.0f - altura_linha * 2;
                    float y_opcao1 = ALTURA_TELA / 2.0f;
                    float y_opcao2 = y_opcao1 + altura_linha + margem_entre_opcoes;

                    al_draw_text(fonte, cor_padrao, LARGURA_TELA / 2, y_titulo, ALLEGRO_ALIGN_CENTER, "Run 'n Gun");
                        
                    al_draw_text(fonte, (opcao_menu_selecionada == 0 ? cor_selecionada : cor_padrao), LARGURA_TELA / 2, y_opcao1, ALLEGRO_ALIGN_CENTER, "Iniciar Jogo");
                    al_draw_text(fonte, (opcao_menu_selecionada == 1 ? cor_selecionada : cor_padrao), LARGURA_TELA / 2, y_opcao2, ALLEGRO_ALIGN_CENTER, "Sair");
                    break;

                case JOGANDO:
                    
                    player_update(jogador);
                    for (int i = 0; i < MAX_BULLETS; i++) { if (bullets[i].ativo) bullet_update(&bullets[i], camera_x); }
                    for (int i = 0; i < MAX_INIMIGOS; i++) { 
                        if (inimigos[i].ativo) enemy_update(&inimigos[i], jogador, bullets, MAX_BULLETS);
                    }
                    player_fire(jogador, bullets, MAX_BULLETS);

                    // --- LÓGICA DE COLISÃO DO JOGADOR ---
                    for (int i = 0; i < MAX_BULLETS; i++) {
                        if (!bullets[i].ativo) continue;

                        // Caixa de colisão do projétil (continua a mesma)
                        float bullet_x = bullets[i].x, bullet_y = bullets[i].y;
                        float bullet_w = LARGURA_BULLET, bullet_h = ALTURA_BULLET;

                        // --- LÓGICA DA HITBOX DO JOGADOR ATUALIZADA ---
                        float player_w = jogador->hitbox_largura * ESCALA;
                        float player_h = jogador->hitbox_altura * ESCALA;
                        float player_x = jogador->x + (jogador->hitbox_offset_x * ESCALA);
                        float player_y = jogador->y + (jogador->hitbox_offset_y * ESCALA);
                        // -------------------------------------------------

                        // A verificação de colisão agora usa a hitbox correta para o estado atual
                        if (bullets[i].vel_x * jogador->direcao < 0 && 
                            check_collision(bullet_x, bullet_y, bullet_w, bullet_h, player_x, player_y, player_w, player_h)) {
                            
                            bullets[i].ativo = false;
                            jogador->hp--;
                        }
                    }

                    // Verifica se o jogador "morreu"
                    if (jogador->hp <= 0) {
                        estado_atual = FIM_DE_JOGO;
                    }
    
                    // --- NOVA LÓGICA DE COMBATE E COLISÃO ---
                    for (int i = 0; i < MAX_BULLETS; i++) {
                        if (!bullets[i].ativo) continue; // Pula projéteis inativos
        
                        for (int j = 0; j < MAX_INIMIGOS; j++) {
                            if (!inimigos[j].ativo) continue; // Pula inimigos inativos
        
                            // Define as "caixas de colisão" para o projétil e o inimigo
                            float bullet_x = bullets[i].x, bullet_y = bullets[i].y;
                            float bullet_w = LARGURA_BULLET, bullet_h = ALTURA_BULLET;
        
                            // --- LÓGICA DA HITBOX DO INIMIGO ATUALIZADA ---
                            float enemy_w = HITBOX_INIMIGO_LARGURA * ESCALA;
                            float enemy_h = HITBOX_INIMIGO_ALTURA * ESCALA;
                            float enemy_x = inimigos[j].x + (HITBOX_INIMIGO_OFFSET_X * ESCALA);
                            float enemy_y = inimigos[j].y + (HITBOX_INIMIGO_OFFSET_Y * ESCALA);
                            // ---------------------------------------------

        
                            // 1. Verifica se houve colisão
                            if (check_collision(bullet_x, bullet_y, bullet_w, bullet_h, enemy_x, enemy_y, enemy_w, enemy_h)) {
                                
                                // 2. Se colidiu, aplica as consequências
                                bullets[i].ativo = false; // O projétil desaparece
                                inimigos[j].hp--;         // O inimigo perde 1 ponto de vida
        
                                // 3. Verifica se o inimigo "morreu"
                                if (inimigos[j].hp <= 0) {
                                    inimigos[j].ativo = false; // Desativa o inimigo
                                }
                                
                                // O projétil já acertou um alvo, então podemos parar de verificá-lo
                                break; 
                            }
                        }
                    }

                    camera_x = jogador->x - LARGURA_TELA / 2.0;
                    if (camera_x < 0) camera_x = 0;

                    int offset_x = (int)camera_x % LARGURA_MUNDO;
                    al_draw_bitmap(cenario, -offset_x, 0, 0);
                    al_draw_bitmap(cenario, LARGURA_MUNDO - offset_x, 0, 0);

                    player_draw(jogador, camera_x, camera_y);
                    for (int i = 0; i < MAX_BULLETS; i++) { bullet_draw(&bullets[i], bullet_sprite, camera_x, camera_y); }
                    enemy_draw(inimigos, MAX_INIMIGOS, camera_x);
                    al_draw_textf(fonte, al_map_rgb(255, 255, 0), 10, 10, 0, "VIDA: %d", jogador->hp);

                    float player_w = jogador->hitbox_largura * ESCALA;
                    float player_h = jogador->hitbox_altura * ESCALA;
                    float player_x = jogador->x + (jogador->hitbox_offset_x * ESCALA);
                    float player_y = jogador->y + (jogador->hitbox_offset_y * ESCALA);
                    al_draw_rectangle(player_x - camera_x, player_y, player_x - camera_x + player_w, player_y + player_h, al_map_rgb(0, 255, 0), 1);

                    break; // Fim do case JOGANDO (desenho)
                case FIM_DE_JOGO:
                    al_draw_text(fonte, al_map_rgb(255, 0, 0), LARGURA_TELA / 2, ALTURA_TELA / 2 - 40, ALLEGRO_ALIGN_CENTER, "FIM DE JOGO");
                    al_draw_text(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 2 + 20, ALLEGRO_ALIGN_CENTER, "Pressione ENTER para tentar novamente");
                    al_draw_text(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 2 + 40, ALLEGRO_ALIGN_CENTER, "Pressione ESC para voltar ao menu");
                    int altura_linha = al_get_font_line_height(fonte);
                    int margem_entre_opcoes = 10;
                        
                    ALLEGRO_COLOR cor_selecionada = al_map_rgb(255, 255, 0); // Amarelo
                    ALLEGRO_COLOR cor_padrao = al_map_rgb(255, 255, 255);   // Branco

                    // Posições Y calculadas dinamicamente
                    float y_titulo = ALTURA_TELA / 2.0f - altura_linha * 2;
                    float y_opcao1 = ALTURA_TELA / 2.0f;
                    float y_opcao2 = y_opcao1 + altura_linha + margem_entre_opcoes;

                    al_draw_text(fonte, al_map_rgb(255, 0, 0), LARGURA_TELA / 2, y_titulo, ALLEGRO_ALIGN_CENTER, "FIM DE JOGO");
                        
                    al_draw_text(fonte, (opcao_menu_selecionada == 0 ? cor_selecionada : cor_padrao), LARGURA_TELA / 2, y_opcao1, ALLEGRO_ALIGN_CENTER, "Iniciar Jogo");
                    al_draw_text(fonte, (opcao_menu_selecionada == 1 ? cor_selecionada : cor_padrao), LARGURA_TELA / 2, y_opcao2, ALLEGRO_ALIGN_CENTER, "Sair");
                    break;
            }

            al_flip_display();
        }
    }

    // --- Finalização ---
    al_destroy_font(fonte);
    player_destroy(jogador);
    al_destroy_bitmap(cenario);
    al_destroy_bitmap(bullet_sprite);
    al_destroy_bitmap(enemy_sprite);
    al_destroy_timer(timer);
    al_destroy_event_queue(fila_eventos);
    al_destroy_display(janela);

    return 0;
}