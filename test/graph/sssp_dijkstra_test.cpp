#include <gtest/gtest.h>
#include "loquat/graph/edge.hpp"
#include "loquat/graph/sssp_dijkstra.hpp"
#include "random_graph_generator.hpp"
#include "sssp_validator.hpp"

TEST(SSSPDijkstraTest, IntegerWeight){
	using edge = loquat::edge<loquat::edge_param::weight<int>>;
	std::default_random_engine engine;
	for(const size_t n : { 2, 10, 50, 100, 500 }){
		auto graph =
			loquat::test::random_graph_generator<edge>(n, 0.05)
				.has_self_loop(true)
				.generate(engine);
		loquat::test::randomize_weights(
			graph, engine, std::uniform_int_distribution<int>(0, 100));
		const loquat::vertex_t source = 1;
		const auto actual = loquat::sssp_dijkstra(source, graph);
		EXPECT_TRUE(loquat::test::validate_sssp_result(actual, source, graph));
	}
}

TEST(SSSPDijkstraTest, FloatingWeight){
	using edge = loquat::edge<loquat::edge_param::weight<double>>;
	std::default_random_engine engine;
	for(const size_t n : { 2, 10, 50, 100, 500 }){
		auto graph =
			loquat::test::random_graph_generator<edge>(n, 0.05)
				.has_self_loop(true)
				.generate(engine);
		loquat::test::randomize_weights(
			graph, engine, std::uniform_real_distribution<double>(0, 1.0));
		const loquat::vertex_t source = 1;
		const auto actual = loquat::sssp_dijkstra(source, graph);
		EXPECT_TRUE(loquat::test::validate_sssp_result(actual, source, graph));
	}
}

