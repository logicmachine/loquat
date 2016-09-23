#pragma once
#include <utility>

namespace loquat {

template <typename T>
static std::pair<T, T> extended_gcd(T a, T b){
	if(b == 0){ return std::pair<T, T>(1, 0); }
	const auto p = extended_gcd(b, a % b);
	return std::pair<T, T>(p.second, p.first - a / b * p.second);
}

}

