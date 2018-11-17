#pragma once

#include "includes.h"

//Frees media and shuts down SDL
void close(Uint32**, Uint32***, SDL_Window*, SDL_Renderer*, SDL_Texture*, SDL_Texture*, SDL_Texture*);

//Starts up SDL and creates window
bool initSDL(SDL_Window**, SDL_Renderer**, SDL_Texture**, SDL_Texture**, SDL_Rect const );

//Initialize a texture
bool initTexture(SDL_Renderer**, SDL_Texture**, SDL_Rect const);

//Initializes the pixel arrays
bool initPixels(Uint32**, Uint32***, SDL_Rect const);

void save_texture(SDL_Renderer*, SDL_Texture*, const char*);