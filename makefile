# Nome do executável final
TARGET = run_and_gun

# Compilador
CC = gcc

# Diretórios
SRCDIR = src
INCDIR = include
BINDIR = bin

# --- LÓGICA DE DETECÇÃO DE ARQUIVOS ---
# Encontra todos os arquivos .c em src/
SOURCES = $(wildcard $(SRCDIR)/*.c)
# Gera os nomes dos arquivos objeto .o correspondentes
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(BINDIR)/%.o, $(SOURCES))
# Gera os nomes dos arquivos de dependência .d correspondentes
DEPS = $(patsubst $(SRCDIR)/%.c, $(BINDIR)/%.d, $(SOURCES))


# --- FLAGS DE COMPILAÇÃO E LINK ---
# CFLAGS: Adicionamos -MMD e -MP para gerar os arquivos de dependência
CFLAGS = -I$(INCDIR) -g -MMD -MP

# LDFLAGS: Continua o mesmo
LDFLAGS = $(shell pkg-config --libs --cflags allegro-5 allegro_main-5 allegro_primitives-5 allegro_image-5 allegro_font-5 allegro_ttf-5)


# --- REGRAS DE COMPILAÇÃO ---

# Regra principal: o que acontece quando você digita 'make'
all: $(TARGET)

# Regra para criar o executável final
$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "Compilação concluída! Executável criado em $(BINDIR)/$(TARGET)"

# Regra para compilar cada arquivo .c em um arquivo .o (agora também cria o .d)
$(BINDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para limpar os arquivos compilados (agora também apaga os .d)
clean:
	@rm -rf $(BINDIR)
	@echo "Arquivos compilados e dependências removidos."

# --- MÁGICA FINAL ---
# Inclui silenciosamente todos os arquivos de dependência (.d) que foram gerados.
# Se um .d disser que um .o depende de um .h, o make agora saberá disso.
-include $(DEPS)

# Define que 'all' e 'clean' não são nomes de arquivos
.PHONY: all clean
