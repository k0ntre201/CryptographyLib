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
	std::array<uint64_t, 8> a = { 0x1823c6e887b8014f,0x36a6d2f5796f9152,0x60bc9b8ea30c7b35,0x1de0d7c22e4bfe57 ,0x157737e59ff04ada ,0x58c9290ab1a06b85 ,0xbd5d10f4cb3e0567 ,0xe427418ba77d95d8 };
	CryptoLib::Hash::Whirpool w;
	std::cout << std::hex << w.whirpoolOperation(a, 0) << "\n";
}