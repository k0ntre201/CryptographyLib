#include "pch.h"
#include "../CryptographyLib/Hash/whirpool.hpp"

TEST(Whirpool_Hash_test, Constant_generator) 
{
	using namespace CryptoLib::Hash;
	auto y = InitializeSBox::PrecalculateSBoxValue<0, 0>::y;
	EXPECT_EQ(y, 0x0e);
	auto z1 = InitializeSBox::PrecalculateSBoxValue<0, 0>::z1;
	EXPECT_EQ(z1, 0x0);
	auto z2 = InitializeSBox::PrecalculateSBoxValue<0, 0>::z2;
	EXPECT_EQ(z2, 0x0e);
	auto SBox00 = InitializeSBox::PrecalculateSBoxValue<0, 0>::value;
	EXPECT_EQ(SBox00, 0x18);
	auto SBox01 = InitializeSBox::SBoxTable<>::table[1];
	EXPECT_EQ(SBox01, 0x23);

	auto SBoxB3 = InitializeSBox::SBoxTable<>::table[0x0b*16 + 0x03];
	EXPECT_EQ(SBoxB3, 0x8c);
}

TEST(Whirpool_Hash_test, Is_odd)
{
	using namespace CryptoLib::Hash;
	
	EXPECT_EQ(InitializeSBox::is_odd<9>::value, false);
	EXPECT_EQ(InitializeSBox::is_odd<4>::value, true);
}
