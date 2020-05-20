#include <iostream>
#include <tchar.h>
#include <Windows.h>
#include <cstdlib>
#include <fstream>
#include <string>

using namespace std;


HANDLE hThread1;
HANDLE hThread2;
HANDLE hThread3;

DWORD IDThread1;
DWORD IDThread2;
DWORD IDThread3;
DWORD dwRet;

CRITICAL_SECTION cs;


void CALLBACK a_proc10(DWORD p);
void CALLBACK a_proc100(DWORD p);
void CALLBACK a_writeFile(DWORD p);
void CALLBACK a_readFile(DWORD p);
DWORD WINAPI add1(LPVOID ptr);
DWORD WINAPI add2(LPVOID ptr);
DWORD WINAPI file(LPVOID ptr);


int _tmain(int argc, _TCHAR* argv[])
{
	DWORD count = 10;

	InitializeCriticalSection(&cs);

	hThread1 = CreateThread(NULL, 0, add1, &count, 0, &IDThread1);
	Sleep(100);
	dwRet = QueueUserAPC(a_proc10, hThread1, (DWORD)&count);

	hThread2 = CreateThread(NULL, 0, add2, &count, 0, &IDThread2);
	Sleep(100);
	dwRet = QueueUserAPC(a_proc100, hThread2, (DWORD)&count);

	hThread3 = CreateThread(NULL, 0, file, NULL, 0, &IDThread3);
	Sleep(100);
	dwRet = QueueUserAPC(a_writeFile, hThread3, NULL);
	dwRet = QueueUserAPC(a_readFile, hThread3, NULL);


	WaitForSingleObject(hThread1, INFINITE);
	CloseHandle(hThread1);

	WaitForSingleObject(hThread2, INFINITE);
	CloseHandle(hThread2);

	WaitForSingleObject(hThread3, INFINITE);
	CloseHandle(hThread3);	
}

void CALLBACK a_proc10(DWORD p)
{
	DWORD* ptr = (DWORD*)p;
	*ptr += 10;
	cout << "Async add 10 ended\n" << endl;
}

void CALLBACK a_proc100(DWORD p)
{
	DWORD* ptr = (DWORD*)p;
	*ptr += 100;
	cout << "Async add 100 ended\n" << endl;
}

void CALLBACK a_writeFile(DWORD p)
{
	EnterCriticalSection(&cs);
	ofstream out;

	out.open("C:\\file.txt");
	if (out.is_open())
	{
		out << "hello world" << endl;
	}
	LeaveCriticalSection(&cs);
	cout << "Async writeFile ended" << endl;
}

void CALLBACK a_readFile(DWORD p)
{
	string line;
	EnterCriticalSection(&cs);
	ifstream in("C:\\file.txt");
	cout << "From file: ";
	if (in.is_open())
	{
		while (getline(in, line))
		{
			cout << line << endl;
		}
	}
	in.close();
	LeaveCriticalSection(&cs);
	cout << "Async readFile ended" << endl;
}

DWORD WINAPI add1(LPVOID ptr)
{
	DWORD* zz = (DWORD*)ptr;
	SleepEx(INFINITE, TRUE);
	cout << "Thread says: Final count from async proc is " << *zz << endl;
	return 0;
}

DWORD WINAPI add2(LPVOID ptr)
{
	DWORD* zz = (DWORD*)ptr;
	SleepEx(INFINITE, TRUE);
	cout << "Thread says: Final count from async proc is " << *zz << endl;
	return 0;
}

DWORD WINAPI file(LPVOID ptr)
{
	cout << "Thread file says: START" << endl;
	SleepEx(INFINITE, TRUE);
	cout << "Thread file says: END!" << endl;
	return 0;
}
