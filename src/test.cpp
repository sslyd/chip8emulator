#include "chip8.h"
#include "test.h"


CHIP8_WORD test_opcode_table[ALL_TEST_OPCODE] = { 0x00e0,
							 0x00ee,
							 0x1abc,
							 0x2abc,
							 0x3fab,
							 0x4ecd,
							 0x5ef0,
							 0x6913,
							 0x753a,
							 0x8460,
							 0x8171,
							 0x8d52,
							 0x8b83,
							 0x8a24,
							 0x8965,
							 0x8316,
							 0x8457,
							 0x897e,
							 0x9540,
							 0xa123,
							 0xb123,
							 0xc245,
							 0xd123,
							 0xe19e,
							 0xe2a1,
							 0xf107,
							 0xf20a,
							 0xf30a,
							 0xf415,
							 0xf518,
							 0xf61e,
							 0xf729,
							 0xf833,
							 0xf855,
							 0xf965
};

void ShowStatus(CHIP8_BYTE* v, CHIP8_WORD* pc, CHIP8_WORD* stack, CHIP8_WORD* sp, CHIP8_WORD* i, CHIP8_WORD* delay_timer, CHIP8_WORD* sound_timer)
{
	printf("PC = %#x\n\n", *pc);
	for (int i = 0; i < CHIP8STACK; ++i)
	{
		printf("stack%d = %#x ", i, *(stack + i));
	}
	printf("\n");
	printf("sp = %#x i = %#x\n ", *sp, *i);
	for (int c = 0; c < CHIP8REG; ++c)
	{
		printf("V%x = %#x ", c, *(v + c));
	}
	printf("\ndelay_timer = %#x  sound_timer = %#x\n", *delay_timer, *sound_timer);
}

CHIP8_BYTE CheckResult(CHIP8_WORD opcode, 
					   CHIP8_BYTE* v, 
					   CHIP8_WORD* stack, 
					   CHIP8_WORD* pc, 
					   CHIP8_WORD* sp, 
					   CHIP8_WORD* i)
{
	switch (opcode & 0xf000)
	{
	case 0x0000:
	{

		switch (opcode & 0xff)
		{
		case 0xe0:
			//TODO Clear The Screen
			return 1;
			//break;

		case 0xee:
			//TODO Return From subroutine
			return 1;
			//break;
			/*default:
				pc += CHIP8_PC;
				break;*/
		}
	}
	case 0x1000:
		if ((*pc) == (opcode & 0xfff))
			return 1;
		else return 0;
		//break;
	case 0x2000:
		if (stack[(*sp) - 1] == (*pc))
			return 1;
		else return 0;
		//break;
	case 0x3000:
		return 1;
		//break;
	default:
		return 0;
	}
}

void showram(CHIP8_BYTE* memory)
{
	//printf("fuck you\n");
	for (int y = 0; y < 0x43; ++y)
	{
		printf("%#5x ", y * 0x43);
		for (int x = 0; x < 0x3d; ++x)
		{
			if ((y * 0x43 + x)<= 0x1000)printf("%2x ", memory[y * 0x43 + x]);
		}
		printf("\n");
	}
}


void showvram(CHIP8_BYTE* videomemory)
{
	//printf("fuck you\n");
	for (int y = 0; y < CHIP8VIDEOHEIGHT; ++y)
	{
		printf("%#5x ", y * 64);
		for (int x = 0; x < CHIP8VIDEOWIDTH; ++x)
		{
			if (videomemory[y * 64 + x])printf("* ");
			else printf("  ");
		}
		printf("\n");
	}
}

void shownowopcode(CHIP8_BYTE* memory, CHIP8_WORD* pc)
{
	printf("m << 8 = %#x , m + 2 = %#x \n", memory[*pc], memory[(*pc) + 0x2]);
	printf("Now the oppcode = %#x\n", ((memory[(*pc)] << 8) + (memory[(*pc) + CHIP8_PC])));
}