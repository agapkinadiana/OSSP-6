#include <iostream>
#include <string>
#include <Windows.h>
#include <time.h>
#include <stdio.h>

using namespace std;


const string currentDateTime();
void CALLBACK Proc2(DWORD dwErrorCode, DWORD dwNumberOfBytesTransferred, LPOVERLAPPED lpOverlapped);
void CALLBACK completion_routine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransferred, LPOVERLAPPED lpOverlapped);

int main()
{
	setlocale(LC_ALL, "rus");
	void(__stdcall *funEnd)(DWORD, DWORD, LPOVERLAPPED) = &completion_routine;
	HANDLE hFile;
	OVERLAPPED ovl;
	ovl.Offset = 0; // ������� ����� �������� ����� 0 
	ovl.OffsetHigh = 0; // ������� ����� �������� ����� 0 

	hFile = CreateFile(
		L"D:\\university\\3 course\\2 sem\\OS\\labs\\Lab10\\Lab10\\file.txt",
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		OPEN_ALWAYS,
		FILE_FLAG_OVERLAPPED,
		NULL
	);
	
	if (hFile == INVALID_HANDLE_VALUE)
	{
		cerr << "���� �� ������" << endl << "������: " << GetLastError() << endl;
		return 0;
	}

	for (int i = 0; i < 10; ++i)
	{
		if (i == 1)
		{
			funEnd = &Proc2;
		}
		else
		{
			funEnd = &completion_routine;
		}

		DWORD dwRet;
		if (!WriteFileEx(hFile, &i, sizeof(i), &ovl, funEnd))
		{
			dwRet = GetLastError();
			if (dwRet == ERROR_IO_PENDING)
			{
				cout << "Write file pending." << endl;
			}
			else
			{
				cout << "������ ����������� �������" << endl << "O����a: " << dwRet << endl;
				return 0;
			}
		}
		SleepEx(INFINITE, TRUE);
		// ����������� �������� � ����� 
		ovl.Offset += sizeof(i);
		cout << "������ ���������" << endl;
	}
	CloseHandle(hFile);
}



const string currentDateTime() {
	tm bt;
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	localtime_s(&bt, &now);
	tstruct = bt;
	strftime(buf, sizeof(buf), "%X", &tstruct);
	return buf;
}

void CALLBACK Proc2(DWORD dwErrorCode, DWORD dwNumberOfBytesTransferred, LPOVERLAPPED lpOverlapped)
{
	cout << "<< --- async proc 2" << endl;
	cout << currentDateTime() << " ��������� ��������� � �����������: " << endl;
	cout << "\t��� ������: " << dwErrorCode << endl;
	cout << "\t�����  ���������� ������: " << dwNumberOfBytesTransferred << endl;
	cout << "\tOffsets: " << (*lpOverlapped).OffsetHigh << ' ' << (*lpOverlapped).Offset << endl;
}

void CALLBACK completion_routine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransferred, LPOVERLAPPED lpOverlapped)
{
	cout << "<< --- async proc 1" << endl;
	cout << currentDateTime() << " ��������� ��������� � �����������: " << endl;
	cout << "\t��� ������: " << dwErrorCode << endl;
	cout << "\t�����  ���������� ������: " << dwNumberOfBytesTransferred << endl;
	cout << "\tOffsets: " << (*lpOverlapped).OffsetHigh << ' ' << (*lpOverlapped).Offset << endl;
}
