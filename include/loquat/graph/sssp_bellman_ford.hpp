#pragma once
#include <vector>
#include <queue>
#include "loquat/graph/adjacency_list.hpp"
#include "loquat/math/infinity.hpp"
#include "loquat/misc/exceptions.hpp"

namespace loquat {

template <typename EdgeType>
std::vector<typename EdgeType::weight_type>
sssp_bellman_ford(vertex_t source, const adjacency_list<EdgeType>& graph){
	using weight_type = typename EdgeType::weight_type;
	const auto inf = positive_infinity<weight_type>();
	const auto n = graph.size();
	std::vector<weight_type> result(n, inf);
	result[source] = weight_type();
	bool finished = false;
	for(size_t iter = 0; !finished && iter < n; ++iter){
		finished = true;
		for(loquat::vertex_t u = 0; u < n; ++u){
			if(loquat::is_positive_infinity(result[u])){ continue; }
			for(const auto& e : graph[u]){
				const auto v = e.to;
				if(result[u] + e.weight < result[v]){
					result[v] = result[u] + e.weight;
					finished = false;
				}
			}
		}
	}
	if(!finished){
		throw no_solution_error("graph has a negative cycle");
	}
	return result;
}

}

