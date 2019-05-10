#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <SDL.h>

#ifndef CHIP8EMU_H_
#define CHIP8EMU_H_


#define CHIP8_PC 0x2

#define CHIP8_BYTE unsigned char
#define CHIP8_WORD unsigned short

#define CHIP8MEM 0x1000
//chip8 has 4096 KB memory

#define CHIP8REG 0x10
//chip8 has 0xf registers

#define CHIP8STACK 0x10
//chip8 have at least 16 levels stack

#define CHIP8VIDEOWIDTH 64
#define CHIP8VIDEOHEIGHT 32
//chip 8 video preference

#define CHIP8FREQ 1
//chip8 frequence 60HZ

#define CHIP8MS 1000

//chip8 memory map
//0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
//0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
//0x200-0xFFF - Program ROM and work RAM

struct CHIP8CPU
{
	CHIP8_BYTE* V = NULL;		//chip8 registers
	CHIP8_WORD* stack = NULL;	//chip8 stack
	CHIP8_WORD* sp;				//chip8 stack pointer
	CHIP8_WORD* pc;				//chip8 program counter
	CHIP8_WORD* delay_timer;	//chip8 delay timer
	CHIP8_WORD* sound_timer;	//chip8 sound timer
	CHIP8_WORD* i;				//chip8 index register
};

void chip8_quit(CHIP8_BYTE* memory,
	CHIP8_BYTE* v,
	CHIP8_WORD* stack,
	CHIP8_WORD* pc,
	CHIP8_WORD* sp,
	CHIP8_WORD* i,
	CHIP8_WORD* delay_timer,
	CHIP8_WORD* sound_timer,
	CHIP8_BYTE* key,
	CHIP8_BYTE* videomemory,
	SDL_Window* window,
	SDL_Renderer* renderer);

void Init_CPU(CHIP8_BYTE** memory,
	CHIP8_BYTE** v,
	CHIP8_WORD** stack,
	CHIP8_WORD** pc,
	CHIP8_WORD** sp,
	CHIP8_WORD** i,
	CHIP8_WORD** delay_timer,
	CHIP8_WORD** sound_timer,
	CHIP8_BYTE** key,
	CHIP8_BYTE** videoMemory
	);


CHIP8_WORD CPU_Interrupte(CHIP8_WORD opcode,
	CHIP8_BYTE* v,
	CHIP8_WORD* stack,
	CHIP8_WORD* pc,
	CHIP8_WORD* sp,
	CHIP8_WORD* i,
	CHIP8_BYTE* key,
	CHIP8_WORD* delay_timer,
	CHIP8_WORD* sound_timer,
	CHIP8_BYTE* memory,
	CHIP8_BYTE* videomemory);

CHIP8_WORD CPU_Cycle(CHIP8_BYTE* v,
	CHIP8_WORD* stack,
	CHIP8_WORD* pc,
	CHIP8_WORD* sp,
	CHIP8_WORD* i,
	CHIP8_BYTE* key,
	CHIP8_WORD* delay_timer,
	CHIP8_WORD* sound_timer,
	CHIP8_BYTE* memory,
	CHIP8_BYTE* videomeory);

void quit_sdl(SDL_Window* window, SDL_Renderer* renderer);

void Init_Memory(CHIP8_BYTE** memory);

void Test_Memory(CHIP8_BYTE** memory);

#endif // !CHIP8EMU_H_