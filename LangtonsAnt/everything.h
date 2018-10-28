#include <SDL.h>
#include <SDL_image.h>
#include <SDL2_gfxprimitives.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>


//Stores the properties of the ant
typedef struct Ant {
	int x, y, heading, state, lasttile, currenttile, turn[18];
}Ant;

enum AntHeading
{
	UP = 0,
	RIGHT = 90,
	DOWN = 180,
	LEFT = 270
};

enum HEXARGB
{	
	BLACK = 0xFF000000,
	WHITE = 0xFFFFFFFF,
	MAROON = 0xFF800000,
	BROWN = 0xFF9A6324,
	OLIVE = 0xFF808000,
	TEAL = 0xFF469990,
	NAVY = 0xFF000075,
	RED = 0xFFE6194B,
	ORANGE = 0xFFF58231,
	YELLOW= 0xFFFFE119,
	LIME = 0xFFBFEF45,
	GREEN = 0xFF3CB44B,
	CYAN = 0xFF42D4F4,
	BLUE = 0xFF4363D8,
	PURPLE = 0xFF911EB4,
	MAGENTA = 0xFFF032E6,
	PINK = 0xFFFABEBE,
	APRICOT = 0xFFFFD8B1,
	BEIGE = 0xFFFFFAC8,
	MINT = 0xFFAAFFC3,
	LAVENDER = 0xFFE6BEFF
};

enum HEXRGBA
{
	altWHITE = 0xFFFFFFFF,
	altDARKWHITE = 0xDDDDDDFF,
	altGRAY = 0x333333FF,
	altBLACK = 0x000000FF
};

//memset with 32 bit unsigned integer
void memset32(void*, Uint32, uintptr_t);

//Frees media and shuts down SDL
void close(Uint32**, Uint32***, SDL_Window**, SDL_Renderer**, SDL_Texture**);

//Starts up SDL and creates window
bool initSDL(SDL_Window**, SDL_Renderer**, SDL_Texture**, SDL_Texture**, int const, int const);

//Initializes the pixel arrays
bool initPixels(Uint32**, Uint32***, int const, int const);

//Converts pixel texture to sdl pixel format
bool convertPixels(Uint32**, Uint32***, int const, int const);

//Loads the configuration into int variables
void loadintFromConfig(FILE*, char*, int*, char*);

//Loads the configuration into char variables
void loadcharFromConfig(FILE*, char*, char*, char*);

//Base tickrate of the simulation
Uint32 ftick(Uint32, void*);

//Turn the ant into the right direction
bool turnAnt(Ant*, int);

//The alogorithm for the ants movement
bool antgorithm(Uint32***, Ant*, int const, int const, int const, int const, int);

//move ant and invert past location
bool moveAnt(Uint32***, Ant*, int*, int const, int const, int const, int const, int const, int, char*);