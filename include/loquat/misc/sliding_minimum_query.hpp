#pragma once
#include <deque>
#include <functional>
#include <utility>
#include <cassert>

namespace loquat {

template <typename T, typename Comparator = std::less<T>>
class sliding_minimum_query {

public:
	using value_type = T;
	using comparator_type = Comparator;

private:
	using pair_type = std::pair<value_type, size_t>;

	std::deque<pair_type> m_deque;
	comparator_type m_comparator;
	size_t m_push_count;
	size_t m_pop_count;

public:
	explicit sliding_minimum_query(
		const comparator_type& comparator = comparator_type())
		: m_deque()
		, m_comparator(comparator)
		, m_push_count(0)
		, m_pop_count(0)
	{ }

	bool empty() const {
		return m_deque.empty();
	}

	const value_type& query() const {
		assert(!m_deque.empty());
		return m_deque.front().first;
	}

	void push(value_type x){
		while(!m_deque.empty() && m_comparator(x, m_deque.back().first)){
			m_deque.pop_back();
		}
		m_deque.emplace_back(x, m_push_count);
		++m_push_count;
	}

	void pop(){
		if(m_deque.front().second == m_pop_count){
			m_deque.pop_front();
		}
		++m_pop_count;
	}

};

}
