CC = gcc
CFLAGS = $(shell sdl2-config --cflags) -g -fsanitize=address -fno-omit-frame-pointer
LIBS = $(shell sdl2-config --libs) -lSDL2_image -lm

FILES = animations.c characters.c collisionHandling.c enemies.c inventory.c objects.c randomSpawn.c selectRandomMap.c 
SRC = main.c $(FILES) 
SRC_TEST = tests/test.c $(FILES) tests/testInventory.c tests/testObjects.c
UNITY_SRC = Unity/src/unity.c

EXE = main
TEST = test

all: run tests

$(EXE): $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(LIBS) -o $(EXE)

$(TEST): $(SRC_TEST)
	$(CC) $(SRC_TEST) $(UNITY_SRC) $(CFLAGS) $(LIBS) -o $(TEST)

run: $(EXE)
	@./$(EXE)

tests: $(TEST)
	@./$(TEST)

clean:
	rm -f $(EXE) $(TEST)
