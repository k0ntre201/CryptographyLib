#ifndef CRYPTOLIB_HASH_MD5_HPP
#define CRYPTOLIB_HASH_MD5_HPP

#include "../Utils/utils.hpp"
#include "GenericHash.hpp"

#include <array>
#include <functional>
#include <vector>

namespace CryptoLib::Hash
{
	namespace
	{
		struct MD5InitContext
		{
			static constexpr std::array<unsigned int, 4> initialTable = { 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476 };
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
				*d_first++ = *(uint32_t*)(number);
			}
			return d_first;
		}
	}

	template<typename _StringIter>
	std::string MD5CreateHash(_StringIter first, _StringIter last)
	{
		// Define lanbdas
		auto F = [](uint32_t x, uint32_t y, uint32_t z) -> uint32_t { return ((x & y) | ((~x) & z)); };
		auto G = [](uint32_t x, uint32_t y, uint32_t z) -> uint32_t { return ((x & z) | ((~z) & y)); };
		auto H = [](uint32_t x, uint32_t y, uint32_t z) -> uint32_t { return (x ^ y ^ z); };
		auto I = [](uint32_t x, uint32_t y, uint32_t z) -> uint32_t { return (y ^ (x | (~z))); };


		// Calculate word length
		uint64_t wordLength = std::distance(first, last);
		// Allocate memory for word
		std::string word(first, last);
		// Add signal bit on the end of word
		word.push_back(0x80);

		const uint32_t offset{ 16 };
		constexpr std::array<unsigned int, offset> padding =
		{
			0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0
		};

		// Calculate capacity for aligned word with padding bits
		size_t alignedWordSize = word.size() / 4 + ((word.size() % 4) ? 1 : 0);
		size_t rest = (offset - alignedWordSize % offset);

		// Create aligned word container
		std::vector<uint32_t> alignedWord(alignedWordSize);
		convert8bitContainerTo32Bit(word.begin(), word.end(), alignedWord.begin());

		alignedWord.insert(alignedWord.end(), padding.begin(), padding.begin() + rest);
		// Append length of word on two last bytes
		*(uint64_t*)&alignedWord[alignedWord.size() - 2] = (wordLength * 8);

		// Calculate magic numbers
		std::array<uint32_t, 64> t;
		for (size_t i{ 0 }; i < 64; ++i)
		{
			t[i] = std::floor(std::pow(2, 32) * std::abs(std::sin(i + 1)));
		}

		auto rotateLeft = [](uint32_t x, uint32_t s)
		{
			return ((x << s) | x >> (32 - s));
		};

		auto operation = [&](uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t k, uint32_t s, uint32_t i, std::function<uint32_t(uint32_t, uint32_t, uint32_t)> f) -> uint32_t
		{
			return rotateLeft(((a + f(b, c, d) + k + i)), s) + b;
		};

		auto [a, b, c, d] = MD5InitContext::initialTable;

		for (size_t i{ 0 }; i < alignedWord.size() / offset; i++)
		{
			size_t index{ 0 };
			auto [aa, bb, cc, dd] = std::array<unsigned int, 4>{ a, b, c, d };
			// Round 1
			a = operation(a, b, c, d, alignedWord[i * offset + 0], 7, t[index++], F);
			d = operation(d, a, b, c, alignedWord[i * offset + 1], 12, t[index++], F);
			c = operation(c, d, a, b, alignedWord[i * offset + 2], 17, t[index++], F);
			b = operation(b, c, d, a, alignedWord[i * offset + 3], 22, t[index++], F);

			a = operation(a, b, c, d, alignedWord[i * offset + 4], 7, t[index++], F);
			d = operation(d, a, b, c, alignedWord[i * offset + 5], 12, t[index++], F);
			c = operation(c, d, a, b, alignedWord[i * offset + 6], 17, t[index++], F);
			b = operation(b, c, d, a, alignedWord[i * offset + 7], 22, t[index++], F);

			a = operation(a, b, c, d, alignedWord[i * offset + 8], 7, t[index++], F);
			d = operation(d, a, b, c, alignedWord[i * offset + 9], 12, t[index++], F);
			c = operation(c, d, a, b, alignedWord[i * offset + 10], 17, t[index++], F);
			b = operation(b, c, d, a, alignedWord[i * offset + 11], 22, t[index++], F);

			a = operation(a, b, c, d, alignedWord[i * offset + 12], 7, t[index++], F);
			d = operation(d, a, b, c, alignedWord[i * offset + 13], 12, t[index++], F);
			c = operation(c, d, a, b, alignedWord[i * offset + 14], 17, t[index++], F);
			b = operation(b, c, d, a, alignedWord[i * offset + 15], 22, t[index++], F);

			// Round 2
			a = operation(a, b, c, d, alignedWord[i * offset + 1], 5, t[index++], G);
			d = operation(d, a, b, c, alignedWord[i * offset + 6], 9, t[index++], G);
			c = operation(c, d, a, b, alignedWord[i * offset + 11], 14, t[index++], G);
			b = operation(b, c, d, a, alignedWord[i * offset + 0], 20, t[index++], G);

			a = operation(a, b, c, d, alignedWord[i * offset + 5], 5, t[index++], G);
			d = operation(d, a, b, c, alignedWord[i * offset + 10], 9, t[index++], G);
			c = operation(c, d, a, b, alignedWord[i * offset + 15], 14, t[index++], G);
			b = operation(b, c, d, a, alignedWord[i * offset + 4], 20, t[index++], G);

			a = operation(a, b, c, d, alignedWord[i * offset + 9], 5, t[index++], G);
			d = operation(d, a, b, c, alignedWord[i * offset + 14], 9, t[index++], G);
			c = operation(c, d, a, b, alignedWord[i * offset + 3], 14, t[index++], G);
			b = operation(b, c, d, a, alignedWord[i * offset + 8], 20, t[index++], G);

			a = operation(a, b, c, d, alignedWord[i * offset + 13], 5, t[index++], G);
			d = operation(d, a, b, c, alignedWord[i * offset + 2], 9, t[index++], G);
			c = operation(c, d, a, b, alignedWord[i * offset + 7], 14, t[index++], G);
			b = operation(b, c, d, a, alignedWord[i * offset + 12], 20, t[index++], G);

			// Round 3
			a = operation(a, b, c, d, alignedWord[i * offset + 5], 4, t[index++], H);
			d = operation(d, a, b, c, alignedWord[i * offset + 8], 11, t[index++], H);
			c = operation(c, d, a, b, alignedWord[i * offset + 11], 16, t[index++], H);
			b = operation(b, c, d, a, alignedWord[i * offset + 14], 23, t[index++], H);

			a = operation(a, b, c, d, alignedWord[i * offset + 1], 4, t[index++], H);
			d = operation(d, a, b, c, alignedWord[i * offset + 4], 11, t[index++], H);
			c = operation(c, d, a, b, alignedWord[i * offset + 7], 16, t[index++], H);
			b = operation(b, c, d, a, alignedWord[i * offset + 10], 23, t[index++], H);

			a = operation(a, b, c, d, alignedWord[i * offset + 13], 4, t[index++], H);
			d = operation(d, a, b, c, alignedWord[i * offset + 0], 11, t[index++], H);
			c = operation(c, d, a, b, alignedWord[i * offset + 3], 16, t[index++], H);
			b = operation(b, c, d, a, alignedWord[i * offset + 6], 23, t[index++], H);

			a = operation(a, b, c, d, alignedWord[i * offset + 9], 4, t[index++], H);
			d = operation(d, a, b, c, alignedWord[i * offset + 12], 11, t[index++], H);
			c = operation(c, d, a, b, alignedWord[i * offset + 15], 16, t[index++], H);
			b = operation(b, c, d, a, alignedWord[i * offset + 2], 23, t[index++], H);

			// Round 4
			a = operation(a, b, c, d, alignedWord[i * offset + 0], 6, t[index++], I);
			d = operation(d, a, b, c, alignedWord[i * offset + 7], 10, t[index++], I);
			c = operation(c, d, a, b, alignedWord[i * offset + 14], 15, t[index++], I);
			b = operation(b, c, d, a, alignedWord[i * offset + 5], 21, t[index++], I);

			a = operation(a, b, c, d, alignedWord[i * offset + 12], 6, t[index++], I);
			d = operation(d, a, b, c, alignedWord[i * offset + 3], 10, t[index++], I);
			c = operation(c, d, a, b, alignedWord[i * offset + 10], 15, t[index++], I);
			b = operation(b, c, d, a, alignedWord[i * offset + 1], 21, t[index++], I);

			a = operation(a, b, c, d, alignedWord[i * offset + 8], 6, t[index++], I);
			d = operation(d, a, b, c, alignedWord[i * offset + 15], 10, t[index++], I);
			c = operation(c, d, a, b, alignedWord[i * offset + 6], 15, t[index++], I);
			b = operation(b, c, d, a, alignedWord[i * offset + 13], 21, t[index++], I);

			a = operation(a, b, c, d, alignedWord[i * offset + 4], 6, t[index++], I);
			d = operation(d, a, b, c, alignedWord[i * offset + 11], 10, t[index++], I);
			c = operation(c, d, a, b, alignedWord[i * offset + 2], 15, t[index++], I);
			b = operation(b, c, d, a, alignedWord[i * offset + 9], 21, t[index++], I);

			a += aa;
			b += bb;
			c += cc;
			d += dd;
		}

		std::array<unsigned int, 4> hash = { a,b,c,d };

		return changeByteArrayToHexStringReprezentation(hash, _byteswap_ulong);
	}

	class MD5 : public GenericHash<MD5>
	{
	public:
		std::string generateHash(std::string message);
	};
}

#endif // !CRYPTOLIB_HASH_MD5_HPP
