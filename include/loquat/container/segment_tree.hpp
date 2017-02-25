#pragma once
#include <vector>
#include <initializer_list>
#include <algorithm>
#include <iterator>
#include <cassert>
#include "loquat/math/bitmanip.hpp"

namespace loquat {

template <typename Behavior>
class segment_tree {

public:
	using behavior_type = Behavior;
	using value_type = typename behavior_type::value_type;
	using const_iterator = typename std::vector<value_type>::const_iterator;


private:
	size_t m_actual_size;
	std::vector<value_type> m_values;
	behavior_type m_behavior;


	void initialize(){
		const auto m = m_values.size() / 2;
		for(size_t i = 0; i < m; ++i){
			const auto k = m - 1 - i;
			m_values[k] = m_behavior.merge(
				m_values[k * 2 + 1], m_values[k * 2 + 2]);
		}
	}


public:
	segment_tree() noexcept
		: m_actual_size(0)
		, m_values()
		, m_behavior()
	{ }

	segment_tree(
		size_t size,
		const behavior_type& behavior = behavior_type())
		: m_actual_size(size)
		, m_values(bitmanip::clp2(m_actual_size) * 2 - 1)
		, m_behavior(behavior)
	{
		const auto it = m_values.begin() + m_values.size() / 2;
		std::fill(it, it + m_actual_size, value_type());
		initialize();
	}

	segment_tree(
		size_t size,
		const value_type& x,
		const behavior_type& behavior = behavior_type())
		: m_actual_size(size)
		, m_values(bitmanip::clp2(m_actual_size) * 2 - 1)
		, m_behavior(behavior)
	{
		const auto it = m_values.begin() + m_values.size() / 2;
		std::fill(it, it + m_actual_size, x);
		initialize();
	}

	template <typename Iterator>
	segment_tree(
		Iterator first,
		Iterator last,
		const behavior_type& behavior = behavior_type())
		: m_actual_size(std::distance(first, last))
		, m_values(bitmanip::clp2(m_actual_size) * 2 - 1)
		, m_behavior(behavior)
	{
		const auto it = m_values.begin() + m_values.size() / 2;
		std::copy(first, last, it);
		initialize();
	}

	segment_tree(
		std::initializer_list<value_type> il,
		const behavior_type& behavior = behavior_type())
		: m_actual_size(il.size())
		, m_values(bitmanip::clp2(m_actual_size) * 2 - 1)
		, m_behavior(behavior)
	{
		const auto it = m_values.begin() + m_values.size() / 2;
		std::copy(il.begin(), il.end(), it);
		initialize();
	}


	size_t size() const {
		return m_actual_size;
	}


	const_iterator begin() const {
		return m_values.cbegin() + (m_values.size() / 2);
	}

	const_iterator cbegin() const {
		return m_values.cbegin() + (m_values.size() / 2);
	}

	const_iterator end() const {
		return begin() + m_actual_size;
	}

	const_iterator cend() const {
		return cbegin() + m_actual_size;
	}


	const value_type& operator[](size_t i) const {
		return m_values[m_values.size() / 2 + i];
	}


	void update(size_t i, const value_type& x){
		i += m_values.size() / 2;
		m_values[i] = x;
		while(i > 0){
			i = (i - 1) / 2;
			m_values[i] = m_behavior.merge(
				m_values[i * 2 + 1], m_values[i * 2 + 2]);
		}
	}

	void update(const_iterator it, const value_type& x){
		update(it - begin(), x);
	}


	value_type query(size_t left, size_t right) const {
		const auto m = m_values.size() / 2;
		left += m; right += m;
		value_type l_value = m_behavior.identity(), r_value = l_value;
		while(left < right){
			if((left & 1u) == 0u){
				const auto& x = m_values[left];
				l_value = m_behavior.merge(l_value, x);
			}
			if((right & 1u) == 0u){
				const auto& x = m_values[right - 1];
				r_value = m_behavior.merge(x, r_value);
			}
			left  = left / 2;
			right = (right - 1) / 2;
		}
		return m_behavior.merge(l_value, r_value);
	}

	value_type query(const_iterator left, const_iterator right) const {
		return query(left - begin(), right - begin());
	}

};


template <typename Behavior>
segment_tree<Behavior> make_segment_tree(size_t n, Behavior&& behavior){
	return segment_tree<Behavior>(n, std::forward<Behavior>(behavior));
}

template <typename Iterator, typename Behavior>
segment_tree<Behavior> make_segment_tree(
	Iterator first,
	Iterator last,
	Behavior&& behavior)
{
	return segment_tree<Behavior>(
		first, last, std::forward<Behavior>(behavior));
}

}

