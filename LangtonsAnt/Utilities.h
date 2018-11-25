#pragma once

#include "includes.h"

//memset with 32 bit unsigned integer
void memset32(void * dest, Uint32 value, uintptr_t size);

//Base tickrate of the simulation
Uint32 ftick(Uint32 ms, void *param);

//Converts pixel texture to sdl pixel format
void convertPixels(Uint32** pixels, Uint32*** pixelTex, SDL_Rect SCREEN);

//Draw text into the middle of a button
void drawTextintoButton(SDL_Renderer* gRenderer, TTF_Font* font, SDL_Texture** tStrings, SDL_Rect* lStrings, SDL_Rect button, char* text, SDL_Color color);

//Loads the configuration into int variables
void loadintFromConfig(FILE* wDefConf, char* buffer, int* variable, char* variableName);

//Loads the configuration into char variables
void loadcharFromConfig(FILE* wDefConf, char* buffer, char* variable, char* variableName);

//Load everything from the config file
void loadConfig(FILE* file, Settings* settings, int* instructnum);