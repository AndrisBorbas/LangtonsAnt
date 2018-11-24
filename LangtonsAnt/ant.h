#pragma once

#include "includes.h"

//Stores the properties of the ant
typedef struct Ant {
	int x, y, heading, state, lasttile, currenttile, turn[18];
}Ant;

//The heading of the ant
enum AntHeading
{
	UP = 0,
	RIGHT = 90,
	DOWN = 180,
	LEFT = 270
};

//move ant and invert past location
bool moveAnt(Uint32*** pixelTex, Ant* ant, int* lepes, SDL_Rect SCREEN, int SCALE, int SPACING, int ANTMARGIN, int instructnum, char* instructionset, FILE* fAntOut);

//Turn the ant into the right direction
bool turnAnt(Ant* ant, int tile, FILE* fAntOut);

//The alogorithm for the ants movement
bool antgorithm(Uint32*** pixelTex, Ant* ant, SDL_Rect SCREEN, int SCALE, int SPACING, int ANTMARGIN, FILE* fAntOut);
