#include <gtest/gtest.h>
#include "loquat/graph/edge.hpp"
#include "loquat/graph/residual_network.hpp"
#include "loquat/graph/maxflow_dinitz.hpp"
#include "loquat/graph/mincostflow_primal_dual.hpp"
#include "random_graph_generator.hpp"
#include "mincostflow_validator.hpp"

namespace {

template <typename EdgeType>
typename EdgeType::capacity_type
compute_flow_limit(
	loquat::vertex_t source,
	loquat::vertex_t sink,
	loquat::adjacency_list<EdgeType> residual)
{
	return loquat::maxflow_dinitz(source, sink, residual);
}

}

TEST(MincostflowPrimalDualTest, RandomFlow){
	using edge = loquat::edge<
		loquat::edge_param::weight<int>,
		loquat::edge_param::capacity<int>>;
	std::default_random_engine engine;
	for(const size_t n : { 2, 10, 50, 100 }){
		auto graph =
			loquat::test::random_graph_generator<edge>(n, 0.1)
				.has_self_loop(true)
				.generate(engine);
		loquat::test::randomize_weights(
			graph, engine, std::uniform_int_distribution<int>(1, 100));
		loquat::test::randomize_capacities(
			graph, engine, std::uniform_int_distribution<int>(0, 100));
		auto in_residual = loquat::make_residual(graph);
		const loquat::vertex_t source = 0, sink = 1;
		const auto limit = compute_flow_limit(source, sink, in_residual);
		{	// impossible
			auto out_residual = in_residual;
			EXPECT_THROW(
				loquat::mincostflow_primal_dual(
					limit + 1, source, sink, out_residual),
				loquat::no_solution_error);
		}
		{	// possible (1)
			auto out_residual = in_residual;
			const auto actual =
				loquat::mincostflow_primal_dual(
					limit / 2, source, sink, out_residual);
			EXPECT_TRUE(loquat::test::validate_mincostflow_result(
				actual, out_residual, source, sink, limit / 2, in_residual));
		}
		{	// possible (2)
			auto out_residual = in_residual;
			const auto actual =
				loquat::mincostflow_primal_dual(
					limit, source, sink, out_residual);
			EXPECT_TRUE(loquat::test::validate_mincostflow_result(
				actual, out_residual, source, sink, limit, in_residual));
		}
	}
}

