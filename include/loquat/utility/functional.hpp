#pragma once
#include <algorithm>

namespace loquat {

template <typename T>
struct min {
	T operator()(const T& lhs, const T& rhs) const {
		return std::min(lhs, rhs);
	}
};

template <typename T>
struct max {
	T operator()(const T& lhs, const T& rhs) const {
		return std::max(lhs, rhs);
	}
};

}

