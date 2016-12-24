#pragma once
#include <utility>
#include "loquat/graph/adjacency_list.hpp"

namespace loquat {

template <typename EdgeType, typename Predicate>
adjacency_list<EdgeType> filter(
	const adjacency_list<EdgeType>& graph,
	Predicate pred)
{
	const size_t n = graph.size();
	adjacency_list<EdgeType> result(n);
	for(vertex_t u = 0; u < n; ++u){
		for(const auto& e : graph[u]){
			if(pred(u, e)){ result.add_edge(u, e); }
		}
	}
	return result;
}

}

