#include <SDL.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

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

//Screen dimension constants
volatile int SCREEN_WIDTH = 960;
volatile int SCREEN_HEIGHT = 960;
volatile int SCALE = 10;
volatile int SPACING = 1;
volatile int ANTMARGIN = 1;

enum AntHeading
{
	UP = 0,
	RIGHT = 90,
	DOWN = 180,
	LEFT = 270
};

enum HEXARGB
{
	WHITE = 0xFFFFFFFF,
	DARKWHITE = 0xFFDDDDDD,
	GRAY = 0xFF242424,
	BLACK = 0xFF000000
};

typedef struct Ant
{
	int x, y, heading, lasttile;
}Ant;

//Starts up SDL and creates window
void close(Uint32**, SDL_Window**, SDL_Renderer**, SDL_Texture**);

//Frees media and shuts down SDL
bool init(Uint32**, Uint32, SDL_Window**, SDL_Renderer**, SDL_Texture**);

//meow
void antgorithm(Uint32**, Ant*);

//move ant and invert past location
void moveAnt(Uint32**, Ant*);

void moveAnt(Uint32** pixels, Ant* ant)
{
	antgorithm(pixels, ant);
	//printf("%d  %d\n", ant->y, ant->y - SCALE / 2 + SPACING);
	//(*pixels)[(ant->x - SCALE - SPACING + ANTMARGIN) + (ant->y - SCALE - SPACING + ANTMARGIN) * SCREEN_WIDTH] = 0xFF00FF00;

	//invert last location
	for (int i = (ant->x - SCALE); i < (ant->x + SCALE - SPACING); i++)
	{
		for (int j = (ant->y - SCALE); j < (ant->y + SCALE - SPACING); j++)
		{
			switch (ant->lasttile)
			{
			case GRAY:
				(*pixels)[i + j * SCREEN_WIDTH] = DARKWHITE;
				break;
			case DARKWHITE:
				(*pixels)[i + j * SCREEN_WIDTH] = GRAY;
				break;
			default:
				break;
			}
		}
	}



	//printf("%d ", ant->heading);
	switch (ant->heading)
	{
	case UP:
		ant->y = ant->y - SCALE * 2;
		break;
	case DOWN:
		ant->y = ant->y + SCALE * 2;
		break;
	case RIGHT:
		ant->x = ant->x + SCALE * 2;
		break;
	case LEFT:
		ant->x = ant->x - SCALE * 2;
		break;
	default:
		break;
	}
	//printf("%d\n", ant->heading);
	for (int i = (ant->x - SCALE + ANTMARGIN); i < (ant->x + SCALE - SPACING - ANTMARGIN); i++)
	{
		for (int j = (ant->y - SCALE + ANTMARGIN); j < (ant->y + SCALE - SPACING - ANTMARGIN); j++)
		{
			(*pixels)[i + j * SCREEN_WIDTH] = 0xFFFF0000;
		}
	}

}

void antgorithm(Uint32** pixels, Ant* ant) {
	//printf("%u\n%u\n%u\n\n", ((*pixels)[(ant->x - SCALE - SPACING + ANTMARGIN) + ((ant->y - SCALE - SPACING + ANTMARGIN)*SCREEN_WIDTH)]), BLACK, 0xFF000000);
	if (((*pixels)[(ant->x - SCALE - SPACING + ANTMARGIN) + ((ant->y - SCALE - SPACING + ANTMARGIN)*SCREEN_WIDTH)]) == BLACK || ((*pixels)[(ant->x - SCALE - SPACING + ANTMARGIN) + ((ant->y - SCALE - SPACING + ANTMARGIN)*SCREEN_WIDTH)]) == GRAY)
	{
		//printf("%u", ant->x - SCALE / 2 + SPACING);
		ant->heading = ant->heading + 90;
		if (ant->heading > 270) ant->heading = ant->heading - 360;
		ant->lasttile = GRAY;
		printf("jobb");
	}
	if (((*pixels)[(ant->x - SCALE - SPACING + ANTMARGIN) + ((ant->y - SCALE - SPACING + ANTMARGIN)*SCREEN_WIDTH)]) == DARKWHITE)
	{
		ant->heading = ant->heading - 90;
		if (ant->heading > 270) ant->heading = ant->heading + 360;
		ant->lasttile = DARKWHITE;
		printf("bal");
	}
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

			//Initialize 2D pixel texture array
			*pixelarr = (Uint32 ***)malloc(sizeof(Uint32 *) * SCREEN_WIDTH);
			*pixelarr[0] = (Uint32 **)malloc(sizeof(Uint32) * SCREEN_HEIGHT * SCREEN_WIDTH);
			for (int i = 0; i < SCREEN_WIDTH; i++)
				(*pixelarr)[i] = (**pixelarr + SCREEN_HEIGHT * i);

			//Override all pixels
			memset32(*pixels, BLACK, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
			memset32(**pixelarr, BLACK, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
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

void gmain(int argc, char ** argv)
{
	Ant ant;
	ant.x = SCREEN_WIDTH / 2;
	ant.y = SCREEN_HEIGHT / 2;
	ant.heading = UP;
	ant.lasttile = GRAY;

	bool quit = false;

	bool leftMouseButtonDown = false;

	bool Running = false;

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
					//moveAnt(&pixels, &ant);
					//SDL_UpdateTexture(gTexture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));
					if (Running)Running = false;
					else Running = true;
					break;
				case SDLK_w:
					ant.heading = UP;
					moveAnt(&pixels, &ant);
					SDL_UpdateTexture(gTexture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));
					break;
				case SDLK_s:
					ant.heading = DOWN;
					moveAnt(&pixels, &ant);
					SDL_UpdateTexture(gTexture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));
					break;
				case SDLK_d:
					ant.heading = RIGHT;
					moveAnt(&pixels, &ant);
					SDL_UpdateTexture(gTexture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));
					break;
				case SDLK_a:
					ant.heading = LEFT;
					moveAnt(&pixels, &ant);
					SDL_UpdateTexture(gTexture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));
					break;
				/*case SDL_MOUSEBUTTONUP:
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
						for (int x = 0; x < SCREEN_WIDTH; x++)
						{
							for (int y = 0; y < SCREEN_HEIGHT; y++)
							{
								pixels[x + y * SCREEN_WIDTH] = pixelarr[x][y];
							}
						}
						//pixels[mouseY * SCREEN_WIDTH + mouseX] = 0xFF00FF00;
						//printf("%d |", pixels[mouseY * SCREEN_WIDTH + mouseX]);
					}
					break;*/
				}
				while (Running)
				{
					moveAnt(&pixels, &ant);
					SDL_UpdateTexture(gTexture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));
				}
				SDL_RenderClear(gRenderer);
				SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
				SDL_RenderPresent(gRenderer);
			}
		}

		//Free resources and close SDL
		close(&pixels, &gWindow, &gRenderer, &gTexture);

	}
}