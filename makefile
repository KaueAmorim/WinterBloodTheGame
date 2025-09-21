TARGET = game
CC = gcc
SRCDIR = src
INCDIR = include
BINDIR = bin

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(BINDIR)/%.o, $(SOURCES))
DEPS = $(patsubst $(SRCDIR)/%.c, $(BINDIR)/%.d, $(SOURCES))
CFLAGS = -I$(INCDIR) -g -MMD -MP
LDFLAGS = $(shell pkg-config --libs --cflags allegro-5 allegro_main-5 allegro_image-5 allegro_font-5 allegro_ttf-5)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(BINDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(BINDIR) $(TARGET)

-include $(DEPS)

.PHONY: all clean
