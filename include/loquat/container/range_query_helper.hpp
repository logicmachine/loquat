#pragma once
#include "loquat/math/identity.hpp"

namespace loquat {

template <typename T, typename F>
class generic_range_query_behavior {

public:
	using value_type = T;
	using function_type = F;

private:
	function_type m_function;
	value_type m_identity;

public:
	generic_range_query_behavior(
		const function_type& func,
		const value_type& identity)
		: m_function(func)
		, m_identity(identity)
	{ }

	const value_type& identity() const {
		return m_identity;
	}

	value_type merge(const value_type& a, const value_type& b) const {
		return m_function(a, b);
	}

};


template <typename T, typename F>
auto make_range_query_behavior(F&& f)
	-> generic_range_query_behavior<T, F>
{
	return generic_range_query_behavior<T, F>(
		std::forward<F>(f), identity<F>::value());
}

template <typename T, typename F>
auto make_range_query_behavior(F&& f, const T& identity)
	-> generic_range_query_behavior<T, F>
{
	return generic_range_query_behavior<T, F>(std::forward<F>(f), identity);
}

}

