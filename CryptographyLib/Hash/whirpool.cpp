#include "whirpool.hpp"
#include <iomanip>
#include <sstream>

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

	std::string hashCode;
	for (uint64_t h : hash)
	{
		std::ostringstream ss;
		ss << std::setfill('0') << std::setw(16) << std::hex << h;
		std::string hexValue = ss.str();

		hashCode.insert(hashCode.end(), hexValue.begin(), hexValue.end());
	}

	return hashCode;
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
