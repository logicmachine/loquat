#include <gtest/gtest.h>
#include <queue>
#include "loquat/graph/strongly_connected_components.hpp"
#include "random_graph_generator.hpp"

TEST(StronglyConnectedComponentsTest, Random){
	using edge = loquat::edge<>;
	std::default_random_engine engine;
	for(const size_t n : { 2, 10, 50, 60, 75, 80, 90, 100 }){
		auto graph =
			loquat::test::random_graph_generator<edge>(n, 2.0 / n)
				.has_self_loop(true)
				.generate(engine);
		std::vector<std::vector<bool>> matrix(n, std::vector<bool>(n));
		for(loquat::vertex_t r = 0; r < n; ++r){
			std::queue<loquat::vertex_t> q;
			q.push(r);
			matrix[r][r] = true;
			while(!q.empty()){
				const auto u = q.front();
				q.pop();
				for(const auto& e : graph[u]){
					if(matrix[r][e.to]){ continue; }
					matrix[r][e.to] = true;
					q.push(e.to);
				}
			}
		}
		const auto scc = loquat::strongly_connected_components(graph);
		for(loquat::vertex_t u = 0; u < n; ++u){
			EXPECT_EQ(scc[scc[u]], scc[u]);
			for(loquat::vertex_t v = 0; v < n; ++v){
				if(scc[u] == scc[v]){
					EXPECT_TRUE(matrix[u][v] && matrix[v][u]);
				}else{
					EXPECT_FALSE(matrix[u][v] && matrix[v][u]);
				}
			}
		}
	}
}

