#pragma once

#include "includes.h"
#include "colors.h"

//Frees media and shuts down SDL
void close(Uint32**, Uint32***, SDL_Window**, SDL_Renderer**, SDL_Texture**);

//Starts up SDL and creates window
bool initSDL(SDL_Window**, SDL_Renderer**, SDL_Texture**, SDL_Texture**, int const, int const);

//Initializes the pixel arrays
bool initPixels(Uint32**, Uint32***, int const, int const);