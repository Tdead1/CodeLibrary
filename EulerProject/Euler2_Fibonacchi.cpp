// This is also on my github :)
//#include "D:\Repros\CodeLibrary\GenericFunctionLibrary\GenericFunctionLibrary\Helper.h"
constexpr int stack_limit = 24;

struct Fib
{
	template<int N>
	constexpr static long long Get() {
		if constexpr (N == 1)
			return 1;
		if constexpr (N == 2)
			return 1;
		if constexpr (N > stack_limit)
		{
			long long nMinusOne = Get<stack_limit - 1>();
			long long nMinusTwo = Get<stack_limit - 2>();
			int total = 0;
			// No longer constexpr, otherwise we run out of stack.
			for (int i = stack_limit; i <= N; i++)
			{
				total = nMinusOne + nMinusTwo;
				nMinusTwo = nMinusOne;
				nMinusOne = total;
			}
			return total;
		}
		if constexpr (N > 2)
			return Fib::Get<N - 1>() + Fib::Get<N - 2>();
		if constexpr (N < 1)
			return 0;
	}

	static long long Get(const int aValueExeedingLimit)
	{
		if (aValueExeedingLimit < stack_limit)
			return FibUntilStackLimit[aValueExeedingLimit];

		// No longer constexpr, otherwise we run out of stack.
		long long nMinusOne = FibUntilStackLimit[stack_limit - 1];
		long long nMinusTwo = FibUntilStackLimit[stack_limit - 2];
		long long total = 0;
		for (int i = stack_limit; i <= aValueExeedingLimit; i++)
		{
			total = nMinusOne + nMinusTwo;
			nMinusTwo = nMinusOne;
			nMinusOne = total;
		}
		return total;
	}

	// How do I turn this into something... not horrible? useful?
	constexpr static long long inline FibUntilStackLimit[stack_limit] = {
	Fib::Get<0>(),
	Fib::Get<1>(),
	Fib::Get<2>(),
	Fib::Get<3>(),
	Fib::Get<4>(),
	Fib::Get<5>(),
	Fib::Get<6>(),
	Fib::Get<7>(),
	Fib::Get<8>(),
	Fib::Get<9>(),
	Fib::Get<10>(),
	Fib::Get<11>(),
	Fib::Get<12>(),
	Fib::Get<13>(),
	Fib::Get<14>(),
	Fib::Get<15>(),
	Fib::Get<16>(),
	Fib::Get<17>(),
	Fib::Get<18>(),
	Fib::Get<19>(),
	Fib::Get<20>(),
	Fib::Get<21>(),
	Fib::Get<22>(),
	Fib::Get<23>()
	// This array must be filled up until stack_limit - 1, but no way to do that using defines or constexprs :(
	};

};

int main()
{
	//Timer t;
	//t.Start();
	long long total = 0;
	long long c = 0;
	long long i = 0;

	for (constexpr long long t = 9000000;; i++)
	{
		c = Fib::Get(static_cast<int>(i));
		if (c > t)
			break;
		if (!(c & 1))
			total += c;
	}
	//t.Stop();
	//t.Print();
	//std::cout << "Fib #" << i << " is the first to exceed 4 milion, bringing the total of all evaluated even numbers to: " << total << "\n";
	return 0;
}