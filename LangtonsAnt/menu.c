#include "menu.h"

void drawMenu(TTF_Font** StartFont, TTF_Font** MenuFont, TTF_Font** InstructFont, TTF_Font** HelpFont, SDL_Texture** tStrings, SDL_Rect* lStrings,
	SDL_Window** gWindow, SDL_Renderer** gRenderer, SDL_Texture** tPixelTexture, SDL_Texture** tMainMenu, Settings settings, SDL_Rect HelpButton1, char help[5][14],
	SDL_Rect StartButton, SDL_Rect StartButtonStroke, SDL_Rect ResButton, SDL_Rect ResUp, SDL_Rect ResDown, SDL_Rect ScaleButton, SDL_Rect InstructButton)
{
	SDL_RenderClear(*gRenderer);
	//Draw menu background
	SDL_RenderCopy(*gRenderer, *tMainMenu, NULL, NULL);
	SDL_RenderPresent(*gRenderer);

	//Draw help texts
	for (int j = 0; j < 6; j++)
	{
		roundedBoxColor(*gRenderer, HelpButton1.x, HelpButton1.y, HelpButton1.x + HelpButton1.w, HelpButton1.y + HelpButton1.h, 6, altGRAYALPHA);
		drawTextintoButton(*gRenderer, *HelpFont, tStrings, lStrings, HelpButton1, help[j], TextDARKORANGE);
		SDL_RenderCopy(*gRenderer, *tStrings, NULL, lStrings);
		HelpButton1.y = HelpButton1.y + HelpButton1.h - 6;
	}

	//Draw Start button
	roundedBoxColor(*gRenderer, StartButtonStroke.x, StartButtonStroke.y, StartButtonStroke.x + StartButtonStroke.w, StartButtonStroke.y + StartButtonStroke.h, 12, altDARKWHITE);
	roundedBoxColor(*gRenderer, StartButton.x, StartButton.y, StartButton.x + StartButton.w, StartButton.y + StartButton.h, 12, altGRAY);
	drawTextintoButton(*gRenderer, *StartFont, tStrings, lStrings, StartButton, "Start", TextORANGE);
	SDL_RenderCopy(*gRenderer, *tStrings, NULL, lStrings);

	//Draw resolution button
	roundedBoxColor(*gRenderer, ResButton.x, ResButton.y, ResButton.x + ResButton.w, ResButton.y + ResButton.h, 12, altGRAY);
	char buff[sizeof(int) * 2 + 2];
	sprintf(buff, "%dx%d", settings.SCREEN.w, settings.SCREEN.h);
	drawTextintoButton(*gRenderer, *MenuFont, tStrings, lStrings, ResButton, &buff, TextORANGE);
	SDL_RenderCopy(*gRenderer, *tStrings, NULL, lStrings);

	//Draw resolution change buttons
	roundedBoxColor(*gRenderer, ResUp.x, ResUp.y, ResUp.x + ResUp.w, ResUp.y + ResUp.h, 6, altGRAY);
	filledTrigonColor(*gRenderer, ResUp.x + 6, ResUp.y + 6, ResUp.x + 6, ResUp.y + ResUp.h - 6, ResUp.x + ResUp.w - 6, ResUp.y + ResUp.h / 2, altDARKWHITE);
	roundedBoxColor(*gRenderer, ResDown.x, ResDown.y, ResDown.x + ResDown.w, ResDown.y + ResDown.h, 6, altGRAY);
	filledTrigonColor(*gRenderer, ResDown.x + ResDown.w - 6, ResDown.y + 6, ResDown.x + ResDown.w - 6, ResDown.y + ResDown.h - 6, ResDown.x + 6, ResDown.y + ResDown.h / 2, altDARKWHITE);

	//Draw scale button
	roundedBoxColor(*gRenderer, ScaleButton.x, ScaleButton.y, ScaleButton.x + ScaleButton.w, ScaleButton.y + ScaleButton.h, 6, altGRAY);
	char sbuff[sizeof(int) * 1 + 8];
	sprintf(sbuff, "Scale: %d", settings.SCALE);
	drawTextintoButton(*gRenderer, *MenuFont, tStrings, lStrings, ScaleButton, &sbuff, TextORANGE);
	SDL_RenderCopy(*gRenderer, *tStrings, NULL, lStrings);

	//Draw instructionset button
	roundedBoxColor(*gRenderer, InstructButton.x, InstructButton.y, InstructButton.x + InstructButton.w, InstructButton.y + InstructButton.h, 6, altGRAY);
	drawTextintoButton(*gRenderer, *InstructFont, tStrings, lStrings, InstructButton, settings.instructionset, TextORANGE);
	SDL_RenderCopy(*gRenderer, *tStrings, NULL, lStrings);

	//Render everything
	SDL_RenderPresent(*gRenderer);
}

void refreshMenu(SDL_Window** gWindow, SDL_Renderer** gRenderer, SDL_Texture** tPixelTexture, SDL_Texture** tMainMenu, SDL_Rect SCREEN, int Strokesize,
	SDL_Rect* StartButton, SDL_Rect* StartButtonStroke, SDL_Rect* ResButton, SDL_Rect* ResUp, SDL_Rect* ResDown, SDL_Rect* ScaleButton, SDL_Rect* InstructButton)
{
	//Resize window
	SDL_SetWindowSize(*gWindow, SCREEN.w, SCREEN.h);
	//Set buttons location on the resized window
	setButtons(SCREEN, Strokesize, StartButton, StartButtonStroke, ResButton, ResUp, ResDown, ScaleButton, InstructButton);
}

void setButtons(SDL_Rect SCREEN, int Strokesize, SDL_Rect* StartButton, SDL_Rect* StartButtonStroke,
	SDL_Rect* ResButton, SDL_Rect* ResUp, SDL_Rect* ResDown, SDL_Rect* ScaleButton, SDL_Rect* InstructButton)
{
	setStartButton(SCREEN, StartButton);
	setStartButtonStroke(Strokesize, StartButton, StartButtonStroke);
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

void setStartButtonStroke(int Strokesize, SDL_Rect* StartButton, SDL_Rect* StartButtonStroke)
{
	StartButtonStroke->w = StartButton->w + (Strokesize * 2);
	StartButtonStroke->h = StartButton->h + (Strokesize * 2);
	StartButtonStroke->x = StartButton->x - Strokesize;
	StartButtonStroke->y = StartButton->y - Strokesize;
}

void setResButton(SDL_Rect SCREEN, SDL_Rect* ResButton)
{
	if (SCREEN.w >= 1000 || SCREEN.h >= 1000)ResButton->w = 160;
	else ResButton->w = 144;
	ResButton->x = SCREEN.w / 2 + SCREEN.w / 4 - ResButton->w / 2;
	ResButton->y = SCREEN.h - ResButton->h - 25;
}

void setResUp(SDL_Rect SCREEN, SDL_Rect* ResButton, SDL_Rect* ResUp)
{
	ResUp->x = ResButton->x + ResButton->w + 8;
	ResUp->y = ResButton->y + 3;
}

void setResDown(SDL_Rect SCREEN, SDL_Rect* ResButton, SDL_Rect* ResDown)
{
	ResDown->x = ResButton->x - ResDown->w - 8;
	ResDown->y = ResButton->y + 3;
}

void setInstructButton(SDL_Rect  SCREEN, SDL_Rect* InstructButton)
{
	InstructButton->x = SCREEN.w / 2 - SCREEN.w / 4 - InstructButton->w / 2;
	InstructButton->y = SCREEN.h - InstructButton->h - 25;
}

void setScaleButton(SDL_Rect SCREEN, SDL_Rect* ScaleButton) 
{
	ScaleButton->x = SCREEN.w - ScaleButton->w - 25;
	ScaleButton->y = SCREEN.h / 2 - ScaleButton->h / 2;
}