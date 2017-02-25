#pragma once
#include <type_traits>
#include <functional>
#include <string>
#include "loquat/utility/functional.hpp"

namespace loquat {

template <typename F, typename = void>
struct identity;


template <typename T>
struct identity<
	std::plus<T>,
	typename std::enable_if<std::is_arithmetic<T>::value>::type>
{
	static constexpr T value() noexcept {
		return T(0);
	}
};

template <>
struct identity<std::plus<std::string>> {
	static std::string value(){
		return std::string();
	}
};


template <typename T>
struct identity<
	std::multiplies<T>,
	typename std::enable_if<std::is_arithmetic<T>::value>::type>
{
	static constexpr T value() noexcept {
		return T(1);
	}
};


template <typename T>
struct identity<
	std::bit_and<T>,
	typename std::enable_if<std::is_integral<T>::value>::type>
{
	static constexpr T value() noexcept {
		return ~T(0);
	}
};

template <typename T>
struct identity<
	std::bit_or<T>,
	typename std::enable_if<std::is_integral<T>::value>::type>
{
	static constexpr T value() noexcept {
		return T(0);
	}
};

template <typename T>
struct identity<
	std::bit_xor<T>,
	typename std::enable_if<std::is_integral<T>::value>::type>
{
	static constexpr T value() noexcept {
		return T(0);
	}
};


template <typename T>
struct identity<
	min<T>,
	typename std::enable_if<std::is_integral<T>::value>::type>
{
	static constexpr T value() noexcept {
		return std::numeric_limits<T>::max();
	}
};

template <typename T>
struct identity<
	min<T>,
	typename std::enable_if<std::is_floating_point<T>::value>::type>
{
	static constexpr T value() noexcept {
		return std::numeric_limits<T>::infinity();
	}
};


template <typename T>
struct identity<
	max<T>,
	typename std::enable_if<std::is_integral<T>::value>::type>
{
	static constexpr T value() noexcept {
		return std::numeric_limits<T>::min();
	}
};

template <typename T>
struct identity<
	max<T>,
	typename std::enable_if<std::is_floating_point<T>::value>::type>
{
	static constexpr T value() noexcept {
		return -std::numeric_limits<T>::infinity();
	}
};

template <>
struct identity<max<std::string>> {
	static std::string value(){
		return std::string();
	}
};

}

