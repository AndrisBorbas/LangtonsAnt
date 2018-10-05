#include <SDL.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int SCALE = 2;

enum AntHeading
{
	North,
	East = 90,
	South = 180,
	West = 270
};

typedef struct Ant {
	int x, y, heading;
}Ant;

//Starts up SDL and creates window
void close(Uint32**, SDL_Window**, SDL_Renderer**, SDL_Texture**);

//Frees media and shuts down SDL
bool init(Uint32**, Uint32, SDL_Window**, SDL_Renderer**, SDL_Texture**);

//ant
void moveAnt(Uint32**, Ant*);

void moveAnt(Uint32** pixels, Ant* ant)
{
	ant->x = ant->x + SCALE;
	//printf("%d", ant->x);
	*pixels[ant->y * SCREEN_WIDTH + ant->x] = 0xFFFF0000;
}

bool init(Uint32** pixels, Uint32*** pixelarr, SDL_Window** gWindow, SDL_Renderer** gRenderer, SDL_Texture** gTexture)
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Create window
		*gWindow = SDL_CreateWindow("Langton's Ant", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (*gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Get window surface
			//gScreenSurface = SDL_GetWindowSurface(gWindow);

			//Initialize the renderer
			*gRenderer = SDL_CreateRenderer(*gWindow, -1, 0);

			//Initialize screen texture
			*gTexture = SDL_CreateTexture(*gRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);

			//Initialize pixel array
			*pixels = malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(**pixels));
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*pixels[500 * SCREEN_WIDTH + 500] = 0xFFFF0000;

			//Initialize 2D pixel texture array
			*pixelarr = (Uint32 ***)malloc(sizeof(Uint32 *) * SCREEN_WIDTH);
			*pixelarr[0] = (Uint32 **)malloc(sizeof(Uint32) * SCREEN_HEIGHT * SCREEN_WIDTH);
			for (int i = 0; i < SCREEN_WIDTH; i++)
				(*pixelarr)[i] = (**pixelarr + SCREEN_HEIGHT * i);

			//Override all pixels to white
			memset(*pixels, 255255, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
			memset(**pixelarr, 255255, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
		}
	}

	return success;
}

void close(Uint32** pixels, SDL_Window** gWindow, SDL_Renderer** gRenderer, SDL_Texture** gTexture)
{
	//Free up allocated pixel array
	free(*pixels);

	//Destroy texture
	SDL_DestroyTexture(*gTexture);
	*gTexture = NULL;

	//Destroy renderer
	SDL_DestroyRenderer(*gRenderer);
	*gRenderer = NULL;

	//Destroy window
	SDL_DestroyWindow(*gWindow);
	*gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

int main(int argc, char ** argv)
{
	Ant ant;
	ant.x = SCREEN_WIDTH / 2;
	ant.y = SCREEN_HEIGHT / 2;
	ant.heading = North;

	bool quit = false;

	bool leftMouseButtonDown = false;

	//The window we'll be rendering to
	SDL_Window *gWindow = NULL;

	//The pixel renderer
	SDL_Renderer *gRenderer = NULL;

	SDL_Texture *gTexture = NULL;
	Uint32 *pixels = NULL;
	Uint32 **pixelarr = NULL;

	//Start up SDL and create window
	if (!init(&pixels, &pixelarr, &gWindow, &gRenderer, &gTexture))
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		SDL_Event event;

		while (!quit)
		{


			SDL_WaitEvent(&event);

			switch (event.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_SPACE:
					moveAnt(&pixels, &ant);
					//pixels[ant.y * SCREEN_WIDTH + ant.x] = 0xFFFF0000;
					SDL_UpdateTexture(gTexture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));
					break;

				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT)
						leftMouseButtonDown = false;
					break;
				case SDL_MOUSEBUTTONDOWN:
					if (event.button.button == SDL_BUTTON_LEFT)
						leftMouseButtonDown = true;
				case SDL_MOUSEMOTION:
					if (leftMouseButtonDown)
					{
						//Get mouse position
						int mouseX = event.motion.x;
						int mouseY = event.motion.y;
						printf("X:%d Y:%d    ", mouseX, mouseY);

						//Set 2D texture
						pixelarr[mouseX][mouseY] = 0xFFFF0000;
						pixelarr[mouseX][mouseY + 1] = 0xFFFF0000;
						pixelarr[mouseX + 1][mouseY] = 0xFFFF0000;
						pixelarr[mouseX + 1][mouseY + 1] = 0xFFFF0000;

						//printf("%d  ", pixelarr[mouseX][mouseY]);

						//Convert 2D texture to 1D pixel array
						for (int x = 0; x < SCREEN_WIDTH; x++) {
							for (int y = 0; y < SCREEN_HEIGHT; y++) {
								pixels[x + y * SCREEN_WIDTH] = pixelarr[x][y];
							}
						}
						//pixels[mouseY * SCREEN_WIDTH + mouseX] = 0xFF00FF00;
						//printf("%d |", pixels[mouseY * SCREEN_WIDTH + mouseX]);
					}
					break;
				}
				SDL_UpdateTexture(gTexture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));
				SDL_RenderClear(gRenderer);
				SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
				SDL_RenderPresent(gRenderer);
			}
		}

		//Free resources and close SDL
		close(&pixels, &gWindow, &gRenderer, &gTexture);

		return 0;
	}
}