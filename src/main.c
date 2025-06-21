#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "boss.h"
#include "config.h" 
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "item.h"

// Struct para guardar os dados de spawn de um inimigo
typedef struct {
    EnemyType tipo;
    float x;
    float y;
} EnemySpawnInfo;

typedef struct {
    enum ItemType tipo;
    float x;
    float y;
} ItemSpawnInfo;

static const EnemySpawnInfo level1_spawns[] = {
    {SOLDADO_ESCUDO, 800, FLOOR_Y},
    {SOLDADO_ESCUDO, 1800, FLOOR_Y},
    {SOLDADO_ESPINGARDA, 1900, FLOOR_Y},
    {SOLDADO_ESCUDO, 2900, FLOOR_Y},
    {SOLDADO_ESPINGARDA, 3000, FLOOR_Y},
    {SOLDADO_ESCUDO, 3100, FLOOR_Y},
    {SOLDADO_ESPINGARDA, 3200, FLOOR_Y}
};

// Calcula automaticamente quantos inimigos temos na lista
static const int num_level1_spawns = sizeof(level1_spawns) / sizeof(level1_spawns[0]);


static const ItemSpawnInfo level1_items[] = {
    {VODKA, 3600, FLOOR_Y + ALTURA_JOGADOR_VISUAL - 96}
};

// Calcula automaticamente quantos itens temos na lista
static const int num_level1_items = sizeof(level1_items) / sizeof(level1_items[0]);


// Função que verifica se dois retângulos (r1 e r2) estão colidindo
int check_collision(float r1x, float r1y, float r1w, float r1h, float r2x, float r2y, float r2w, float r2h) {
    if (r1x + r1w >= r2x && // Borda direita de r1 >= Borda esquerda de r2
        r1x <= r2x + r2w && // Borda esquerda de r1 <= Borda direita de r2
        r1y + r1h >= r2y && // Borda de baixo de r1 >= Borda de cima de r2
        r1y <= r2y + r2h) { // Borda de cima de r1 <= Borda de baixo de r2
        return 1;
    }
    return 0;
}

// A assinatura da função agora precisa de todos os ponteiros para os sprites e configs
void resetar_jogo(struct Player *p, Enemy inimigos[], struct Bullet bullets[], struct Item itens[], Boss *chefe, float *camera_x, int *inimigos_derrotados, int *vitoria, 
                const EnemyConfig *config_soldado_espingarda, const EnemyConfig *config_soldado_escudo, ALLEGRO_BITMAP *item_heart_sprite) {
    
    printf("Resetando o jogo...\n");

    // 1. Reseta o estado do Jogador
    p->hp = 5;
    p->x = 100;
    p->y = FLOOR_Y;
    p->vel_x = 0;
    p->vel_y = 0;
    p->estado = PARADO;
    p->direcao = 1;
    p->anim_atual = p->anim_parado;
    animation_reset(p->anim_atual);
    controls_reset(p->controles);

    // 2. Reseta as variáveis de estado do Jogo
    *camera_x = 0;
    *inimigos_derrotados = 0;
    *vitoria = 0;

    // 3. Limpa todos os projéteis ativos
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].ativo = 0;
    }

    // 4. Limpa e recria os Inimigos Normais a partir do mapa da fase
    enemy_destroy_animations(inimigos, MAX_INIMIGOS);
    enemy_init(inimigos, MAX_INIMIGOS);
    for (int i = 0; i < num_level1_spawns; i++) {
        const EnemyConfig *config_atual = NULL;
        if (level1_spawns[i].tipo == SOLDADO_ESPINGARDA) config_atual = config_soldado_espingarda;
        else if (level1_spawns[i].tipo == SOLDADO_ESCUDO) config_atual = config_soldado_escudo;

        if (config_atual) {
            enemy_spawn(inimigos, MAX_INIMIGOS, config_atual, level1_spawns[i].x, level1_spawns[i].y);
        }
    }

    // 5. Limpa e recria os Itens a partir do mapa da fase
    item_init(itens, MAX_ITENS);
    for (int i = 0; i < num_level1_items; i++) {
        if (level1_items[i].tipo == VODKA) {
            item_spawn(itens, MAX_ITENS, level1_items[i].tipo, item_heart_sprite, level1_items[i].x, level1_items[i].y);
        }
    }

    // 6. Garante que o Chefe está inativo
    boss_destroy(chefe); // Libera as animações antigas do chefe
    boss_init(chefe);    // Reinicializa a struct do chefe para o estado padrão (inativo)
}

int main() {

    al_init();
    al_init_image_addon();
    al_install_keyboard();
    al_install_mouse();
    al_init_font_addon();
    al_init_ttf_addon();

    ALLEGRO_DISPLAY *janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    ALLEGRO_EVENT_QUEUE *fila_eventos = al_create_event_queue();
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);

    ALLEGRO_BITMAP *menu_background = al_load_bitmap("assets/menu_background.png");
    ALLEGRO_BITMAP *victory_background = al_load_bitmap("assets/victory_background.png");
    ALLEGRO_BITMAP *gameover_background = al_load_bitmap("assets/gameover_background.png");
    ALLEGRO_BITMAP *cenario = al_load_bitmap("assets/cenario.webp");
    ALLEGRO_BITMAP *heart_sprite = al_load_bitmap("assets/heart.png");

    ALLEGRO_FONT *fonte = al_load_font("assets/PressStart2P-Regular.ttf", 50, 0);
    ALLEGRO_FONT *titulo_fonte = al_load_font("assets/PressStart2P-Regular.ttf", 62, 0);

    struct Bullet bullets[MAX_BULLETS];
    ALLEGRO_BITMAP *bullet_sprite = al_load_bitmap("assets/bullet.png");

    struct Player *jogador = player_create(100, FLOOR_Y);
    Enemy inimigos[MAX_INIMIGOS];

    struct Item itens[MAX_ITENS];
    ALLEGRO_BITMAP *item_sprite = al_load_bitmap("assets/item.png");

    Boss chefe;
    ALLEGRO_BITMAP *boss_sprite_move = al_load_bitmap("assets/boss_move.png");
    ALLEGRO_BITMAP *boss_sprite_fire = al_load_bitmap("assets/boss_fire.png");

    if (!janela || !fila_eventos || !timer || !menu_background || !victory_background || !gameover_background || !cenario 
        || !heart_sprite || !fonte || !titulo_fonte || !bullet_sprite || !jogador || !item_sprite || !boss_sprite_move || !boss_sprite_fire) {
        printf("ERRO: Falha em um dos componentes de inicialização.\n");
        return -1;
    }

    // Sprites do SOLDADO_ESPINGARDA
    ALLEGRO_BITMAP *enemy1_sprite_idle = al_load_bitmap("assets/enemy1_idle.png");
    ALLEGRO_BITMAP *enemy1_sprite_shooting = al_load_bitmap("assets/enemy1_shot.png");
    ALLEGRO_BITMAP *enemy1_sprite_death = al_load_bitmap("assets/enemy1_death.png");

    // Sprites do SOLDADO_ESCUDO
    ALLEGRO_BITMAP *enemy2_sprite_idle = al_load_bitmap("assets/enemy2_idle.png");
    ALLEGRO_BITMAP *enemy2_sprite_shooting = al_load_bitmap("assets/enemy2_shot.png");
    ALLEGRO_BITMAP *enemy2_sprite_death = al_load_bitmap("assets/enemy2_death.png");

    if (!enemy1_sprite_idle || !enemy1_sprite_shooting || !enemy1_sprite_death || !enemy2_sprite_idle || !enemy2_sprite_shooting || !enemy2_sprite_death) {
        printf("ERRO: Falha ao carregar os sprites dos inimigos.\n");
        return -1;
    }

    EnemyConfig config_soldado_espingarda = {
        .tipo = SOLDADO_ESPINGARDA, .hp = 5,
        .num_frames_parado = 6, .num_frames_atirando = 7, .num_frames_morrendo = 5,
        .folha_sprite_parado = enemy1_sprite_idle,
        .folha_sprite_atirando = enemy1_sprite_shooting,
        .folha_sprite_morrendo = enemy1_sprite_death
    };
    EnemyConfig config_soldado_escudo = {
        .tipo = SOLDADO_ESCUDO, .hp = 8,
        .num_frames_parado = 6, .num_frames_atirando = 5, .num_frames_morrendo = 6,
        .folha_sprite_parado = enemy2_sprite_idle,
        .folha_sprite_atirando = enemy2_sprite_shooting,
        .folha_sprite_morrendo = enemy2_sprite_death
    };

    enemy_init(inimigos, MAX_INIMIGOS);
    
    // --- SPAWN INICIAL ---
    for (int i = 0; i < num_level1_spawns; i++) {
        const EnemyConfig *config_atual = NULL;
        if (level1_spawns[i].tipo == SOLDADO_ESPINGARDA) {
            config_atual = &config_soldado_espingarda;
        } else {
            config_atual = &config_soldado_escudo;
        }
        enemy_spawn(inimigos, MAX_INIMIGOS, config_atual, level1_spawns[i].x, level1_spawns[i].y);
    }

    for (int i = 0; i < MAX_BULLETS; i++) { bullets[i].ativo = 0; }

    item_init(itens, MAX_ITENS);
    for (int i = 0; i < num_level1_items; i++) {
        if (level1_items[i].tipo == VODKA) {
            item_spawn(itens, MAX_ITENS, level1_items[i].tipo, item_sprite, level1_items[i].x, level1_items[i].y);
        }
    }

    boss_init(&chefe);

    float camera_x = 0, camera_y = 0;
    int rodando = 1;

    // --- Variáveis de Estado do Jogo ---
    GameState estado_atual = MENU;
    int opcao_menu_selecionada = 0; // 0 = Iniciar, 1 = Sair
    int opcao_gameover_selecionada = 0; // 0 = Tentar de Novo, 1 = Sair
    int inimigos_derrotados = 0;
    int vitoria = 0;

    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);
    al_register_event_source(fila_eventos, al_get_mouse_event_source());

    ALLEGRO_EVENT evento;
    al_start_timer(timer);

    while (rodando) {
        al_wait_for_event(fila_eventos, &evento);

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            estado_atual = SAIR;
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
                break;
            case JOGANDO:
                controls_handle_input(jogador->controles, &evento);
                break;
            case FIM_DE_JOGO:
                { // Escopo local para variáveis
                    int altura_linha = al_get_font_line_height(fonte);
                    float y_opcao1_inicio = ALTURA_TELA / 2 + 20;
                    float y_opcao1_fim = y_opcao1_inicio + altura_linha;
                    float y_opcao2_inicio = y_opcao1_fim + 10;
                    float y_opcao2_fim = y_opcao2_inicio + altura_linha;

                    if (evento.type == ALLEGRO_EVENT_MOUSE_AXES) {
                        int mouse_y = evento.mouse.y;
                        if (mouse_y >= y_opcao1_inicio && mouse_y < y_opcao1_fim) opcao_gameover_selecionada = 0;
                        else if (mouse_y >= y_opcao2_inicio && mouse_y < y_opcao2_fim) opcao_gameover_selecionada = 1;
                    }

                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                        if (opcao_gameover_selecionada == 0) {
                            resetar_jogo(jogador, inimigos, bullets, itens, &chefe, &camera_x, &inimigos_derrotados, &vitoria, &config_soldado_espingarda, &config_soldado_escudo, item_sprite);
                            estado_atual = JOGANDO;
                        } else if (opcao_gameover_selecionada == 1) {
                            resetar_jogo(jogador, inimigos, bullets, itens, &chefe, &camera_x, &inimigos_derrotados, &vitoria, &config_soldado_espingarda, &config_soldado_escudo, item_sprite);
                            estado_atual = MENU;
                        }
                    }

                    if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
                        if (evento.keyboard.keycode == ALLEGRO_KEY_UP) opcao_gameover_selecionada = 0;
                        if (evento.keyboard.keycode == ALLEGRO_KEY_DOWN) opcao_gameover_selecionada = 1;
                        if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                            if (opcao_gameover_selecionada == 0) {
                                resetar_jogo(jogador, inimigos, bullets, itens, &chefe, &camera_x, &inimigos_derrotados, &vitoria, &config_soldado_espingarda, &config_soldado_escudo, item_sprite);
                                estado_atual = JOGANDO;
                            } else if (opcao_gameover_selecionada == 1) {
                                resetar_jogo(jogador, inimigos, bullets, itens, &chefe, &camera_x, &inimigos_derrotados, &vitoria, &config_soldado_espingarda, &config_soldado_escudo, item_sprite);
                                estado_atual = MENU;
                            }
                        }
                    }
                }
                break;
            case SAIR:
                rodando = 0; // Marca para sair do loop principal
                break;
        }

        ALLEGRO_COLOR cor_selecionada = al_map_rgb(255, 255, 0); // Amarelo
        ALLEGRO_COLOR cor_padrao = al_map_rgb(255, 255, 255);   // Branco

        // --- Lógica de Atualização e Desenho (baseada em timer) ---
        if (evento.type == ALLEGRO_EVENT_TIMER) {
            switch (estado_atual) {
                case MENU:

                    al_draw_bitmap(menu_background, 0, 0, 0);

                    al_draw_text(titulo_fonte, al_map_rgb(255, 0, 0), LARGURA_TELA / 2, 180, ALLEGRO_ALIGN_CENTER, "Winter's Blood");

                    al_draw_text(fonte, (opcao_menu_selecionada == 0 ? cor_selecionada : cor_padrao), LARGURA_TELA / 2, 300, ALLEGRO_ALIGN_CENTER, "Iniciar Jogo");
                    al_draw_text(fonte, (opcao_menu_selecionada == 1 ? cor_selecionada : cor_padrao), LARGURA_TELA / 2, 370, ALLEGRO_ALIGN_CENTER, "Sair");
                    break;
                case JOGANDO:
                    
                    player_update(jogador);
                    for (int i = 0; i < MAX_INIMIGOS; i++) { if (inimigos[i].ativo) enemy_update(&inimigos[i], jogador, bullets, MAX_BULLETS); }
                    for (int i = 0; i < MAX_BULLETS; i++) { if (bullets[i].ativo) bullet_update(&bullets[i], camera_x); }
                    
                    if (chefe.ativo) { boss_update(&chefe, jogador, bullets, MAX_BULLETS); }

                    player_fire(jogador, bullets, MAX_BULLETS);

                    // --- LÓGICA DE COMBATE E COLISÃO ATUALIZADA ---
                    for (int i = 0; i < MAX_BULLETS; i++) {
                        if (!bullets[i].ativo) continue;

                        // Caixa de colisão do projétil
                        float bullet_x = bullets[i].x, bullet_y = bullets[i].y;
                        float bullet_w = LARGURA_BULLET, bullet_h = ALTURA_BULLET;

                        // VERIFICA SE O PROJÉTIL DO JOGADOR ACERTOU UM INIMIGO
                        if (bullets[i].owner == PLAYER) {
                            for (int j = 0; j < MAX_INIMIGOS; j++) {
                                if (!inimigos[j].ativo) continue;

                                // --- LÓGICA DA HITBOX DO INIMIGO ---
                                float enemy_w = HITBOX_INIMIGO_LARGURA * ESCALA;
                                float enemy_h = HITBOX_INIMIGO_ALTURA * ESCALA;
                                float enemy_x = inimigos[j].x + (HITBOX_INIMIGO_OFFSET_X * ESCALA);
                                float enemy_y = inimigos[j].y + (HITBOX_INIMIGO_OFFSET_Y * ESCALA);

                                if (check_collision(bullet_x, bullet_y, bullet_w, bullet_h, enemy_x, enemy_y, enemy_w, enemy_h)) {
                                    bullets[i].ativo = 0; 
                                    inimigos[j].hp--;
                                    
                                    // SE o inimigo morreu E ele ainda não estava morrendo
                                    if (inimigos[j].hp <= 0 && inimigos[j].estado != INIMIGO_MORRENDO) {
                                        inimigos[j].estado = INIMIGO_MORRENDO;
                                        inimigos[j].anim_atual = inimigos[j].anim_morrendo;
                                        animation_reset(inimigos[j].anim_morrendo);
                                        inimigos_derrotados++;
                                    }
                                    break;
                                }
                            }

                            // Testa colisão com o chefe (se ele estiver ativo)
                            if (chefe.ativo) {
                                // Calcula a hitbox do chefe
                                float boss_hitbox_x = chefe.x + (chefe.hitbox_offset_x * ESCALA);
                                float boss_hitbox_y = chefe.y + (chefe.hitbox_offset_y * ESCALA);

                                // Verifica a colisão
                                if (check_collision(bullet_x, bullet_y, bullet_w, bullet_h, boss_hitbox_x, boss_hitbox_y, chefe.hitbox_largura, chefe.hitbox_altura)) {
                                
                                    bullets[i].ativo = 0; // Projétil desaparece
                                    chefe.hp--;               // Chefe perde vida

                                    // --- NOVA LÓGICA DE DERROTA ---
                                    // Se a vida do chefe chegar a zero...
                                    if (chefe.hp <= 0) {
                                        chefe.ativo = 0; // ...ele é simplesmente desativado.
                                        estado_atual = FIM_DE_JOGO; // Muda o estado do jogo para "Fim de Jogo"
                                        vitoria = 1; // Marca que o jogador venceu
                                        printf("CHEFE DERROTADO!\\n");
                                    }
                                }
                            }
                        }
                        // VERIFICA SE O PROJÉTIL DO INIMIGO ACERTOU O JOGADOR
                        else if (bullets[i].owner == ENEMY) {

                            // --- LÓGICA DA HITBOX DO JOGADOR ---
                            float player_w = jogador->hitbox_largura * ESCALA;
                            float player_h = jogador->hitbox_altura * ESCALA;
                            float player_x = jogador->x + (jogador->hitbox_offset_x * ESCALA);
                            float player_y = jogador->y + (jogador->hitbox_offset_y * ESCALA);

                            if (check_collision(bullet_x, bullet_y, bullet_w, bullet_h, player_x, player_y, player_w, player_h)) {
                                bullets[i].ativo = 0;
                                jogador->hp--;
                                if (jogador->hp <= 0) {
                                    estado_atual = FIM_DE_JOGO;
                                }
                            }
                        
                        }
                    }

                    // --- LÓGICA DE COLISÃO DO JOGADOR COM ITENS ---
                    for (int i = 0; i < MAX_ITENS; i++) {
                        if (itens[i].ativo) {
                    
                            // Pega a hitbox atual do jogador
                            float player_hitbox_x = jogador->x + (jogador->hitbox_offset_x * ESCALA);
                            float player_hitbox_y = jogador->y + (jogador->hitbox_offset_y * ESCALA);
                            float player_hitbox_w = jogador->hitbox_largura * ESCALA;
                            float player_hitbox_h = jogador->hitbox_altura * ESCALA;

                            // Calcula a posição e o tamanho da hitbox do item
                            float item_hitbox_x = itens[i].x + HITBOX_ITEM_OFFSET_X;
                            float item_hitbox_y = itens[i].y + HITBOX_ITEM_OFFSET_Y;
                            float item_hitbox_w = HITBOX_ITEM_LARGURA;
                            float item_hitbox_h = HITBOX_ITEM_ALTURA;

                            // Verifica a colisão
                            if (check_collision(player_hitbox_x, player_hitbox_y, player_hitbox_w, player_hitbox_h, item_hitbox_x, item_hitbox_y, item_hitbox_w, item_hitbox_h)) {
                                if (itens[i].tipo == VODKA) {
                                    jogador->hp += 3;
                                    itens[i].ativo = 0; // Desativa o item para não ser pego de novo
                                }
                            }
                        }
                    }

                    // --- LÓGICA DE COLISÃO: JOGADOR vs CHEFE (DANO DE TOQUE) ---
                    // Só verifica se o chefe e o jogador estão vivos e ativos
                    if (chefe.ativo && jogador->hp > 0) {
                        
                        // --- HITBOX DO JOGADOR (CORRIGIDA COM ESCALA) ---
                        float player_w = jogador->hitbox_largura * ESCALA;
                        float player_h = jogador->hitbox_altura * ESCALA;
                        float player_x = jogador->x + (jogador->hitbox_offset_x * ESCALA);
                        float player_y = jogador->y + (jogador->hitbox_offset_y * ESCALA);

                        // Pega a hitbox do chefe
                        float boss_w = chefe.hitbox_largura;
                        float boss_h = chefe.hitbox_altura;
                        float boss_x = chefe.x + chefe.hitbox_offset_x;
                        float boss_y = chefe.y + chefe.hitbox_offset_y;

                        // Se as hitboxes se tocarem, o jogador morre instantaneamente
                        if (check_collision(player_x, player_y, player_w, player_h, boss_x, boss_y, boss_w, boss_h)) {
                            printf("Colisão entre Jogador e Chefe! Fim de jogo.\\n");
                            jogador->hp = 0; // Zera a vida do jogador
                        }
                    }

                    if (!chefe.ativo && inimigos_derrotados >= num_level1_spawns) {
                        boss_spawn(&chefe, 5000, boss_sprite_move, boss_sprite_fire);
                    }

                    camera_x = jogador->x;
                    if (camera_x < 0) camera_x = 0;

                    int offset_x = (int)camera_x % LARGURA_MUNDO;
                    al_draw_bitmap(cenario, -offset_x, 0, 0);
                    al_draw_bitmap(cenario, LARGURA_MUNDO - offset_x, 0, 0);

                    player_draw(jogador, camera_x, camera_y);
                    for (int i = 0; i < MAX_BULLETS; i++) { bullet_draw(&bullets[i], bullet_sprite, camera_x, camera_y); }
                    enemy_draw(inimigos, MAX_INIMIGOS, camera_x, camera_y);
                    item_draw(itens, MAX_ITENS, camera_x, camera_y);
                    if (chefe.ativo) {
                        boss_draw(&chefe, camera_x, camera_y);
                    }

                    if (heart_sprite) {
                        int padding = 5; // Espaçamento entre os corações

                        for (int i = 0; i < jogador->hp; i++) {
                            // Calcula a posição X de cada coração
                            float x_pos = 10 + i * (LARGURA_CORACAO + padding);
                            float y_pos = 10;
                            
                            // Desenha o sprite do coração no tamanho definido em config.h
                            al_draw_scaled_bitmap(
                                heart_sprite,
                                0, 0, // Coordenadas de origem na imagem (começa do canto)
                                al_get_bitmap_width(heart_sprite), // Largura original total da imagem
                                al_get_bitmap_height(heart_sprite),// Altura original total da imagem
                                x_pos, y_pos,                       // Posição de destino na tela
                                LARGURA_CORACAO,                    // Largura final na tela
                                ALTURA_CORACAO,                     // Altura final na tela
                                0
                            );
                        }
                    }

                    break;
                case FIM_DE_JOGO:

                    if(vitoria){
                        al_draw_bitmap(victory_background, 0, 0, 0);
                        al_draw_text(titulo_fonte, al_map_rgb(0, 255, 0), LARGURA_TELA / 2, 180, ALLEGRO_ALIGN_CENTER, "VITORIA");
                    }
                    else{
                        al_draw_bitmap(gameover_background, 0, 0, 0);
                        al_draw_text(titulo_fonte, al_map_rgb(255, 0, 0), LARGURA_TELA / 2, 180, ALLEGRO_ALIGN_CENTER, "DERROTA");
                    }

                    al_draw_text(fonte, (opcao_gameover_selecionada == 0 ? cor_selecionada : cor_padrao), LARGURA_TELA / 2, 300, ALLEGRO_ALIGN_CENTER, "Tentar Novamente");
                    al_draw_text(fonte, (opcao_gameover_selecionada == 1 ? cor_selecionada : cor_padrao), LARGURA_TELA / 2, 380, ALLEGRO_ALIGN_CENTER, "Voltar para o Menu");
                    break;
            }

            al_flip_display();
        }
    }

    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
    al_destroy_timer(timer);
    al_destroy_bitmap(menu_background);
    al_destroy_bitmap(victory_background);
    al_destroy_bitmap(gameover_background);
    al_destroy_bitmap(cenario);
    al_destroy_bitmap(heart_sprite);
    al_destroy_font(fonte);
    al_destroy_font(titulo_fonte);
    al_destroy_bitmap(bullet_sprite);
    player_destroy(jogador);
    al_destroy_bitmap(item_sprite);
    boss_destroy(&chefe);
    al_destroy_bitmap(boss_sprite_move);
    al_destroy_bitmap(boss_sprite_fire);
    enemy_destroy_animations(inimigos, MAX_INIMIGOS);
    al_destroy_bitmap(enemy1_sprite_idle);
    al_destroy_bitmap(enemy1_sprite_shooting);
    al_destroy_bitmap(enemy1_sprite_death);
    al_destroy_bitmap(enemy2_sprite_idle);
    al_destroy_bitmap(enemy2_sprite_shooting);
    al_destroy_bitmap(enemy2_sprite_death);

    return 0;
}