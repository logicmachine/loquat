#include <gtest/gtest.h>
#include "loquat/graph/edge.hpp"
#include "loquat/graph/adjacency_list.hpp"
#include "loquat/graph/algorithms.hpp"
#include "random_graph_generator.hpp"

TEST(GraphAlgorithmsTest, Filter){
	using edge = loquat::edge<loquat::edge_param::weight<int>>;
	const size_t n = 50;
	std::default_random_engine engine;
	auto graph =
		loquat::test::random_graph_generator<edge>(n, 0.1)
			.has_self_loop(true)
			.generate(engine);
	loquat::test::randomize_weights(
		graph, engine, std::uniform_int_distribution<int>(-100, 100));
	const auto actual = loquat::filter(
		graph, [](loquat::vertex_t, const edge& e){ return e.weight >= 0; });
	EXPECT_EQ(actual.size(), graph.size());
	std::vector<std::vector<int>> expect_mat(n, std::vector<int>(n));
	std::vector<std::vector<int>> actual_mat(n, std::vector<int>(n));
	for(loquat::vertex_t u = 0; u < graph.size(); ++u){
		for(const auto& e : graph[u]){
			if(e.weight >= 0){ expect_mat[u][e.to] = e.weight; }
		}
		for(const auto& e : actual[u]){
			actual_mat[u][e.to] = e.weight;
		}
	}
	EXPECT_EQ(actual_mat, expect_mat);
}

