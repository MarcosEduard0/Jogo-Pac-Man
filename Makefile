all: PacMan.c
	gcc -o "PacMan"  "PacMan.c"  -lm -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -Wall
