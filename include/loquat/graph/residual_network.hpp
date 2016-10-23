#pragma once
#include <type_traits>
#include "loquat/graph/edge_traits.hpp"
#include "loquat/graph/adjacency_list.hpp"

namespace loquat {

namespace detail {

template <typename EdgeType>
auto negate_weight(EdgeType& e)
	-> typename std::enable_if<has_weight<EdgeType>::value, void>::type
{
	e.weight = -e.weight;
}

template <typename EdgeType>
auto negate_weight(EdgeType&)
	-> typename std::enable_if<!has_weight<EdgeType>::value, void>::type
{ }

}


template <typename EdgeType>
struct residual_edge : public EdgeType {

	using base_type = EdgeType;

	size_t rev;

	residual_edge()
		: base_type()
		, rev(0)
	{ }

	template <typename... Args>
	residual_edge(size_t to, size_t rev, Args&&... args)
		: base_type(to, std::forward<Args>(args)...)
		, rev(rev)
	{ }

	residual_edge(const base_type& e, size_t rev)
		: base_type(e)
		, rev(rev)
	{ }

};


template <typename EdgeType>
adjacency_list<residual_edge<EdgeType>>
make_residual(const adjacency_list<EdgeType>& graph){
	using edge_type = EdgeType;
	using residual_type = residual_edge<edge_type>;
	using capacity_type = typename edge_type::capacity_type;
	const size_t n = graph.size();
	adjacency_list<residual_type> result(n);
	for(size_t u = 0; u < n; ++u){
		for(const auto& e : graph[u]){
			result.add_edge(u, residual_type(e, 0));
		}
	}
	for(size_t u = 0; u < n; ++u){
		const size_t m = graph[u].size();
		for(size_t i = 0; i < m; ++i){
			auto e = graph[u][i];
			const auto v = e.to;
			e.to = u;
			e.capacity = capacity_type();
			detail::negate_weight(e);
			result[u][i].rev = result[v].size();
			result.add_edge(v, residual_type(e, i));
		}
	}
	return result;
}


}

