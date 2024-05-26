CC = gcc
CFLAGS = $(shell sdl2-config --cflags) -g -fsanitize=address -fno-omit-frame-pointer
LIBS = $(shell sdl2-config --libs) -lSDL2_image -lm

FILES = animations.c characters.c collisionHandling.c enemies.c inventory.c objects.c randomSpawn.c selectRandomMap.c 
SRC = main.c $(FILES) 
SRC_TEST = tests/test.c $(FILES) tests/testInventory.c tests/testObjects.c tests/testCollisionHandling.c tests/testSelectRandomMap.c tests/testAnimations.c tests/testCharacter.c tests/testEnemies.c tests/testRandomSpawn.c
UNITY_SRC = Unity/src/unity.c

EXE = main
TEST = few_tests

run: $(EXE)
	@./$(EXE)

$(EXE): $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(LIBS) -o $(EXE)

$(TEST): $(SRC_TEST)
	$(CC) $(SRC_TEST) $(UNITY_SRC) $(CFLAGS) $(LIBS) -o $(TEST)

all: run test

test: $(TEST)
	@./$(TEST)

clean:
	rm -f $(EXE) $(TEST)
