#include <gtest/gtest.h>
#include "loquat/graph/edge.hpp"
#include "loquat/graph/residual_network.hpp"
#include "random_graph_generator.hpp"

TEST(ResidualNetworkTest, MakeResidual){
	using edge = loquat::edge<loquat::edge_param::capacity<int>>;
	std::default_random_engine engine;
	for(const size_t n : { 2, 10, 50, 100 }){
		auto graph =
			loquat::test::random_graph_generator<edge>(n, 0.05)
				.has_self_loop(true)
				.generate(engine);
		loquat::test::randomize_capacities(
			graph, engine, std::uniform_int_distribution<int>(0, 100));
		const auto residual = loquat::make_residual(graph);
		std::vector<std::vector<bool>> checked(n);
		for(size_t i = 0; i < n; ++i){
			EXPECT_GE(residual[i].size(), graph[i].size());
			checked[i].assign(residual[i].size(), false);
		}
		for(size_t u = 0; u < n; ++u){
			const auto m = graph[u].size();
			for(size_t i = 0; i < m; ++i){
				const auto& orig = graph[u][i];
				const auto& e = residual[u][i];
				EXPECT_EQ(e.to, orig.to);
				EXPECT_EQ(e.capacity, orig.capacity);
				const auto v = e.to;
				const auto j = e.rev;
				EXPECT_FALSE(checked[u][i]);
				EXPECT_FALSE(checked[v][j]);
				const auto& r = residual[v][j];
				EXPECT_EQ(r.to, u);
				EXPECT_EQ(r.rev, i);
				EXPECT_EQ(r.capacity, 0);
				checked[u][i] = checked[v][j] = true;
			}
		}
		for(const auto& v : checked){
			for(const auto x : v){ EXPECT_TRUE(x); }
		}
	}
}

TEST(ResidualNetworkTest, MakeResidualWithWeight){
	using edge = loquat::edge<
		loquat::edge_param::capacity<int>,
		loquat::edge_param::weight<int>>;
	std::default_random_engine engine;
	for(const size_t n : { 2, 10, 50, 100 }){
		auto graph =
			loquat::test::random_graph_generator<edge>(n, 0.05)
				.has_self_loop(true)
				.generate(engine);
		loquat::test::randomize_weights(
			graph, engine, std::uniform_int_distribution<int>(0, 100));
		loquat::test::randomize_capacities(
			graph, engine, std::uniform_int_distribution<int>(0, 100));
		const auto residual = loquat::make_residual(graph);
		std::vector<std::vector<bool>> checked(n);
		for(size_t i = 0; i < n; ++i){
			EXPECT_GE(residual[i].size(), graph[i].size());
			checked[i].assign(residual[i].size(), false);
		}
		for(size_t u = 0; u < n; ++u){
			const auto m = graph[u].size();
			for(size_t i = 0; i < m; ++i){
				const auto& orig = graph[u][i];
				const auto& e = residual[u][i];
				EXPECT_EQ(e.to, orig.to);
				EXPECT_EQ(e.capacity, orig.capacity);
				EXPECT_EQ(e.weight, orig.weight);
				const auto v = e.to;
				const auto j = e.rev;
				EXPECT_FALSE(checked[u][i]);
				EXPECT_FALSE(checked[v][j]);
				const auto& r = residual[v][j];
				EXPECT_EQ(r.to, u);
				EXPECT_EQ(r.rev, i);
				EXPECT_EQ(r.capacity, 0);
				EXPECT_EQ(r.weight, -e.weight);
				checked[u][i] = checked[v][j] = true;
			}
		}
		for(const auto& v : checked){
			for(const auto x : v){ EXPECT_TRUE(x); }
		}
	}
}

