#pragma once

namespace loquat {

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
	static const bool value =
		decltype(check_type<EdgeType>(nullptr))::value &&
		decltype(check_member(std::declval<EdgeType>()))::value;

};


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
	static const bool value =
		decltype(check_type<EdgeType>(nullptr))::value &&
		decltype(check_member(std::declval<EdgeType>()))::value;

};

}

