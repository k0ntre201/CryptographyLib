#ifndef CRYPTOLIB_UTILS_HPP
#define CRYPTOLIB_UTILS_HPP

#include <iomanip>
#include <string>
#include <sstream>

template <typename Array, typename ByteSwapp>
std::string changeByteArrayToHexStringReprezentation(Array hash, ByteSwapp swap)
{
	std::string hashCode;
	for (auto h : hash)
	{
		std::ostringstream ss;
		ss << std::setfill('0') << std::setw(sizeof(h)*2) << std::hex << swap(h);
		std::string hexValue = ss.str();
		hashCode.insert(hashCode.end(), hexValue.begin(), hexValue.end());
	}
	return hashCode;
}

#endif // !CRYPTOLIB_UTILS_HPP

