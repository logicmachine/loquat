#pragma once
#include <functional>
#include <random>
#include "loquat/graph/adjacency_list.hpp"

namespace loquat {
namespace test {

template <typename EdgeType>
class random_graph_generator {

public:
	using edge_type = EdgeType;
	using self_type = random_graph_generator<edge_type>;


private:
	size_t m_num_vertices;
	double m_edge_rate;

	bool m_has_self_loop;

public:
	random_graph_generator(size_t n, double p)
		: m_num_vertices(n)
		, m_edge_rate(p)
		, m_has_self_loop(false)
	{ }


	self_type& has_self_loop(bool x){
		m_has_self_loop = x;
		return *this;
	}


	template <typename Random>
	adjacency_list<edge_type> generate(Random& random){
		const auto n = m_num_vertices;
		std::uniform_real_distribution<> e_dist(0.0, 1.0);
		adjacency_list<edge_type> graph(n);
		for(loquat::vertex_t u = 0; u < n; ++u){
			for(loquat::vertex_t v = 0; v < n; ++v){
				if(u == v && !m_has_self_loop){ continue; }
				if(e_dist(random) >= m_edge_rate){ continue; }
				edge_type e;
				e.to = v;
				graph.add_edge(u, e);
			}
		}
		return graph;
	}

};


template <typename EdgeType, typename Random, typename Distribution>
void randomize_weights(
	adjacency_list<EdgeType>& graph,
	Random& random,
	Distribution distribution)
{
	const auto n = graph.size();
	for(loquat::vertex_t u = 0; u < n; ++u){
		for(auto& e : graph[u]){ e.weight = distribution(random); }
	}
}

template <typename EdgeType, typename Random, typename Distribution>
void randomize_capacities(
	adjacency_list<EdgeType>& graph,
	Random& random,
	Distribution distribution)
{
	const auto n = graph.size();
	for(loquat::vertex_t u = 0; u < n; ++u){
		for(auto& e : graph[u]){ e.capacity = distribution(random); }
	}
}

}
}

