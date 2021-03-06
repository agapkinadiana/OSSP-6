#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <stack>
#include <string.h>

using namespace std;
bool task1(char *word);
bool task2(char *word);
bool task3(char *word);
bool task4(char *word);
bool task5(char *word);
bool additionalTaskBrackets(char *word);
bool is_correct(const std::string& str);
void all(char *word);

bool isNumber(char s);

template< class T >
class Stack {

private:
	int MAX;
	int top;
	T* items;

public:
	Stack(int size) {
		MAX = size;
		top = -1;
		items = new T[MAX];
	}

	~Stack() { delete[] items; }

	void push(T c) {
		if (full()) {
			cout << "Stack Full!" << endl;
			exit(1);
		}

		items[++top] = c;
	}

	T pop() {
		if (empty()) {
			cout << "Stack Empty!" << endl;
			return NULL;
			exit(1);
		}


		return items[top--];
	}

	T free() {
		top = -1;
		return true;
	}

	int empty() { return top == -1; }

	int full() { return top + 1 == MAX; }
};

int what;
char word[200] = { " " };

// MAIN function
int main() {
	setlocale(0, "");
	for (;;)
	{
		cout << endl << "\nEnter expression:\n";
		char p[2] = { " " };
		gets_s(word);
		strcat_s(word, p);
		all(word);
	}

	system("pause");
	return 0;
}

// ALL function
void all(char *word)
{
	cout << endl << " Fixed point number: ";
	if (task1(word))
		cout << "\tTrue";
	else
		cout << "\tFalse";

	cout << endl << " Floating point number: ";
	if (task2(word))
		cout << "\tTrue";
	else
		cout << "\tFalse";

	cout << endl << "Complex number: ";
	if (task3(word))
		cout << "\tTrue";
	else
		cout << "\tFalse";

	cout << endl << "Simple algebraic expression";
	if (task4(word))
		cout << "\tTrue";
	else
		cout << "\tFalse";

	cout << endl << "Task5: The simplest algebraic expression(with brackets)";
	if (additionalTaskBrackets(word) && task5(word))
		cout << "\tTrue";
	else
		cout << "\tFalse";

	/*cout << endl << "(Additional): Собюлюдена ли правильность открытия/закрытия скобок? - ";
	if (additionalTaskBrackets(word))
		cout << "\tTrue";
	else
		cout << "\tFalse";*/
}

// TASK1 function
bool task1(char *word)
{
	Stack<int> s(5);
	s.free();
	s.push(0);
	int len = strlen(word);
	for (int i = 0; i < len; i++)
	{
		int sost = s.pop();
		if (sost == 0)
		{
			if (isNumber(word[i]) || word[i] == '-')
				s.push(1);
			else
			{
				s.push(-1);
			}
		}
		else if (sost == 1)
		{
			if (isNumber(word[i]))
				s.push(1);
			else if (word[i] == '.')
			{
				s.push(2);
			}
			else
			{
				s.push(-1);
			}
		}
		else if (sost == 2)
		{
			if (isNumber(word[i]))
				s.push(3);
			else
			{
				s.push(-1);
			}
		}
		else if (sost == 3)
		{
			if (isNumber(word[i]))
				s.push(3);
			else if (word[i] == ' ')
			{
				return true;
			}
			else
			{
				s.push(-1);
			}
		}
		else if (sost == -1)
		{
			return false;
		}
		else
		{
			s.push(-1);
		}
	}

	if (s.pop() == -1)
	{
		return false;
	}


	return true;
}

// TASK2 function
bool task2(char *word)
{
	Stack<int> s(8);
	s.free();
	s.push(0);
	int len = strlen(word);
	for (int i = 0; i < len; i++)
	{
		int sost = s.pop();
		if (sost == 0)
		{
			if (isNumber(word[i]) || word[i] == '-' || word[i] == '+')
				s.push(1);
			else
			{
				s.push(-1);
			}
		}
		else if (sost == 1)
		{
			if (isNumber(word[i]))
				s.push(1);
			else if (word[i] == '.')
			{
				s.push(2);
			}
			else
			{
				s.push(-1);
			}
		}
		else if (sost == 2)
		{
			if (isNumber(word[i]))
				s.push(3);
			else
			{
				s.push(-1);
			}
		}
		else if (sost == 3)
		{
			if (isNumber(word[i]))
				s.push(3);
			else if (word[i] == 'e')
			{
				s.push(4);
			}
			else
			{
				s.push(-1);
			}
		}
		else if (sost == 4)
		{
			if (word[i] == '+' || word[i] == '-')
				s.push(5);
			else
			{
				s.push(-1);
			}
		}
		else if (sost == 5)
		{
			if (isNumber(word[i]))
				s.push(6);
			else
			{
				s.push(-1);
			}
		}
		else if (sost == 6)
		{
			if (isNumber(word[i]))
				s.push(6);
			else if (word[i] == ' ')
			{
				return true;
			}
			else
			{
				s.push(-1);
			}
		}
		else if (sost == -1)
		{
			return false;
		}
		else
		{
			s.push(-1);
		}
	}

	if (s.pop() == -1)
	{
		return false;
	}


	return true;
}

// TASK3 function
bool task3(char *word)
{
	Stack<int> s(5);
	s.free();
	s.push(0);
	int len = strlen(word);
	for (int i = 0; i < len; i++)
	{
		int sost = s.pop();
		if (sost == 0)
		{
			if (isNumber(word[i]) || word[i] == '-' || word[i] == '+')
				s.push(1);
			else
			{
				s.push(-1);
			}
		}
		else if (sost == 1)
		{
			if (isNumber(word[i]))
				s.push(1);
			else if (word[i] == '-' || word[i] == '+')
			{
				s.push(2);
			}
			else
			{
				s.push(-1);
			}
		}
		else if (sost == 2)
		{
			if (isNumber(word[i]))
				s.push(2);
			else if (word[i] = 'j')
			{
				s.push(3);
			}
			else
			{
				s.push(-1);
			}
		}
		else if (sost == 3)
		{
			if (word[i] == ' ')
			{
				return true;
			}
			else
			{
				s.push(-1);
			}
		}
		else if (sost == -1)
		{
			return false;
		}
		else
		{
			s.push(-1);
		}
	}

	if (s.pop() == -1)
	{
		return false;
	}


	return true;
}

// TASK4 function
bool task4(char *word)
{
	Stack<int> s(5);
	s.free();
	s.push(0);
	int len = strlen(word);
	for (int i = 0; i < len; i++)
	{
		int sost = s.pop();
		if (sost == 0)
		{
			if (word[i] == 'b' || word[i] == 'a')
				s.push(1);
			else
			{
				s.push(-1);
			}
		}
		else if (sost == 1)
		{
			if (word[i] == '+' || word[i] == '-' || word[i] == '*' || word[i] == '/')
			{
				s.push(0);
			}
			else if (word[i] == ' ')
			{
				return true;
			}
			else
			{
				s.push(-1);
			}
		}
		else if (sost == -1)
		{
			return false;
		}
		else
		{
			s.push(-1);
		}
	}

	if (s.pop() == -1)
	{

		return false;
	}


	return true;
}

// task 5
bool task5(char *word)
{
	Stack<int> s(3);
	s.free();
	s.push(2);
	int len = strlen(word);
	for (int i = 0; i < len; i++)
	{
		int sost = s.pop();

		if (sost == 0)
		{
			if (word[i] == 'b' || word[i] == 'a')
				s.push(1);
			else
			{
				s.push(-1);
			}
		}
		else if (sost == 1)
		{
			if (word[i] == '+' || word[i] == '-' || word[i] == '*' || word[i] == '/' || word[i] == '(' || word[i] == ')')
			{
				if (word[i] == ')' || word[i] == '(')
					s.push(1);
				else
					s.push(0);
			}
			else if (word[i] == ' ')
			{
				return true;
			}
			else
			{
				s.push(-1);
			}
		}
		else if (sost == 2)
		{
			if (word[i] == '(')
				s.push(0);
			else
				s.push(-1);
		}
		else if (sost == -1)
		{
			return false;
		}
		else
		{
			s.push(-1);
		}
	}

	if (s.pop() == -1)
	{

		return false;
	}


	return true;
}



// TASK5 (additional) function
bool additionalTaskBrackets(char *word)
{
	return is_correct(word);
}


bool is_correct(const std::string& str)
{
	int charCounter = 0;
	std::stack<char> stack;
	int counterScobok = 0;

	for (int i = 0; i < (int)str.length(); ++i)
	{
		charCounter = isalpha(str[i]) ? charCounter + 1 : 0;

		if (str[i] == '(')
		{
			stack.push(str[i]);
			counterScobok++;
		}
		else if (str[i] == ')')
		{
			counterScobok++;
			if (stack.empty() || ((str[i] == ')') ^ (stack.top() == '(')))
			{
				return false;
			}
			stack.pop();
		}
		if ((int)str.length() - 1 == counterScobok || counterScobok == 0)
		{
			return true;
		}
	}

	return stack.empty();
}

// for match with numbers (task1 and task2 and task 3)
bool isNumber(char s)
{
	switch (s)
	{
	case '1': return true; break;
	case '2': return true; break;
	case '3': return true; break;
	case '4': return true; break;
	case '5': return true; break;
	case '6': return true; break;
	case '7': return true; break;
	case '8': return true; break;
	case '9': return true; break;
	case '0': return true; break;

	default: return false;
		break;
	}
}


