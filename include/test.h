#include "chip8.h"
#ifndef TEST_H_
#define TEST_H_

#define ALL_TEST_OPCODE 35

CHIP8_BYTE CheckResult( CHIP8_WORD opcode, 
						CHIP8_BYTE* v, 
						CHIP8_WORD* stack, 
						CHIP8_WORD* pc, 
						CHIP8_WORD* sp, 
						CHIP8_WORD* i);

void ShowStatus(CHIP8_BYTE* v, CHIP8_WORD* pc, CHIP8_WORD* stack, CHIP8_WORD* sp, CHIP8_WORD* i, CHIP8_WORD* delay_timer, CHIP8_WORD* sound_timer);

void showvram(CHIP8_BYTE* videomemory);

void showram(CHIP8_BYTE* memory);

void shownowopcode(CHIP8_BYTE* memory, CHIP8_WORD* pc);

#endif // !TEST_H_