#pragma once
#include <vector>
#include <functional>

namespace loquat {

template <typename T, typename F = std::plus<T>>
class fenwick_tree {

public:
	using value_type = T;


private:
	std::vector<T> m_data;
	F m_func;


public:
	fenwick_tree()
		: m_data(1)
		, m_func()
	{ }

	explicit fenwick_tree(
		size_t n,
		const value_type& x = value_type(),
		const F& f = F())
		: m_data(n + 1)
		, m_func(f)
	{
		m_data[0] = value_type();
		for(size_t i = 1; i <= n; ++i){
			m_data[i] = x;
			for(size_t j = i; (j & 1) == 0u; j >>= 1){
				m_data[i] = m_func(m_data[j], m_data[i]);
			}
		}
	}


	size_t size() const {
		return m_data.size() - 1;
	}


	value_type query(size_t i) const {
		value_type s = m_data[i];
		for(i -= i & -i; i > 0; i -= i & -i){
			s = m_func(m_data[i], s);
		}
		return s;
	}

	void modify(size_t i, const value_type& x){
		const auto n = m_data.size();
		for(++i; i < n; i += i & -i){
			m_data[i] = m_func(m_data[i], x);
		}
	}

};

}

