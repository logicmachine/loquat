#pragma once
#include <vector>
#include "loquat/graph/types.hpp"

namespace loquat {

using belonging_components_t = std::vector<vertex_t>;

template <typename EdgeType>
adjacency_list<EdgeType> make_component_graph(
	const adjacency_list<EdgeType>& graph,
	const belonging_components_t& component_map)
{
	using edge_type = EdgeType;
	const auto n = graph.size();
	adjacency_list<edge_type> cgraph(n);
	for(vertex_t u = 0; u < n; ++u){
		for(const auto& e : graph[u]){
			const auto cu = component_map[u];
			const auto cv = component_map[e.to];
			if(cu == cv){ continue; }
			edge_type ce = e;
			ce.to = cv;
			cgraph.add_edge(cu, ce);
		}
	}
	return cgraph;
}

}

