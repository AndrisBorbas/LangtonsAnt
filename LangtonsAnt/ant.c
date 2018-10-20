#include <SDL.h>
#include <SDL_image.h>
#include <SDL2_gfxprimitives.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "everything.h"

bool moveAnt(Uint32*** pixelTex, Ant* ant, int* lepes, int const SCREEN_WIDTH, int const SCREEN_HEIGHT, int const SCALE, int const SPACING, int ANTMARGIN)
{
	(*lepes)++;
	printf("%d. lepes: ", *lepes);
	if (!antgorithm(pixelTex, ant, SCREEN_WIDTH, SCREEN_HEIGHT, SCALE, SPACING, ANTMARGIN))return false;

	//printf("%d  %d\n", ant->y, ant->y - SCALE / 2 + SPACING);
	//(*pixels)[(ant->x - SCALE - SPACING + ANTMARGIN) + (ant->y - SCALE - SPACING + ANTMARGIN) * SCREEN_WIDTH] = 0xFF00FF00;

	//color last location
	for (int i = (ant->x - SCALE); i < (ant->x + SCALE - SPACING); i++)
	{
		for (int j = (ant->y - SCALE); j < (ant->y + SCALE - SPACING); j++)
		{
			if (i<0 || i>SCREEN_WIDTH || j<0 || j>SCREEN_HEIGHT)
			{
				printf("ERROR: ant out of bounds at x=%d, y=%d.", i, j);
				return false;
			}
			switch (ant->lasttile)
			{
			case GRAY:
				(*pixelTex)[i][j] = DARKWHITE;
				break;
			case DARKWHITE:
				(*pixelTex)[i][j] = ORANGE;
				break;
			case ORANGE:
				(*pixelTex)[i][j] = YELLOW;
				break;
			case YELLOW:
				(*pixelTex)[i][j] = GRAY;
				break;
			default:
				break;
			}
		}
	}



	//printf("%d ", ant->heading);
	switch (ant->heading)
	{
	case UP:
		ant->y = ant->y - SCALE * 2;
		break;
	case DOWN:
		ant->y = ant->y + SCALE * 2;
		break;
	case RIGHT:
		ant->x = ant->x + SCALE * 2;
		break;
	case LEFT:
		ant->x = ant->x - SCALE * 2;
		break;
	default:
		break;
	}
	//printf("%d\n", ant->heading);
	for (int i = (ant->x - SCALE + ANTMARGIN); i < (ant->x + SCALE - SPACING - ANTMARGIN); i++)
	{
		for (int j = (ant->y - SCALE + ANTMARGIN); j < (ant->y + SCALE - SPACING - ANTMARGIN); j++)
		{
			if (i<0 || i>SCREEN_WIDTH || j<0 || j>SCREEN_HEIGHT)
			{
				printf("ERROR: ant out of bounds at x=%d, y=%d.", i, j);
				return false;
			}
			(*pixelTex)[i][j] = 0xFFFF0000;
		}
	}
	return true;
}

bool antgorithm(Uint32*** pixelTex, Ant* ant, int const SCREEN_WIDTH, int const SCREEN_HEIGHT, int const SCALE, int const SPACING, int ANTMARGIN) {
	//printf("%u\n%u\n%u\n\n", ((*pixels)[(ant->x - SCALE - SPACING + ANTMARGIN) + ((ant->y - SCALE - SPACING + ANTMARGIN)*SCREEN_WIDTH)]), BLACK, 0xFF000000);
	int xpos = (ant->x - SCALE - SPACING + ANTMARGIN);
	int ypos = (ant->y - SCALE - SPACING + ANTMARGIN);

	if (xpos < 0 || xpos > SCREEN_WIDTH || ypos < 0 || ypos > SCREEN_HEIGHT)return false;

	if (((*pixelTex)[xpos][ypos]) == BLACK || ((*pixelTex)[xpos][ypos]) == GRAY)
	{
		ant->heading = ant->heading + 90;
		if (ant->heading > 270) ant->heading = ant->heading - 360;
		if (ant->heading < 0) ant->heading = ant->heading + 360;
		ant->lasttile = GRAY;
		printf("jobb\n");
		return true;
	}
	if (((*pixelTex)[xpos][ypos]) == DARKWHITE)
	{
		ant->heading = ant->heading - 90;
		if (ant->heading > 270) ant->heading = ant->heading - 360;
		if (ant->heading < 0) ant->heading = ant->heading + 360;
		ant->lasttile = DARKWHITE;
		printf("jobb\n");
		return true;
	}
	if (((*pixelTex)[xpos][ypos]) == ORANGE)
	{
		ant->heading = ant->heading + 90;
		if (ant->heading > 270) ant->heading = ant->heading - 360;
		if (ant->heading < 0) ant->heading = ant->heading + 360;
		ant->lasttile = ORANGE;
		printf("bal\n");
		return true;
	}
	if (((*pixelTex)[xpos][ypos]) == YELLOW)
	{
		ant->heading = ant->heading - 90;
		if (ant->heading > 270) ant->heading = ant->heading - 360;
		if (ant->heading < 0) ant->heading = ant->heading + 360;
		ant->lasttile = YELLOW;
		printf("bal\n");
		return true;
	}
	return false;
}