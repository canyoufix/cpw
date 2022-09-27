#include <Windows.h>
#include <stdio.h> 
#define BUF_SIZE 256

int wmain(int argc, wchar_t** argv) {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	HANDLE hIn, hOut;
	DWORD nIn, nOut;
	CHAR Buffer[BUF_SIZE];

	if (argc != 3) {
		printf("Использование: cpw test.txt out.txt\n");
		return  1;
	}

	/*LPCWSTR name1 = L"C:"; Для дебага
	LPCWSTR name2 = L"C:*/

	hIn = CreateFile(argv[1], GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hIn == INVALID_HANDLE_VALUE) {
		printf("Невозможно открыть входной файл. Ошибка: %x\n", GetLastError());
		return  2;
	}

	hOut = CreateFile(argv[2], GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hOut == INVALID_HANDLE_VALUE) {
		printf("Невозможно открыть выходной файл. Ошибка: %x\n", GetLastError());
		return 3;
	}

	while (ReadFile(hIn, Buffer, BUF_SIZE, &nIn, NULL) && nIn > 0) {
		INT i = 0, numReplace = 0;
		while (i < nIn) {
			if (Buffer[i] >= '0' && Buffer[i] <= '9') {
				Buffer[i] = ' ';
				numReplace++;
			}
			i++;
		}

		WriteFile(hOut, Buffer, nIn, &nOut, NULL);
		if (nIn != nOut) {
			printf("Неустранимая ошибка записи: %x\n", GetLastError());
			return 4;
		}
		printf("Количество замен: %d", numReplace);
	}

	CloseHandle(hIn);
	CloseHandle(hOut);
	return 0;
}