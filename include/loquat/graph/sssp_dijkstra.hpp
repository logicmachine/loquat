#pragma once
#include <vector>
#include <queue>
#include "loquat/graph/adjacency_list.hpp"

namespace loquat {

template <typename EdgeType>
std::vector<typename EdgeType::weight_type>
sssp_dijkstra(vertex_t source, const adjacency_list<EdgeType>& graph){
	using weight_type = typename EdgeType::weight_type;
	using pair_type = std::pair<weight_type, vertex_t>;
	using queue_type = std::priority_queue<
		pair_type, std::vector<pair_type>, std::greater<pair_type>>;
	const auto inf = std::numeric_limits<weight_type>::has_infinity
		? std::numeric_limits<weight_type>::infinity()
		: std::numeric_limits<weight_type>::max();
	const auto n = graph.size();
	std::vector<weight_type> result(n, inf);
	queue_type pq;
	result[source] = 0;
	pq.emplace(0, source);
	while(!pq.empty()){
		const auto x = pq.top().first;
		const auto u = pq.top().second;
		pq.pop();
		if(result[u] < x){ continue; }
		for(const auto& e : graph[u]){
			const auto v = e.to;
			const auto y = x + e.weight;
			if(y < result[v]){
				result[v] = y;
				pq.emplace(y, v);
			}
		}
	}
	return result;
}

}

