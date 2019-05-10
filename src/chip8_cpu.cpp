#include <limits.h>
#include "chip8.h"
#include "test.h"

void Init_CPU(CHIP8_BYTE** memory,
	CHIP8_BYTE** v,
	CHIP8_WORD** stack,
	CHIP8_WORD** pc,
	CHIP8_WORD** sp,
	CHIP8_WORD** i,
	CHIP8_WORD** delay_timer,
	CHIP8_WORD** sound_timer,
	CHIP8_BYTE** key,
	CHIP8_BYTE** videomemory)
{
	*memory = (CHIP8_BYTE*)malloc(sizeof(CHIP8_BYTE) * CHIP8MEM);
	memset(*memory, 0, sizeof(CHIP8_BYTE) * CHIP8MEM);

	*v = (CHIP8_BYTE*)malloc(sizeof(CHIP8_BYTE) * CHIP8REG);
	memset(*v, 0, sizeof(CHIP8_BYTE) * CHIP8REG);
	
	*stack = (CHIP8_WORD*)malloc(sizeof(CHIP8_WORD) * CHIP8STACK);
	memset(*stack, 0, sizeof(CHIP8_WORD) * CHIP8STACK);

	*pc = (CHIP8_WORD*)malloc(sizeof(CHIP8_WORD));
	memset(*pc, 0, sizeof(CHIP8_WORD));
	
	*sp = (CHIP8_WORD*)malloc(sizeof(CHIP8_WORD));
	memset(*sp, 0, sizeof(CHIP8_WORD));

	*i = (CHIP8_WORD*)malloc(sizeof(CHIP8_WORD));
	memset(*i, 0, sizeof(CHIP8_WORD));

	*delay_timer = (CHIP8_WORD*)malloc(sizeof(CHIP8_WORD));
	memset(*delay_timer, 0, sizeof(CHIP8_WORD));

	*sound_timer = (CHIP8_WORD*)malloc(sizeof(CHIP8_WORD));
	memset(*sound_timer, 0, sizeof(CHIP8_WORD));
	
	*key = (CHIP8_BYTE*)malloc(sizeof(CHIP8_BYTE));
	memset(*key, 0, sizeof(CHIP8_BYTE));

	*videomemory = (CHIP8_BYTE*)malloc(sizeof(CHIP8_BYTE) * CHIP8VIDEOWIDTH*CHIP8VIDEOHEIGHT);
	memset(*videomemory, 0, sizeof(CHIP8_BYTE) * CHIP8VIDEOWIDTH * CHIP8VIDEOHEIGHT);

	srand(time(NULL));//Set random seed;

	**sp = 0;
	**pc = 0x200;	//System run from 0x200
	**delay_timer = 0;
	**sound_timer = 0;
	**i = 0;
}

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
	CHIP8_BYTE* videomeory)
{

	int coordinaryX = 0;
	int coordinaryY = 0;

	CHIP8_BYTE pixel;

	switch (opcode & 0xF000)
	{
	case 0x0000:
	{

		switch (opcode & 0xff)
		{
		case 0xe0:
			memset(videomeory, 0, sizeof(CHIP8_BYTE) * CHIP8VIDEOWIDTH * CHIP8VIDEOHEIGHT);
			return 1;
			//break;

		case 0xee:
			//TODO Return From subroutine
			(*sp)--;
			*pc = stack[*sp];
			return 1;
		
		default:
			return 0;
		}
	}
	case 0x1000:
		*pc = opcode & 0xfff;
		return 2;
		//break;

	case 0x2000:
		stack[(*sp)] = (*pc);
		(*sp)++;
		*pc = opcode & 0xfff;
		return 2;
		//break;

	case 0x3000:
		//Skips the next instruction if VX equals NN. (Usually the next instruction is a jump to skip a code block)
		if ((v[(opcode & 0x0f00) / 0xff]) == (opcode & 0xff))
			*pc += CHIP8_PC;
		return 1;
		//break;

	case 0x4000:
		//Skips the next instruction if VX doesn't equal NN. (Usually the next instruction is a jump to skip a code block) 
		if ((v[(opcode & 0x0f00) / 0xff]) != (opcode & 0xff))
			*pc += CHIP8_PC;
		return 1;

	case 0x5000:
		//Skips the next instruction if VX equals VY. (Usually the next instruction is a jump to skip a code block) 
		if (((v[(opcode & 0xf00) / 0xff]) == (v[(opcode & 0xf0) / 0xf]))&&((opcode & 0xf) == 0x0))
			*pc += CHIP8_PC;
		return 1;

	case 0x6000:
		v[(opcode & 0xf00) / 0xff] = (opcode & 0xff);
		return 1;

	case 0x7000:
		v[(opcode & 0xf00) / 0xff] += (opcode & 0xff);
		return 1;

	case 0x8000:
		switch (opcode & 0xf)
		{
		case 0x0:
			v[(opcode & 0xf00) / 0xff] = v[(opcode & 0xf0) / 0xf];
			return 1;

		case 0x1:
			v[(opcode & 0xf00) / 0xff] = (v[(opcode & 0xf00) / 0xff]) | (v[(opcode & 0xf0) / 0xf]);
			return 1;

		case 0x2:
			v[(opcode & 0xf00) / 0xff] = (v[(opcode & 0xf00) / 0xff]) & (v[(opcode & 0xf0) / 0xf]);
			return 1;
			
		case 0x3:
			v[(opcode & 0xf00) / 0xff] = (v[(opcode & 0xf00) / 0xff]) ^ (v[(opcode & 0xf0) / 0xf]);
			return 1;

		case 0x4:

			if (((v[(opcode & 0xf00) / 0xff]) + (v[(opcode & 0xf0) / 0xf]) < v[(opcode & 0xf00) / 0xff]) || ((v[(opcode & 0xf00) / 0xff]) + (v[(opcode & 0xf0) / 0xf]) < v[(opcode & 0xf0) / 0xf])) //detect overflow
				v[0xf] = 1;
			else v[0xf] = 0;
			v[(opcode & 0xf00) / 0xff] = (v[(opcode & 0xf00) / 0xff]) + (v[(opcode & 0xf0) / 0xf]);
			return 1;

		case 0x5:
			if (((v[(opcode & 0xf00) / 0xff]) - (v[(opcode & 0xf0) / 0xf]) > v[(opcode & 0xf00) / 0xff]) || ((v[(opcode & 0xf00) / 0xff]) - (v[(opcode & 0xf0) / 0xf]) > v[(opcode & 0xf0) / 0xf]))//detect overflow
				v[0xf] = 1;
			else v[0xf] = 0;
			v[(opcode & 0xf00 / 0xff)] = (v[(opcode & 0xf00) / 0xff]) - (v[(opcode & 0xf0) / 0xf]);
			return 1;

		case 0x6:
			v[0xf] = v[(opcode & 0xf00) / 0xff] & 0x1;
			v[(opcode & 0xf00) / 0xff] >>= 1;
			return 1;

		case 0x7:
			if (((v[(opcode & 0xf00) / 0xff]) - (v[(opcode & 0xf0) / 0xf]) > v[(opcode & 0xf00) / 0xff]) || ((v[(opcode & 0xf00) / 0xff]) - (v[(opcode & 0xf0) / 0xf]) > v[(opcode & 0xf0) / 0xf]))//detect overflow
				v[0xf] = 1;
			else v[0xf] = 0;
			v[(opcode & 0xf00 / 0xff)] =  (v[(opcode & 0xf0) / 0xf]) -  (v[(opcode & 0xf00) / 0xff]);
			return 1;

		case 0xe:
			v[0xf] = ( (v[(opcode & 0xf00) / 0xff]) & 0x80 )/0xfff;
			v[(opcode & 0xf00) / 0xff] <<= 1;
		}

	case 0x9000:
		if ((v[(opcode & 0x0f00) / 0xff]) == (v[(opcode & 0x0f0) / 0xf]) && ((opcode & 0xf) == 0x0))
		{
			*pc += CHIP8_PC;
			return 1;
		}
		return 1;

	case 0xa000:
		*i = opcode & 0xfff;
		return 1;

	case 0xb000:
		*i = opcode & 0xfff + v[0x0];
		return 1;

	case 0xc000:
		v[(opcode & 0x0f00) / 0xff] = (rand() % 256) & (opcode & 0xff);
		return 1;
	
	case 0xd000:
		//TODE Draw Sprite;
		coordinaryX = v[(opcode & 0xf00) / 0xff];
		coordinaryY = v[(opcode & 0xf0) / 0xf];
		for (int yline = 0; yline < (opcode & 0xf); ++yline)
		{
			pixel = memory[yline + (*i)];
			for (int xline = 0; xline < 0x8; ++xline)
			{
				if (pixel & (0x80 >> xline))
				{
					if (videomeory[(yline+coordinaryY) * CHIP8VIDEOWIDTH + (xline+coordinaryX)])
						v[0xf] = 1;
					videomeory[(yline + coordinaryY) * CHIP8VIDEOWIDTH + (xline + coordinaryX)] ^= 1;
				}
			}
		}
		return 1;

	case 0xe000:
		switch (opcode & 0xff)
		{
		case 0x9e:
			if (v[(opcode & 0xf00) / 0xff] == *key)
				* pc += CHIP8_PC;
			return 1;

		case 0xa1:
			if (v[(opcode & 0xf00) / 0xff] != *key)
				* pc += CHIP8_PC;
			return 1;
		}

	case 0xf000:
		switch (opcode & 0xff)
		{
		case 0x07:
			v[(opcode & 0xf00) / 0xff] = *delay_timer;
			return 1;
		
		case 0x0a:
			v[(opcode & 0xf00) / 0xff] = *key;
			return 1;
		
		case 0x15:
			*delay_timer = v[(opcode & 0xf00) / 0xff];
			return 3;

		case 0x18:
			*sound_timer = v[(opcode & 0xf00) / 0xff];
			return 3;

		case 0x1e:
			*i += v[(opcode & 0xf00) / 0xff];
			return 1;

		case 0x29:

			return 1;

		case 0x33:
			memory[*i] = (CHIP8_WORD)(v[(opcode & 0xf00) / 0xff] / 0x64);
			memory[(*i) + 1] = (v[(opcode & 0xf00) / 0xff] / 0xa ) % 0xa;
			memory[(*i) + 2] = (CHIP8_WORD)v[(opcode & 0xf00) / 0xff] & 0xa;
			return 1;

		case 0x55:
			for (CHIP8_BYTE count = 0; count < ((opcode & 0xf00) / 0xff); ++count)
				memory[(*i) + count] = v[count];
			return 1;

		case 0x65:
			for (CHIP8_BYTE count = 0; count < ((opcode & 0xf00) / 0xff); ++count)
				v[count] = memory[(*i) + count];
			return 1;

		}

	default:
		return 0;
	}
}

CHIP8_WORD CPU_Cycle(CHIP8_BYTE* v,
	CHIP8_WORD* stack,
	CHIP8_WORD* pc,
	CHIP8_WORD* sp,
	CHIP8_WORD* i,
	CHIP8_BYTE* key,
	CHIP8_WORD* delay_timer,
	CHIP8_WORD* sound_timer,
	CHIP8_BYTE* memory,
	CHIP8_BYTE* videomeory)
{
	CHIP8_WORD opcode;

	//opcode = ((*memory)[(*pc)] << 8) + (*memory)[*(pc + 2)];
	//CPU_Interrupte(opcode, stack, pc, sp, i);
	//*pc += CHIP8_PC;
	opcode = (memory[*pc] << 8) + (memory[(*pc) + 0x1]);
	
	int returnValue = CPU_Interrupte(opcode, v, stack, pc, sp, i, key, delay_timer, sound_timer, memory, videomeory);
	if(returnValue != 2)*pc += CHIP8_PC;
	if (*delay_timer > 0 || returnValue == 3)(*delay_timer)--;
	if (*sound_timer > 0 || returnValue == 3)(*sound_timer)--;
	
	//printf("\nNow the opcode = %#x\n\n", opcode);
	//showvram(videomeory);
	//printf("m << 8 = %#x , m + 2 = %#x \n", memory[*pc], memory[(*pc) + 0x1]);
	//printf("pc = %#hx when works like a sb = %#x\n", *pc, (memory[*pc] << 8) + (memory[(*pc) + 0x1]));
	
	return returnValue;
}

void Init_Memory(CHIP8_BYTE * *memory)
{
	(*memory) = (CHIP8_BYTE*)malloc(sizeof(CHIP8_BYTE) * CHIP8MEM);
	Test_Memory(memory);
}

void quit_sdl(SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

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
	SDL_Renderer* renderer)
{
	//free memory when init
	free(memory);
	free(v);
	free(stack);
	free(pc);
	free(sp);
	free(i);
	free(delay_timer);
	free(sound_timer);
	free(key);
	free(videomemory);

	//quit SDL
	quit_sdl(window, renderer);
}

void Test_Memory(CHIP8_BYTE * *memory)
{
	CHIP8_BYTE* m = *memory;
	m[0x200] = 0x10;
}
