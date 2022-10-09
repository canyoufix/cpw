#include <Windows.h>
#include <stdio.h> 

#define BUF_SIZE 256

INT(*changeBuffer)(CHAR[], DWORD*, INT);

int wmain(int argc, wchar_t** argv) {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	HANDLE hIn, hOut;
	HINSTANCE hLib;

	DWORD nIn, nOut;
	CHAR Buffer[BUF_SIZE];
	LPCWSTR outFile = L"out.txt";

	if (argc != 3) { //�������� ���������� ����������
		printf("�������������: cpw test.txt �����\n");
		return  1;
	}
	printf("\n");

	//DLL
	hLib = LoadLibrary(L"DLL.dll");
	if (hLib == NULL) {
		printf("�� ������� ��������� ����������.");
		exit(-1);
	}
	changeBuffer = (INT(*)(CHAR[], DWORD*, INT))GetProcAddress(hLib, "changeBuffer");
	if (changeBuffer == NULL) {
		printf("���������� ������� �� �������");
		exit(-2);
	}



	hIn = CreateFile(argv[1], GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hIn == INVALID_HANDLE_VALUE) {
		printf("���������� ������� ������� ����. ������: %x\n", GetLastError());
		return  2;
	}

	hOut = CreateFile(outFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hOut == INVALID_HANDLE_VALUE) {
		printf("���������� ������� �������� ����. ������: %x\n", GetLastError());
		return 3;
	}

	while (ReadFile(hIn, Buffer, BUF_SIZE, &nIn, NULL) && nIn > 0) {
		INT numFromCMD = _wtoi(argv[2]);

		INT numOfReplace = (*changeBuffer)(Buffer, &nIn, numFromCMD);
		if (numOfReplace == -1) {
			return 5;
		}

		WriteFile(hOut, Buffer, nIn, &nOut, NULL);
		if (nIn != nOut) {
			printf("������������ ������ ������: %x\n", GetLastError());
			return 4;
		}
		printf("���������� ���������� ����: %d", numOfReplace);
	}

	CloseHandle(hIn);
	CloseHandle(hOut);
	FreeLibrary(hLib);
	return 0;
}