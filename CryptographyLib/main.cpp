#include <iostream>
#include "Hash/whirpool.hpp"
#include "Hash/md5.hpp"


//template <typename T>
//class Amount
//{
//public:
//	double getValue() const
//	{
//		return static_cast<T const&>(*this).getValue();
//	}
//};
//
//class Constant42 : public Amount<Constant42>
//{
//public:
//	double getValue() const { return 42; }
//};
//
//class Variable : public Amount<Variable>
//{
//public:
//	explicit Variable(int value) : value_(value) {}
//	double getValue() const { return value_; }
//private:
//	int value_;
//};
//
//template<typename T>
//void print(Amount<T> const& amount)
//{
//	std::cout << amount.getValue() << '\n';
//}

template <typename HashTest>
std::string generateHash(std::string message, CryptoLib::Hash::GenericHash<HashTest> hashTest)
{
	return hashTest.generateHash(message);
}

int main()
{
	/*auto c0 = CryptoLib::Hash::InitializeSBox::generate_one_cyclic_array_for_extended_sBox<1>::C0[0];
	std::cout << std::hex << c0 << "\n";
	uint64_t mask = 0xff00000000000000;
	for (int i{ 1 }; i < 8; ++i)
	{
		auto v1 = ((c0 >> (8*i))& (~mask));
		auto v2 = ((c0 << ((8-i)*8)) & (mask));
		auto c1 = v1 | v2;
		std::cout << std::hex << c1 << "\n";
		mask |= (mask >> 8);
	}*/
	CryptoLib::Hash::Whirpool w;
	std::cout << generateHash("", w) << "\n";

	CryptoLib::Hash::MD5 md5;
	std::cout << generateHash("", md5) << "\n";
}