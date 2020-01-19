#include "whirpool.hpp"

uint64_t CryptoLib::Hash::Whirpool::whirpoolOperation(std::array<uint64_t,8> block, int shift)
{
	uint64_t result{ 0 };

	for (int j{ 8 }, i{ 0 }; j >= 1; --j,++i)
	{
		uint32_t index = (shift + j) & 7;
		auto val = (block[index] >> ((j - 1) * 8))&0xff;
		result ^= fullInitialSBox[i][val];
	}

	return result;
}
