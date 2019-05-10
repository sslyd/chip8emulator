#include <windows.h>
#include <stdio.h>

#define BUFSIZE 2048

HANDLE openrom(const wchar_t* filepath)
{
	return CreateFileW(filepath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
}

void readrom(HANDLE hFile, unsigned char* memory)
{
	LONG IDistance = 0;

	unsigned char buf[1];
	DWORD sb = 0;

	if (SetFilePointer(hFile, IDistance, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)printf("Set File Pointer Failed!");
	int error = 0;
	memory += 0x200;
	do
	{
		ReadFile(hFile, buf, 1, &sb, NULL);
		memcpy(memory, buf, 1);
		memory += 0x1;
		//printf("%x  ", buf[0]);
		error = GetLastError();
	} while (sb != 0);
}

void closerom(HANDLE hFile)
{
	CloseHandle(hFile);
}

//void showmemory(unsigned char* memory)
//{
//	for (int y = 0; y < 0x40; ++y)
//	{
//		printf("%#5x ", y * 0x40);
//		for (int x = 0; x < 0x40; ++x)
//		{
//			printf("%5x ", memory[y * 0x40 + x]);
//		}
//	}
//}