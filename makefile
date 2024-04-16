CC = gcc
CFLAGS = $(shell sdl2-config --cflags) -g -fsanitize=address -fno-omit-frame-pointer
LIBS = $(shell sdl2-config --libs) -lSDL2_image -lm

SRC = $(wildcard *.c)

EXE = main

all: $(EXE)
	@./$(EXE)

$(EXE): $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(LIBS) -o $(EXE)

run: $(EXE)
	@./$(EXE)

clean:
	rm -f $(EXE)
