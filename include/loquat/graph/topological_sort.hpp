#pragma once
#include <vector>
#include <queue>
#include "loquat/graph/adjacency_list.hpp"
#include "loquat/misc/exceptions.hpp"

namespace loquat {

template <typename EdgeType>
std::vector<vertex_t> topological_sort(const adjacency_list<EdgeType>& graph){
	const size_t n = graph.size();
	std::vector<size_t> input_degrees(n);
	for(vertex_t u = 0; u < n; ++u){
		for(const auto& e : graph[u]){ ++input_degrees[e.to]; }
	}
	std::queue<size_t> q;
	for(vertex_t v = 0; v < n; ++v){
		if(input_degrees[v] == 0){ q.push(v); }
	}
	std::vector<vertex_t> order;
	order.reserve(n);
	while(!q.empty()){
		const auto u = q.front();
		q.pop();
		order.push_back(u);
		for(const auto& e : graph[u]){
			const auto v = e.to;
			if(--input_degrees[v] == 0){ q.push(v); }
		}
	}
	if(order.size() != n){
		throw no_solution_error("graph is not a DAG");
	}
	return order;
}

}

