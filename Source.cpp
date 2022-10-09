#include <Windows.h>
#include <stdio.h> 

#define BUF_SIZE 256

int wmain(int argc, wchar_t** argv) {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	HANDLE hIn, hOut;
	DWORD nIn, nOut;
	CHAR Buffer[BUF_SIZE];
	LPCWSTR outFile = L"out.txt";

	if (argc != 3) {
		printf("�������������: cpw test.txt �����\n");
		return  1;
	}
	printf("\n");


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
		INT numOfNum = 0, index = 0;
		while (index < nIn) { //���������� ���� �� ���� �������
			if (Buffer[index] >= '0' && Buffer[index] <= '9') {
				numOfNum++;
			}
			index++;
		}

		INT numFromCMD = _wtoi(argv[2]);
		if (numOfNum == 0 || numFromCMD <= 0) { //�������� ���������� �������� ����
			printf("������ �������� 0 ����(�), ���� � ��������� ��� �����������.\n");
			return 5;
		}
		if (numOfNum >= numFromCMD) {
			printf("����� �������� %d ����(�).\n", numFromCMD);
		}
		else {
			printf("�� ���� ��������� %d ����(�), � �� %d, ������� ����� �������� ���.\n\n", numOfNum, numFromCMD);
			numFromCMD = numOfNum;
		}

		index = 0;
		INT numOfReplace = 0;
		while (index < nIn) {
			if (Buffer[index] >= '0' && Buffer[index] <= '9') {
				Buffer[index] = ' ';
				numOfReplace++;
				if (numOfReplace == numFromCMD) {
					index = nIn;
				}
			}
			index++;
		}

		WriteFile(hOut, Buffer, nIn, &nOut, NULL);
		if (nIn != nOut) {
			printf("������������ ������ ������: %x\n", GetLastError());
			return 4;
		}
		printf("���������� ���������� ����: %d", numFromCMD);
	}

	CloseHandle(hIn);
	CloseHandle(hOut);
	return 0;
}