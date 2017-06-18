#pragma once
#include <vector>

namespace loquat {

class disjoint_set {

public:
	using value_type = size_t;


private:
	std::vector<value_type> m_parents;
	std::vector<value_type> m_ranks;


public:
	disjoint_set()
		: m_parents()
		, m_ranks()
	{ }

	explicit disjoint_set(size_t n)
		: m_parents(n)
		, m_ranks(n)
	{
		for(size_t i = 0; i < n; ++i){
			m_parents[i] = i;
		}
	}


	size_t size() const {
		return m_parents.size();
	}


	value_type find(size_t x){
		if(m_parents[x] == x){ return x; }
		m_parents[x] = find(m_parents[x]);
		return m_parents[x];
	}

	value_type unite(size_t x, size_t y){
		x = find(x);
		y = find(y);
		if(x == y){ return x; }
		if(m_ranks[x] < m_ranks[y]){
			m_parents[x] = y;
			return y;
		}else if(m_ranks[x] > m_ranks[y]){
			m_parents[y] = x;
			return x;
		}else{
			m_parents[y] = x;
			++m_ranks[x];
			return x;
		}
	}

	bool same(size_t x, size_t y){
		return find(x) == find(y);
	}

};

}

