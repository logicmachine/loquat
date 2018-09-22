/**
 * @file edge_traits.hpp
 */
#pragma once

namespace loquat {

/**
 * @brief 辺が重みを持つかを調べる。
 */
template <typename EdgeType>
struct has_weight {

private:
	template <typename U>
	static std::true_type check_type(typename U::weight_type *);
	template <typename U>
	static std::false_type check_type(...);

	template <typename U>
	static auto check_member(const U& x)
		-> decltype(x.weight, std::true_type());
	static std::false_type check_member(...);

public:
	/**
	 * @brief 辺データ型が重みを持つかどうかを表す真偽値。
	 *
	 * 辺データ型EdgeTypeが重みを持つならtrue、そうでなければfalse。
	 */
	static const bool value =
		decltype(check_type<EdgeType>(nullptr))::value &&
		decltype(check_member(std::declval<EdgeType>()))::value;

};

/**
 * @brief 辺の重みを得る。
 *
 * 辺データ型が重みを持つ場合、その値をそのまま返す。
 */
template <typename EdgeType>
auto weight(const EdgeType& e) -> decltype(std::declval<EdgeType>().weight) {
	return e.weight;
}

/**
 * @brief 辺の重みを得る。
 *
 * 辺データ型が重みを持たない場合、整数1を返す。
 */
template <typename EdgeType>
auto weight(const EdgeType&)
	-> typename std::enable_if<!has_weight<EdgeType>::value, size_t>::type
{
	return 1;
}


/**
 * @brief 辺が容量を持つかを調べる。
 */
template <typename EdgeType>
struct has_capacity {

private:
	template <typename U>
	static std::true_type check_type(typename U::capacity_type *);
	static std::false_type check_type(...);

	template <typename U>
	static auto check_member(const U& x)
		-> decltype(x.capacity, std::true_type());
	static std::false_type check_member(...);

public:
	/**
	 * @brief 辺データ型が容量を持つかどうかを表す真偽値。
	 *
	 * 辺データ型EdgeTypeが容量を持つならtrue、そうでなければfalse。
	 */
	static const bool value =
		decltype(check_type<EdgeType>(nullptr))::value &&
		decltype(check_member(std::declval<EdgeType>()))::value;

};

}

