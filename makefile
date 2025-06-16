# Nome do executável final
TARGET = run_and_gun

# Compilador
CC = gcc

# Diretórios
SRCDIR = src
INCDIR = include
BINDIR = bin

# Arquivos fonte .c (encontra todos os .c em src/)
SOURCES = $(wildcard $(SRCDIR)/*.c)

# Arquivos objeto .o (substitui src/*.c por bin/*.o)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(BINDIR)/%.o, $(SOURCES))

# Flags de compilação
# -I$(INCDIR) -> Diz ao compilador para procurar arquivos .h na pasta include/
# -g -> Inclui informações para debugging
CFLAGS = -I$(INCDIR) -g

# Flags de bibliotecas (Linker)
# Aqui listamos todas as bibliotecas do Allegro que vamos usar
LDFLAGS = $(shell pkg-config --libs --cflags allegro-5 allegro_main-5 allegro_primitives-5 allegro_image-5 allegro_font-5 allegro_ttf-5)
# --- REGRAS DE COMPILAÇÃO ---

# Regra principal: o que acontece quando você digita 'make'
all: $(BINDIR)/$(TARGET)

# Regra para criar o executável final
$(BINDIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "Compilação concluída! Executável criado em $(BINDIR)/$(TARGET)"

# Regra para compilar cada arquivo .c em um arquivo .o
$(BINDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para limpar os arquivos compilados
# Executada quando você digita 'make clean'
clean:
	@rm -rf $(BINDIR)
	@echo "Arquivos compilados removidos."

.PHONY: all clean
