#include <iostream>
#include <windows.h>

using namespace std;


HANDLE hFile;
OVERLAPPED ovl;

HANDLE hCompletionPort;


DWORD WINAPI ReadThread(LPVOID);
DWORD WINAPI WriteThread(LPVOID);
DWORD WINAPI thread3(LPVOID);


int main()
{
	ULONG ulKey;
	HANDLE hThread;
	HANDLE hThread2;
	HANDLE hThread3;
	DWORD dwThreadID[3];

	ovl.Offset = 0;
	ovl.OffsetHigh = 0;
	ovl.hEvent = 0;

	cout << "Input a number for file key (non-zero): ";
	cin >> ulKey;
	if (ulKey == 0)
	{
		cerr << "The file key can't be equal to zero" << endl;
		return 1;
	}
	
	hFile = CreateFile(
		TEXT("C:\\file.txt"),
		GENERIC_ALL,
		FILE_SHARE_WRITE,
		NULL,
		OPEN_ALWAYS,
		FILE_FLAG_OVERLAPPED,
		NULL
	);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		cerr << "Create file failed: " << GetLastError() << endl;
		return 0;
	}

	Sleep(100);

	// ??????? ???? ?????????? ? ?????????? ? ???? ???? 
	hCompletionPort = CreateIoCompletionPort(hFile, NULL, ulKey, 0);
	// ????????? ?? ???????? ???????? 
	if (hCompletionPort == NULL)
	{
		cerr << "Create completion port failed: " << GetLastError() << endl;
		return 0;
	}
	
	// ????????? ??????
	hThread = CreateThread(NULL, 0, ReadThread, NULL, 0, &dwThreadID[0]);
	hThread2 = CreateThread(NULL, 0, WriteThread, NULL, 0, &dwThreadID[1]);
	Sleep(100);
	hThread3 = CreateThread(NULL, 0, thread3, NULL, 0, &dwThreadID[2]);

	// ???????? ????? ? ???????? ?? ?????????? ?????? 
	PostQueuedCompletionStatus(hCompletionPort, 0, 0, NULL);

	WaitForSingleObject(hThread, INFINITE);
	
	CloseHandle(hFile);
	CloseHandle(hCompletionPort);
	CloseHandle(hThread);
	CloseHandle(hThread2);
	CloseHandle(hThread3);
}

DWORD WINAPI ReadThread(LPVOID)
{
	int i = 0; // ?????????? ?????????? ???????  
	DWORD dwNumberOfBytes; // ?????????? ?????????? ?????? 
	ULONG ulCompletionKey; // ???? ?????  
	LPOVERLAPPED lpOverlapped; // ????????? ?? ????????? ???? OVERLAPPED 
	cout << "The thread is started." << endl;
	// ?????????? ????? ? ????? 
	while (GetQueuedCompletionStatus(hCompletionPort, &dwNumberOfBytes, &ulCompletionKey, &lpOverlapped, INFINITE))  //???????? dwMilliseconds ?????? ????????? ?????????? ???????????,  
		//??????? ?????????? ????? ???????? ???????? GetQueuedCompletionStatus ?????????? ??????????? ???????? ????? - ??????.

	{
		// ????????? ????? ?? ?????????? ?????? 
		if (ulCompletionKey == 0)
		{
			cout << endl << "The thread is finished." << endl;
			break;
		}
		else
		{
			cout << "Package number: " << ++i << endl;
			cout << "Number of bytes: " << dwNumberOfBytes << endl;
			cout << "Completion key: " << ulCompletionKey << endl;
		}
	}
	return 0;
}

DWORD WINAPI WriteThread(LPVOID)
{
	DWORD dwBytesWrite;
	DWORD dwRet;
	char a[250] = "hello world";
	if (!WriteFile(hFile, &a, sizeof(a), &dwBytesWrite, &ovl))
	{
		dwRet = GetLastError();
		if (dwRet == ERROR_IO_PENDING)
		{
			cout << "Write file pending." << endl;
		}
		else
		{
			cout << "Write file failed." << endl;
			cout << "The last error code: " << dwRet << endl;
			return 0;
		}
	}
	WaitForSingleObject(hFile, INFINITE);
	return 0;
}

DWORD WINAPI thread3(LPVOID)
{
	char Buffer[250] = "";
	DWORD dwNumberOfBytes;
	ReadFile(hFile, Buffer, sizeof(Buffer), &dwNumberOfBytes, &ovl);
	cout << "Read: " << Buffer << '\n';
	return 0;
}
