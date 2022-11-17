#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h> 

#define BUF_SIZE 256

INT(*changeBuffer)(CHAR[], DWORD*, INT);

int main(int argc, char* argv[]) {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	HANDLE hIn, hOut;
	HINSTANCE hLib;
	DWORD nIn, nOut;
	CHAR Buffer[BUF_SIZE], newNameOut[BUF_SIZE];

	if (argc < 3) {
		printf("Неправильное количество аргументов!\n");
		exit(-1);
	}
	char* cmdLine = (char*)GetCommandLine();
	printf("\n");

	//DLL
	hLib = LoadLibrary("DLL.dll");
	if (hLib == NULL) {
		printf("Не удалось загрузить библиотеку.\n");
		exit(-1);
	}
	changeBuffer = (INT(*)(CHAR[], DWORD*, INT))GetProcAddress(hLib, "changeBuffer");
	if (changeBuffer == NULL) {
		printf("Вызываемая функция не найдена\n");
		exit(-2);
	}

	hIn = CreateFile(argv[1], GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hIn == INVALID_HANDLE_VALUE) {
		printf("Невозможно открыть входной файл. Ошибка: %x\n", GetLastError());
		return  2;
	}

	int i = 0;
	while (*(argv[1] + i) != '.') {
		newNameOut[i] = *(argv[1] + i);
		i++;
	}
	newNameOut[i] = '\0';
	strcat(newNameOut, "_New.txt");

	hOut = CreateFile(newNameOut, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hOut == INVALID_HANDLE_VALUE) {
		printf("Невозможно открыть выходной файл. Ошибка: %x\n", GetLastError());
		return 3;
	}

	INT numOfReplace = 0;
	while (ReadFile(hIn, Buffer, BUF_SIZE, &nIn, NULL) && nIn > 0) {
		INT numFromCMD = atoi(argv[argc - 1]);

		numOfReplace = (*changeBuffer)(Buffer, &nIn, numFromCMD);
		if (numOfReplace == -1) {
			return 5;
		}

		WriteFile(hOut, Buffer, nIn, &nOut, NULL);
		if (nIn != nOut) {
			printf("Неустранимая ошибка записи: %x\n", GetLastError());
			return 4;
		}
		printf("Количество замененных цифр: %d\n", numOfReplace);
	}

	printf("(Process[ID]: %lu), File [%s] replace = %d\n", GetCurrentProcessId(), argv[1], numOfReplace);
	CloseHandle(hIn);
	CloseHandle(hOut);
	FreeLibrary(hLib);

	return numOfReplace;
}