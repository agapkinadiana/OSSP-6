#include <iostream>
#include <Windows.h>

using namespace std;

typedef wchar_t*(*SpecifyDocPathPointer)();
typedef void(*RunMSWordPointer)();

int main()
//D:\\LAB2_ActiveX.doc
{
	HINSTANCE libraryId = LoadLibrary(TEXT(".\\Dll.dll"));

	if (!libraryId) {
		cout << "Error while loading library, code: " << endl << GetLastError() << endl;
		return EXIT_FAILURE;
	}

	SpecifyDocPathPointer SpecifyDocPath = (SpecifyDocPathPointer)GetProcAddress(libraryId, "SpecifyDocPath");
	RunMSWordPointer RunMSWord = (RunMSWordPointer)GetProcAddress(libraryId, "RunMSWord");

	if (!SpecifyDocPath || !RunMSWord) {
		cout << "Error while exporting the function, code: " << GetLastError() << endl;
		FreeLibrary(libraryId);
		return EXIT_FAILURE;
	}

	RunMSWord();

	FreeLibrary(libraryId);
}
