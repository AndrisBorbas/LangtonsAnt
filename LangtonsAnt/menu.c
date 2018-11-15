#include "menu.h"

void refreshMenu(SDL_Window** gWindow, SDL_Renderer** gRenderer, SDL_Texture** tPixelTexture, SDL_Texture** tMainMenu, SDL_Rect const SCREEN, int Strokesize, SDL_Rect* StartButton, SDL_Rect* StartButtonStroke, SDL_Rect* ResButton, SDL_Rect* ResUp, SDL_Rect* ResDown)
{
	SDL_SetWindowSize(*gWindow, SCREEN.w, SCREEN.h);
	initTextures(*gRenderer, *tPixelTexture, *tMainMenu, SCREEN);
	//initPixels();

	*tMainMenu = IMG_LoadTexture(*gRenderer, "MainMenu.png");
	if (!*tMainMenu)
	{
		SDL_Log("Failed to open image: %s", IMG_GetError());
		exit(1);
	}

	SDL_Point nSB500 = { 124,25 };
	SDL_Point nSB720 = { 234,25 };
	SDL_Point nSB900 = { 324,25 };
	SDL_Point nSB960 = { 354,25 };
	SDL_Point nSB1000 = { 374,25 };

	SDL_Point nRB500 = { 303,435 };
	SDL_Point nRB720 = { 468,655 };
	SDL_Point nRB900 = { 603,835 };
	SDL_Point nRB960 = { 648,895 };
	SDL_Point nRB1000 = { 670,935 };

	SDL_Point nRU500 = { 455,438 };
	SDL_Point nRU720 = { 620,658 };
	SDL_Point nRU900 = { 755,838 };
	SDL_Point nRU960 = { 800,898 };
	SDL_Point nRU1000 = { 838,938 };

	SDL_Point nRD500 = { 261,438 };
	SDL_Point nRD720 = { 426,658 };
	SDL_Point nRD900 = { 561,838 };
	SDL_Point nRD960 = { 606,898 };
	SDL_Point nRD1000 = { 628,938 };

	switch (SCREEN.w) {

	case 500:
		setButtons(SCREEN, Strokesize, StartButton, nSB500, StartButtonStroke, ResButton, nRB500, ResUp, nRU500, ResDown, nRD500);
		break;
	case 720:
		setButtons(SCREEN, Strokesize, StartButton, nSB720, StartButtonStroke, ResButton, nRB720, ResUp, nRU720, ResDown, nRD720);
		break;
	case 900:
		setButtons(SCREEN, Strokesize, StartButton, nSB900, StartButtonStroke, ResButton, nRB900, ResUp, nRU900, ResDown, nRD900);
		break;
	case 960:
		setButtons(SCREEN, Strokesize, StartButton, nSB960, StartButtonStroke, ResButton, nRB960, ResUp, nRU960, ResDown, nRD960);
		break;
	case 1000:
		setButtons(SCREEN, Strokesize, StartButton, nSB1000, StartButtonStroke, ResButton, nRB1000, ResUp, nRU1000, ResDown, nRD1000);
		break;
	default:
		setButtons(SCREEN, Strokesize, StartButton, nSB960, StartButtonStroke, ResButton, nRB960, ResUp, nRU960, ResDown, nRD960);
		break;
	}
}

void drawMenu(SDL_Surface** sStrings, TTF_Font** StartFont, TTF_Font** MenuFont, SDL_Texture** tStrings, SDL_Rect** lStrings, SDL_Window** gWindow, SDL_Renderer** gRenderer, SDL_Texture** tPixelTexture, SDL_Texture** tMainMenu, SDL_Rect const SCREEN, SDL_Rect* StartButton, SDL_Rect* StartButtonStroke, SDL_Rect* ResButton, SDL_Rect* ResUp, SDL_Rect* ResDown)
{
	SDL_Color TextDARKWHITE = { 221,221,221 }, TextORANGE = { 247,99,12 };
	SDL_RenderClear(*gRenderer);
	SDL_RenderCopy(*gRenderer, *tMainMenu, NULL, NULL);
	SDL_RenderPresent(*gRenderer);
	roundedBoxColor(*gRenderer, StartButtonStroke->x, StartButtonStroke->y, StartButtonStroke->x + StartButtonStroke->w, StartButtonStroke->y + StartButtonStroke->h, 12, altDARKWHITE);
	roundedBoxColor(*gRenderer, StartButton->x, StartButton->y, StartButton->x + StartButton->w, StartButton->y + StartButton->h, 12, altGRAY);
	drawTextintoButton(*gRenderer, sStrings, *StartFont, tStrings, lStrings, *StartButton, "Start", TextORANGE);
	SDL_RenderCopy(*gRenderer, *tStrings, NULL, lStrings);
	roundedBoxColor(*gRenderer, ResButton->x, ResButton->y, ResButton->x + ResButton->w, ResButton->y + ResButton->h, 6, altGRAY);
	char buff[sizeof(int) * 4 + 2];
	snprintf(buff, sizeof buff, "%dx%d", SCREEN.w, SCREEN.h);
	drawTextintoButton(*gRenderer, sStrings, *MenuFont, tStrings, lStrings, *ResButton, buff, TextORANGE);
	SDL_RenderCopy(*gRenderer, *tStrings, NULL, lStrings);
	roundedBoxColor(*gRenderer, ResUp->x, ResUp->y, ResUp->x + ResUp->w, ResUp->y + ResUp->h, 6, altGRAY);
	roundedBoxColor(*gRenderer, ResDown->x, ResDown->y, ResDown->x + ResDown->w, ResDown->y + ResDown->h, 6, altGRAY);
	SDL_RenderPresent(*gRenderer);
}

void setButtons(SDL_Rect const SCREEN, int Strokesize, SDL_Rect* StartButton, SDL_Point newStartButton, SDL_Rect* StartButtonStroke, SDL_Rect* ResButton, SDL_Point newResButton, SDL_Rect* ResUp, SDL_Point newResUp, SDL_Rect* ResDown, SDL_Point newResDown)
{
	setStartButton(newStartButton, SCREEN, StartButton);
	setStartButtonStroke(SCREEN, Strokesize, StartButton, StartButtonStroke);
	setResButton(newResButton, SCREEN, StartButton, ResButton, true);
	setResButton(newResUp, SCREEN, StartButton, ResUp, false);
	setResButton(newResDown, SCREEN, StartButton, ResDown, false);
}

void setStartButton(SDL_Point newStartButton, SDL_Rect const SCREEN, SDL_Rect* StartButton)
{
	StartButton->x = newStartButton.x;
	StartButton->y = newStartButton.y;
}

void setStartButtonStroke(SDL_Rect const SCREEN, int Strokesize, SDL_Rect* StartButton, SDL_Rect* StartButtonStroke)
{
	StartButtonStroke->w = StartButton->w + (Strokesize * 2);
	StartButtonStroke->h = StartButton->h + (Strokesize * 2);
	StartButtonStroke->x = StartButton->x - Strokesize;
	StartButtonStroke->y = StartButton->y - Strokesize;
}

void setResButton(SDL_Point newResButton, SDL_Rect const SCREEN, SDL_Rect* StartButton, SDL_Rect* ResButton, bool isRes)
{
	if (isRes) {
		if (SCREEN.w >= 1000 || SCREEN.h >= 1000)ResButton->w = 160;
	}
	ResButton->x = newResButton.x;
	ResButton->y = newResButton.y;
}