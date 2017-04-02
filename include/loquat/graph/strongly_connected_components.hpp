#pragma once
#include <vector>
#include <stack>
#include "loquat/graph/edge.hpp"
#include "loquat/graph/adjacency_list.hpp"
#include "loquat/graph/component_graph.hpp"

namespace loquat {

template <typename EdgeType>
belonging_components_t strongly_connected_components(
	const adjacency_list<EdgeType>& graph)
{
	const auto n = graph.size();
	adjacency_list<edge<>> inv_graph(n);
	for(vertex_t u = 0; u < n; ++u){
		for(const auto& e : graph[u]){ inv_graph.add_edge(e.to, u); }
	}
	std::vector<bool> used(n);
	std::vector<vertex_t> order;
	for(vertex_t r = 0; r < n; ++r){
		if(used[r]){ continue; }
		std::stack<std::pair<vertex_t, size_t>> stack;
		stack.emplace(r, 0);
		while(!stack.empty()){
			const auto u = stack.top().first;
			const auto j = stack.top().second;
			stack.pop();
			if(j == 0){ used[u] = true; }
			if(j == graph[u].size()){
				order.push_back(u);
			}else{
				stack.emplace(u, j + 1);
				const auto v = graph[u][j].to;
				if(!used[v]){ stack.emplace(v, 0); }
			}
		}
	}
	belonging_components_t scc(n, n);
	for(int i = static_cast<int>(n) - 1; i >= 0; --i){
		if(scc[order[i]] != n){ continue; }
		std::stack<std::pair<vertex_t, size_t>> stack;
		stack.emplace(order[i], 0);
		while(!stack.empty()){
			const auto u = stack.top().first;
			const auto j = stack.top().second;
			stack.pop();
			if(j == 0){ scc[u] = order[i]; }
			if(j == inv_graph[u].size()){ continue; }
			stack.emplace(u, j + 1);
			const auto v = inv_graph[u][j].to;
			if(scc[v] == n){ stack.emplace(v, 0); }
		}
	}
	return std::move(scc);
}

}

