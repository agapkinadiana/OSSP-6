#include <iostream>

using namespace std;

void Task1();
void Task2();
void Task3();
void Task4();
void Task5();

int main()
{
	Task1();
	Task2();
	Task3();
	Task4();
	Task5();
}


void Task1()
{
	int A = 6, B = 126;

	__asm
	{
		mov eax, A
		mov ebx, B

		start:
		cmp eax, ebx
		je aEqualB
		ja aGreaterB
			sub ebx, eax
			jmp start

		aGreaterB:
			sub eax, ebx
			jmp start
		aEqualB:
			mov A, eax
	}
	cout << "A: " << A << endl;
}

void Task2()
{
	int A = 9, B = 18, C = 54;

	__asm
	{
		mov eax, A
		mov ebx, B

		start2: 
			cmp eax, ebx
			je end_start2
			jg c3
			jl c4

		c3:
			sub eax, ebx
			jmp start2

		c4:
			sub ebx, eax
			jmp start2

		end_start2:
			mov A, eax
			mov eax, A
			mov ebx, C

		start3:
			cmp eax, ebx
			je end_start3
			jg c5
			jl c6

		c5:
			sub eax, ebx
			jmp start3

		c6:
			sub ebx, eax
			jmp start3

		end_start3:
			mov A, eax
	}
	cout << "A: " << A << endl;
}

void Task3()
{
	// nunmber = 11111011
	char number = 11;

	__asm
	{
		mov eax, 0
		mov ah, number
		AND ah, 00000100B
		mov number, ah
	}
	if (number == 0)
	{
		cout << "number: " << (int)number << " has no bit on the third place from the right" << endl;
	}
	else
	{
		cout << "number: " << (int)number << " has a bit on the third place from the right" << endl;
	}
}

void Task4()
{
	char A[] = "Hello world";
	char* B;

	__asm
	{
		lea eax, A
		mov bl, '$'
		mov [eax], bl
		mov B, eax
	}
	cout << "B: " << B << endl;
}

void Task5()
{
	float A = 4, B = 3, result = -1;
	__asm
	{
		FINIT
		FLD A
		FLD B
		FUCOMI st, st(1)
		ja c11
		jb c22
		je c33
		c22:
			FSTP B
			FMUL A
			loop END

		c11:
			FMUL B
			loop END

		c33:
			FMUL B
			loop END

		END:
			FST result
	}
	cout << "A: " << A << ", B: " << B << ", result: " << result << endl;
}
