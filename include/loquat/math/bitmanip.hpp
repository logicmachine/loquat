#pragma once
#include <type_traits>
#include <cstdint>

namespace loquat {
namespace bitmanip {

template <typename T>
inline auto ctz(T x) noexcept ->
	typename std::enable_if<sizeof(T) <= sizeof(int), unsigned int>::type
{
	return __builtin_ctz(x);
}

template <typename T>
inline auto ctz(T x) noexcept ->
	typename std::enable_if<sizeof(int) < sizeof(T), unsigned int>::type
{
	return __builtin_ctzll(x);
}


template <typename T>
inline auto clz(T x) noexcept ->
	typename std::enable_if<sizeof(T) <= sizeof(int), unsigned int>::type
{
	return __builtin_clz(x);
}

template <typename T>
inline auto clz(T x) noexcept ->
	typename std::enable_if<sizeof(int) < sizeof(T), unsigned int>::type
{
	return __builtin_clzll(x);
}


template <typename T>
inline auto popcount(T x) noexcept ->
	typename std::enable_if<sizeof(T) <= sizeof(int), unsigned int>::type
{
	return __builtin_popcount(x);
}

template <typename T>
inline auto popcount(T x) noexcept ->
	typename std::enable_if<sizeof(int) < sizeof(T), unsigned int>::type
{
	return __builtin_popcountll(x);
}


template <typename T>
inline T clp2(T x) noexcept {
	if(x == 0u){ return 0u; }
	return T(1u) << (sizeof(T) * 8u - clz(x - 1));
}

template <typename T>
inline T flp2(T x) noexcept {
	if(x == 0u){ return 0u; }
	return T(1u) << (sizeof(T) * 8u - 1u - clz(x));
}

}
}

