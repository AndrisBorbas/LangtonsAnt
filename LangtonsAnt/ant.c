#include "ant.h"

bool moveAnt(Uint32*** pixelTex, Ant* ant, int* lepes, SDL_Rect const SCREEN, int const SCALE, int const SPACING, int const ANTMARGIN, int instructnum, char* instructionset)
{
	(*lepes)++;

#ifdef DEBUG
	printf("%d. lepes: ", *lepes);
#endif



	if (!antgorithm(pixelTex, ant, SCREEN, SCALE, SPACING, ANTMARGIN))return false;

	if (ant->lasttile == instructnum-1)ant->lasttile = 18;

	//color last location
	for (int i = (ant->x - SCALE); i < (ant->x + SCALE - SPACING); i++)
	{
		for (int j = (ant->y - SCALE); j < (ant->y + SCALE - SPACING); j++)
		{
			if (i<0 || i>=SCREEN.w || j<0 || j>=SCREEN.h)
			{
				printf("ERROR: last location out of bounds at x=%d, y=%d.\n", i, j);
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

	for (int i = (ant->x - SCALE + ANTMARGIN); i < (ant->x + SCALE - SPACING - ANTMARGIN); i++)
	{
		for (int j = (ant->y - SCALE + ANTMARGIN); j < (ant->y + SCALE - SPACING - ANTMARGIN); j++)
		{
			if (i<0 || i>=SCREEN.w || j<0 || j>=SCREEN.h)
			{
				printf("ERROR: ant out of bounds at x=%d, y=%d.\n", i, j);
				return false;
			}
			(*pixelTex)[i][j] = WHITE;
		}
	}
	return true;
}

bool turnAnt(Ant* ant, int tile)
{
	ant->currenttile = tile;
	ant->heading = ant->heading + ant->turn[ant->currenttile];
	if (ant->heading > 270) ant->heading = ant->heading - 360;
	if (ant->heading < 0) ant->heading = ant->heading + 360;
	ant->lasttile = tile;
#ifdef DEBUG
	switch (ant->turn[ant->currenttile]) {
	case 90:
		printf("jobb\n");
		break;
	case -90:
		printf("bal\n");
		break;
	case 0:
		printf("elore\n");
		break;
	case 180:
		printf("vissza\n");
		break;
	}
#endif
	return true;
}

bool antgorithm(Uint32*** pixelTex, Ant* ant, SDL_Rect const SCREEN, int const SCALE, int const SPACING, int ANTMARGIN) {
	//printf("%u\n%u\n%u\n\n", ((*pixels)[(ant->x - SCALE - SPACING + ANTMARGIN) + ((ant->y - SCALE - SPACING + ANTMARGIN)*SCREEN_WIDTH)]), BLACK, 0xFF000000);
	int xpos = (ant->x - SCALE - SPACING + ANTMARGIN);
	int ypos = (ant->y - SCALE - SPACING + ANTMARGIN);

	if (xpos < 0 || xpos > SCREEN.w || ypos < 0 || ypos > SCREEN.h)
	{
		printf("Error: antgorithm out of bounds at x=%d, y=%d.\nSDL Error: %s", xpos, ypos, SDL_GetError());
		return false;
	}

	switch ((*pixelTex)[xpos][ypos]) 
	{
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
	printf("Error: antgorithm couldn't find matching color.\nSDL Error: %s", SDL_GetError());
	return false;
}