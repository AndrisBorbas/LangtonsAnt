// Copyright 2018 © Andirs Borbás™, All Rights ¯\_(ツ)_/¯®.

#include "main.h"

int main(int argc, char ** argv)
{
	//Default config name
	char configfilename[52] = "config.ini";
	//Set config name from command line
	if (argc > 1)strcpy(configfilename, argv[1]);
	//Filename to save instructions to
	char antoutfilename[100];

	//Time for filenames
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	//The config file
	FILE *fDefConf;
	fDefConf = fopen(configfilename, "r+");
	if (fDefConf == NULL)
	{
		SDL_Log("Could not open config file: %s", configfilename);
	}


	////////////////
	//Default values
	////////////////

	Settings settings;
	//Width of the window
	settings.SCREEN.w = 960;
	//Height of the window
	settings.SCREEN.h = 960;
	//Number of pixels per grid square (has to be at least 3 or the ant wont be visible, but it will still work)
	settings.SCALE = 4;
	//Number of pixels between grid squares
	settings.SPACING = 1;
	//Number of pixels the ant is smaller than the grid (has to be at least 1, can't be more than SCALE)
	settings.ANTMARGIN = 1;
	//The tickrate of the simulation in ms
	settings.MSTICK = 16;
	//Instructionset for the ant
	strcpy(settings.instructionset, "RL");

	//Base instructions for the settings to cycle through
	char instructions[9][19];
	strcpy(instructions[0], "RL");
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

	//The number of instructions in the instructionset
	int instructnum = strlen(settings.instructionset);

	loadConfig(fDefConf, &settings, &instructnum);

	/////////
	//Buttons
	/////////
	SDL_Rect StartButton;
	StartButton.w = 252;
	StartButton.h = 80;
	StartButton.y = 25;

	SDL_Rect StartButtonStroke;
	int Strokesize = 2;

	SDL_Rect ResButton;
	if (settings.SCREEN.w >= 1000)ResButton.w = 160;
	else ResButton.w = 144;
	ResButton.h = 40;

	SDL_Rect ResUp;
	ResUp.w = 34;
	ResUp.h = 34;

	SDL_Rect ResDown;
	ResDown.w = 34;
	ResDown.h = 34;

	SDL_Rect InstructButton;
	InstructButton.w = 200;
	InstructButton.h = 40;

	SDL_Rect ScaleButton;
	ScaleButton.w = 144;
	ScaleButton.h = 40;

	SDL_Rect HelpButton1;
	HelpButton1.w = 150;
	HelpButton1.h = 36;
	HelpButton1.x = 25;
	HelpButton1.y = settings.SCREEN.h / 4 - 24;


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
	//Was the simulation paused before exit
	bool paused = false;

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

	//Helpers for printing text on the screen
	SDL_Texture *tStrings = NULL;
	SDL_Rect lStrings = { 1,1,0,0 };

	//Mouse location
	SDL_Point mouse = { 0,0 };

	//Start up SDL and create window
	if (!initSDL(&gWindow, &gRenderer, &tPixelTexture, &tMainMenu, settings.SCREEN))
	{
		SDL_Log("Failed to initialize: %s", SDL_GetError());
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

	//Load InstructionsFont
	TTF_Font *InstructFont = TTF_OpenFont("ExodusDemo-Striped.otf", 20);
	if (!InstructFont)
	{
		SDL_Log("Failed to open font: %s", TTF_GetError());
		exit(13);
	}

	//Load HelpFont
	TTF_Font *HelpFont = TTF_OpenFont("ExodusDemo-Striped.otf", 20);
	if (!HelpFont)
	{
		SDL_Log("Failed to open font: %s", TTF_GetError());
		exit(14);
	}

	//Load main menu 4k background
	tMainMenu = IMG_LoadTexture(gRenderer, "menu4k.png");
	if (!tMainMenu)
	{
		SDL_Log("Failed to open 4k image: %s", IMG_GetError());
		//Fallback 1k image
		tMainMenu = IMG_LoadTexture(gRenderer, "menuhd.png");
		if (!tMainMenu)
		{
			SDL_Log("Failed to open HD image: %s", IMG_GetError());
			exit(21);
		}
	}

	//Temporarily store values to load on hitting apply
	SDL_Rect tempSCREEN = settings.SCREEN;

	//The events that handle keypresses and timers
	SDL_Event event;

	//Iterator to go through the loaded instructionsets
	int iter = 0;
	//Number of restarts to the menu
	int num = 0;

	//Help texts
	char help[6][14] = { "space: pause", "p: menu", "f: step 1", "g: jump 100", "h: jump 1000", "j: jump 10000" };

	//The ant
	Ant ant;

	//Start of the main loop
startup:

	//Increase the number of runs
	num++;

	//Set the position of the buttons
	refreshMenu(&gWindow, &gRenderer, &tPixelTexture, &tMainMenu, settings.SCREEN, Strokesize, &StartButton, &StartButtonStroke, &ResButton, &ResUp, &ResDown, &ScaleButton, &InstructButton);

	//Draw main menu
	drawMenu(&StartFont, &MenuFont, &InstructFont, &HelpFont, &tStrings, &lStrings, &gWindow, &gRenderer, &tPixelTexture, &tMainMenu,
		settings, HelpButton1, help, StartButton, StartButtonStroke, ResButton, ResUp, ResDown, ScaleButton, InstructButton);

	//Handle events in the menu
	while (!quit && !start)
	{
		//Queue up input and timer events
		SDL_WaitEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			goto end;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				quit = true;
				goto end;
				break;
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
					drawTextintoButton(gRenderer, StartFont, &tStrings, &lStrings, StartButton, "Start", TextDarkOrange);
					SDL_RenderCopy(gRenderer, tStrings, NULL, &lStrings);
					SDL_RenderPresent(gRenderer);
				}

				if (SDL_PointInRect(&mouse, &ResButton))
				{
					roundedBoxColor(gRenderer, ResButton.x, ResButton.y, ResButton.x + ResButton.w, ResButton.y + ResButton.h, 6, altDARKGRAY);
					char buff[sizeof(int) * 19 + 2];
					sprintf(buff, "%dx%d", settings.SCREEN.w, settings.SCREEN.h);
					drawTextintoButton(gRenderer, MenuFont, &tStrings, &lStrings, ResButton, buff, TextDarkOrange);
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

				if (SDL_PointInRect(&mouse, &ScaleButton))
				{
					roundedBoxColor(gRenderer, ScaleButton.x, ScaleButton.y, ScaleButton.x + ScaleButton.w, ScaleButton.y + ScaleButton.h, 6, altDARKGRAY);
					char sbuff[sizeof(int) * 1 + 8];
					sprintf(sbuff, "Scale: %d", settings.SCALE);
					drawTextintoButton(gRenderer, MenuFont, &tStrings, &lStrings, ScaleButton, sbuff, TextDarkOrange);
					SDL_RenderCopy(gRenderer, tStrings, NULL, &lStrings);
					SDL_RenderPresent(gRenderer);
				}

				if (SDL_PointInRect(&mouse, &InstructButton))
				{
					roundedBoxColor(gRenderer, InstructButton.x, InstructButton.y, InstructButton.x + InstructButton.w, InstructButton.y + InstructButton.h, 6, altDARKGRAY);
					drawTextintoButton(gRenderer, InstructFont, &tStrings, &lStrings, InstructButton, settings.instructionset, TextDarkOrange);
					SDL_RenderCopy(gRenderer, tStrings, NULL, &lStrings);
					SDL_RenderPresent(gRenderer);
				}
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				SDL_GetMouseState(&mouse.x, &mouse.y);

				//Start on clicking start button
				if (SDL_PointInRect(&mouse, &StartButton))
				{
					start = true;
				}

				//Apply resolution change on resolution display button
				if (SDL_PointInRect(&mouse, &ResButton))
				{
					settings.SCREEN = tempSCREEN;
					refreshMenu(&gWindow, &gRenderer, &tPixelTexture, &tMainMenu, settings.SCREEN, Strokesize, &StartButton, &StartButtonStroke, &ResButton, &ResUp, &ResDown, &ScaleButton, &InstructButton);
					drawMenu(&StartFont, &MenuFont, &InstructFont, &HelpFont, &tStrings, &lStrings, &gWindow, &gRenderer, &tPixelTexture, &tMainMenu,
						settings, HelpButton1, help, StartButton, StartButtonStroke, ResButton, ResUp, ResDown, ScaleButton, InstructButton);
				}

				//Change resolution when clicking the arrow to the right
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
#ifdef Debug
					printf("%d\n", tempSCREEN.w);
#endif
					roundedBoxColor(gRenderer, ResButton.x, ResButton.y, ResButton.x + ResButton.w, ResButton.y + ResButton.h, 6, altGRAY);
					char buff[sizeof(int) * 2 + 2];
					sprintf(buff, "%dx%d", tempSCREEN.w, tempSCREEN.h);
					drawTextintoButton(gRenderer, MenuFont, &tStrings, &lStrings, ResButton, buff, TextOrange);
					SDL_RenderCopy(gRenderer, tStrings, NULL, &lStrings);

					roundedBoxColor(gRenderer, ResUp.x, ResUp.y, ResUp.x + ResUp.w, ResUp.y + ResUp.h, 6, altGRAY);
					filledTrigonColor(gRenderer, ResUp.x + 6, ResUp.y + 6, ResUp.x + 6, ResUp.y + ResUp.h - 6, ResUp.x + ResUp.w - 6, ResUp.y + ResUp.h / 2, altWHITE);

					SDL_RenderPresent(gRenderer);
				}

				//Change resolution when clicking the arrow to the left
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
#ifdef Debug
					printf("%d\n", tempSCREEN.w);
#endif
					roundedBoxColor(gRenderer, ResButton.x, ResButton.y, ResButton.x + ResButton.w, ResButton.y + ResButton.h, 6, altGRAY);
					char buff[sizeof(int) * 2 + 2];
					sprintf(buff, "%dx%d", tempSCREEN.w, tempSCREEN.h);
					drawTextintoButton(gRenderer, MenuFont, &tStrings, &lStrings, ResButton, buff, TextOrange);
					SDL_RenderCopy(gRenderer, tStrings, NULL, &lStrings);

					roundedBoxColor(gRenderer, ResDown.x, ResDown.y, ResDown.x + ResDown.w, ResDown.y + ResDown.h, 6, altGRAY);
					filledTrigonColor(gRenderer, ResDown.x + ResDown.w - 6, ResDown.y + 6, ResDown.x + ResDown.w - 6, ResDown.y + ResDown.h - 6, ResDown.x + 6, ResDown.y + ResDown.h / 2, altWHITE);
					SDL_RenderPresent(gRenderer);
				}

				//Change instructionset when clicking that button
				if (SDL_PointInRect(&mouse, &InstructButton))
				{
					strcpy(settings.instructionset, instructions[iter]);
					instructnum = strlen(settings.instructionset);
					roundedBoxColor(gRenderer, InstructButton.x, InstructButton.y, InstructButton.x + InstructButton.w, InstructButton.y + InstructButton.h, 6, altGRAY);
					drawTextintoButton(gRenderer, InstructFont, &tStrings, &lStrings, InstructButton, settings.instructionset, TextOrange);
					SDL_RenderCopy(gRenderer, tStrings, NULL, &lStrings);
					SDL_RenderPresent(gRenderer);
					if (iter >= 8)iter = 0;
					else iter++;
				}

				//Change the size of the pixels when clicking on that button
				if (SDL_PointInRect(&mouse, &ScaleButton))
				{
					if (settings.SCALE == 4) {
						settings.SCALE = 1;
						settings.SPACING = 0;
					}
					else
					{
						settings.SCALE = 4;
						settings.SPACING = 1;
					}
					roundedBoxColor(gRenderer, ScaleButton.x, ScaleButton.y, ScaleButton.x + ScaleButton.w, ScaleButton.y + ScaleButton.h, 6, altGRAY);
					char sbuff[sizeof(int) * 1 + 8];
					sprintf(sbuff, "Scale: %d", settings.SCALE);
					drawTextintoButton(gRenderer, MenuFont, &tStrings, &lStrings, ScaleButton, sbuff, TextOrange);
					SDL_RenderCopy(gRenderer, tStrings, NULL, &lStrings);
					SDL_RenderPresent(gRenderer);
				}

				//Redraw menu when clicked elsewhere so buttons don't stay darkened
				if (!SDL_PointInRect(&mouse, &InstructButton) && !SDL_PointInRect(&mouse, &ResDown) && !SDL_PointInRect(&mouse, &ResUp) &&
					!SDL_PointInRect(&mouse, &ResButton) && !SDL_PointInRect(&mouse, &StartButton))
				{
					drawMenu(&StartFont, &MenuFont, &InstructFont, &HelpFont, &tStrings, &lStrings, &gWindow, &gRenderer, &tPixelTexture, &tMainMenu,
						settings, HelpButton1, help, StartButton, StartButtonStroke, ResButton, ResUp, ResDown, ScaleButton, InstructButton);
				}
			}
			break;
		}
	}

	//Setting the ants start values
	ant.x = settings.SCREEN.w / 2;
	ant.y = settings.SCREEN.h / 2;
	ant.heading = UP;
	ant.lasttile = BLACK;
	//Converting instructionset to actual turns
	convertToTurns(settings.instructionset, &ant);

	//Initialize pixel texture
	initTexture(&gRenderer, &tPixelTexture, settings.SCREEN);
	//Initialize pixel arrays
	initPixels(&pixels, &pixelTex, settings.SCREEN);

	//Create base tickrate
	SDL_TimerID tick = SDL_AddTimer(settings.MSTICK, ftick, NULL);

	SDL_RenderClear(gRenderer);

	//Output text file
	FILE *fAntOut = NULL;

	if (!quit)
	{
		//Output folder
		mkdir("./Runs");
		//Filename based on programs start time
		sprintf(antoutfilename, "./Runs/antout_%d-%d-%d_%dh-%dm-%ds(%d).txt", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, num);

		fAntOut = fopen(antoutfilename, "w");
		if (fAntOut == NULL)
		{
			SDL_Log("Could not open output file: %s", antoutfilename);
			exit(32);
		}
		fprintf(fAntOut, "Dimensions: %dx%d, Scale: %d, Spacing: %d, Margin: %d, Instructionset: %s\n\n", settings.SCREEN.w, settings.SCREEN.h, settings.SCALE, settings.SPACING, settings.ANTMARGIN, settings.instructionset);
	}

	SDL_Log("Dimensions: %dx%d, Scale: %d, Spacing: %d, Margin: %d, Instructionset: %s\n\n", settings.SCREEN.w, settings.SCREEN.h, settings.SCALE, settings.SPACING, settings.ANTMARGIN, settings.instructionset);

	Running = true;

	//Simulation loop
	while (!quit)
	{
		SDL_WaitEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			paused = false;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				Running = false;
				paused = false;
				quit = true;
				break;
			case SDLK_p:
				paused = true;
				Running = false;
				start = false;

				//Save on returning to menu
				fprintf(fAntOut, "\nThis run was sponsored by ant gang.");
				fclose(fAntOut);

				char antouttexturename[100];
				sprintf(antouttexturename, "./Runs/antout_%d-%d-%d_%dh-%dm-%ds(%d).bmp", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, num);
				save_texture(gRenderer, tPixelTexture, antouttexturename);
				SDL_Log("Saved instructions as TXT to \"%s\"\n", antoutfilename);

				//Return to menu
				goto startup;
			case SDLK_f:
			j1:
				if (!moveAnt(&pixelTex, &ant, &lepes, settings, instructnum, fAntOut))
				{
					SDL_Log("Error while trying to move ant: %s", SDL_GetError());
					ERROR = true;
					quit = true;
					break;
				}
				convertPixels(&pixels, &pixelTex, settings.SCREEN);
				SDL_UpdateTexture(tPixelTexture, NULL, pixels, settings.SCREEN.w * sizeof(Uint32));
				break;
			case SDLK_g:
			j100:
				for (int i = 0; i < 104; i++)
				{
					if (!moveAnt(&pixelTex, &ant, &lepes, settings, instructnum, fAntOut))
					{
						SDL_Log("Error while trying to move ant: %s", SDL_GetError());
						ERROR = true;
						quit = true;
						break;
					}
				}
				convertPixels(&pixels, &pixelTex, settings.SCREEN);
				SDL_UpdateTexture(tPixelTexture, NULL, pixels, settings.SCREEN.w * sizeof(Uint32));
				break;
			case SDLK_h:
			j1000:
				for (int i = 0; i < 1040; i++)
				{
					if (!moveAnt(&pixelTex, &ant, &lepes, settings, instructnum, fAntOut))
					{
						SDL_Log("Error while trying to move ant: %s", SDL_GetError());
						ERROR = true;
						quit = true;
						break;
					}
				}
				convertPixels(&pixels, &pixelTex, settings.SCREEN);
				SDL_UpdateTexture(tPixelTexture, NULL, pixels, settings.SCREEN.w * sizeof(Uint32));
				break;
			case SDLK_j:
			j10000:
				for (int i = 0; i < 10400; i++)
				{
					if (!moveAnt(&pixelTex, &ant, &lepes, settings, instructnum, fAntOut))
					{
						SDL_Log("Error while trying to move ant: %s", SDL_GetError());
						ERROR = true;
						quit = true;
						break;
					}
				}
				convertPixels(&pixels, &pixelTex, settings.SCREEN);
				SDL_UpdateTexture(tPixelTexture, NULL, pixels, settings.SCREEN.w * sizeof(Uint32));
				break;
			case SDLK_SPACE:
				Running = true;
				break;
			}
		//Base tickrate
		case SDL_USEREVENT:
			while (Running && !quit)
			{
				if (!moveAnt(&pixelTex, &ant, &lepes, settings, instructnum, fAntOut))
				{
					SDL_Log("Error while trying to move ant: %s", SDL_GetError());
					ERROR = true;
					quit = true;
				}
				convertPixels(&pixels, &pixelTex, settings.SCREEN);
				SDL_UpdateTexture(tPixelTexture, NULL, pixels, settings.SCREEN.w * sizeof(Uint32));
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
					paused = false;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
					case SDLK_SPACE:
						Running = false;
						break;
					case SDLK_ESCAPE:
						Running = false;
						paused = false;
						quit = true;
						break;
					case SDLK_p:
						paused = true;
						Running = false;
						start = false;

						//Save on returning to menu
						fprintf(fAntOut, "\nThis run was sponsored by ant gang.");
						fclose(fAntOut);

						char antouttexturename[100];
						sprintf(antouttexturename, "./Runs/antout_%d-%d-%d_%dh-%dm-%ds(%d).bmp", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, num);
						save_texture(gRenderer, tPixelTexture, antouttexturename);
						SDL_Log("Saved instructions as TXT to \"%s\"\n", antoutfilename);

						//Return to menu start on hitting p
						goto startup;
					case SDLK_f:
						goto j1;
						break;
					case SDLK_g:
						goto j100;
						break;
					case SDLK_h:
						goto j1000;
						break;
					case SDLK_j:
						goto j10000;
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
				switch (event.type)
				{
				case SDL_QUIT:
					equit = true;
					paused = false;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						Running = false;
						paused = false;
						equit = true;
						break;
					case SDLK_p:
						paused = true;
						Running = false;
						ERROR = false;
						quit = false;
						start = false;

						//Save on returning to menu
						fprintf(fAntOut, "\nThis run was sponsored by ant gang.");
						fclose(fAntOut);

						char antouttexturename[100];
						sprintf(antouttexturename, "./Runs/antout_%d-%d-%d_%dh-%dm-%ds(%d).bmp", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, num);
						save_texture(gRenderer, tPixelTexture, antouttexturename);
						SDL_Log("Saved instructions as TXT to \"%s\"\n", antoutfilename);

						//Return to menu start on hitting p
						goto startup;
					}
				}
			}
		}
		else
		{
			SDL_RenderCopy(gRenderer, tPixelTexture, NULL, NULL);
			SDL_RenderPresent(gRenderer);
		}
	}

	fprintf(fAntOut, "This run was sponsored by ant gang.");
	SDL_Log("Saved instructions as TXT to \"%s\"\n", antoutfilename);
	fclose(fAntOut);

	if (!paused)
	{
		char antouttexturename[100];
		sprintf(antouttexturename, "./Runs/antout_%d-%d-%d_%dh-%dm-%ds(%d).bmp", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, num);
		save_texture(gRenderer, tPixelTexture, antouttexturename);
	}

end:
	//Free resources and close SDL
	close(&pixels, &pixelTex, gWindow, gRenderer, tPixelTexture, tMainMenu, tStrings);
	return 0;
}