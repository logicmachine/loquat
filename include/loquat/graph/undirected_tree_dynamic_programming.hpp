#pragma once
#include <vector>
#include <stack>
#include <utility>
#include "loquat/graph/adjacency_list.hpp"

namespace loquat {

template <typename EdgeType, typename Behavior, typename... Args>
std::vector<typename Behavior::state_type>
undirected_tree_dynamic_programming(
	const adjacency_list<EdgeType>& graph,
	Behavior behavior,
	Args&&... args)
{
	using state_type = typename Behavior::state_type;
	using frame_type = std::pair<vertex_t, size_t>;

	const size_t n = graph.size();
	std::vector<state_type> temporary(n), result(n);
	for(vertex_t v = 0; v < n; ++v){
		temporary[v] = behavior.initial(v, args...);
	}

	std::stack<frame_type> frames;
	frames.emplace(0, 0);
	while(!frames.empty()){
		const auto frame = frames.top();
		frames.pop();
		const auto u = frame.first;
		const auto i = frame.second;
		const vertex_t p = (frames.empty() ? u : frames.top().first);
		if(i == graph[u].size()){
			for(const auto& e : graph[u]){
				if(e.to == p){ continue; }
				const auto& x = temporary[e.to];
				const auto& y = temporary[u];
				temporary[u] = behavior.merge(y, x, u, e, args...);
			}
		}else{
			const auto& e = graph[u][i];
			frames.emplace(u, i + 1);
			if(e.to != p){ frames.emplace(e.to, 0); }
		}
	}

	frames.emplace(0, 0);
	result[0] = temporary[0];
	while(!frames.empty()){
		const auto frame = frames.top();
		frames.pop();
		const auto u = frame.first;
		const auto i = frame.second;
		const vertex_t p = (frames.empty() ? u : frames.top().first);
		if(i == 0){
			for(const auto& e : graph[u]){
				if(e.to == p){
					const auto& x = temporary[e.to];
					const auto& y = temporary[u];
					result[u] = behavior.merge(y, x, u, e, args...);
					break;
				}
			}
		}
		if(i != graph[u].size()){
			const auto& e = graph[u][i];
			frames.emplace(u, i + 1);
			if(e.to != p){
				const auto& x = temporary[e.to];
				const auto& y = result[u];
				temporary[u] = behavior.purge(y, x, u, e, args...);
				frames.emplace(e.to, 0);
			}
		}
	}

	return result;
}

}
