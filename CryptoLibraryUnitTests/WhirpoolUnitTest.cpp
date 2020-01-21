#include "pch.h"
#include "../CryptographyLib/Hash/whirpool.hpp"
#include "../CryptographyLib/Hash/whirpool.cpp"

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

TEST(Whirpool_Hash_test, really_short_key)
{
	using namespace CryptoLib::Hash;
	Whirpool w;
	EXPECT_EQ(w.generateHash(std::string("std::array")),
		std::string("587ccdcadd084415f7a903b467a9b0d18543a48267c421cee005d626d6ad8677ab8a85712bc453c559a48639e1b585ff1df5c2478969aa2862415a1e78bcca5f"));
}


TEST(Whirpool_Hash_test, short_key)
{
	using namespace CryptoLib::Hash;
	CryptoLib::Hash::Whirpool w;
	std::string is = w.generateHash("The quick brown fox jumps over the lazy dog");
	std::string expect = "b97de512e91e3828b40d2b0fdce9ceb3c4a71f9bea8d88e75c4fa854df36725fd2b52eb6544edcacd6f8beddfea403cb55ae31f03ad62a5ef54e42ee82c3fb35";
	EXPECT_EQ(is,expect);
}

TEST(Whirpool_Hash_test, long_key)
{
	using namespace CryptoLib::Hash;
	Whirpool w;

	EXPECT_EQ(w.generateHash("std::array is a container that encapsulates fixed size arrays. This container is an aggregate type with the same semantics as a struct holding a C-style array T[N] as its only non-static data member. Unlike a C-style array, it doesn't decay to T* automatically. As an aggregate type, it can be initialized with aggregate-initialization given at most N initializers that are convertible to T: std::array<int, 3> a = {1,2,3};. The struct combines the performance and accessibility of a C-style array with the benefits of a standard container, such as knowing its own size, supporting assignment, random access iterators, etc. std::array satisfies the requirements of Container and ReversibleContainer except that default-constructed array is not empty and that the complexity of swapping is linear, satisfies the requirements of ContiguousContainer, (since C++17) and partially satisfies the requirements of SequenceContainer. There is a special case for a zero-length array (N == 0). In that case, array.begin() == array.end(), which is some unique value. The effect of calling front() or back() on a zero-sized array is undefined. An array can also be used as a tuple of N elements of the same type."),
		"18a887de24e27011aa4fe09e7d29291c7c9eba73f5bd94e13d8b1b01bc40cf4ecc14dae3c5cc17ee7203e0db23fb3dded19c31277b66eaab2e84e547057bf5f1");
}