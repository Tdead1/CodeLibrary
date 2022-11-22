#include <iostream>
#include <random>
#include <algorithm>
#include <string>
#include <ctime>
#include <vector>

//int main()
//{
//
//}

struct pos
{
	int x = 0, y = 0;
};

int main()
{
	bool nan = false;
	pos ploc, tloc;

	std::string s;
	std::cout << "\nPick a whole number for playspace size (recommended: 20).\n";
	std::cin >> s;

	std::for_each(s.begin(), s.end(), [&nan](char c){ if(!isdigit(c)){nan = true;}; });

	while (nan)
	{
		nan = false;
		std::cout << "\nIt's not difficult, just pick a whole number.\n" << std::endl;
		s.clear();
		std::cin >> s;
		std::for_each(s.begin(), s.end(), [&nan](char c) { if (!isdigit(c)) { nan = true; }; });
	}

	int playspace = std::stoi(s);
	time_t current_time;
	srand(time(&current_time));

	ploc.x = rand() % playspace;
	tloc.y = rand() % playspace;
	ploc.x = rand() % playspace;
	tloc.y = rand() % playspace;

	while (!(ploc.x == tloc.x && ploc.y == tloc.y))
	{
		s.clear();
		std::cout << "Forward (f), Right (r), Left (l) or Back (b)?\n";
		std::cin >> s;
		while (s != "f" && s != "r" &&s != "l" &&s != "b")
		{
			s.clear();
			std::cout << "Just choose between Up (w), Down (s), Left (a) or Right (d).\n";
			std::cin >> s;
		}
	
		pos plocprev = ploc;
		switch (s[0]) {
		case 'f':
			ploc.y++;
			break;
		case 'r':
			ploc.x++;
			break;
		case 'l':
			ploc.x--;
			break;
		case 'b':
			ploc.y--;
			break;
		}
		if (ploc.x >= playspace || ploc.x < 0 || ploc.y >= playspace || ploc.y < 0)
		{
			std::cout << "\ninvalid move. Try again.\n";
			ploc = plocprev;
		}
		else
		{
			float dist = abs((ploc.x - tloc.x) * (ploc.x - tloc.x) + (ploc.y - tloc.y) * (ploc.y - tloc.y));
			float distprev = abs((plocprev.x - tloc.x) * (plocprev.x - tloc.x) + (plocprev.y - tloc.y) * (plocprev.y - tloc.y));
			if (dist > distprev)
				std::cout << "\nYou moved away from the target.\n";
			else
				std::cout << "\nYou moved closer to the target.\n";
		}
	}
	std::cout << " You made it! Congrats!\n";
	std::cin.get();

	return 0;
}


//int main()
//{
//	bool nan = false;
//	// int 64 to int.
//	time_t current_time;
//	srand(time(&current_time));
//
//	int number = rand();
//	int guess = 0;
//	
//	guess = -1;
//	while (guess != number)
//	{
//		std::string s;
//		std::cout << "\nPick a whole number.\n";
//		std::cin >> s;
//	
//		std::for_each(s.begin(), s.end(), [&nan](char c){ if(!isdigit(c)){nan = true;}; });
//
//		while (nan)
//		{
//			nan = false;
//			std::cout << "\nIt's not difficult, just pick a whole number." << std::endl;
//			s.clear();
//			std::cin >> s;
//			std::for_each(s.begin(), s.end(), [&nan](char c) { if (!isdigit(c)) { nan = true; }; });
//		}
//
//		guess = std::stoi(s);
//		(guess < number) ? std::cout << "\nIt's higher." : std::cout << "\nIt's lower.";	
//	}
//
//	std::cout << "\nYou're done!";
//
//	system("PAUSE");
//}