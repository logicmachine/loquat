#pragma once
#include <vector>
#include <algorithm>
#include <iterator>
#include "loquat/math/bitmanip.hpp"

namespace loquat {

template <typename Behavior>
class lazy_segment_tree {

public:
	using behavior_type = Behavior;
	using value_type = typename behavior_type::value_type;
	using modifier_type = typename behavior_type::modifier_type;


private:
	size_t m_actual_size;
	std::vector<value_type> m_values;
	std::vector<modifier_type> m_modifiers;
	behavior_type m_behavior;

	void initialize(){
		const int n = m_values.size() / 2;
		for(int i = n - 1; i >= 0; --i){
			m_values[i] = m_behavior.merge_value(
				m_values[i * 2 + 1], m_values[i * 2 + 2]);
		}
	}

	value_type modify(
		size_t a, size_t b, size_t k, size_t l, size_t r,
		const modifier_type& modifier)
	{
		if(r <= a || b <= l){
			return m_behavior.modify(r - l, m_values[k], m_modifiers[k]);
		}else if(a <= l && r <= b){
			m_modifiers[k] =
				m_behavior.merge_modifier(m_modifiers[k], modifier);
			return m_behavior.modify(r - l, m_values[k], m_modifiers[k]);
		}
		const size_t c = l + (r - l) / 2;
		const size_t lk = k * 2 + 1, rk = lk + 1;
		const auto mp = m_behavior.split_modifier(m_modifiers[k], c - l);
		m_modifiers[k] = m_behavior.identity_modifier();
		modify(l, c, lk, l, c, mp.first);
		modify(c, r, rk, c, r, mp.second);
		const auto mq = (a < c)
			? m_behavior.split_modifier(modifier, c - std::max(a, l))
			: std::make_pair(m_behavior.identity_modifier(), modifier);
		const auto vl = modify(a, b, lk, l, c, mq.first);
		const auto vr = modify(a, b, rk, c, r, mq.second);
		m_values[k] = m_behavior.merge_value(vl, vr);
		return m_values[k];
	}

	value_type query(size_t a, size_t b, size_t k, size_t l, size_t r){
		if(r <= a || b <= l){
			return m_behavior.identity_value();
		}else if(a <= l && r <= b){
			return m_behavior.modify(r - l, m_values[k], m_modifiers[k]);
		}
		const size_t c = l + (r - l) / 2;
		const size_t lk = k * 2 + 1, rk = k * 2 + 2;
		const auto mp = m_behavior.split_modifier(m_modifiers[k], c - l);
		m_values[k] = m_behavior.modify(r - l, m_values[k], m_modifiers[k]);
		m_modifiers[k] = m_behavior.identity_modifier();
		modify(l, c, lk, l, c, mp.first);
		modify(c, r, rk, c, r, mp.second);
		const auto vl = query(a, b, lk, l, c);
		const auto vr = query(a, b, rk, c, r);
		return m_behavior.merge_value(vl, vr);
	}

public:
	lazy_segment_tree()
		: m_actual_size(0)
		, m_values()
		, m_modifiers()
		, m_behavior()
	{ }

	lazy_segment_tree(
		size_t size,
		const behavior_type& behavior = behavior_type())
		: m_actual_size(size)
		, m_values(bitmanip::clp2(size) * 2 - 1, behavior.identity_value())
		, m_modifiers(m_values.size(), behavior.identity_modifier())
		, m_behavior(behavior)
	{
		initialize();
	}

	template <typename Iterator>
	lazy_segment_tree(
		Iterator first,
		Iterator last,
		const behavior_type& behavior = behavior_type())
		: m_actual_size(std::distance(first, last))
		, m_values(
			bitmanip::clp2(m_actual_size) * 2 - 1,
			behavior.identity_value())
		, m_modifiers(m_values.size(), behavior.identity_modifier())
		, m_behavior(behavior)
	{
		const auto leaf_offset = m_values.size() / 2;
		std::copy(first, last, m_values.begin() + leaf_offset);
		initialize();
	}


	size_t size() const {
		return m_actual_size;
	}


	void modify(size_t left, size_t right, const modifier_type& modifier){
		const auto clp2_n = (m_values.size() + 1) / 2;
		modify(left, right, 0, 0, clp2_n, modifier);
	}

	value_type query(size_t left, size_t right){
		const auto clp2_n = (m_values.size() + 1) / 2;
		return query(left, right, 0, 0, clp2_n);
	}

};

}

