int main()
{
	std::string finalNumber = "1";
	for (int i = 0; i < 1000; i++)
	{
		int newNumber = 0;
		int carryover = 0;
		for (int characterCounter = finalNumber.size() - 1; characterCounter >= 0; --characterCounter)
		{
			char& c = finalNumber[characterCounter];
			int newNumber = (c - '0') * 2 + carryover;
			c = (char)(newNumber % 10) + '0';
			carryover = newNumber >= 10 ? 1 : 0;
		}

		if (carryover)
			finalNumber.insert(finalNumber.begin(), '1');
	}

	long total = 0;
	for (char c : finalNumber)
	{
		total += c - '0';
	}

	return 0;
}
