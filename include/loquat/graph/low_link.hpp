#pragma once
#include <vector>
#include <stack>
#include "loquat/graph/adjacency_list.hpp"

namespace loquat {

class low_link {

public:
	using index_type = size_t;

private:
	std::vector<index_type> m_ord;
	std::vector<index_type> m_low;

public:
	low_link()
		: m_ord()
		, m_low()
	{ }

	template <typename EdgeType>
	low_link(const adjacency_list<EdgeType>& g)
		: m_ord(g.size(), std::numeric_limits<index_type>::max())
		, m_low(g.size(), std::numeric_limits<index_type>::max())
	{
		struct frame_type {
			vertex_t u, p;
			size_t i, m;
			frame_type() : u(0), p(0), i(0), m(0) { }
			frame_type(vertex_t u, vertex_t p, size_t i, size_t m)
				: u(u), p(p), i(i), m(m)
			{ }
		};
		const auto nil = std::numeric_limits<index_type>::max();
		const auto n = g.size();
		index_type next_ord = 0;
		for(vertex_t root = 0; root < n; ++root){
			if(m_ord[root] != nil){ continue; }
			std::stack<frame_type> frame_stack;
			frame_stack.emplace(root, nil, 0, 0);
			while(!frame_stack.empty()){
				const auto f = frame_stack.top();
				frame_stack.pop();
				if(f.i == 0){
					if(m_ord[f.u] != nil){ continue; }
					m_ord[f.u] = next_ord++;
				}
				if(f.i < g[f.u].size()){
					const auto v = g[f.u][f.i].to;
					if(v != f.p){
						frame_stack.emplace(f.u, f.p, f.i + 1, f.m);
						frame_stack.emplace(v, f.u, 0, 0);
					}else{
						frame_stack.emplace(f.u, f.p, f.i + 1, f.m + 1);
					}
				}else{
					for(const auto& e : g[f.u]){
						if(e.to == f.p && f.m < 2){ continue; }
						m_low[f.u] = std::min({m_low[f.u], m_low[e.to], m_ord[e.to]});
					}
				}
			}
		}
	}

	index_type ord(vertex_t v) const { return m_ord[v]; }
	index_type low(vertex_t v) const { return m_low[v]; }

};

}
