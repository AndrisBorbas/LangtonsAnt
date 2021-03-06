﻿#pragma once

#include "includes.h"

//Colors for coloring the pixels drawn by the ant
enum HEXARGB
{
	BLACK = 0xFF000000,
	WHITE = 0xFFFFFFFF,
	MAROON = 0xFF800000,
	BROWN = 0xFF9A6324,
	OLIVE = 0xFF808000,
	TEAL = 0xFF469990,
	NAVY = 0xFF000075,
	RED = 0xFFE6194B,
	ORANGE = 0xFFF58231,
	YELLOW = 0xFFFFE119,
	LIME = 0xFFBFEF45,
	GREEN = 0xFF3CB44B,
	CYAN = 0xFF42D4F4,
	BLUE = 0xFF4363D8,
	PURPLE = 0xFF911EB4,
	MAGENTA = 0xFFF032E6,
	PINK = 0xFFFABEBE,
	APRICOT = 0xFFFFD8B1,
	BEIGE = 0xFFFFFAC8,
	MINT = 0xFFAAFFC3,
	LAVENDER = 0xFFE6BEFF
};

//Colors for drawing the menu
enum HEXRGBA
{
	altWHITE = 0xFFFFFFFF,
	altDARKWHITE = 0xDDDDDDFF,
	altGRAY = 0x333333FF,
	altGRAYALPHA = 0x333333CC,
	altDARKGRAY = 0x111111FF,
	altBLACK = 0x000000FF
};

//Colors for texts
extern SDL_Color TextOrange, TextDarkOrange;