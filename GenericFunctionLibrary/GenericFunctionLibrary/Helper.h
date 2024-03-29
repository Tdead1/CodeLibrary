#pragma once
#include <type_traits>
//! Contains some useful fast functions for math related operations.
struct Math
{
	//! Odd implementation for integral types.
	template <typename T>
	static bool IsOdd(T number)
	{
		static_assert(std::is_integral<T>::value, "Element needs to be an integral number.");
		return (number & 1);
	};
	//! Even implementation for integral types.
	template <typename T>
	static bool IsEven(T number)
	{
		return !Math::IsOdd(number);
	};
};

//! 4 types of loggables, a problem, a success, a warning or a message.
enum class LogType
{
	PROBLEM = 1,
	SUCCESS = 2,
	WARNING = 3,
	MESSAGE = 4
};


#include <string>
#include <iostream>
//! Logger prints anything text based based on it's LogType and on the LogDebugLevel.
struct Log
{
public:
	// The log level defines what is printed and what isn't: Making the LogDebugLevel higher will print more (0-4), 0 will print everything.
	#define LogDebugLevel 0

	template <typename T>
	static void Print(T log, const LogType type = LogType::MESSAGE)
	{
		// Checks if the input element is printable for std::cout.
		static_assert(
			   std::is_arithmetic<T>::value 
			|| std::is_base_of<std::string, T>::value 
			|| std::is_same<const char*, T>::value, "Printed Element needs to be either a number, a string, or a character.");
		if (LogDebugLevel == 0 || (int)type <= LogDebugLevel)
		{
			switch (type)
			{
			case LogType::PROBLEM:
				std::cout << "<X>  ";
				break;
			case LogType::SUCCESS:
				std::cout << "<+>  ";
				break;
			case LogType::WARNING:
				std::cout << "<W>  ";
				break;
			case LogType::MESSAGE:
				std::cout << "<i>  ";
				break;
			default:
				std::cout << "<i>  ";
				break;
			}
			std::cout << log << "\n";
		}
	};
	#undef LogDebugLevel
};

#include <chrono>
//! A static header only timer class. Call Start() to start counting, call Stop() to stop the timer and then call Print() to print it.
struct Timer
{
	//! Starts the clock.
	void Start()
	{
		startTime = std::chrono::high_resolution_clock::now();
	};
	//! Stops the clock.
	float Stop()
	{
		endTime = std::chrono::high_resolution_clock::now();
		deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
		// In seconds
		return static_cast<float>(deltaTime * 0.000000001);
	};
	//! Prints end time minus start time.
	void Print()
	{
		deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
		if (deltaTime > 1000000000)
			Log::Print("This took " + std::to_string(static_cast<double>(deltaTime) * 0.000000001) + " seconds.");
		else
			Log::Print("This took " + std::to_string(deltaTime) + " nano seconds.");
	};

private:
	std::chrono::steady_clock::time_point startTime;
	std::chrono::steady_clock::time_point endTime;
	long long deltaTime = 0;
};