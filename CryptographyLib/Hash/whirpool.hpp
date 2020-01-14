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

		
		template <int r = 0, int c = 0, uint32_t ...SBoxItem>
		struct SBoxTable : SBoxTable<r + 1, c + 1, SBoxItem..., PrecalculateSBoxValue<r/16, c%16>::value> {};
		
		template <uint32_t ...SBoxItem>
		struct SBoxTable<tableSize, tableSize, SBoxItem...>
		{
			static constexpr std::array<uint32_t, tableSize> table = { SBoxItem... };
		};
	};

	class Whirpool
	{
	public:
		static constexpr std::array<uint32_t, 256> sboxTable = InitializeSBox::SBoxTable<>::table;
	private:

	};
}

#endif // !CRYPTOGTAPHY_LIB_HASH_WHIRPOOL

