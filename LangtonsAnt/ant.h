#pragma once

#include "includes.h"
#include "colors.h"

//Stores the properties of the ant
typedef struct Ant {
	int x, y, heading, state, lasttile, currenttile, turn[18];
}Ant;

enum AntHeading
{
	UP = 0,
	RIGHT = 90,
	DOWN = 180,
	LEFT = 270
};

//Turn the ant into the right direction
bool turnAnt(Ant*, int);

//The alogorithm for the ants movement
bool antgorithm(Uint32***, Ant*, SDL_Rect const, int const, int const, int);

//move ant and invert past location
bool moveAnt(Uint32***, Ant*, int*, SDL_Rect const, int const, int const, int const, int, char*);