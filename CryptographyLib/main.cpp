#include <iostream>
#include "Hash/whirpool.hpp"

int main()
{
	/*auto c0 = CryptoLib::Hash::InitializeSBox::generate_one_cyclic_array_for_extended_sBox<1>::C0[0];
	std::cout << std::hex << c0 << "\n";
	uint64_t mask = 0xff00000000000000;
	for (int i{ 1 }; i < 8; ++i)
	{
		auto v1 = ((c0 >> (8*i))& (~mask));
		auto v2 = ((c0 << ((8-i)*8)) & (mask));
		auto c1 = v1 | v2;
		std::cout << std::hex << c1 << "\n";
		mask |= (mask >> 8);
	}*/
	CryptoLib::Hash::Whirpool w;
	std::string a = w.generateHash("");

	std::cout << a << "\n";
}