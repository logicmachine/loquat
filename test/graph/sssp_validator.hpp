#pragma once
#include "loquat/graph/adjacency_list.hpp"

namespace loquat {
namespace test {

template <typename EdgeType>
bool validate_sssp_result(
	const std::vector<typename EdgeType::weight_type>& result,
	vertex_t source,
	const adjacency_list<EdgeType>& graph)
{
	using weight_type = typename EdgeType::weight_type;
	const size_t n = graph.size();
	adjacency_list<EdgeType> inv_graph(n);
	for(vertex_t u = 0; u < n; ++u){
		for(const auto& e : graph[u]){
			inv_graph.add_edge(e.to, u, e.weight);
		}
	}
	auto is_infinity = [](weight_type x) -> bool {
		return std::numeric_limits<weight_type>::has_infinity
			? (x > 0 && std::isinf(x))
			: (x == std::numeric_limits<weight_type>::max());
	};

	if(result.size() != n){ return false; }
	if(result[source] != 0){ return false; }
	for(vertex_t v = 0; v < n; ++v){
		if(v == source){ continue; }
		auto expect = std::numeric_limits<weight_type>::has_infinity
			? std::numeric_limits<weight_type>::infinity()
			: std::numeric_limits<weight_type>::max();
		for(const auto& e : inv_graph[v]){
			if(!is_infinity(result[e.to])){
				expect = std::min(expect, result[e.to] + e.weight);
			}
		}
		if(is_infinity(expect)){
			if(!is_infinity(result[v])){ return false; }
		}else{
			if(expect != result[v]){ return false; }
		}
	}

	return true;
}

}
}
