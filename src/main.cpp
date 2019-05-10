#define SDL_MAIN_HANDLED

#include <SDL.h>

#include "chip8.h"
#include "test.h"
#include "readrom.h"

#define PIXELOFFSET 10
#define WINDOWWIDTH 640
#define WINDOWHEGIHT 320

const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

void InitWindow(SDL_Window* window)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_CreateWindow("MyChip8Emulator", 100, 100, WINDOWWIDTH, WINDOWHEGIHT, NULL);
}

void CleanScreen(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
}

void DrawPixel(SDL_Renderer* renderer, int x, int y)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	for (int doty = 0; doty < PIXELOFFSET; ++doty)
	{
		for (int dotx = 0; dotx < PIXELOFFSET; ++dotx)
		{
			SDL_RenderDrawPoint(renderer, dotx+x*PIXELOFFSET, doty+y*PIXELOFFSET);
		}
	}
}

void DrawScreen(SDL_Renderer* renderer, CHIP8_BYTE* videomemory)
{
	for (int y = 0; y < CHIP8VIDEOHEIGHT; ++y)
	{
		for (int x = 0; x < CHIP8VIDEOWIDTH; ++x)
		{
			if (videomemory[y * 64 + x])DrawPixel(renderer, x, y);
		}
	}
}

int main(void)
{
	CHIP8_BYTE* v = NULL;
	CHIP8_WORD* stack = NULL;
	CHIP8_WORD* pc = NULL;
	CHIP8_WORD* sp = NULL;
	CHIP8_WORD* i = NULL;
	CHIP8_WORD* delay_timer = NULL;
	CHIP8_WORD* sound_timer = NULL;
	CHIP8_BYTE* memory	= NULL;	//chip8 memory

	CHIP8_BYTE* videoMemory = NULL;
	
	CHIP8_BYTE* key = NULL;			//chip8 key down
	
	SDL_Window* window = NULL;
	SDL_Event event;
	SDL_Renderer* renderer = NULL;

	Uint32 starttime = SDL_GetTicks();

	int countframe = 0;
	//Uint32 tick

	SDL_Init(SDL_INIT_EVERYTHING);
	//InitWindow(window);
	Init_CPU(&memory, &v, &stack, &pc, &sp, &i, &delay_timer, &sound_timer, &key, &videoMemory);
	readrom(openrom(L"1.ch8"), memory);

	SDL_CreateWindowAndRenderer(WINDOWWIDTH, WINDOWHEGIHT, 0, &window, &renderer);

	int stop = 0;
	int quit = 0;
	while (!quit)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				quit = 1;
				break;
			case SDL_KEYDOWN:
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_d:
					showvram(videoMemory);
					break;
				case SDLK_a:
					stop = 1;
					break;
				case SDLK_s:
					ShowStatus(v, pc, stack, sp, i, delay_timer, sound_timer);
					//showram(memory);
					CPU_Cycle(v, stack, pc, sp, i, key, delay_timer, sound_timer, memory, videoMemory);
					break;
				}
			}
			}
		}
		if ((SDL_GetTicks() - starttime) >= ((float)(1/60))*1000)
		{
			starttime = SDL_GetTicks();
			if (!stop)CPU_Cycle(v, stack, pc, sp, i, key, delay_timer, sound_timer, memory, videoMemory);
		}

		CleanScreen(renderer);
		DrawScreen(renderer, videoMemory);
		SDL_RenderPresent(renderer);

		countframe++;
	}
	chip8_quit(memory, v, stack, pc, sp, i, delay_timer, sound_timer, key, videoMemory, window, renderer);

	return 0;
}