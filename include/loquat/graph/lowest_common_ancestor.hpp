#pragma once
#include <vector>
#include <utility>
#include "loquat/graph/adjacency_list.hpp"
#include "loquat/graph/breadth_first_search.hpp"
#include "loquat/misc/exceptions.hpp"

namespace loquat {

class lowest_common_ancestor {

private:
	std::vector<size_t> m_depth;
	std::vector<std::vector<vertex_t>> m_ancestor_table;

public:
	lowest_common_ancestor()
		: m_depth()
		, m_ancestor_table()
	{ }

	template <typename EdgeType>
	lowest_common_ancestor(vertex_t root, const adjacency_list<EdgeType>& g)
		: m_depth(g.size())
		, m_ancestor_table()
	{
		const size_t n = g.size();
		std::vector<vertex_t> parents(n, n);
		breadth_first_search(
			g, root,
			[this, &parents](vertex_t u, const EdgeType& e){
				m_depth[e.to] = m_depth[u] + 1;
				parents[e.to] = u;
			});
		m_ancestor_table.push_back(std::move(parents));
		for(size_t step = 2; step < n; step <<= 1){
			const auto& last = m_ancestor_table.back();
			std::vector<vertex_t> next(n, n);
			for(vertex_t a = 0; a < n; ++a){
				if(last[a] != n){ next[a] = last[last[a]]; }
			}
			m_ancestor_table.push_back(std::move(next));
		}
	}

	size_t size() const {
		return m_depth.size();
	}

	size_t depth(vertex_t v) const {
		return m_depth[v];
	}

	vertex_t kth_ancestor(vertex_t v, size_t k) const {
		if(k > m_depth[v]){
			throw no_solution_error("vertex does not have k-th ancestor");
		}
		for(size_t s = 0; (1u << s) <= k; ++s){
			if(k & (1u << s)){ v = m_ancestor_table[s][v]; }
		}
		return v;
	}

	vertex_t query(vertex_t a, vertex_t b) const {
		if(m_depth[a] > m_depth[b]){
			a = kth_ancestor(a, m_depth[a] - m_depth[b]);
		}else if(m_depth[a] < m_depth[b]){
			b = kth_ancestor(b, m_depth[b] - m_depth[a]);
		}
		if(a == b){ return a; }
		const vertex_t n = m_depth.size();
		for(size_t s = m_ancestor_table.size(); s > 0; --s){
			const auto& row = m_ancestor_table[s - 1];
			if(row[a] == n || row[b] == n){ continue; }
			if(row[a] != row[b]){
				a = row[a];
				b = row[b];
			}
		}
		return m_ancestor_table[0][a];
	}

};

}
