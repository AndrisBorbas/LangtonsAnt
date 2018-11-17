#pragma once

#include "includes.h"

//millisecond based delay
int delay(int number_of_milliseconds);

//memset with 32 bit unsigned integer
void memset32(void*, Uint32, uintptr_t);

//Converts pixel texture to sdl pixel format
bool convertPixels(Uint32**, Uint32***, SDL_Rect const);

//Loads the configuration into int variables
void loadintFromConfig(FILE*, char*, int*, char*);

//Loads the configuration into char variables
void loadcharFromConfig(FILE*, char*, char*, char*);

//Base tickrate of the simulation
Uint32 ftick(Uint32, void*);

//Draw text into the middle of a button
void drawTextintoButton(SDL_Renderer*, SDL_Surface*, TTF_Font*, SDL_Texture**, SDL_Rect*, SDL_Rect, char, SDL_Color);

