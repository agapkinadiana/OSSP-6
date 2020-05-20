#include <iostream>
#include <Windows.h>
#include "Math.h"

using namespace std;

typedef int(*GetSumPointer)(int a, int b);
typedef float(*GetMulPointer)(int a, int b);
typedef float(*GetDivPointer)(int a, int b);

int main()
{
	HINSTANCE libraryId = LoadLibrary(TEXT(".\\Dll.dll"));

	if (!libraryId) {
		cout << "Error while loading library, code: " << endl << GetLastError() << endl;
		return EXIT_FAILURE;
	}

	int a = 12, b = 4;

	GetSumPointer GetSum = (GetSumPointer)GetProcAddress(libraryId, "GetSum");
	GetMulPointer GetMul = (GetMulPointer)GetProcAddress(libraryId, "GetMul");
	GetDivPointer GetDiv = (GetDivPointer)GetProcAddress(libraryId, "GetDiv");

	if (!GetSum || !GetMul || !GetDiv) {
		cout << "Error while exporting the function, code: " << GetLastError() << endl;
		FreeLibrary(libraryId);
		return EXIT_FAILURE;
	}

	cout << "Sum: " << GetSum(a, b) << endl;
	cout << "Mul: " << GetMul(a, b) << endl;
	cout << "Div: " << GetDiv(a, b) << endl;

	FreeLibrary(libraryId);
}
