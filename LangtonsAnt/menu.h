#pragma once

#include "includes.h"
#include "Utilities.h"

//Draw the main menu and every button
void drawMenu(TTF_Font** StartFont, TTF_Font** MenuFont, TTF_Font** InstructFont, TTF_Font** HelpFont, SDL_Texture** tStrings, SDL_Rect* lStrings,
	SDL_Window** gWindow, SDL_Renderer** gRenderer, SDL_Texture** tPixelTexture, SDL_Texture** tMainMenu, SDL_Rect SCREEN, SDL_Rect HelpButton1, char help[5][14],
	SDL_Rect StartButton, SDL_Rect StartButtonStroke, SDL_Rect ResButton, SDL_Rect ResUp, SDL_Rect ResDown, SDL_Rect ScaleButton, int SCALE, SDL_Rect InstructButton, char* instructionset);

//Refresh the buttons size and position to match the current resolution
void refreshMenu(SDL_Window** gWindow, SDL_Renderer** gRenderer, SDL_Texture** tPixelTexture, SDL_Texture** tMainMenu, SDL_Rect SCREEN, int Strokesize,
	SDL_Rect* StartButton, SDL_Rect* StartButtonStroke, SDL_Rect* ResButton, SDL_Rect* ResUp, SDL_Rect* ResDown, SDL_Rect* ScaleButton, SDL_Rect* InstructButton);

//Set all buttons location to match current resolution
void setButtons(SDL_Rect SCREEN, int Strokesize, SDL_Rect* StartButton, SDL_Rect* StartButtonStroke,
	SDL_Rect* ResButton, SDL_Rect* ResUp, SDL_Rect* ResDown, SDL_Rect* ScaleButton, SDL_Rect* InstructButton);
void setStartButton(SDL_Rect const SCREEN, SDL_Rect* StartButton);
void setStartButtonStroke(int Strokesize, SDL_Rect* StartButton, SDL_Rect* StartButtonStroke);
void setResButton(SDL_Rect SCREEN, SDL_Rect* ResButton);
void setResUp(SDL_Rect SCREEN, SDL_Rect* ResButton, SDL_Rect* ResUp);
void setResDown(SDL_Rect SCREEN, SDL_Rect* ResButton, SDL_Rect* ResDown);
void setInstructButton(SDL_Rect  SCREEN, SDL_Rect* InstructButton);
void setScaleButton(SDL_Rect SCREEN, SDL_Rect* ScaleButton);