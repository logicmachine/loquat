#pragma once
#include <vector>
#include <queue>
#include "loquat/graph/adjacency_list.hpp"
#include "loquat/graph/algorithms.hpp"
#include "loquat/graph/sssp_bellman_ford.hpp"

namespace loquat {

template <typename EdgeType>
typename EdgeType::weight_type
mincostflow_primal_dual(
	typename EdgeType::capacity_type flow,
	vertex_t source,
	vertex_t sink,
	adjacency_list<EdgeType>& graph)
{
	using edge_type = EdgeType;
	using weight_type = typename edge_type::weight_type;
	const auto inf = positive_infinity<weight_type>();
	const auto n = graph.size();
	const auto predicate =
		[](vertex_t, const edge_type& e) -> bool { return e.capacity > 0; };
	auto h = sssp_bellman_ford(source, filter(graph, predicate));
	std::vector<vertex_t> prev_vertex(n);
	std::vector<size_t> prev_edge(n);
	weight_type result = 0;
	while(flow > 0){
		using pair_type = std::pair<weight_type, vertex_t>;
		std::priority_queue<
			pair_type, std::vector<pair_type>, std::greater<pair_type>> pq;
		std::vector<weight_type> d(n, inf);
		pq.emplace(0, source);
		d[source] = weight_type();
		while(!pq.empty()){
			const auto p = pq.top();
			pq.pop();
			const auto u = p.second;
			if(d[u] < p.first){ continue; }
			for(size_t i = 0; i < graph[u].size(); ++i){
				const auto& e = graph[u][i];
				if(e.capacity <= 0){ continue; }
				const auto v = e.to;
				const auto t = d[u] + e.weight + h[u] - h[v];
				if(d[v] <= t){ continue; }
				d[v] = t;
				prev_vertex[v] = u;
				prev_edge[v] = i;
				pq.emplace(t, v);
			}
		}
		if(is_positive_infinity(d[sink])){
			throw no_solution_error("there are no enough capacities to flow");
		}
		for(size_t i = 0; i < n; ++i){ h[i] += d[i]; }
		weight_type f = flow;
		for(vertex_t v = sink; v != source; v = prev_vertex[v]){
			const auto u = prev_vertex[v];
			f = std::min(f, graph[u][prev_edge[v]].capacity);
		}
		flow -= f;
		result += f * h[sink];
		for(vertex_t v = sink; v != source; v = prev_vertex[v]){
			const auto u = prev_vertex[v];
			auto& e = graph[u][prev_edge[v]];
			e.capacity -= f;
			graph[v][e.rev].capacity += f;
		}
	}
	return result;
}

}

