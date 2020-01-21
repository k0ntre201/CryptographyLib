#include "../Utils/utils.hpp"
#include "whirpool.hpp"


namespace
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
		struct SBoxTable : SBoxTable<r + 1, c + 1, SBoxItem..., PrecalculateSBoxValue<r / 16, c % 16>::value> {};

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
			static constexpr std::array<int, 4> reducePolynomials = { 0x8e8,0x474,0x23a,0x11d };
			static constexpr std::array<int, 4> binaryFlags = { 0x800,0x400,0x200,0x100 };
		};


		template<int V, int N = 3>
		struct divide_by_reduce_polynomial :
			divide_by_reduce_polynomial <(V & divide_by_reduce_polynomial_helper::binaryFlags[N]) ?
			V ^ divide_by_reduce_polynomial_helper::reducePolynomials[N] : V,
			N - 1>
		{};

		template <int V>
		struct divide_by_reduce_polynomial<V, 0>
		{
			static constexpr int value = (V & divide_by_reduce_polynomial_helper::binaryFlags[0]) ? V ^ divide_by_reduce_polynomial_helper::reducePolynomials[0] : V;
		};

		template <int V, int Mul>
		struct generate_multiply_value_to_extended_sBox
		{
			static constexpr uint64_t value = Mul == 1 ? V : is_odd<Mul>::value == true ? divide_by_reduce_polynomial<V * (Mul - 1)>::value ^ V : divide_by_reduce_polynomial<V * Mul>::value;
		};

		static constexpr std::array<int, 8> cyclicValues = { 0x01,0x01,0x04,0x01,0x08,0x05,0x02,0x09 };

		template <int Value, int Level, uint64_t NextValue, int Index = 0>
		struct generate_value_from_cyclic_matrix_helper :
			generate_value_from_cyclic_matrix_helper<
			Value,
			Level,
			NextValue | (generate_multiply_value_to_extended_sBox<Value, cyclicValues[(Index + (8 - Level)) % 8]>::value << (8 * (7 - Index))),
			Index + 1>
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
}

std::string CryptoLib::Hash::Whirpool::generateHash(std::string message)
{
	uint64_t messageLength = message.size();
	int blocks = messageLength / 64;
	
	std::array<uint64_t, 8> hash = { 0,0,0,0,0,0,0,0 };
	for (int i{ 0 }; i < blocks; ++i)
	{
		std::array<uint64_t, 8> block = { 0,0,0,0,0,0,0,0 };
		std::string messageBlock(std::begin(message) + i * 64, std::begin(message) + (i + 1) * 64);

		for (size_t j{ 0 }; j < block.size(); ++j)
		{
			for (size_t k{ 0 }; k < 8; ++k)
				block[j] |= (uint64_t)messageBlock[k + 8*j] << (7 - k) * 8;
		}
		hash = processOneBlock(block, hash);
	}

	uint64_t restBytes = messageLength % 64;
	uint64_t restSubBlocks = (restBytes / 8);
	std::array<uint64_t, 8> block = { 0,0,0,0,0,0,0,0 };
	std::string messageBlock(std::begin(message) + blocks * 64, std::end(message));

	int i{ 0 };
	for (size_t j{ 0 }; j < restSubBlocks + 1; ++j)
	{
		for (size_t k{ 0 }; k < 8; ++k)
		{
			if (i == restBytes)
				break;
			block[j] |= (uint64_t)messageBlock[i] << (7 - k) * 8;
			++i;
		}
	}
	//std::cout << std::hex << block[restSubBlocks] << "\n";
	block[restSubBlocks] |= (uint64_t)0x80 << (7 - restBytes%8) * 8;
	//std::cout << std::hex << block[restSubBlocks] << "\n";
	if (restSubBlocks >= 4)
	{
		hash = processOneBlock(block, hash);
		block = { 0,0,0,0,0,0,0,0 };
	}

	block[7] = messageLength << 3;
	hash = processOneBlock(block, hash);

	return changeByteArrayToHexStringReprezentation(hash, [](uint64_t h) {return h; });
}

uint64_t CryptoLib::Hash::Whirpool::whirpoolOperation(const std::array<uint64_t,8>& block, int shift)
{
	uint64_t result{ 0 };

	for (int j{ 8 }, i{ 0 }; j >= 1; --j,++i)
	{
		uint32_t index = (shift + j) & 7;
		auto val = (block[index] >> ((j - 1) * 8))&0xff;
		result ^= fullInitialSBox[i][val];
	}

	return result;
}

std::array<uint64_t, 8> CryptoLib::Hash::Whirpool::processOneBlock(const std::array<uint64_t, 8>& block, const std::array<uint64_t,8>& hash)
{
	std::array<uint64_t, 8> keyProcess;
	std::array<uint64_t, 8> tempKeyProcess;
	std::array<uint64_t, 8> textProcces;
	std::array<uint64_t, 8> outputHash;
	std::array<uint64_t, 8> tempHash;

	
	auto xorOperation = [](uint64_t x, uint64_t y) -> uint64_t { return x ^ y; };

	std::copy(std::begin(hash), std::end(hash), std::begin(keyProcess));
	std::copy(std::begin(block), std::end(block), std::begin(textProcces));
	std::transform(std::begin(keyProcess), std::end(keyProcess), std::begin(textProcces), std::begin(textProcces), xorOperation);
	std::copy(std::begin(textProcces), std::end(textProcces), std::begin(tempHash));

	for (size_t r{ 0 }; r < rounds; ++r)
	{
		for (size_t i{ 0 }; i < 8; ++i)
		{
			tempKeyProcess[i] = whirpoolOperation(keyProcess, i);
		}
		std::copy(std::begin(tempKeyProcess), std::end(tempKeyProcess), std::begin(keyProcess));
		keyProcess[0] ^= rc[r];
		
		for (size_t i{ 0 }; i < 8; ++i)
		{
			outputHash[i] = whirpoolOperation(textProcces, i);
		}
		std::copy(std::begin(outputHash), std::end(outputHash), std::begin(textProcces));
		std::transform(std::begin(keyProcess), std::end(keyProcess), std::begin(textProcces), std::begin(textProcces), xorOperation);
	}

	std::transform(std::begin(tempHash), std::end(tempHash), std::begin(textProcces), std::begin(textProcces), xorOperation);

	return textProcces;
}
