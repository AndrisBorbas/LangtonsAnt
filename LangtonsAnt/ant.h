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

//Turn the ant into the right direction
bool turnAnt(Ant*, int, FILE*);

//The alogorithm for the ants movement
bool antgorithm(Uint32***, Ant*, SDL_Rect const, int const, int const, int, FILE*);

//move ant and invert past location
bool moveAnt(Uint32***, Ant*, int*, SDL_Rect const, int const, int const, int const, int, char*, FILE*);