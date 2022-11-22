#include <iostream>
#include <chrono>

int main()
{
	std::srand(time(0));
	std::cout << "Welcome to casino Prompt! \nPress any key to start playing some blackjack!\n";
	std::cin.get();
	system("cls");


	bool won = false;
	bool lost = false;

	while (!lost && !won)
	{
		bool finished = false;
		
		short card = rand() % 13;
		short total = card;
		std::cout << "Your first card is a " << card << ".\n";
		short dealertotal = card;
		std::cout << "The dealer's first card is a " << card << ".\n";

		while (!finished)
		{
			char answer = '.';
			std::cout << "Do you wish to draw another? (y/n)\n";
			std::cin >> answer;

			while (!(answer == 'y' || answer == 'n' || answer == 'N' || answer == 'Y'))
			{
				std::cout << "\nPlease input either y or n.\n";
				std::cin >> answer;
			}

			if (answer == 'y' || answer == 'Y')
			{
				card = rand() % 13;
				total += card;
				std::cout << "You drew " << rand() % 13 << "\nYour total is now " << total << ".\n";
				if (total > 21)
				{
					std::cout << "Busted!";
					finished = true;
					lost = true;
				}
				if (total == 21)
				{
					std::cout << "BLACKJACK!";
					finished = true;
					won = true;
				}
			}
			if (answer == 'n' || answer == 'N')
			{
				finished = true;
			}

		}
	}
	return 0;
}