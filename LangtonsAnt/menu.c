﻿#include "menu.h"

void drawMenu(TTF_Font** StartFont, TTF_Font** MenuFont, TTF_Font** InstructFont, TTF_Font** HelpFont, SDL_Texture** tStrings, SDL_Rect** lStrings,
	SDL_Window** gWindow, const SDL_Renderer** gRenderer, SDL_Texture** tPixelTexture, SDL_Texture** tMainMenu, SDL_Rect const SCREEN, SDL_Rect HelpButton1, char help[5][14],
	SDL_Rect StartButton, SDL_Rect StartButtonStroke, SDL_Rect ResButton, SDL_Rect ResUp, SDL_Rect ResDown, SDL_Rect ScaleButton, int SCALE, SDL_Rect InstructButton, char* instructionset)
{
	SDL_RenderClear(*gRenderer);
	SDL_RenderCopy(*gRenderer, *tMainMenu, NULL, NULL);
	SDL_RenderPresent(*gRenderer);

	for (int j = 0; j < 5; j++)
	{
		roundedBoxColor(*gRenderer, HelpButton1.x, HelpButton1.y, HelpButton1.x + HelpButton1.w, HelpButton1.y + HelpButton1.h, 6, altGRAYALPHA);
		drawTextintoButton(*gRenderer, *HelpFont, tStrings, lStrings, HelpButton1, help[j], TextDARKORANGE);
		SDL_RenderCopy(*gRenderer, *tStrings, NULL, lStrings);
		HelpButton1.y = HelpButton1.y + HelpButton1.h - 6;
	}

	roundedBoxColor(*gRenderer, StartButtonStroke.x, StartButtonStroke.y, StartButtonStroke.x + StartButtonStroke.w, StartButtonStroke.y + StartButtonStroke.h, 12, altDARKWHITE);
	roundedBoxColor(*gRenderer, StartButton.x, StartButton.y, StartButton.x + StartButton.w, StartButton.y + StartButton.h, 12, altGRAY);
	drawTextintoButton(*gRenderer, *StartFont, tStrings, lStrings, StartButton, "Start", TextORANGE);
	SDL_RenderCopy(*gRenderer, *tStrings, NULL, lStrings);

	roundedBoxColor(*gRenderer, ResButton.x, ResButton.y, ResButton.x + ResButton.w, ResButton.y + ResButton.h, 12, altGRAY);
	char buff[sizeof(int) * 2 + 2];
	snprintf(buff, sizeof buff, "%dx%d", SCREEN.w, SCREEN.h);
	drawTextintoButton(*gRenderer, *MenuFont, tStrings, lStrings, ResButton, &buff, TextORANGE);
	SDL_RenderCopy(*gRenderer, *tStrings, NULL, lStrings);

	roundedBoxColor(*gRenderer, ResUp.x, ResUp.y, ResUp.x + ResUp.w, ResUp.y + ResUp.h, 6, altGRAY);
	filledTrigonColor(*gRenderer, ResUp.x + 6, ResUp.y + 6, ResUp.x + 6, ResUp.y + ResUp.h - 6, ResUp.x + ResUp.w - 6, ResUp.y + ResUp.h / 2, altDARKWHITE);
	roundedBoxColor(*gRenderer, ResDown.x, ResDown.y, ResDown.x + ResDown.w, ResDown.y + ResDown.h, 6, altGRAY);
	filledTrigonColor(*gRenderer, ResDown.x + ResDown.w - 6, ResDown.y + 6, ResDown.x + ResDown.w - 6, ResDown.y + ResDown.h - 6, ResDown.x + 6, ResDown.y + ResDown.h / 2, altDARKWHITE);

	roundedBoxColor(*gRenderer, ScaleButton.x, ScaleButton.y, ScaleButton.x + ScaleButton.w, ScaleButton.y + ScaleButton.h, 6, altGRAY);
	char sbuff[sizeof(int) * 1 + 8];
	snprintf(sbuff, sizeof sbuff, "Scale: %d", SCALE);
	drawTextintoButton(*gRenderer, *MenuFont, tStrings, lStrings, ScaleButton, &sbuff, TextORANGE);
	SDL_RenderCopy(*gRenderer, *tStrings, NULL, lStrings);

	roundedBoxColor(*gRenderer, InstructButton.x, InstructButton.y, InstructButton.x + InstructButton.w, InstructButton.y + InstructButton.h, 6, altGRAY);
	drawTextintoButton(*gRenderer, *InstructFont, tStrings, lStrings, InstructButton, &instructionset, TextORANGE);
	SDL_RenderCopy(*gRenderer, *tStrings, NULL, lStrings);

	SDL_RenderPresent(*gRenderer);
}

void refreshMenu(SDL_Window** gWindow, const SDL_Renderer** gRenderer, SDL_Texture** tPixelTexture, SDL_Texture** tMainMenu, SDL_Rect const SCREEN, int Strokesize,
	SDL_Rect* StartButton, SDL_Rect* StartButtonStroke, SDL_Rect* ResButton, SDL_Rect* ResUp, SDL_Rect* ResDown, SDL_Rect* ScaleButton, SDL_Rect* InstructButton)
{
	SDL_SetWindowSize(*gWindow, SCREEN.w, SCREEN.h);

	setButtons(SCREEN, Strokesize, StartButton, StartButtonStroke, ResButton, ResUp, ResDown, ScaleButton, InstructButton);
}

void setButtons(SDL_Rect const SCREEN, int Strokesize, SDL_Rect* StartButton, SDL_Rect* StartButtonStroke,
	SDL_Rect* ResButton, SDL_Rect* ResUp, SDL_Rect* ResDown, SDL_Rect* ScaleButton, SDL_Rect* InstructButton)
{
	setStartButton(SCREEN, StartButton);
	setStartButtonStroke(SCREEN, Strokesize, StartButton, StartButtonStroke);
	setResButton(SCREEN, ResButton);
	setResUp(SCREEN, ResButton, ResUp);
	setResDown(SCREEN, ResButton, ResDown);
	setInstructButton(SCREEN, InstructButton);
	setScaleButton(SCREEN, ScaleButton);
}

void setStartButton(SDL_Rect const SCREEN, SDL_Rect* StartButton)
{
	StartButton->x = SCREEN.w / 2 - StartButton->w / 2;
}

void setStartButtonStroke(SDL_Rect const SCREEN, int Strokesize, SDL_Rect* StartButton, SDL_Rect* StartButtonStroke)
{
	StartButtonStroke->w = StartButton->w + (Strokesize * 2);
	StartButtonStroke->h = StartButton->h + (Strokesize * 2);
	StartButtonStroke->x = StartButton->x - Strokesize;
	StartButtonStroke->y = StartButton->y - Strokesize;
}

void setResButton(SDL_Rect const SCREEN, SDL_Rect* ResButton)
{
	if (SCREEN.w >= 1000 || SCREEN.h >= 1000)ResButton->w = 160;
	else ResButton->w = 144;
	ResButton->x = SCREEN.w / 2 + SCREEN.w / 4 - ResButton->w / 2;
	ResButton->y = SCREEN.h - ResButton->h - 25;
}

void setResUp(SDL_Rect const SCREEN, SDL_Rect* ResButton, SDL_Rect* ResUp)
{
	ResUp->x = ResButton->x + ResButton->w + 8;
	ResUp->y = ResButton->y + 3;
}

void setResDown(SDL_Rect const SCREEN, SDL_Rect* ResButton, SDL_Rect* ResDown)
{
	ResDown->x = ResButton->x - ResDown->w - 8;
	ResDown->y = ResButton->y + 3;
}

void setInstructButton(SDL_Rect const SCREEN, SDL_Rect* InstructButton)
{
	InstructButton->x = SCREEN.w / 2 - SCREEN.w / 4 - InstructButton->w / 2;
	InstructButton->y = SCREEN.h - InstructButton->h - 25;
}

void setScaleButton(SDL_Rect const SCREEN, SDL_Rect* ScaleButton) 
{
	ScaleButton->x = SCREEN.w - ScaleButton->w - 25;
	ScaleButton->y = SCREEN.h / 2 - ScaleButton->h / 2;
}