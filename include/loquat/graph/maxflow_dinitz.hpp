#pragma once
#include <vector>
#include <queue>
#include "loquat/graph/adjacency_list.hpp"

namespace loquat {

namespace detail {

template <typename EdgeType>
class maxflow_dinitz_impl {

public:
	using edge_type = EdgeType;
	using capacity_type = typename edge_type::capacity_type;


private:
	adjacency_list<edge_type>& m_graph;
	std::vector<size_t> m_levels;
	std::vector<size_t> m_iterations;
	vertex_t m_source;
	vertex_t m_sink;

	maxflow_dinitz_impl() = delete;
	maxflow_dinitz_impl(const maxflow_dinitz_impl&) = delete;

	explicit maxflow_dinitz_impl(
		vertex_t source,
		vertex_t sink,
		adjacency_list<edge_type>& graph)
		: m_graph(graph)
		, m_levels()
		, m_iterations()
		, m_source(source)
		, m_sink(sink)
	{ }

	std::vector<size_t> compute_level_map(vertex_t source) const {
		const auto n = m_graph.size();
		std::vector<size_t> levels(n, 0);
		std::queue<vertex_t> q;
		levels[source] = 1;
		q.push(source);
		while(!q.empty()){
			const auto u = q.front();
			q.pop();
			for(const auto& e : m_graph[u]){
				const auto v = e.to;
				if(e.capacity <= 0 || levels[v] != 0){ continue; }
				levels[v] = levels[u] + 1;
				q.push(v);
			}
		}
		return levels;
	}

	capacity_type dfs_recur(vertex_t u, capacity_type limit){
		if(u == m_sink){ return limit; }
		for(; m_iterations[u] < m_graph[u].size(); ++m_iterations[u]){
			auto& e = m_graph[u][m_iterations[u]];
			const auto v = e.to;
			if(e.capacity <= 0 || m_levels[u] >= m_levels[v]){ continue; }
			const auto diff = dfs_recur(v, std::min(e.capacity, limit));
			if(diff > 0){
				e.capacity -= diff;
				m_graph[v][e.rev].capacity += diff;
				return diff;
			}
		}
		return 0;
	}

	capacity_type solve(){
		const auto inf = std::numeric_limits<capacity_type>::has_infinity
			? std::numeric_limits<capacity_type>::infinity()
			: std::numeric_limits<capacity_type>::max();
		const auto n = m_graph.size();
		capacity_type flow = 0;
		while(true){
			m_levels = compute_level_map(m_source);
			if(m_levels[m_sink] == 0){ break; }
			m_iterations.assign(n, 0);
			while(true){
				const auto f = dfs_recur(m_source, inf);
				if(f <= 0){ break; }
				flow += f;
			}
		}
		return flow;
	}


public:
	static capacity_type solve(
		vertex_t source, vertex_t sink, adjacency_list<EdgeType>& graph)
	{
		maxflow_dinitz_impl<edge_type> self(source, sink, graph);
		return self.solve();
	}

};

}


template <typename EdgeType>
typename EdgeType::capacity_type
maxflow_dinitz(
	vertex_t source,
	vertex_t sink,
	adjacency_list<EdgeType>& graph)
{
	using edge_type = EdgeType;
	return detail::maxflow_dinitz_impl<edge_type>::solve(source, sink, graph);
}

}

