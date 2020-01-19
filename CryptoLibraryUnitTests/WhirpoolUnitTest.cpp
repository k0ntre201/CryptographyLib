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
	
	EXPECT_EQ(InitializeSBox::is_odd<9>::value, true);
	EXPECT_EQ(InitializeSBox::is_odd<4>::value, false);
}


TEST(Whirpool_Hash_test, Check_is_numbers_to_sBox_entended_generated_correctly)
{
	using namespace CryptoLib::Hash;

	auto v1 = InitializeSBox::generate_multiply_value_to_extended_sBox<0x7c, 9>::value;
	EXPECT_EQ(v1, 0xbb);
	auto v2 = InitializeSBox::generate_multiply_value_to_extended_sBox<0x7c, 4>::value;
	EXPECT_EQ(v2, 0xed);
	auto v3 = InitializeSBox::generate_multiply_value_to_extended_sBox<0x18, 9>::value;
	EXPECT_EQ(v3, 0xd8);
	auto v4 = InitializeSBox::generate_multiply_value_to_extended_sBox<0x18, 4>::value;
	EXPECT_EQ(v4, 0x60);
}

TEST(Whirpool_Hash_test, Check_if_cyclic_vector_are_generated_correctly_for_first_cyclic)
{
	using namespace CryptoLib::Hash;

	auto v1 = InitializeSBox::generate_value_from_cyclic_matrix<0x18,0>::value;
	EXPECT_EQ(v1, 0x18186018c07830d8);
	auto v2 = InitializeSBox::generate_value_from_cyclic_matrix<0x7c,0>::value;
	EXPECT_EQ(v2, 0x7c7ced7cc791f8bb);
	auto v3 = InitializeSBox::generate_value_from_cyclic_matrix<0xff,0>::value;
	EXPECT_EQ(v3, 0xffffdbffab24e354);
}

TEST(Whirpool_Hash_test, Check_if_matrix_are_generated_correctly_for_first_cyclic)
{
	using namespace CryptoLib::Hash;

	auto v1 = InitializeSBox::generate_one_cyclic_array_for_extended_sBox<0>::C0[0];
	EXPECT_EQ(v1, 0x18186018c07830d8);
	auto v2 = InitializeSBox::generate_one_cyclic_array_for_extended_sBox<0>::C0[16*4+2];
	EXPECT_EQ(v2, 0x7c7ced7cc791f8bb);
	auto v3 = InitializeSBox::generate_one_cyclic_array_for_extended_sBox<0>::C0[16*6+8];
	EXPECT_EQ(v3, 0xffffdbffab24e354);
}

TEST(Whirpool_Hash_test, Check_is_key_item_generated_correctly)
{
	using namespace CryptoLib::Hash;
	auto k0 = InitializeSBox::generate_initial_keys<>::K0[0];
	EXPECT_EQ(k0, 0x1823c6e887b8014f);
	auto k1 = InitializeSBox::generate_initial_keys<>::K0[1];
	EXPECT_EQ(k1, 0x36A6D2F5796F9152);
	auto k2 = InitializeSBox::generate_initial_keys<>::K0[2];
	EXPECT_EQ(k2, 0x60bc9b8ea30c7b35);
}
