#include "SDLHandler.h"

bool initSDL(SDL_Window** gWindow, SDL_Renderer** gRenderer, SDL_Texture** tPixelTexture, SDL_Texture** tMainMenu, SDL_Rect const SCREEN)
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	else
	{
		//Create window
		*gWindow = SDL_CreateWindow("Langton's Ant", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN.w, SCREEN.h, SDL_WINDOW_SHOWN);
		if (!*gWindow)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			return false;
		}
		else
		{
			//Initialize the renderer
			*gRenderer = SDL_CreateRenderer(*gWindow, -1, 0);

			//initTexture(gRenderer, tPixelTexture, SCREEN);
			initTexture(gRenderer, tMainMenu, SCREEN);

			//Initialize the font engine
			TTF_Init();
		}
	}
#ifdef __Win32__
	freopen("CON", "w", stdout);
	freopen("CON", "w", stderr);
#endif
	return true;
}

bool initTexture(const SDL_Renderer** gRenderer, SDL_Texture** tTexture, SDL_Rect const SCREEN) 
{
	//Initialize screen texture
	*tTexture = SDL_CreateTexture(*gRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SCREEN.w, SCREEN.h);
}

bool initPixels(Uint32** pixels, Uint32*** pixelTex, SDL_Rect const SCREEN) 
{
	if (*pixels)free(*pixels);
	if (*pixelTex)free(*pixelTex);

	//Initialize pixel array
	*pixels = malloc(SCREEN.w * SCREEN.h * sizeof(**pixels));

	//Initialize 2D pixel texture array
	*pixelTex = (Uint32 ***)malloc(sizeof(Uint32 *) * SCREEN.w);
	*pixelTex[0] = (Uint32 **)malloc(sizeof(Uint32) * SCREEN.h * SCREEN.w);
	for (int i = 0; i < SCREEN.w; i++)
		(*pixelTex)[i] = (**pixelTex + SCREEN.h * i);

	//Override all pixels
	memset32(*pixels, BLACK, SCREEN.w * SCREEN.h * sizeof(Uint32));
	memset32(**pixelTex, BLACK, SCREEN.w * SCREEN.h * sizeof(Uint32));
	return true;
}

void close(Uint32** pixels, Uint32*** pixelTex, SDL_Window* gWindow, SDL_Renderer* gRenderer, SDL_Texture* tPixelTexture, SDL_Texture* tMainMenu, SDL_Texture* tStrings, FILE* fAntOut)
{
	fprintf(fAntOut, "This run was sponsored by ant gang.");
	//close output file
	fclose(fAntOut);
	//fcloseall();

	//Free up allocated pixel arrays
	free(*pixels);
	free(*pixelTex[0]);
	free(*pixelTex);

	//Destroy textures
	SDL_DestroyTexture(tStrings);
	SDL_DestroyTexture(tPixelTexture);
	SDL_DestroyTexture(tMainMenu);
	tMainMenu = NULL;
	tPixelTexture = NULL;

	//Destroy renderer
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;

	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}