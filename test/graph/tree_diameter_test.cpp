#include <gtest/gtest.h>
#include <queue>
#include <algorithm>
#include "loquat/graph/edge.hpp"
#include "loquat/graph/adjacency_list.hpp"
#include "loquat/graph/tree_diameter.hpp"
#include "loquat/math/infinity.hpp"
#include "random_graph_generator.hpp"

TEST(TreeDiameterTest, Random){
	using edge = loquat::edge<>;
	std::default_random_engine engine;
	for(const size_t n : { 2, 10, 50, 100 }){
		auto graph =
			loquat::test::random_tree_generator<edge>(n)
				.generate(engine);
		size_t expect = 0;
		for(loquat::vertex_t r = 0; r < n; ++ r){
			const auto unreachable = loquat::positive_infinity<size_t>();
			std::vector<size_t> d(n, unreachable);
			d[r] = 0;
			std::queue<loquat::vertex_t> q;
			q.push(r);
			while(!q.empty()){
				const auto u = q.front();
				q.pop();
				for(const auto& e : graph[u]){
					const auto v = e.to;
					if(d[v] == unreachable){
						d[v] = d[u] + 1;
						q.push(v);
					}
				}
			}
			expect = std::max(
				expect, *std::max_element(d.begin(), d.end()));
		}
		const auto actual = loquat::tree_diameter(graph);
		EXPECT_EQ(expect, actual);
	}
}

TEST(TreeDiameterTest, WeightedRandom){
	using edge = loquat::edge<loquat::edge_param::weight<double>>;
	std::default_random_engine engine;
	for(const size_t n : { 2, 10, 50, 100 }){
		auto graph =
			loquat::test::random_tree_generator<edge>(n)
				.generate(engine);
		loquat::test::randomize_weights(
			graph, engine, std::uniform_real_distribution<double>(0.0, 1.0));
		double expect = 0;
		for(loquat::vertex_t r = 0; r < n; ++ r){
			const auto inf = loquat::positive_infinity<double>();
			std::vector<double> d(n, inf);
			d[r] = 0;
			std::queue<loquat::vertex_t> q;
			q.push(r);
			while(!q.empty()){
				const auto u = q.front();
				q.pop();
				for(const auto& e : graph[u]){
					const auto v = e.to;
					if(loquat::is_positive_infinity(d[v])){
						d[v] = d[u] + e.weight;
						q.push(v);
					}
				}
			}
			expect = std::max(
				expect, *std::max_element(d.begin(), d.end()));
		}
		const auto actual = loquat::tree_diameter(graph);
		EXPECT_DOUBLE_EQ(expect, actual);
	}
}
