#include <iostream>
#include "Hash/md5.hpp"

int main()
{
	std::string word("message digest");
	std::string expected("f96b697d7cb7938d525a2f31aaf161d0");
	auto hash = CryptoLib::Hash::MD5CreateHash(std::begin(word), std::end(word));
}