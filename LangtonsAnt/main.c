﻿// Copyright 2018 © Andirs Borbás™, All Rights ¯\_(ツ)_/¯®.

#include "main.h"

int main(int argc, char ** argv)
{
	char configfilename[52] = "config.ini";

	if (argc > 1)strcpy(configfilename, argv[1]);

	//The config file
	FILE *fDefConf;
	fDefConf = fopen(configfilename, "r+");
	if (fDefConf == NULL)
	{
		printf("Could not open config file.");
		exit(31);
	}

	char antoutfilename[100];

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	mkdir("./Runs");

	snprintf(antoutfilename, sizeof antoutfilename, "./Runs/antout_%d-%d-%d_%d-%d-%d.txt", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

	FILE *fAntOut;
	fAntOut = fopen(antoutfilename, "w");
	if (fAntOut == NULL)
	{
		printf("Could not open output file: %s", antoutfilename);
		exit(32);
	}

	//Buffer for reading from file
	char buffer[52] = "";

	////////////////
	//Default values
	////////////////

	//Window parameter container
	volatile SDL_Rect SCREEN;
	//Width of the window
	SCREEN.w = 960;
	//Height of the window
	SCREEN.h = 960;
	//Number of pixels per grid square (has to be at least 3 or the ant wont be visible, but it will still work)
	volatile int SCALE = 4;
	//Number of pixels between grid squares
	volatile int SPACING = 1;
	//Number of pixels the ant is smaller than the grid (has to be at least 1, can't be more than SCALE)
	int const ANTMARGIN = 1;
	//The tickrate of the simulation in ms
	volatile int MSTICK = 16;
	//Instructionset for the ant
	volatile char instructionset[19] = "LR";

	//Base instructions for the settings to cycle through
	char instructions[9][19];
	strcpy(instructions[0], "LR");
	strcpy(instructions[1], "RLR");
	strcpy(instructions[2], "LLRR");
	strcpy(instructions[3], "RLLR");
	strcpy(instructions[4], "LRRRL");
	strcpy(instructions[5], "LRRRRRLLR");
	strcpy(instructions[6], "LRLRLRLRLRR");
	strcpy(instructions[7], "LLRRRLRLRLLR");
	strcpy(instructions[8], "RRLLLRLLLRRR");

	//Number of steps made by the ant;
	int lepes = 0;

	//Load values from config file
	while ((strstr(buffer, "endconfig;")) == NULL)
	{
		fscanf(fDefConf, "%s", &buffer);
		if (buffer[0] == '/')continue;

		if (buffer[0] == '#')
		{
			loadintFromConfig(fDefConf, buffer, &SCREEN.w, "SCREEN_WIDTH");
			loadintFromConfig(fDefConf, buffer, &SCREEN.h, "SCREEN_HEIGHT");
			loadintFromConfig(fDefConf, buffer, &SCALE, "SCALE");
			loadintFromConfig(fDefConf, buffer, &SPACING, "SPACING");
			loadintFromConfig(fDefConf, buffer, &ANTMARGIN, "ANTMARGIN");
			loadintFromConfig(fDefConf, buffer, &MSTICK, "MSTICK");
			loadcharFromConfig(fDefConf, buffer, &instructionset, "INSTRUCTIONSET");
		}
	}
	fclose(fDefConf);

	int instructnum = strlen(instructionset);

	//Buttons
	SDL_Rect StartButton;
	StartButton.w = 252;
	StartButton.h = 80;
	StartButton.x = SCREEN.w / 2 - StartButton.w / 2;
	StartButton.y = 25;

	SDL_Rect StartButtonStroke;
	int Strokesize = 2;
	StartButtonStroke.w = StartButton.w + (Strokesize * 2);
	StartButtonStroke.h = StartButton.h + (Strokesize * 2);
	StartButtonStroke.x = StartButton.x - Strokesize;
	StartButtonStroke.y = StartButton.y - Strokesize;

	SDL_Rect ResButton;
	if (SCREEN.w >= 1000)ResButton.w = 160;
	else ResButton.w = 144;
	ResButton.h = 40;
	ResButton.x = SCREEN.w / 2 + SCREEN.w / 4 - ResButton.w / 2;
	ResButton.y = SCREEN.h - ResButton.h - 25;

	SDL_Rect ResUp;
	ResUp.w = 34;
	ResUp.h = 34;
	ResUp.x = ResButton.x + ResButton.w + 8;
	ResUp.y = ResButton.y + 3;

	SDL_Rect ResDown;
	ResDown.w = 34;
	ResDown.h = 34;
	ResDown.x = ResButton.x - ResDown.w - 8;
	ResDown.y = ResButton.y + 3;


	SDL_Rect InstructButton;
	InstructButton.w = 200;
	InstructButton.h = 40;
	InstructButton.x = SCREEN.w / 2 - SCREEN.w / 4 - InstructButton.w / 2;
	InstructButton.y = SCREEN.h - InstructButton.h - 25;


	//Handle normal quit
	bool quit = false;
	//Handle quit on error
	bool equit = false;
	//Start the algorithm
	bool start = false;
	//Let the algorithm run
	bool Running = true;
	//Is there an error
	bool ERROR = false;

	//The viewport window
	SDL_Window *gWindow = NULL;

	//The renderer
	SDL_Renderer *gRenderer = NULL;

	//The pixel arrays
	Uint32 *pixels = NULL;
	Uint32 **pixelTex = NULL;

	//Textues
	SDL_Texture *tPixelTexture = NULL;
	SDL_Texture *tMainMenu = NULL;

	//Strings
	SDL_Texture *tStrings = NULL;
	SDL_Surface *sStrings = NULL;
	SDL_Rect lStrings = { 1,0,0,0 };

	//Mouse location
	SDL_Point mouse = { 0,0 };

	//Start up SDL and create window
	if (!initSDL(&gWindow, &gRenderer, &tPixelTexture, &tMainMenu, SCREEN))
	{
		printf("Failed to initialize: %s", SDL_GetError());
		exit(1);
	}

	//Load StartFont
	TTF_Font *StartFont = TTF_OpenFont("ExodusDemo-Striped.otf", 60);
	if (!StartFont)
	{
		SDL_Log("Failed to open font: %s", TTF_GetError());
		exit(11);
	}

	//Load MenuFont
	TTF_Font *MenuFont = TTF_OpenFont("ExodusDemo-Striped.otf", 30);
	if (!MenuFont)
	{
		SDL_Log("Failed to open font: %s", TTF_GetError());
		exit(12);
	}

	TTF_Font *InstructFont = TTF_OpenFont("ExodusDemo-Striped.otf", 20);
	if (!InstructFont)
	{
		SDL_Log("Failed to open font: %s", TTF_GetError());
		exit(13);
	}

	//Load main menu background
	tMainMenu = IMG_LoadTexture(gRenderer, "MainMenu.png");
	if (!tMainMenu)
	{
		SDL_Log("Failed to open image: %s", IMG_GetError());
		exit(21);
	}

	SDL_Rect tempSCREEN = SCREEN;

	//Draw main menu
	drawMenu(&sStrings, &StartFont, &MenuFont, &InstructFont, &tStrings, &lStrings, &gWindow, &gRenderer, &tPixelTexture, &tMainMenu,
		SCREEN, &StartButton, &StartButtonStroke, &ResButton, &ResUp, &ResDown, &InstructButton, instructionset);

	SDL_Event event;

	int n = 0;

	while (!quit && !start)
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

			}
		case SDL_MOUSEBUTTONDOWN:
			//Darken buttons on mousedown
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				SDL_GetMouseState(&mouse.x, &mouse.y);

				if (SDL_PointInRect(&mouse, &StartButton))
				{
					roundedBoxColor(gRenderer, StartButtonStroke.x, StartButtonStroke.y, StartButtonStroke.x + StartButtonStroke.w, StartButtonStroke.y + StartButtonStroke.h, 12, altBLACK);
					roundedBoxColor(gRenderer, StartButton.x, StartButton.y, StartButton.x + StartButton.w, StartButton.y + StartButton.h, 12, altDARKGRAY);
					drawTextintoButton(gRenderer, &sStrings, StartFont, &tStrings, &lStrings, StartButton, "Start", TextORANGE);
					SDL_RenderCopy(gRenderer, tStrings, NULL, &lStrings);
					SDL_RenderPresent(gRenderer);
				}

				if (SDL_PointInRect(&mouse, &ResButton))
				{
					roundedBoxColor(gRenderer, ResButton.x, ResButton.y, ResButton.x + ResButton.w, ResButton.y + ResButton.h, 6, altDARKGRAY);
					char buff[sizeof(int) * 19 + 2];
					snprintf(buff, sizeof buff, "%dx%d", SCREEN.w, SCREEN.h);
					drawTextintoButton(gRenderer, &sStrings, MenuFont, &tStrings, &lStrings, ResButton, buff, TextORANGE);
					SDL_RenderCopy(gRenderer, tStrings, NULL, &lStrings);
					SDL_RenderPresent(gRenderer);
				}

				if (SDL_PointInRect(&mouse, &ResUp))
				{
					roundedBoxColor(gRenderer, ResUp.x, ResUp.y, ResUp.x + ResUp.w, ResUp.y + ResUp.h, 6, altDARKGRAY);
					filledTrigonColor(gRenderer, ResUp.x + 6, ResUp.y + 6, ResUp.x + 6, ResUp.y + ResUp.h - 6, ResUp.x + ResUp.w - 6, ResUp.y + ResUp.h / 2, altGRAY);
					SDL_RenderPresent(gRenderer);
				}

				if (SDL_PointInRect(&mouse, &ResDown))
				{
					roundedBoxColor(gRenderer, ResDown.x, ResDown.y, ResDown.x + ResDown.w, ResDown.y + ResDown.h, 6, altDARKGRAY);
					filledTrigonColor(gRenderer, ResDown.x + ResDown.w - 6, ResDown.y + 6, ResDown.x + ResDown.w - 6, ResDown.y + ResDown.h - 6, ResDown.x + 6, ResDown.y + ResDown.h / 2, altGRAY);
					SDL_RenderPresent(gRenderer);
				}

				if (SDL_PointInRect(&mouse, &InstructButton))
				{
					roundedBoxColor(gRenderer, InstructButton.x, InstructButton.y, InstructButton.x + InstructButton.w, InstructButton.y + InstructButton.h, 6, altDARKGRAY);
					drawTextintoButton(gRenderer, &sStrings, InstructFont, &tStrings, &lStrings, InstructButton, instructionset, TextORANGE);
					SDL_RenderCopy(gRenderer, tStrings, NULL, &lStrings);
					SDL_RenderPresent(gRenderer);
				}
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				SDL_GetMouseState(&mouse.x, &mouse.y);

				if (SDL_PointInRect(&mouse, &StartButton))
				{
					start = true;
				}

				if (SDL_PointInRect(&mouse, &ResButton))
				{
					SCREEN = tempSCREEN;
					refreshMenu(&gWindow, &gRenderer, &tPixelTexture, &tMainMenu, SCREEN, Strokesize, &StartButton, &StartButtonStroke, &ResButton, &ResUp, &ResDown, &InstructButton);
					drawMenu(&sStrings, &StartFont, &MenuFont, &InstructFont, &tStrings, &lStrings, &gWindow, &gRenderer, &tPixelTexture, &tMainMenu,
						SCREEN, &StartButton, &StartButtonStroke, &ResButton, &ResUp, &ResDown, &InstructButton, instructionset);
				}

				if (SDL_PointInRect(&mouse, &ResUp))
				{
					switch (tempSCREEN.w)
					{
					case 500:
						tempSCREEN.w = 720;
						tempSCREEN.h = 720;
						break;
					case 720:
						tempSCREEN.w = 900;
						tempSCREEN.h = 900;
						break;
					case 900:
						tempSCREEN.w = 960;
						tempSCREEN.h = 960;
						break;
					case 960:
						tempSCREEN.w = 1000;
						tempSCREEN.h = 1000;
						break;
					case 1000:
						tempSCREEN.w = 1920;
						tempSCREEN.h = 1000;
						break;
					case 1920:
						tempSCREEN.w = 1920;
						tempSCREEN.h = 1000;
						break;
					default:
						tempSCREEN.w = 960;
						tempSCREEN.h = 960;
						break;
					}
#ifdef DEBUG
					printf("%d\n", tempSCREEN.w);
#endif
					roundedBoxColor(gRenderer, ResButton.x, ResButton.y, ResButton.x + ResButton.w, ResButton.y + ResButton.h, 6, altGRAY);
					char buff[sizeof(int) * 4 + 2];
					snprintf(buff, sizeof buff, "%dx%d", tempSCREEN.w, tempSCREEN.h);
					drawTextintoButton(gRenderer, &sStrings, MenuFont, &tStrings, &lStrings, ResButton, buff, TextORANGE);
					SDL_RenderCopy(gRenderer, tStrings, NULL, &lStrings);
					SDL_RenderPresent(gRenderer);
				}

				if (SDL_PointInRect(&mouse, &ResDown))
				{
					switch (tempSCREEN.w)
					{
					case 1920:
						tempSCREEN.w = 1000;
						tempSCREEN.h = 1000;
						break;
					case 1000:
						tempSCREEN.w = 960;
						tempSCREEN.h = 960;
						break;
					case 960:
						tempSCREEN.w = 900;
						tempSCREEN.h = 900;
						break;
					case 900:
						tempSCREEN.w = 720;
						tempSCREEN.h = 720;
						break;
					case 720:
						tempSCREEN.w = 500;
						tempSCREEN.h = 500;
						break;
					case 500:
						tempSCREEN.w = 500;
						tempSCREEN.h = 500;
						break;
					default:
						tempSCREEN.w = 960;
						tempSCREEN.h = 960;
						break;
					}
#ifdef DEBUG
					printf("%d\n", tempSCREEN.w);
#endif
					roundedBoxColor(gRenderer, ResButton.x, ResButton.y, ResButton.x + ResButton.w, ResButton.y + ResButton.h, 6, altGRAY);
					char buff[sizeof(int) * 4 + 2];
					snprintf(buff, sizeof buff, "%dx%d", tempSCREEN.w, tempSCREEN.h);
					drawTextintoButton(gRenderer, &sStrings, MenuFont, &tStrings, &lStrings, ResButton, buff, TextORANGE);
					SDL_RenderCopy(gRenderer, tStrings, NULL, &lStrings);
					SDL_RenderPresent(gRenderer);
				}

				if (SDL_PointInRect(&mouse, &InstructButton))
				{
					strcpy(instructionset, instructions[n]);
					roundedBoxColor(gRenderer, InstructButton.x, InstructButton.y, InstructButton.x + InstructButton.w, InstructButton.y + InstructButton.h, 6, altGRAY);
					drawTextintoButton(gRenderer, &sStrings, InstructFont, &tStrings, &lStrings, InstructButton, instructionset, TextORANGE);
					SDL_RenderCopy(gRenderer, tStrings, NULL, &lStrings);
					SDL_RenderPresent(gRenderer);
					if (n >= 8)n = 0;
					else n++;
				}

				if (!SDL_PointInRect(&mouse, &InstructButton) && !SDL_PointInRect(&mouse, &ResDown) && !SDL_PointInRect(&mouse, &ResUp) &&
					!SDL_PointInRect(&mouse, &ResButton) && !SDL_PointInRect(&mouse, &StartButton))
				{
					drawMenu(&sStrings, &StartFont, &MenuFont, &InstructFont, &tStrings, &lStrings, &gWindow, &gRenderer, &tPixelTexture, &tMainMenu,
						SCREEN, &StartButton, &StartButtonStroke, &ResButton, &ResUp, &ResDown, &InstructButton, instructionset);
				}
			}
			break;
		}
	}

	//The ant
	Ant ant = { SCREEN.w / 2 , SCREEN.h / 2 , UP, 0, 0, BLACK };
	for (int i = 0; i < 19; i++)
	{
		if (instructionset[i] == '\0')break;
		switch (instructionset[i])
		{
		case 'R':
		case 'r':
			ant.turn[i] = 90;
			break;
		case 'L':
		case 'l':
			ant.turn[i] = -90;
			break;
		case 'N':
		case 'n':
			ant.turn[i] = 0;
			break;
		case 'U':
		case 'u':
			ant.turn[i] = 180;
			break;
		}
	}



	//Initialize pixel texture
	initTexture(&gRenderer, &tPixelTexture, SCREEN);
	//Initialize pixel arrays
	initPixels(&pixels, &pixelTex, SCREEN);

	//Create base tickrate
	SDL_TimerID tick = SDL_AddTimer(MSTICK, ftick, NULL);

	SDL_RenderClear(gRenderer);

	while (!quit)
	{
		fprintf(fAntOut, "Dimensions: %dx%d, Scale: %d, Spacing: %d, Margin: %d, Instructionset: %s\n\n", SCREEN.w, SCREEN.h, SCALE, SPACING, ANTMARGIN, instructionset);

		SDL_WaitEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_g:
				for (int i = 0; i < 104; i++)
				{
					if (!moveAnt(&pixelTex, &ant, &lepes, SCREEN, SCALE, SPACING, ANTMARGIN, instructnum, instructionset, fAntOut))
					{
						ERROR = true;
						quit = true;
						break;
					}
				}
				convertPixels(&pixels, &pixelTex, SCREEN);
				SDL_UpdateTexture(tPixelTexture, NULL, pixels, SCREEN.w * sizeof(Uint32));
				break;
			case SDLK_h:
				for (int i = 0; i < 1040; i++)
				{
					if (!moveAnt(&pixelTex, &ant, &lepes, SCREEN, SCALE, SPACING, ANTMARGIN, instructnum, instructionset, fAntOut))
					{
						ERROR = true;
						quit = true;
						break;
					}
				}
				convertPixels(&pixels, &pixelTex, SCREEN);
				SDL_UpdateTexture(tPixelTexture, NULL, pixels, SCREEN.w * sizeof(Uint32));
				break;
			case SDLK_j:
				for (int i = 0; i < 10400; i++)
				{
					if (!moveAnt(&pixelTex, &ant, &lepes, SCREEN, SCALE, SPACING, ANTMARGIN, instructnum, instructionset, fAntOut))
					{
						ERROR = true;
						quit = true;
						break;
					}
				}
				convertPixels(&pixels, &pixelTex, SCREEN);
				SDL_UpdateTexture(tPixelTexture, NULL, pixels, SCREEN.w * sizeof(Uint32));
				break;
			case SDLK_SPACE:
				Running = true;
				break;
			}
		case SDL_USEREVENT:
			while (Running && !quit)
			{
				if (!moveAnt(&pixelTex, &ant, &lepes, SCREEN, SCALE, SPACING, ANTMARGIN, instructnum, instructionset, fAntOut))
				{
					printf("Error while trying to move ant: %s", SDL_GetError());
					ERROR = true;
					quit = true;
				}
				convertPixels(&pixels, &pixelTex, SCREEN);
				SDL_UpdateTexture(tPixelTexture, NULL, pixels, SCREEN.w * sizeof(Uint32));
				if (!ERROR)
				{
					SDL_RenderCopy(gRenderer, tPixelTexture, NULL, NULL);
					SDL_RenderPresent(gRenderer);
				}
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
						Running = false;
						break;
					}
				}
			}
			break;
		}
		if (ERROR)
		{
			while (!equit)
			{
				SDL_RenderCopy(gRenderer, tPixelTexture, NULL, NULL);
				SDL_RenderPresent(gRenderer);

				SDL_WaitEvent(&event);
				if (event.type == SDL_QUIT)
				{
					equit = true;
				}
			}
		}
		else
		{
			SDL_RenderCopy(gRenderer, tPixelTexture, NULL, NULL);
			SDL_RenderPresent(gRenderer);
		}
	}

#ifdef DEBUG
	debugmalloc_dump();
#endif

	char antouttexturename[100];
	snprintf(antouttexturename, sizeof antouttexturename, "./Runs/antout_%d-%d-%d_%d-%d-%d.bmp", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	save_texture(gRenderer, tPixelTexture, antouttexturename);

	//Free resources and close SDL
	close(&pixels, &pixelTex, &gWindow, gRenderer, tPixelTexture, tMainMenu, tStrings, fAntOut);
	return 0;
}