#pragma once
#include <cassert>
#include "loquat/math/modulus_integer.hpp"

namespace loquat {

template <int MOD>
inline modulus_integer<MOD> modulus_factorial(int x){
	using mint = modulus_integer<MOD>;
	static std::vector<mint> table(1, mint(1));
	assert(x >= 0);
	while(static_cast<int>(table.size()) <= x){
		const int t = static_cast<int>(table.size());
		table.push_back(table.back() * mint(t));
	}
	return table[x];
}

template <int MOD>
inline modulus_integer<MOD> modulus_inv_factorial(int x){
	using mint = modulus_integer<MOD>;
	static std::vector<mint> table(1, mint(1));
	assert(x >= 0);
	while(static_cast<int>(table.size()) <= x){
		const int t = static_cast<int>(table.size());
		table.push_back(table.back() / mint(t));
	}
	return table[x];
}

template <int MOD>
inline modulus_integer<MOD> modulus_combination(int n, int k){
	if(k < 0 || n < k){ return 0; }
	const auto a = modulus_factorial<MOD>(n);
	const auto b = modulus_inv_factorial<MOD>(n - k);
	const auto c = modulus_inv_factorial<MOD>(k);
	return a * b * c;
}

template <int MOD>
inline modulus_integer<MOD> modulus_multichoose(int n, int k){
	return modulus_combination<MOD>(n + k - 1, k);
}

}

