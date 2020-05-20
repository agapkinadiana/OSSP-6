#include "pch.h"
#include <iostream>
#include <Windows.h>

using namespace std;

typedef void(*StartExcelPointer)();

int main()
{
	const wchar_t* DLL_NAME = L"ExcelDll.dll";

	HINSTANCE hComponent = LoadLibrary(DLL_NAME);
	if (hComponent == NULL)
	{
		cout << "Error " << GetLastError() << ": Cannot load the component ";
		wcout << DLL_NAME << endl;
		return GetLastError();
	}

	StartExcelPointer StartExcel = (StartExcelPointer)GetProcAddress(hComponent, "StartExcel");
	if (StartExcel == NULL)
	{
		cout << "Error: Cannot find the function StartExcel" << endl;
		return GetLastError();
	}

	StartExcel();
}
