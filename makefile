CC = gcc
CFLAGS = $(shell sdl2-config --cflags)
LIBS = $(shell sdl2-config --libs) -lSDL2_image

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
