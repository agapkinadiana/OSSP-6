#include "pch.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
using namespace std;


extern "C++" __declspec(dllexport) void LetterList()
{
	cout << "LetterList()" << endl;
}

extern "C++" __declspec(dllexport) int PutInt(int param)
{
	return param;
}



extern "C++" __declspec(dllexport) bool WriteLineToFile(string line)
{
	ofstream file("file.txt");
	if (!file.is_open())
		return false;

	file << line << endl;
	file.close();
	
	return true;
}

extern "C++" __declspec(dllexport) bool CheckFileInTheFolder(string fileName, string path)
{
	for (const auto& entry : filesystem::directory_iterator(path))
		if (entry.path().filename() == fileName)
			return true;
	return false;
}

extern "C++" __declspec(dllexport) int ExecuteProcess(string processPath)
{
	return system(processPath.c_str());
}