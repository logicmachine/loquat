#include <gtest/gtest.h>
#include "loquat/graph/edge.hpp"
#include "loquat/graph/breadth_first_search.hpp"
#include "random_graph_generator.hpp"

TEST(BreadthFirstSearchTest, RandomGraph){
	using edge = loquat::edge<>;
	std::default_random_engine engine;
	for(const size_t n : { 2, 10, 50, 100 }){
		const auto graph =
			loquat::test::random_graph_generator<edge>(n, 0.05)
				.has_self_loop(true)
				.generate(engine);

		const auto inf = std::numeric_limits<size_t>::max();
		const loquat::vertex_t root = 0;
		std::vector<size_t> d(n, inf);
		d[root] = 0;
		loquat::breadth_first_search(
			graph, root, [&d](loquat::vertex_t u, const edge& e){
				d[e.to] = d[u] + 1;
			});

		loquat::adjacency_list<edge> inv_graph(n);
		for(loquat::vertex_t u = 0; u < n; ++u){
			for(const auto& e : graph[u]){ inv_graph.add_edge(e.to, u); }
		}
		EXPECT_EQ(0u, d[root]);
		for(loquat::vertex_t u = 0; u < n; ++u){
			for(const auto& e : graph[u]){
				if(d[u] != inf){ EXPECT_LE(d[e.to], d[u] + 1); }
			}
		}
		for(loquat::vertex_t u = 0; u < n; ++u){
			size_t min_pred = inf;
			for(const auto& e : inv_graph[u]){
				min_pred = std::min(min_pred, d[e.to]);
			}
			if(u != root){
				if(d[u] == inf){
					EXPECT_EQ(inf, min_pred);
				}else{
					EXPECT_EQ(d[u] - 1, min_pred);
				}
			}
		}
	}
}

