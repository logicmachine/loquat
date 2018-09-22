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


template <typename EdgeType>
class random_tree_generator {

public:
	using edge_type = EdgeType;
	using self_type = random_tree_generator<edge_type>;


private:
	size_t m_num_vertices;


public:
	explicit random_tree_generator(size_t n)
		: m_num_vertices(n)
	{ }


	template <typename Random>
	adjacency_list<edge_type> generate(Random& random){
		const auto n = m_num_vertices;
		std::vector<loquat::vertex_t> parents(n);
		for(loquat::vertex_t u = 1; u < n; ++u){
			std::uniform_int_distribution<loquat::vertex_t> dist(0, u - 1);
			parents[u] = dist(random);
		}
		std::vector<loquat::vertex_t> renamer(n);
		std::iota(renamer.begin(), renamer.end(), 0);
		std::shuffle(renamer.begin(), renamer.end(), random);
		adjacency_list<edge_type> graph(n);
		for(loquat::vertex_t i = 1; i < n; ++i){
			const auto a = renamer[i], b = renamer[parents[i]];
			graph.add_edge(a, b);
			graph.add_edge(b, a);
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


template <typename EdgeType>
adjacency_list<EdgeType> make_undirected(const adjacency_list<EdgeType>& g){
	const auto n = g.size();
	adjacency_list<EdgeType> out(n);
	for(vertex_t u = 0; u < n; ++u){
		for(const auto& e : g[u]){
			auto f = e;
			f.to = u;
			out.add_edge(u, e);
			out.add_edge(e.to, f);
		}
	}
	return out;
}

}
}

