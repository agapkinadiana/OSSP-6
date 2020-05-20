#include <iostream>
#include "DllHeader.h"
#include <conio.h>
using namespace std;

void ExampleTask();



int main()
{
	ExampleTask();

	cout << WriteLineToFile("hello world") << endl;

	string fileExist = CheckFileInTheFolder
	(
		"file.txt",
		"C:\\Lab3\\Lab1CallCppDll"
	) ? "file exist" : "file doesn't exist";
	cout << fileExist << endl;

	//cout << ExecuteProcess("start chrome https://www.google.com/") << endl << endl << endl;
}


void ExampleTask()
{
	int x = PutInt(5);
	LetterList();
	cout << x << endl << endl;
}