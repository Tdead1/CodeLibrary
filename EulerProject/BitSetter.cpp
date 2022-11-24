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