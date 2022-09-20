constexpr int stack_limit = 25;

template<int N>
struct Fib
{
	static constexpr int Get() {
		if constexpr (N == 1)
			return 0;
		if constexpr (N == 2)
			return 1;
		if constexpr (N > stack_limit)
		{
			// No longer constexpr, otherwise we run out of stack.
			int nMinusOne = Fib<stack_limit - 1>::Get();
			int nMinusTwo = Fib<stack_limit - 2>::Get();
			int total = 0;
			for (int i = stack_limit; i <= N; i++)
			{
				total = nMinusOne + nMinusTwo;
				nMinusTwo = nMinusOne;
				nMinusOne = total;
			}
			return total;
		}
		if constexpr (N > 2)
			return Fib<N - 1>::Get() + Fib<N - 2>::Get();
		if constexpr (N < 1)
			return 0;
	}
};

int main()
{
	constexpr int i = Fib<18>::Get();
	return 0;
}