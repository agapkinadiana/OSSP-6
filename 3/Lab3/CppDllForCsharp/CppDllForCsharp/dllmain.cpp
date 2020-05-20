#include "pch.h"
#include <ctime>
#include <string>

extern "C" __declspec(dllexport) int __stdcall SumNumbers(int x, int y)
{
	return x + y;
}

extern "C" __declspec(dllexport) char* __stdcall GetCurrentDate()
{
	std::time_t currentDate = std::time(0);
	size_t length = 50;
	char* currentDateCharP = new char[length];
	ctime_s(currentDateCharP, length, &currentDate);
	return currentDateCharP;
}