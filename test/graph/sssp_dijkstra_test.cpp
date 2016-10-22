#include <gtest/gtest.h>
#include "loquat/graph/edge.hpp"
#include "loquat/graph/sssp_dijkstra.hpp"
#include "random_graph_generator.hpp"
#include "sssp_validator.hpp"

TEST(SSSPDijkstraTest, IntegerWeight){
	using edge = loquat::edge<loquat::edge_param::weight<int>>;
	for(const size_t n : { 2, 10, 50, 100, 500 }){
		std::default_random_engine engine;
		auto graph =
			loquat::test::random_graph_generator<edge>(n, 0.05)
				.has_self_loop(true)
				.generate(engine);
		loquat::test::randomize_weights(
			graph, engine, std::uniform_int_distribution<int>(0, 100));
		const size_t source = 1;
		const auto actual = loquat::sssp_dijkstra(source, graph);
		EXPECT_TRUE(loquat::test::validate_sssp_result(actual, source, graph));
	}
}

TEST(SSSPDijkstraTest, FloatingWeight){
	using edge = loquat::edge<loquat::edge_param::weight<double>>;
	for(const size_t n : { 2, 10, 50, 100, 500 }){
		std::default_random_engine engine;
		auto graph =
			loquat::test::random_graph_generator<edge>(n, 0.05)
				.has_self_loop(true)
				.generate(engine);
		loquat::test::randomize_weights(
			graph, engine, std::uniform_real_distribution<double>(0, 1.0));
		const size_t source = 1;
		const auto actual = loquat::sssp_dijkstra(source, graph);
		EXPECT_TRUE(loquat::test::validate_sssp_result(actual, source, graph));
	}
}

