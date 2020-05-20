#include "pch.h"
#include <iostream>
#include <windows.h>


extern "C++" __declspec(dllexport) BOOL WriteCentreText(HDC hdc, PRECT rect, LPCWSTR message)
{
	int iLength = wcslen(message);
	SIZE size;
	GetTextExtentPoint32(hdc, message, iLength, &size);

	return 
		TextOut
		(
			hdc, 
			(rect->right - rect->left - size.cx) / 2,
			(rect->bottom - rect->top - size.cy) / 2,
			message,
			iLength
		);
}
