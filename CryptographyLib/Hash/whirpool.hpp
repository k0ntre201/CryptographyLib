#ifndef CRYPTOGTAPHY_LIB_HASH_WHIRPOOL
#define CRYPTOGTAPHY_LIB_HASH_WHIRPOOL

#include "GenericHash.hpp"

#include <array>
#include <iostream>

namespace CryptoLib::Hash
{
	class Whirpool : public GenericHash<Whirpool>
	{
	public:
		std::string generateHash(std::string message);
	private:
		uint64_t whirpoolOperation(const std::array<uint64_t, 8>& block, int shift);

		std::array<uint64_t, 8> processOneBlock(const std::array<uint64_t, 8>& block, const std::array<uint64_t, 8>& hash);
	};
}

#endif // !CRYPTOGTAPHY_LIB_HASH_WHIRPOOL

