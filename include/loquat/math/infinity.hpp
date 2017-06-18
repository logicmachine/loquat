#pragma once
#include <limits>
#include <type_traits>
#include <cmath>

namespace loquat {

template <typename T>
constexpr inline auto positive_infinity() noexcept
	-> typename std::enable_if<std::is_integral<T>::value, T>::type
{
	return std::numeric_limits<T>::max();
}

template <typename T>
constexpr inline auto negative_infinity() noexcept
	-> typename std::enable_if<std::is_integral<T>::value, T>::type
{
	return std::numeric_limits<T>::min();
}

template <typename T>
constexpr inline auto is_positive_infinity(T x) noexcept
	-> typename std::enable_if<std::is_integral<T>::value, bool>::type
{
	return x == std::numeric_limits<T>::max();
}

template <typename T>
constexpr inline auto is_negative_infinity(T x) noexcept
	-> typename std::enable_if<std::is_integral<T>::value, bool>::type
{
	return x == std::numeric_limits<T>::min();
}


template <typename T>
constexpr inline auto positive_infinity() noexcept
	-> typename std::enable_if<std::is_floating_point<T>::value, T>::type
{
	return std::numeric_limits<T>::infinity();
}

template <typename T>
constexpr inline auto negative_infinity() noexcept
	-> typename std::enable_if<std::is_floating_point<T>::value, T>::type
{
	return -std::numeric_limits<T>::infinity();
}

template <typename T>
inline auto is_positive_infinity(T x) noexcept
	-> typename std::enable_if<std::is_floating_point<T>::value, bool>::type
{
	return (x > 0) && std::isinf(x);
}

template <typename T>
inline auto is_negative_infinity(T x) noexcept
	-> typename std::enable_if<std::is_floating_point<T>::value, bool>::type
{
	return (x < 0) && std::isinf(x);
}

}

