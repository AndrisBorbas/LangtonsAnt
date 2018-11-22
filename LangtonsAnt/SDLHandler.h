#pragma once

#include "includes.h"
#include "Utilities.h"

//Starts up SDL and creates window
bool initSDL(SDL_Window** gWindow, SDL_Renderer** gRenderer, SDL_Texture** tPixelTexture, SDL_Texture** tMainMenu, SDL_Rect SCREEN);

//Initialize a texture
void initTexture(SDL_Renderer** gRenderer, SDL_Texture** tTexture, SDL_Rect SCREEN);

//Initializes the pixel arrays
void initPixels(Uint32** pixels, Uint32*** pixelTex, SDL_Rect SCREEN);

//Frees media and shuts down SDL
void close(Uint32** pixels, Uint32*** pixelTex, SDL_Window* gWindow, SDL_Renderer* gRenderer, SDL_Texture* tPixelTexture, SDL_Texture* tMainMenu, SDL_Texture* tStrings);

//Save current run to BMP
void save_texture(SDL_Renderer* gRenderer, SDL_Texture* tTexture, const char *filename);