#ifndef CRYPTOGTAPHY_LIB_HASH_WHIRPOOL
#define CRYPTOGTAPHY_LIB_HASH_WHIRPOOL

#include <array>
#include <iostream>

namespace CryptoLib::Hash
{
	class InitializeSBox
	{
	public:
		static constexpr std::array<uint32_t, 16> E = { 0x01,0x0b,0x09,0x0c,0x0d,0x06,0x0f,0x03,0x0e,0x08,0x7,0x04,0x0a,0x02,0x05,0x0 };
		static constexpr std::array<uint32_t, 16> IE = { 0x0f,0x0,0x0d,0x07,0x0b,0x0e,0x05,0x0a,0x09,0x02,0x0c,0x01,0x03,0x04,0x08,0x06 };
		static constexpr std::array<uint32_t, 16> R = { 0x07,0x0c,0x0b,0x0d,0x0e,0x04,0x09,0x0f,0x06,0x03,0x08,0x0a,0x02,0x05,0x01,0x0 };

		template<unsigned char r, unsigned char c>
		struct PrecalculateSBoxValue
		{
			static constexpr uint32_t y = E[r] ^ IE[c];
			static constexpr uint32_t z1 = R[y] ^ E[r];
			static constexpr uint32_t z2 = R[y] ^ IE[c];
			static constexpr uint32_t Ez1 = E[z1] << 4;
			static constexpr uint32_t value = Ez1 | IE[z2];
		};

		static constexpr int tableSize{ 256 };

		
		template <int r = 0, int c = 0, uint64_t ...SBoxItem>
		struct SBoxTable : SBoxTable<r + 1, c + 1, SBoxItem..., PrecalculateSBoxValue<r/16, c%16>::value> {};
		
		template <uint64_t ...SBoxItem>
		struct SBoxTable<tableSize, tableSize, SBoxItem...>
		{
			static constexpr std::array<uint64_t, tableSize> table = { SBoxItem... };
		};

		template <int N>
		struct is_odd
		{
			static constexpr bool value = N % 2;
		};

		struct divide_by_reduce_polynomial_helper
		{
			static constexpr std::array<int, 4> reducePolynomials = { 0x8e8,0x474,0x23a,0x11d};
			static constexpr std::array<int, 4> binaryFlags = { 0x800,0x400,0x200,0x100 };
		};


		template<int V, int N = 3>
		struct divide_by_reduce_polynomial : 
			divide_by_reduce_polynomial <(V & divide_by_reduce_polynomial_helper::binaryFlags[N]) ?
			V ^ divide_by_reduce_polynomial_helper::reducePolynomials[N] : V,
			N - 1>
		{};

		template <int V>
		struct divide_by_reduce_polynomial<V,0>
		{
			static constexpr int value = (V & divide_by_reduce_polynomial_helper::binaryFlags[0]) ? V ^ divide_by_reduce_polynomial_helper::reducePolynomials[0] : V;
		};
		
		template <int V, int Mul>
		struct generate_multiply_value_to_extended_sBox
		{
			static constexpr uint64_t value = Mul == 1 ? V : is_odd<Mul>::value == true ? divide_by_reduce_polynomial<V * (Mul - 1)>::value ^ V : divide_by_reduce_polynomial<V*Mul>::value;
		};

		static constexpr std::array<int, 8> cyclicValues = { 0x01,0x01,0x04,0x01,0x08,0x05,0x02,0x09 };

		template <int Value, int Level, uint64_t NextValue, int Index = 0>
		struct generate_value_from_cyclic_matrix_helper : 
			generate_value_from_cyclic_matrix_helper<
			Value,
			Level,
			NextValue|(generate_multiply_value_to_extended_sBox<Value,cyclicValues[(Index + (8 - Level)) % 8]>::value<<(8*(7-Index))),
			Index+1>
		{};

		template <int Value, int Level, uint64_t NextValue>
		struct generate_value_from_cyclic_matrix_helper<Value, Level, NextValue, 8>
		{
			static constexpr uint64_t value = NextValue;
		};

		template <int Value, int Level>
		struct generate_value_from_cyclic_matrix
		{
			static constexpr uint64_t value = generate_value_from_cyclic_matrix_helper<Value, Level, 0>::value;
		};

		struct SBox_table_constant
		{
			static constexpr std::array<uint64_t, tableSize> sboxTable = InitializeSBox::SBoxTable<>::table;
		};

		template <int Level, uint64_t Index = 0, uint64_t... Values>
		struct generate_one_cyclic_array_for_extended_sBox : 
			generate_one_cyclic_array_for_extended_sBox<Level, 
			Index + 1,
			Values...,
			generate_value_from_cyclic_matrix<SBox_table_constant::sboxTable[Index], Level>::value>
		{};

		template <int Level, uint64_t... Values>
		struct generate_one_cyclic_array_for_extended_sBox<Level, tableSize, Values...>
		{
			static constexpr std::array<uint64_t, tableSize> C0 = { Values... };
		};


		template <int KeyIndex, uint64_t NextValue, int TempIndex = 0>
		struct generate_one_key_item_helper : generate_one_key_item_helper<KeyIndex, NextValue | (SBox_table_constant::sboxTable[KeyIndex * 8 + TempIndex] << (8 * (7 - TempIndex))), TempIndex + 1> {};

		template <int KeyIndex, uint64_t NextValue>
		struct generate_one_key_item_helper<KeyIndex, NextValue, 8>
		{
			static constexpr uint64_t key = NextValue;
		};

		template <int KeyIndex>
		struct generate_one_key_item
		{
			static constexpr uint64_t key = generate_one_key_item_helper<KeyIndex, 0>::key;
		};

		template <int KeyIndex = 0, uint64_t... Keys>
		struct generate_initial_keys : generate_initial_keys<KeyIndex + 1, Keys..., generate_one_key_item<KeyIndex>::key> {};

		template <uint64_t... Keys>
		struct generate_initial_keys<10, Keys...>
		{
			static constexpr std::array<uint64_t, 10> K0 = { Keys... };
		};

	};

	class Whirpool
	{
	public:

	public:
		uint64_t whirpoolOperation(std::array<uint64_t, 8> block, int shift);

	private:

		static constexpr int rounds{ 10 };

		static constexpr std::array<std::array<uint64_t, InitializeSBox::tableSize>, 8> fullInitialSBox =
		{
			InitializeSBox::generate_one_cyclic_array_for_extended_sBox<0>::C0,
			InitializeSBox::generate_one_cyclic_array_for_extended_sBox<1>::C0,
			InitializeSBox::generate_one_cyclic_array_for_extended_sBox<2>::C0,
			InitializeSBox::generate_one_cyclic_array_for_extended_sBox<3>::C0,
			InitializeSBox::generate_one_cyclic_array_for_extended_sBox<4>::C0,
			InitializeSBox::generate_one_cyclic_array_for_extended_sBox<5>::C0,
			InitializeSBox::generate_one_cyclic_array_for_extended_sBox<6>::C0,
			InitializeSBox::generate_one_cyclic_array_for_extended_sBox<7>::C0
		};

		static constexpr std::array<uint64_t, rounds> rc = InitializeSBox::generate_initial_keys<>::K0;
	};
}

#endif // !CRYPTOGTAPHY_LIB_HASH_WHIRPOOL

