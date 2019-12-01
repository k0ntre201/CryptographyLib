#include "pch.h"
#include "../CryptographyLib/Hash/md5.hpp"

TEST(MD5_Hash_test, Empty_string) {
	std::string word("");
	std::string expected("d41d8cd98f00b204e9800998ecf8427e");
	auto hash = CryptoLib::Hash::MD5CreateHash(std::begin(word), std::end(word));

	EXPECT_EQ(hash, expected);
}

TEST(MD5_Hash_test, a) {
	std::string word("a");
	std::string expected("0cc175b9c0f1b6a831c399e269772661");
	auto hash = CryptoLib::Hash::MD5CreateHash(std::begin(word), std::end(word));

	EXPECT_EQ(hash, expected);
}

TEST(MD5_Hash_test, abc) {
	std::string word("abc");
	std::string expected("900150983cd24fb0d6963f7d28e17f72");
	auto hash = CryptoLib::Hash::MD5CreateHash(std::begin(word), std::end(word));

	EXPECT_EQ(hash, expected);
}

TEST(MD5_Hash_test, message_digest) {
	std::string word("message digest");
	std::string expected("f96b697d7cb7938d525a2f31aaf161d0");
	auto hash = CryptoLib::Hash::MD5CreateHash(std::begin(word), std::end(word));

	EXPECT_EQ(hash, expected);
}

TEST(MD5_Hash_test, abcdefghijklmnopqrstuvwxyz) {
	std::string word("abcdefghijklmnopqrstuvwxyz");
	std::string expected("c3fcd3d76192e4007dfb496cca67e13b");
	auto hash = CryptoLib::Hash::MD5CreateHash(std::begin(word), std::end(word));

	EXPECT_EQ(hash, expected);
}

TEST(MD5_Hash_test, ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789) {
	std::string word("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
	std::string expected("d174ab98d277d9f5a5611c2c9f419d9f");
	auto hash = CryptoLib::Hash::MD5CreateHash(std::begin(word), std::end(word));

	EXPECT_EQ(hash, expected);
}

TEST(MD5_Hash_test, 12345678901234567890123456789012345678901234567890123456789012345678901234567890) {
	std::string word("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
	std::string expected("57edf4a22be3c955ac49da2e2107b67a");
	auto hash = CryptoLib::Hash::MD5CreateHash(std::begin(word), std::end(word));

	EXPECT_EQ(hash, expected);
}
