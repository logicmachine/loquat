#pragma once
#include <vector>
#include <initializer_list>
#include <iterator>
#include "loquat/math/bitmanip.hpp"
#include "loquat/container/range_query_behavior.hpp"

namespace loquat {

template <typename Behavior>
class nazo_table {

public:
	using behavior_type = Behavior;
	using value_type = typename behavior_type::value_type;


private:
	std::vector<std::vector<value_type>> m_table;
	range_query_behavior_wrapper<behavior_type> m_behavior;


	template <typename Iterator>
	void forward_fill(Iterator it, int row, int first, int last){
		value_type acc = m_behavior.identity();
		m_table[row][first] = acc;
		for(int i = first + 1; i < last; ++i, ++it){
			acc = m_behavior.merge(acc, *it);
			m_table[row][i] = acc;
		}
	}

	template <typename Iterator>
	void backward_fill(Iterator it, int row, int first, int last){
		value_type acc = m_behavior.identity();
		--it;
		for(int i = first - 1; i >= last; --i, --it){
			acc = m_behavior.merge(*it, acc);
			m_table[row][i] = acc;
		}
	}

	template <typename Iterator>
	void fill_table(Iterator first, Iterator last){
		const int n = std::distance(first, last);
		Iterator it = std::next(first);
		for(int i = 1; i <= n; ++i, ++it){
			const int s = bitmanip::ctz(i);
			while(m_table.size() <= static_cast<size_t>(s)){
				m_table.emplace_back(n + 1);
			}
			backward_fill(it, s, i, i - (1 << s));
			forward_fill(it, s, i, std::min(n + 1, i + (1 << s)));
		}
	}


public:
	nazo_table() noexcept
		: m_table()
		, m_behavior()
	{ }

	template <typename Iterator>
	nazo_table(
		Iterator first,
		Iterator last,
		const behavior_type& behavior = behavior_type())
		: m_table()
		, m_behavior(behavior)
	{
		fill_table(first, last);
	}

	nazo_table(
		std::initializer_list<value_type> il,
		const behavior_type& behavior = behavior_type())
		: m_table()
		, m_behavior(behavior)
	{
		fill_table(il.begin(), il.end());
	}


	size_t size() const noexcept {
		if(m_table.empty()){ return 0; }
		return m_table.front().size() - 1u;
	}


	value_type query(size_t l, size_t r) const {
		if(l == r){ return m_behavior.identity(); }
		const int s = bitmanip::ctz(bitmanip::flp2(l ^ r));
		return m_behavior.merge(m_table[s][l], m_table[s][r]);
	}

};


template <typename Iterator, typename Behavior>
nazo_table<Behavior> make_nazo_table(
	Iterator first,
	Iterator last,
	Behavior&& behavior)
{
	return nazo_table<Behavior>(
		first, last, std::forward<Behavior>(behavior));
}

}

