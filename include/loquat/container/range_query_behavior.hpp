#pragma once

namespace loquat {

template <typename Impl>
class range_query_behavior_wrapper {

public:
	using value_type = typename Impl::value_type;

private:
	Impl m_impl;

public:
	range_query_behavior_wrapper() : m_impl() { }
	range_query_behavior_wrapper(Impl impl) : m_impl(std::move(impl)) { }

	value_type identity() const {
		return m_impl.identity();
	}

	value_type merge(const value_type& a, const value_type& b) const {
		return m_impl.merge(a, b);
	}

};

}

