#include <gtest/gtest.h>
#include "loquat/graph/edge.hpp"
#include "loquat/graph/residual_network.hpp"
#include "loquat/graph/maxflow_dinitz.hpp"
#include "random_graph_generator.hpp"
#include "maxflow_validator.hpp"

TEST(MaxflowDinitzTest, RandomFlow){
	using edge = loquat::edge<loquat::edge_param::capacity<int>>;
	std::default_random_engine engine;
	for(const size_t n : { 2, 10, 50, 100 }){
		auto graph =
			loquat::test::random_graph_generator<edge>(n, 0.1)
				.has_self_loop(true)
				.generate(engine);
		loquat::test::randomize_capacities(
			graph, engine, std::uniform_int_distribution<int>(0, 100));
		auto in_residual = loquat::make_residual(graph);
		auto out_residual = in_residual;
		const loquat::vertex_t source = 0, sink = 1;
		const auto actual =
			loquat::maxflow_dinitz(source, sink, out_residual);
		EXPECT_TRUE(loquat::test::validate_maxflow_result(
			actual, out_residual, source, sink, in_residual));
	}
}

