#include <SDL.h>
#include <SDL_image.h>
#include <SDL2_gfxprimitives.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "everything.h"

bool moveAnt(Uint32*** pixelTex, Ant* ant, int* lepes, int const SCREEN_WIDTH, int const SCREEN_HEIGHT, int const SCALE, int const SPACING, int const ANTMARGIN, int instructnum, char* instructionset)
{
	(*lepes)++;
	printf("%d. lepes: ", *lepes);

	//ant->lasttile = (ant->lasttile + instructnum) % instructnum;
	//if (ant->lasttile == instructnum)ant->lasttile = 3;

	if (!antgorithm(pixelTex, ant, SCREEN_WIDTH, SCREEN_HEIGHT, SCALE, SPACING, ANTMARGIN))return false;

	if (ant->lasttile == instructnum-1)ant->lasttile = 18;

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
			case 0:
				(*pixelTex)[i][j] = BROWN;
				break;
			case 1:
				(*pixelTex)[i][j] = OLIVE;
				break;
			case 2:
				(*pixelTex)[i][j] = TEAL;
				break;
			case 3:
				(*pixelTex)[i][j] = NAVY;
				break;
			case 4:
				(*pixelTex)[i][j] = RED;
				break;
			case 5:
				(*pixelTex)[i][j] = ORANGE;
				break;
			case 6:
				(*pixelTex)[i][j] = YELLOW;
				break;
			case 7:
				(*pixelTex)[i][j] = LIME;
				break;
			case 8:
				(*pixelTex)[i][j] = GREEN;
				break;
			case 9:
				(*pixelTex)[i][j] = CYAN;
				break;
			case 10:
				(*pixelTex)[i][j] = BLUE;
				break;
			case 11:
				(*pixelTex)[i][j] = PURPLE;
				break;
			case 12:
				(*pixelTex)[i][j] = MAGENTA;
				break;
			case 13:
				(*pixelTex)[i][j] = PINK;
				break;
			case 14:
				(*pixelTex)[i][j] = APRICOT;
				break;
			case 15:
				(*pixelTex)[i][j] = BEIGE;
				break;
			case 16:
				(*pixelTex)[i][j] = MINT;
				break;
			case 17:
				(*pixelTex)[i][j] = LAVENDER;
				break;
			case 18:
				(*pixelTex)[i][j] = MAROON;
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

bool turnAnt(Ant * ant, int tile)
{
	ant->currenttile = tile;
	ant->heading = ant->heading + ant->turn[ant->currenttile];
	if (ant->heading > 270) ant->heading = ant->heading - 360;
	if (ant->heading < 0) ant->heading = ant->heading + 360;
	ant->lasttile = tile;
	switch (ant->turn[ant->currenttile]) {
	case 90:
		printf("jobb\n");
		break;
	case -90:
		printf("bal\n");
		break;
	}
	return true;
}

bool antgorithm(Uint32*** pixelTex, Ant* ant, int const SCREEN_WIDTH, int const SCREEN_HEIGHT, int const SCALE, int const SPACING, int ANTMARGIN) {
	//printf("%u\n%u\n%u\n\n", ((*pixels)[(ant->x - SCALE - SPACING + ANTMARGIN) + ((ant->y - SCALE - SPACING + ANTMARGIN)*SCREEN_WIDTH)]), BLACK, 0xFF000000);
	int xpos = (ant->x - SCALE - SPACING + ANTMARGIN);
	int ypos = (ant->y - SCALE - SPACING + ANTMARGIN);

	if (xpos < 0 || xpos > SCREEN_WIDTH || ypos < 0 || ypos > SCREEN_HEIGHT)return false;

	switch ((*pixelTex)[xpos][ypos]) {
	case BLACK:
		return turnAnt(ant, 0);
	case MAROON:
		return turnAnt(ant, 0);
	case BROWN:
		return turnAnt(ant, 1);
	case OLIVE:
		return turnAnt(ant, 2);
	case TEAL:
		return turnAnt(ant, 3);
	case NAVY:
		return turnAnt(ant, 4);
	case RED:
		return turnAnt(ant, 5);
	case ORANGE:
		return turnAnt(ant, 6);
	case YELLOW:
		return turnAnt(ant, 7);
	case LIME:
		return turnAnt(ant, 8);
	case GREEN:
		return turnAnt(ant, 9);
	case CYAN:
		return turnAnt(ant, 10);
	case BLUE:
		return turnAnt(ant, 11);
	case PURPLE:
		return turnAnt(ant, 12);
	case MAGENTA:
		return turnAnt(ant, 13);
	case PINK:
		return turnAnt(ant, 14);
	case APRICOT:
		return turnAnt(ant, 15);
	case BEIGE:
		return turnAnt(ant, 16);
	case MINT:
		return turnAnt(ant, 17);
	case LAVENDER:
		return turnAnt(ant, 18);
	}
	return false;
}