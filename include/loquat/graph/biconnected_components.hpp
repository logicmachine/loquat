#pragma once
#include <vector>
#include "loquat/container/disjoint_set.hpp"
#include "loquat/graph/adjacency_list.hpp"
#include "loquat/graph/component_graph.hpp"
#include "loquat/graph/low_link.hpp"

namespace loquat {

template <typename EdgeType>
belonging_components_t biconnected_components(
	const adjacency_list<EdgeType>& graph)
{
	const auto nil = low_link::nil;
	const auto n = graph.size();
	const low_link ll(graph);
	std::vector<vertex_t> ord2vertex(n);
	for(vertex_t v = 0; v < n; ++v){ ord2vertex[ll.ord(v)] = v; }
	disjoint_set ds(n);
	for(vertex_t u = 0; u < n; ++u){
		if(ll.low(u) > ll.ord(u)){ continue; }
		const auto v = ord2vertex[ll.low(u)];
		ds.unite(u, v);
	}
	belonging_components_t bcc(n, nil);
	for(vertex_t v = 0; v < n; ++v){ bcc[v] = ds.find(v); }
	return bcc;
}

}
