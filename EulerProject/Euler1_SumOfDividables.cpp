	long long int all = 0;
	for (int i = 0; i < 1000; i++)
		all += (i % 3 == 0 || i % 5 == 0) ? i : 0;