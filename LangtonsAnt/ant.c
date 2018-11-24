#include "ant.h"

bool moveAnt(Uint32*** pixelTex, Ant* ant, int* lepes, Settings settings, int instructnum, FILE* fAntOut)
{
	(*lepes)++;

#ifdef Debug
	printf("%d. lepes: ", *lepes);
#endif

	//Print the number of the step
	fprintf(fAntOut, "%d. lepes: ", *lepes);

	//Rotate ant based on the tile under the ant
	if (!antgorithm(pixelTex, ant, settings, fAntOut))return false;

	//Wrap number of colors based on number of instructions in the instructionset
	if (ant->lasttile == instructnum-1)ant->lasttile = 18;

	//color last location
	for (int i = (ant->x - settings.SCALE); i < (ant->x + settings.SCALE - settings.SPACING); i++)
	{
		for (int j = (ant->y - settings.SCALE); j < (ant->y + settings.SCALE - settings.SPACING); j++)
		{
			if (i<0 || i>= settings.SCREEN.w || j<0 || j>= settings.SCREEN.h)
			{
				SDL_Log("ERROR: last location out of bounds at x=%d, y=%d.\n", i, j);
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

	//Move the ant to the next location based on its heading
	switch (ant->heading)
	{
	case UP:
		ant->y = ant->y - settings.SCALE * 2;
		break;
	case DOWN:
		ant->y = ant->y + settings.SCALE * 2;
		break;
	case RIGHT:
		ant->x = ant->x + settings.SCALE * 2;
		break;
	case LEFT:
		ant->x = ant->x - settings.SCALE * 2;
		break;
	default:
		break;
	}

	//Color the tile white where the ant is
	for (int i = (ant->x - settings.SCALE + settings.ANTMARGIN); i < (ant->x + settings.SCALE - settings.SPACING - settings.ANTMARGIN); i++)
	{
		for (int j = (ant->y - settings.SCALE + settings.ANTMARGIN); j < (ant->y + settings.SCALE - settings.SPACING - settings.ANTMARGIN); j++)
		{
			if (i<0 || i>= settings.SCREEN.w || j<0 || j>= settings.SCREEN.h)
			{
				SDL_Log("ERROR: ant out of bounds at x=%d, y=%d.\n", i, j);
				return false;
			}
			(*pixelTex)[i][j] = WHITE;
		}
	}
	return true;
}

bool turnAnt(Ant* ant, int tile, FILE* fAntOut)
{
	ant->lasttile = tile;
	ant->heading = ant->heading + ant->turn[ant->lasttile];
	//Wrap ant heading to not over or underflow 360 or 0 degrees
	if (ant->heading > 270) ant->heading = ant->heading - 360;
	if (ant->heading < 0) ant->heading = ant->heading + 360;
#ifdef Debug
	switch (ant->turn[ant->lasttile]) {
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
	
	//Print the current steps executed instruction
	switch (ant->turn[ant->lasttile]) {
	case 90:
		fprintf(fAntOut, "jobb\n");
		break;
	case -90:
		fprintf(fAntOut, "bal\n");
		break;
	case 0:
		fprintf(fAntOut, "elore\n");
		break;
	case 180:
		fprintf(fAntOut, "hatra\n");
		break;
	}
	return true;
}

bool antgorithm(Uint32*** pixelTex, Ant* ant, Settings settings, FILE* fAntOut)
{
	int xpos = (ant->x - settings.SCALE - settings.SPACING + settings.ANTMARGIN);
	int ypos = (ant->y - settings.SCALE - settings.SPACING + settings.ANTMARGIN);

	if (xpos < 0 || xpos > settings.SCREEN.w || ypos < 0 || ypos > settings.SCREEN.h)
	{
		SDL_Log("Error: antgorithm out of bounds at x=%d, y=%d.\nSDL Error: %s", xpos, ypos, SDL_GetError());
		return false;
	}

	//Execute the correct instruction based on tile color
	switch ((*pixelTex)[xpos][ypos]) 
	{
	case BLACK:
		return turnAnt(ant, 0, fAntOut);
	case MAROON:
		return turnAnt(ant, 0, fAntOut);
	case BROWN:
		return turnAnt(ant, 1, fAntOut);
	case OLIVE:
		return turnAnt(ant, 2, fAntOut);
	case TEAL:
		return turnAnt(ant, 3, fAntOut);
	case NAVY:
		return turnAnt(ant, 4, fAntOut);
	case RED:
		return turnAnt(ant, 5, fAntOut);
	case ORANGE:
		return turnAnt(ant, 6, fAntOut);
	case YELLOW:
		return turnAnt(ant, 7, fAntOut);
	case LIME:
		return turnAnt(ant, 8, fAntOut);
	case GREEN:
		return turnAnt(ant, 9, fAntOut);
	case CYAN:
		return turnAnt(ant, 10, fAntOut);
	case BLUE:
		return turnAnt(ant, 11, fAntOut);
	case PURPLE:
		return turnAnt(ant, 12, fAntOut);
	case MAGENTA:
		return turnAnt(ant, 13, fAntOut);
	case PINK:
		return turnAnt(ant, 14, fAntOut);
	case APRICOT:
		return turnAnt(ant, 15, fAntOut);
	case BEIGE:
		return turnAnt(ant, 16, fAntOut);
	case MINT:
		return turnAnt(ant, 17, fAntOut);
	case LAVENDER:
		return turnAnt(ant, 18, fAntOut);
	}
	SDL_Log("Error: antgorithm couldn't find matching color.\nSDL Error: %s", SDL_GetError());
	return false;
}

void convertToTurns(char* instructionset, Ant* ant)
{
	for (int i = 0; i < 19; i++)
	{
		if (instructionset[i] == '\0')break;
		switch (instructionset[i])
		{
		case 'R':
		case 'r':
			ant->turn[i] = 90;
			break;
		case 'L':
		case 'l':
			ant->turn[i] = -90;
			break;
		case 'N':
		case 'n':
			ant->turn[i] = 0;
			break;
		case 'U':
		case 'u':
			ant->turn[i] = 180;
			break;
		}
	}
}