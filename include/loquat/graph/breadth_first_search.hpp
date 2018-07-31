#pragma once
#include <vector>
#include <queue>
#include "loquat/graph/adjacency_list.hpp"

namespace loquat {

template <typename EdgeType, typename F>
void breadth_first_search(
	const adjacency_list<EdgeType>& g,
	vertex_t root,
	std::vector<bool>& vis,
	F func)
{
	std::queue<vertex_t> q;
	q.push(root);
	vis[root] = true;
	while(!q.empty()){
		const auto u = q.front();
		q.pop();
		for(const auto& e : g[u]){
			const auto v = e.to;
			if(vis[v]){ continue; }
			func(u, e);
			vis[v] = true;
			q.push(v);
		}
	}
}

template <typename EdgeType, typename F>
void breadth_first_search(
	const adjacency_list<EdgeType>& g,
	vertex_t root,
	F func)
{
	std::vector<bool> vis(g.size());
	breadth_first_search(g, root, vis, func);
}

}
