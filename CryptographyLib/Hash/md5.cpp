#include "md5.hpp"

std::string CryptoLib::Hash::MD5::generateHash(std::string message)
{
	return MD5CreateHash(std::begin(message), std::end(message));
}
