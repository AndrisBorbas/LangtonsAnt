#include <SDL.h>
#include <SDL_image.h>
#include <SDL2_gfxprimitives.h>
#include <stdio.h>
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
}

bool isMouseinButton(int mouseX, int mouseY, Button button)
{
	if (button.x > mouseX)return false;
	if (button.x + button.width < mouseX)return false;
	if (button.y > mouseY)return false;
	if (button.y + button.height < mouseY)return false;
	return true;
}