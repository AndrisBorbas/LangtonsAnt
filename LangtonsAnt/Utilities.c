#include "Utilities.h"

int delay(int number_of_milliseconds) 
{
	clock_t start_time = clock();

	while (clock() < start_time + number_of_milliseconds)
		;
	return clock();
}

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

void convertPixels(Uint32** pixels, Uint32*** pixelTex, SDL_Rect SCREEN) 
{
	for (int i = 0; i < SCREEN.w; i++)
	{
		for (int j = 0; j < SCREEN.h; j++)
		{
			(*pixels)[i + j * SCREEN.w] = (*pixelTex)[i][j];
		}
	}
}

void drawTextintoButton(SDL_Renderer* gRenderer, TTF_Font* font, SDL_Texture** tStrings, SDL_Rect* lStrings, SDL_Rect button, char* text, SDL_Color color) 
{
	SDL_Surface* sStrings = TTF_RenderUTF8_Blended(font, text, color);
	*tStrings = SDL_CreateTextureFromSurface(gRenderer, sStrings);
	lStrings->x = (button.x + button.w / 2 - sStrings->w / 2);
	lStrings->y = (button.y + button.h / 2 - sStrings->h / 2);
	lStrings->w = sStrings->w;
	lStrings->h = sStrings->h;
	SDL_FreeSurface(sStrings);
}

void loadintFromConfig(FILE* file, char* buffer, int* variable, char* variableName) 
{
	char ctemp[52] = "#";
	strcat(ctemp, variableName);
	int temp=0;
	if (strcmp(buffer, ctemp) == 0)
	{
		fscanf(file, "%s%d", &buffer, &temp);
#ifdef Debug
		printf("%d\n", temp);
#endif
		*variable = temp;
	}
}

void loadcharFromConfig(FILE* file, char* buffer, char* variable, char* variableName)
{
	char ctemp[52] = "#";
	strcat(ctemp, variableName);
	char temp[52] = "";
	if (strcmp(buffer, ctemp) == 0)
	{
		fscanf(file, "%s%s", &buffer, &temp);
#ifdef Debug
		printf("%s\n", temp);
#endif
		strcpy(variable, temp);
	}
}

void loadConfig(FILE* file, SDL_Rect* SCREEN, int* SCALE, int* SPACING, int* ANTMARGIN, int* MSTICK, char* instructionset, int* instructnum) 
{
	//Buffer for reading from file
	char buffer[52] = "";

	if (file != NULL)
	{
		//Load values from config file
		while ((strstr(buffer, "endconfig;")) == NULL)
		{
			fscanf(file, "%s", &buffer);
			if (buffer[0] == '/')continue;

			if (buffer[0] == '#')
			{
				loadintFromConfig(file, buffer, &SCREEN->w, "SCREEN_WIDTH");
				loadintFromConfig(file, buffer, &SCREEN->h, "SCREEN_HEIGHT");
				loadintFromConfig(file, buffer, SCALE, "SCALE");
				loadintFromConfig(file, buffer, SPACING, "SPACING");
				loadintFromConfig(file, buffer, ANTMARGIN, "ANTMARGIN");
				loadintFromConfig(file, buffer, MSTICK, "MSTICK");
				loadcharFromConfig(file, buffer, instructionset, "INSTRUCTIONSET");
			}
		}
	}
	fclose(file);

	//Number of instructions
	*instructnum = strlen(instructionset);
}