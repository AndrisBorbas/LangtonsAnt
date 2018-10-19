#include <SDL.h>
#include <SDL_image.h>
#include <SDL2_gfxprimitives.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "everything.h"

void memset32(void * dest, Uint32 value, uintptr_t size)
{
	uintptr_t i;
	for (i = 0; i < (size & (~3)); i += 4)
	{
		memcpy(((char*)dest) + i, &value, 4);
	}
	for (; i < size; i++)
	{
		((char*)dest)[i] = ((char*)&value)[i & 3];
	}
}

Uint32 ftick(Uint32 ms, void *param)
{
	SDL_Event ev;
	ev.type = SDL_USEREVENT;
	SDL_PushEvent(&ev);
	return ms;
}

bool convertPixels(Uint32** pixels, Uint32*** pixelTex, int const SCREEN_WIDTH, int const SCREEN_HEIGHT) {
	for (int i = 0; i < SCREEN_WIDTH; i++)
	{
		for (int j = 0; j < SCREEN_HEIGHT; j++)
		{
			(*pixels)[i + j * SCREEN_WIDTH] = (*pixelTex)[i][j];
		}
	}
	return true;
}

bool drawText(SDL_Renderer* gRenderer, SDL_Surface* sStrings, TTF_Font* font, SDL_Texture** tStrings, SDL_Rect* lStrings, SDL_Rect button, char text[], SDL_Color color) {
	sStrings = TTF_RenderUTF8_Blended(font, text, color);
	*tStrings = SDL_CreateTextureFromSurface(gRenderer, sStrings);
	lStrings->x = (button.x + button.w / 2 - sStrings->w / 2);
	lStrings->y = (button.y + button.h / 2 - sStrings->h / 2);
	lStrings->w = sStrings->w;
	lStrings->h = sStrings->h;
	SDL_FreeSurface(sStrings);
}