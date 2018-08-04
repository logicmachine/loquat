#pragma once
#include <utility>

namespace loquat {

template <typename Impl>
class lazy_range_query_behavior_wrapper {

public:
	using value_type = typename Impl::value_type;
	using modifier_type = typename Impl::modifier_type;

private:
	using modifier_pair = std::pair<modifier_type, modifier_type>;

	template <typename T>
	struct no_split_modifier {
	private:
		template <typename U>
		static auto check(const U& x) -> decltype(
			x.split_modifier(std::declval<modifier_type>(), 0),
			std::false_type());
		static std::true_type check(...);
	public:
		static const bool value = decltype(check(std::declval<T>()))::value;
	};

	template <typename T>
	struct no_reverse_value {
	private:
		template <typename U>
		static auto check_unary(const U& x) -> decltype(
			x.reverse_value(std::declval<value_type>()),
			std::false_type());
		static std::true_type check_unary(...);
		template <typename U>
		static auto check_binary(const U& x) -> decltype(
			x.reverse_value(0, std::declval<value_type>()),
			std::false_type());
		static std::true_type check_binary(...);
	public:
		static const bool value =
			decltype(check_unary(std::declval<T>()))::value &&
			decltype(check_binary(std::declval<T>()))::value;
	};

	template <typename T>
	struct no_reverse_modifier {
	private:
		template <typename U>
		static auto check_unary(const U& x) -> decltype(
			x.reverse_modifier(std::declval<modifier_type>()),
			std::false_type());
		static std::true_type check_unary(...);
		template <typename U>
		static auto check_binary(const U& x) -> decltype(
			x.reverse_modifier(0, std::declval<modifier_type>()),
			std::false_type());
		static std::true_type check_binary(...);
	public:
		static const bool value =
			decltype(check_unary(std::declval<T>()))::value &&
			decltype(check_binary(std::declval<T>()))::value;
	};

	Impl m_impl;

	template <typename T>
	static auto split_modifier_helper(
		const T& impl, const modifier_type& m, size_t k)
		-> decltype(impl.split_modifier(m, k))
	{
		return impl.split_modifier(m, k);
	}

	template <typename T>
	static auto split_modifier_helper(
		const T&, const modifier_type& m, size_t)
		-> typename std::enable_if<no_split_modifier<T>::value, modifier_pair>::type
	{
		return std::make_pair(m, m);
	}

	template <typename T>
	static auto modify_helper(
		const T& impl, size_t n, const value_type& v, const modifier_type& m)
		-> decltype(impl.modify(n, v, m))
	{
		return impl.modify(n, v, m);
	}

	template <typename T>
	static auto modify_helper(
		const T& impl, size_t, const value_type& v, const modifier_type& m)
		-> decltype(impl.modify(v, m))
	{
		return impl.modify(v, m);
	}

	template <typename T>
	static auto reverse_value_helper(const T& impl, size_t, const value_type& x)
		-> decltype(impl.reverse_value(x))
	{
		return impl.reverse_value(x);
	}

	template <typename T>
	static auto reverse_value_helper(const T& impl, size_t n, const value_type& x)
		-> decltype(impl.reverse_value(n, x))
	{
		return impl.reverse_value(n, x);
	}

	template <typename T>
	static auto reverse_value_helper(const T&, size_t, const value_type& x)
		-> typename std::enable_if<no_reverse_value<T>::value, value_type>::type
	{
		return x;
	}

	template <typename T>
	static auto reverse_modifier_helper(const T& impl, size_t, const modifier_type& m)
		-> decltype(impl.reverse_modifier(m))
	{
		return impl.reverse_modifier(m);
	}

	template <typename T>
	static auto reverse_modifier_helper(const T& impl, size_t n, const modifier_type& m)
		-> decltype(impl.reverse_modifier(n, m))
	{
		return impl.reverse_modifier(n, m);
	}

	template <typename T>
	static auto reverse_modifier_helper(const T&, size_t, const modifier_type& m)
		-> typename std::enable_if<no_reverse_modifier<T>::value, modifier_type>::type
	{
		return m;
	}

public:
	lazy_range_query_behavior_wrapper() : m_impl() { }
	lazy_range_query_behavior_wrapper(Impl impl) : m_impl(std::move(impl)) { }

	value_type identity_value() const {
		return m_impl.identity_value();
	}

	modifier_type identity_modifier() const {
		return m_impl.identity_modifier();
	}

	std::pair<modifier_type, modifier_type>
	split_modifier(const modifier_type& m, size_t k) const {
		return split_modifier_helper(m_impl, m, k);
	}

	modifier_type merge_modifier(
		const modifier_type& a, const modifier_type& b) const
	{
		return m_impl.merge_modifier(a, b);
	}

	value_type merge_value(
		const value_type& a, const value_type& b) const
	{
		return m_impl.merge_value(a, b);
	}

	value_type modify(size_t n, const value_type& v, const modifier_type& m) const {
		return modify_helper(m_impl, n, v, m);
	}

	value_type reverse_value(size_t n, const value_type& m) const {
		return reverse_value_helper(m_impl, n, m);
	}

	modifier_type reverse_modifier(size_t n, const modifier_type& m) const {
		return reverse_modifier_helper(m_impl, n, m);
	}

};

}

