#include <iostream>

#include "Helper.h"


int main()
{
	Timer t;
	t.Start();
	
	std::cout << std::boolalpha;
	Log::Print("const char*");
	Log::Print(std::string("string"));
	Log::Print(12);

	Log::Print(Math::IsEven(2));
	Log::Print(Math::IsOdd(3));
	Log::Print(Math::IsEven(-3));
	Log::Print(Math::IsOdd(-4));

	t.Stop();
	t.Print();

	system("PAUSE");
	return 0;
}