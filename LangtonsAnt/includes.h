#pragma once

//#define Debug
#define _CRT_SECURE_NO_WARNINGS

#include <SDL.h>
#include <SDL_image.h>
#include <SDL2_gfxprimitives.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <sys/stat.h>

#include "colors.h"

typedef struct Settings {
	//Window parameter container
	SDL_Rect SCREEN;
	//Number of pixels per grid square (has to be at least 3 or the ant wont be visible, but it will still work)
	int SCALE;
	//Number of pixels between grid squares
	int SPACING;
	//Number of pixels the ant is smaller than the grid (has to be at least 1, can't be more than SCALE)
	int ANTMARGIN;
	//The tickrate of the simulation in ms
	int MSTICK;
	//Instructionset for the ant
	char instructionset[19];
}Settings;