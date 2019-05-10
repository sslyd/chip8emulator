#pragma once

#include <windows.h>

#ifndef READROM_H_
#define READROM_H_

HANDLE openrom(const wchar_t* filepath);

void readrom(HANDLE hFile, unsigned char* memory);

void closerom(HANDLE hFile);

#endif //READROM_H_
