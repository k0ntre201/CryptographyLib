#ifndef CRYPTOLIB_HASH_IHASH_HPP
#define CRYPTOLIB_HASH_IHASH_HPP

#include <string>

namespace CryptoLib::Hash
{
	class IHash
	{
	public:
		virtual std::string generateHash(std::string message) = 0;
	};
}

#endif //!CRYPTOLIB_HASH_IHASH_HPP
