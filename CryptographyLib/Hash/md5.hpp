#ifndef CRYPTOLIB_HASH_MD5_HPP
#define CRYPTOLIB_HASH_MD5_HPP

#include <array>
#include <functional>
#include <vector>

namespace CryptoLib::Hash
{
	struct MD5InitContext
	{
		enum Iters
		{
			A,B,C,D
		};
		static constexpr std::array<unsigned int, 4> initialTable = { 0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210 };
	};

	constexpr std::array<unsigned int, 4> MD5InitContext::initialTable;

	template <typename _8BitIter, typename _32BitIter>
	_32BitIter convert8bitContainerTo32Bit(_8BitIter first, _8BitIter last, _32BitIter d_first)
	{
		while (first != last)
		{
			char number[4] = { 0,0,0,0 };
			size_t length = (std::distance(first, last) < 4) ? std::distance(first, last) : 4;
			for (size_t i{ 0 }; i < length; ++i)
			{
				number[i] = *first++;
			}
			*d_first++ = *(int32_t*)(number);
		}
		return d_first;
	}

	template<typename T>
	constexpr T powAbsSinValue(int index)
	{
		return {};
	};

	template<>
	constexpr uint16_t powAbsSinValue(int index)
	{
		return std::floor(std::pow(2, 32) * std::abs(std::sin(index)));
	};

	constexpr unsigned int tablesize{ 64 };

	template<typename T, int... N>
	struct PrecomputedTable {};

	template<typename T, int... N>
	struct PrecomputedTable<T, 1, N...>
	{
		static constexpr std::array<T, sizeof...(N) + 1> table = { {powAbsSinValue<T>(1), N...} };
	};

	template<typename T, int L, int... N>
	struct PrecomputedTable<T, L, N...> :PrecomputedTable<T, L - 1, powAbsSinValue<T>(L), N...> {};

	template<typename T, int ...N>
	constexpr std::array<T, sizeof...(N) + 1> PrecomputedTable<T, 1, N...>::table;

	template<typename _StringIter>
	std::string MD5CreateHash(_StringIter first, _StringIter last)
	{
		std::string hashCode;
		auto F = [](int32_t x, int32_t y, int32_t z) -> int32_t { return ((x & y) | ((~x) & z)); };
		auto G = [](int32_t x, int32_t y, int32_t z) -> int32_t { return ((x & z) | ((~z) & y)); };
		auto H = [](int32_t x, int32_t y, int32_t z) -> int32_t { return (x ^ y ^ z); };
		auto I = [](int32_t x, int32_t y, int32_t z) -> int32_t { return (y ^ (x | (~z))); };
		
		size_t wordLength = std::distance(first, last);

		std::vector<uint32_t> alignedWord(1 + (wordLength + wordLength % 4) / 4);
		convert8bitContainerTo32Bit(first, last, std::begin(alignedWord));

		std::array<char, 64> padding = { 0x80
			,0,0,0,0,0,0,0,0
			,0,0,0,0,0,0,0,0
			,0,0,0,0,0,0,0,0
			,0,0,0,0,0,0,0,0
			,0,0,0,0,0,0,0,0
			,0,0,0,0,0,0,0,0
			,0,0,0,0,0,0,0,0
			,0,0,0,0,0,0,0 };

		constexpr std::array<unsigned int, 64> k{};

		if (size_t rest = alignedWord.size() % 16 != 0)
		{
			alignedWord.resize(alignedWord.size() + rest, 0);
		}
		const std::array<uint32_t, tablesize> K = PrecomputedTable<uint16_t, 64>::values;

		auto operation = [](int32_t a, int32_t b, int32_t c, int32_t d, int32_t k, int32_t s, int32_t i, std::function<int32_t(int32_t, int32_t, int32_t)>) -> int32_t { return 0; };
	}
}

#endif // !CRYPTOLIB_HASH_MD5_HPP