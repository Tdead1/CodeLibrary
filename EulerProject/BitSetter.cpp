using Byte = unsigned char;
constexpr int ReservedSpace = 1000; // results in 8000 bits allocated
Byte* memory;

int GetBit(int bit)
{
	return (*memory >> bit) & 1;
}

void SetBit(int location)
{
	memory[int(location / 8.f)] |= 1 << (location % 8);
}

void UnsetBit(int location)
{
	memory[int(location / 8.f)] &= ~(1 << (location % 8));
}