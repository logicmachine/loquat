#pragma once
#include <algorithm>
#include <functional>
#include "loquat/graph/adjacency_list.hpp"
#include "loquat/graph/edge_traits.hpp"
#include "loquat/graph/undirected_tree_dynamic_programming.hpp"
#include "loquat/misc/top_k.hpp"

namespace loquat {

namespace detail {

template <typename EdgeType>
auto weight(const EdgeType& e) -> decltype(std::declval<EdgeType>().weight) {
	return e.weight;
}

template <typename EdgeType>
auto weight(const EdgeType&)
	-> typename std::enable_if<!has_weight<EdgeType>::value, size_t>::type
{
	return 1;
}

template <typename EdgeType>
struct tree_diameter_behavior {
	using weight_type = decltype(weight(std::declval<EdgeType>()));
	using state_type = top_k<weight_type, 2, std::greater<weight_type>>;
	using edge_type = EdgeType;

	state_type initial(vertex_t) const {
		state_type s;
		s.push(weight_type());
		return s;
	}

	state_type merge(
		state_type y,
		const state_type& x,
		vertex_t,
		const edge_type& e) const
	{
		if(!x.empty()){ y.push(x[0] + weight(e)); }
		return y;
	}

	state_type purge(
		const state_type& y,
		const state_type& x,
		vertex_t,
		const edge_type& e) const
	{
		if(!x.empty() && !y.empty() && y[0] == x[0] + weight(e)){
			state_type z;
			if(y.size() > 1){ z.push(y[1]); }
			return z;
		}else{
			return y;
		}
	}
};

}

template <typename EdgeType>
auto tree_diameter(const adjacency_list<EdgeType>& graph)
	-> decltype(detail::weight(std::declval<EdgeType>()))
{
	using weight_type = decltype(detail::weight(std::declval<EdgeType>()));
	const auto dp = undirected_tree_dynamic_programming(
		graph, detail::tree_diameter_behavior<EdgeType>());
	weight_type result = 0;
	for(const auto& p : dp){
		result = std::max(result, p[0]);
	}
	return result;
}

}
