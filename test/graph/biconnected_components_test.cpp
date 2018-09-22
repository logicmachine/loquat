#include <gtest/gtest.h>
#include <vector>
#include <utility>
#include "loquat/graph/edge.hpp"
#include "loquat/graph/adjacency_list.hpp"
#include "loquat/graph/breadth_first_search.hpp"
#include "loquat/graph/biconnected_components.hpp"
#include "random_graph_generator.hpp"

TEST(BiconnectedComponentsTest, Random){
	using loquat::vertex_t;
	using edge = loquat::edge<>;
	std::default_random_engine engine;
	for(const size_t n : { 4, 10, 30, 50 }){
		auto graph =
			loquat::test::random_graph_generator<edge>(n, 2.0 / n)
				.has_self_loop(true)
				.generate(engine);
		std::vector<std::pair<vertex_t, edge>> edges;
		for(vertex_t u = 0; u < n; ++u){
			for(const auto& e : graph[u]){ edges.emplace_back(u, e); }
		}
		std::vector<std::vector<bool>> matrix(n, std::vector<bool>(n));
		for(size_t i = 0; i < edges.size(); ++i){
			loquat::adjacency_list<edge> g(n);
			for(size_t j = 0; j < edges.size(); ++j){
				if(j == i){ continue; }
				g.add_edge(edges[j].first, edges[j].second);
			}
			g = loquat::test::make_undirected(g);
			for(vertex_t u = 0; u < n; ++u){
				std::vector<bool> vis(n);
				vis[u] = true;
				loquat::breadth_first_search(
					g, u, [&vis](vertex_t, const edge& e){ vis[e.to] = true; });
				for(vertex_t v = 0; v < n; ++v){
					if(!vis[v]){ matrix[u][v] = true; }
				}
			}
		}
		graph = loquat::test::make_undirected(graph);
		const auto bcc = loquat::biconnected_components(graph);
		for(vertex_t u = 0; u < n; ++u){
			for(vertex_t v = 0; v < n; ++v){
				if(bcc[u] == bcc[v]){
					EXPECT_FALSE(matrix[u][v]);
				}else{
					EXPECT_TRUE(matrix[u][v]);
				}
			}
		}
	}
}

