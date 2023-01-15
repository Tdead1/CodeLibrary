// Amount of bits you want to allocate.
template<int TemplateSize>
class BitArray
{
public:
	using Byte = unsigned char;
	BitArray()
	{
		for (Byte& byte : myMemory)
			byte = 0;
	}
	int GetBit(int location) const
	{
		return (myMemory[static_cast<int>(location / 8.f)] >> static_cast<Byte>(location % 8)) & 1;
	}
	void SetBit(int location)
	{
		myMemory[static_cast<int>(location / 8.f)] |= 1 << (location % 8);
	}
	void UnsetBit(int location)
	{
		myMemory[static_cast<int>(location / 8.f)] &= ~(1 << (location % 8));
	}
	static int GetReservedBytes()
	{
		return ReservedSpace;
	}
	constexpr static int GetReservedBits()
	{
		return TemplateSize;
	}

private:
	static constexpr int ReservedSpace = static_cast<int>((TemplateSize + 7) / 8);
	Byte myMemory[ReservedSpace];
};

#include <iostream>
int main()
{
	constexpr int arraySize = 250;
	BitArray<arraySize> bitArray1;
	std::cout << " bit 0 is currently: " << bitArray1.GetBit(40) << std::endl;
	std::cout << "byte reserved space: " << bitArray1.GetReservedBytes() << std::endl;
	std::cout << " bit reserved space: " << bitArray1.GetReservedBits() << std::endl;
	std::cout << " full class size is: " << sizeof(BitArray<7>) << std::endl;

	for (int i = 0; i < bitArray1.GetReservedBits(); i++)
	{
		std::cout << bitArray1.GetBit(i) << "    ";
		if ((i + 1) % 8 == 0)
			std::cout << "\n";
	}
	return 0;
}
