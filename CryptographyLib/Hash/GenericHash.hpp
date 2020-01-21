#ifndef CRYPTOLIB_HASH_GENERICHASH_HPP
#define CRYPTOLIB_HASH_GENERICHASH_HPP

#include <string>

namespace CryptoLib::Hash
{
	template <typename HashAlgorithm>
	class GenericHash
	{
	public:
		std::string generateHash(std::string message)
		{
			return static_cast<HashAlgorithm&>(*this).generateHash(message);
		}
	};
}

#endif // !CRYPTOLIB_HASH_GENERICHASH_HPP

