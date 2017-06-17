#include <gtest/gtest.h>
#include "loquat/graph/edge.hpp"
#include "loquat/graph/sssp_bellman_ford.hpp"
#include "random_graph_generator.hpp"
#include "sssp_validator.hpp"

TEST(SSSPBellmanFordTest, NoNegativeEdges){
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
		const auto actual = loquat::sssp_bellman_ford(source, graph);
		EXPECT_TRUE(loquat::test::validate_sssp_result(actual, source, graph));
	}
}

TEST(SSSPBellmanFordTest, NoNegativeCycles){
	using edge = loquat::edge<loquat::edge_param::weight<int>>;
	std::default_random_engine engine;
	for(const size_t n : { 2, 10, 50, 100, 500 }){
		auto graph =
			loquat::test::random_graph_generator<edge>(n, 0.05)
				.has_self_loop(true)
				.generate(engine);
		loquat::test::randomize_weights(
			graph, engine, std::uniform_int_distribution<int>(2, 100));
		std::uniform_int_distribution<loquat::vertex_t> vertex_dist(0, n - 1);
		loquat::vertex_t u = vertex_dist(engine), v = u;
		while(v == u){ v = vertex_dist(engine); }
		graph.add_edge(u, v, -1);
		const loquat::vertex_t source = 1;
		const auto actual = loquat::sssp_bellman_ford(source, graph);
		EXPECT_TRUE(loquat::test::validate_sssp_result(actual, source, graph));
	}
}

TEST(SSSPBellmanFordTest, HasNegativeCycles){
	using edge = loquat::edge<loquat::edge_param::weight<int>>;
	std::default_random_engine engine;
	for(const size_t n : { 10, 50, 100, 500 }){
		auto graph =
			loquat::test::random_graph_generator<edge>(n, 0.04)
				.has_self_loop(true)
				.generate(engine);
		loquat::test::randomize_weights(
			graph, engine, std::uniform_int_distribution<int>(20, 100));
		const size_t m = std::min<size_t>(n / 2, 20);
		std::vector<loquat::vertex_t> cycle(n);
		std::iota(cycle.begin(), cycle.end(), 0);
		std::shuffle(cycle.begin(), cycle.end(), engine);
		cycle.resize(m);
		std::uniform_int_distribution<int> weight_dist(-2, 2);
		std::vector<int> weights(m);
		for(size_t i = 0; i < m; ++i){ weights[i] = weight_dist(engine); }
		if(std::accumulate(weights.begin(), weights.end(), 0) >= 0){
			for(auto& w : weights){ w *= -1; }
			--weights[0];
		}
		for(size_t i = 0; i < m; ++i){
			graph.add_edge(cycle[i], cycle[(i + 1) % m], weights[i]);
		}
		const loquat::vertex_t source = 1;
		graph.add_edge(source, cycle[0], 100);
		EXPECT_THROW(
			loquat::sssp_bellman_ford(source, graph),
			loquat::no_solution_error);
	}
}

