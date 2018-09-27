#pragma once
#include <type_traits>

namespace loquat {

template <typename Impl>
class range_query_behavior_wrapper {

public:
	using value_type = typename Impl::value_type;

private:
	template <typename T>
	struct no_reverse {
	private:
		template <typename U>
		static auto check_unary(const U& x) -> decltype(
			x.reverse(std::declval<value_type>()),
			std::false_type());
		static std::true_type check_unary(...);
		template <typename U>
		static auto check_biary(const U& x) -> decltype(
			x.reverse(0, std::declval<value_type>()),
			std::false_type());
		static std::true_type check_binary(...);
	public:
		static const bool value =
			decltype(check_unary(std::declval<T>()))::value &&
			decltype(check_binary(std::declval<T>()))::value;
	};

	Impl m_impl;

	template <typename T>
	static auto reverse_helper(const T& impl, size_t, const value_type& x)
		-> decltype(impl.reverse(x))
	{
		return impl.reverse(x);
	}

	template <typename T>
	static auto reverse_helper(const T& impl, size_t n, const value_type& x)
		-> decltype(impl.reverse(n, x))
	{
		return impl.reverse(n, x);
	}

	template <typename T>
	static auto reverse_helper(const T&, size_t, const value_type& x)
		-> typename std::enable_if<no_reverse<T>::value, value_type>::type
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

	value_type reverse(size_t n, const value_type& x) const {
		return reverse_helper(m_impl, n, x);
	}

};

}

