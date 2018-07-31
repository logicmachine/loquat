#pragma once
#include <vector>
#include "loquat/graph/adjacency_list.hpp"
#include "loquat/graph/breadth_first_search.hpp"

namespace loquat {

template <typename EdgeType>
std::vector<size_t> compute_subtree_sizes(
	const adjacency_list<EdgeType>& g,
	vertex_t root)
{
	const size_t n = g.size();
	std::vector<vertex_t> order;
	order.reserve(n);
	order.push_back(root);
	breadth_first_search(g, root, [&order](vertex_t, const EdgeType& e){
		order.push_back(e.to);
	});

	std::vector<size_t> result(n);
	std::reverse(order.begin(), order.end());
	for(const auto u : order){
		size_t sum = 1;
		for(const auto& e : g[u]){
			sum += result[e.to];
		}
		result[u] = sum;
	}
	return result;
}

}
