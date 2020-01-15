#include <iostream>
#include "Hash/whirpool.hpp"

int main()
{
	std::cout << std::hex << CryptoLib::Hash::InitializeSBox::generate_multiply_value_to_extended_sBox<0xb8,9>::value << "\n";
}