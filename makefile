CC = gcc
CFLAGS = $(shell sdl2-config --cflags)
LIBS = $(shell sdl2-config --libs) -lSDL2_image

SRC = moving_cube.c
EXE = moving_cube

all:
	$(CC) $(SRC) $(CFLAGS) $(LIBS) -o $(EXE) && ./$(EXE)

clean:
	rm -f $(EXE)
