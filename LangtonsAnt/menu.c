#include "menu.h"

void refreshMenu(SDL_Window** gWindow, SDL_Renderer** gRenderer, SDL_Texture** tPixelTexture, SDL_Texture** tMainMenu, SDL_Rect const SCREEN, int Strokesize, SDL_Rect* StartButton, SDL_Rect* StartButtonStroke, SDL_Rect* ResButton, SDL_Rect* ResUp, SDL_Rect* ResDown)
{
	SDL_SetWindowSize(*gWindow, SCREEN.w, SCREEN.h);
	initTextures(*gRenderer, *tPixelTexture, *tMainMenu, SCREEN);

	tMainMenu = IMG_LoadTexture(*gRenderer, "MainMenu.png");
	if (!tMainMenu)
	{
		SDL_Log("Failed to open image: %s", IMG_GetError());
		exit(1);
	}

	SDL_Rect nSB500 = { 0,0,0,0 };
	SDL_Rect nSB720 = { 0,0,0,0 };
	SDL_Rect nSB900 = { 0,0,0,0 };
	SDL_Rect nSB960 = { 0,0,0,0 };
	SDL_Rect nSB1000 = { 0,0,0,0 };

	SDL_Rect nRB500 = { 0,0,0,0 };
	SDL_Rect nRB720 = { 0,0,0,0 };
	SDL_Rect nRB900 = { 0,0,0,0 };
	SDL_Rect nRB960 = { 0,0,0,0 };
	SDL_Rect nRB1000 = { 0,0,0,0 };

	switch (SCREEN.w) {

	case 500:
		setButtons(SCREEN, Strokesize, StartButton, nSB500, StartButtonStroke, ResButton, nRB500, ResUp, ResDown);
		break;
	case 720:
		setButtons(SCREEN, Strokesize, StartButton, nSB720, StartButtonStroke, ResButton, nRB720, ResUp, ResDown);
		break;
	case 900:
		setButtons(SCREEN, Strokesize, StartButton, nSB900, StartButtonStroke, ResButton, nRB900, ResUp, ResDown);
		break;
	case 960:
		setButtons(SCREEN, Strokesize, StartButton, nSB960, StartButtonStroke, ResButton, nRB960, ResUp, ResDown);
		break;
	case 1000:
		setButtons(SCREEN, Strokesize, StartButton, nSB1000, StartButtonStroke, ResButton, nRB1000, ResUp, ResDown);
		break;
	default:
		setButtons(SCREEN, Strokesize, StartButton, nSB900, StartButtonStroke, ResButton, nRB900, ResUp, ResDown);
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
	snprintf(buff, sizeof buff, "%d", SCREEN.w);
	drawTextintoButton(*gRenderer, sStrings, *MenuFont, tStrings, lStrings, *ResButton, buff, TextORANGE);
	SDL_RenderCopy(*gRenderer, *tStrings, NULL, lStrings);
	roundedBoxColor(*gRenderer, ResUp->x, ResUp->y, ResUp->x + ResUp->w, ResUp->y + ResUp->h, 6, altGRAY);
	roundedBoxColor(*gRenderer, ResDown->x, ResDown->y, ResDown->x + ResDown->w, ResDown->y + ResDown->h, 6, altGRAY);
	SDL_RenderPresent(*gRenderer);
}

void setButtons(SDL_Rect const SCREEN, int Strokesize, SDL_Rect* StartButton, SDL_Rect newStartButton, SDL_Rect* StartButtonStroke, SDL_Rect* ResButton, SDL_Rect newResButton, SDL_Rect* ResUp, SDL_Rect* ResDown)
{
	setStartButton(newStartButton, SCREEN, StartButton);
	setStartButtonStroke(SCREEN, Strokesize, StartButton, StartButtonStroke);
	setResButton(newResButton, SCREEN, StartButton, ResButton);
}

void setStartButton(SDL_Rect newStartButton, SDL_Rect const SCREEN, SDL_Rect* StartButton)
{
	StartButton->w = newStartButton.w;
	StartButton->h = newStartButton.h;
	StartButton->x = SCREEN.w / 2 - StartButton->w / 2;
	StartButton->y = newStartButton.y;
}

void setStartButtonStroke(SDL_Rect const SCREEN, int Strokesize, SDL_Rect* StartButton, SDL_Rect* StartButtonStroke)
{
	StartButtonStroke->w = StartButton->w + (Strokesize * 2);
	StartButtonStroke->h = StartButton->h + (Strokesize * 2);
	StartButtonStroke->x = StartButton->x - Strokesize;
	StartButtonStroke->y = StartButton->y - Strokesize;
}

void setResButton(SDL_Rect newResButton, SDL_Rect const SCREEN, SDL_Rect* StartButton, SDL_Rect* ResButton)
{
	ResButton->w = newResButton.w;
	ResButton->h = newResButton.h;
	ResButton->x = SCREEN.w / 2 + SCREEN.w / 4 - ResButton->w / 2 + newResButton.x;
	ResButton->y = SCREEN.h - ResButton->h - newResButton.y;
}