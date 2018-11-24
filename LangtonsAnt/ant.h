#pragma once

#include "includes.h"

//Stores the properties of the ant
typedef struct Ant {
	int x, y, heading, lasttile, turn[18];
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
bool moveAnt(Uint32*** pixelTex, Ant* ant, int* lepes, Settings settings, int instructnum, FILE* fAntOut);

//Turn the ant into the right direction
bool turnAnt(Ant* ant, int tile, FILE* fAntOut);

//The alogorithm for the ants movement
bool antgorithm(Uint32*** pixelTex, Ant* ant, Settings settings, FILE* fAntOut);

//Convert instructions to turns for the ant
void convertToTurns(char* instructionset, Ant* ant);