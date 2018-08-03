#pragma once
#include <type_traits>

namespace loquat {

template <typename Impl>
class range_query_behavior_wrapper {

public:
	using value_type = typename Impl::value_type;

private:
	template <typename T>
	struct has_reverse {
	private:
		template <typename U>
		static auto check(const U& x) -> decltype(
			x.reverse(std::declval<value_type>()),
			std::true_type());
		static std::false_type check(...);
	public:
		static const bool value = decltype(check(std::declval<T>()))::value;
	};

	Impl m_impl;

	template <typename T>
	static auto reverse_helper(const T& impl, const value_type& x)
		-> typename std::enable_if<has_reverse<T>::value, value_type>::type
	{
		return impl.reverse(x);
	}

	template <typename T>
	static auto reverse_helper(const T&, const value_type& x)
		-> typename std::enable_if<!has_reverse<T>::value, value_type>::type
	{
		return x;
	}

public:
	range_query_behavior_wrapper() : m_impl() { }
	range_query_behavior_wrapper(Impl impl) : m_impl(std::move(impl)) { }

	value_type identity() const {
		return m_impl.identity();
	}

	value_type merge(const value_type& a, const value_type& b) const {
		return m_impl.merge(a, b);
	}

	value_type reverse(const value_type& x) const {
		return reverse_helper(m_impl, x);
	}

};

}

