#pragma once

#include "includes.h"

//Draw the main menu and every button
void drawMenu(TTF_Font**, TTF_Font**, TTF_Font**, TTF_Font**, SDL_Texture**, SDL_Rect**, SDL_Window**, const SDL_Renderer**, SDL_Texture**, SDL_Texture**,
	SDL_Rect const, SDL_Rect, char**, SDL_Rect, SDL_Rect, SDL_Rect, SDL_Rect, SDL_Rect, SDL_Rect, int, SDL_Rect, char*);

//Refresh the buttons size and position to match the current resolution
void refreshMenu(SDL_Window**, const SDL_Renderer**, SDL_Texture**, SDL_Texture**, SDL_Rect const, int,
	SDL_Rect*, SDL_Rect*, SDL_Rect*, SDL_Rect*, SDL_Rect*, SDL_Rect*, SDL_Rect*);

//Set all buttons location to match current resolution
void setButtons(SDL_Rect const, int, SDL_Rect*, SDL_Rect*, SDL_Rect*, SDL_Rect*, SDL_Rect*, SDL_Rect*, SDL_Rect*);
void setStartButton(SDL_Rect const, SDL_Rect*);
void setStartButtonStroke(SDL_Rect const, SDL_Rect*, SDL_Rect*);
void setResButton(SDL_Rect const, SDL_Rect*);
void setResUp(SDL_Rect const, SDL_Rect*, SDL_Rect*);
void setResDown(SDL_Rect const, SDL_Rect*, SDL_Rect*);
void setInstructButton(SDL_Rect const, SDL_Rect*);
void setScaleButton(SDL_Rect const, SDL_Rect*);