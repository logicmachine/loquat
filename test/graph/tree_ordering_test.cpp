#include <gtest/gtest.h>
#include "loquat/graph/edge.hpp"
#include "loquat/graph/euler_tour_technique.hpp"
#include "loquat/graph/tree_ordering.hpp"
#include "random_graph_generator.hpp"

namespace {

template <typename T>
inline int compare(T a, T b){
	if(a < b){ return -1; }
	if(a > b){ return  1; }
	return 0;
}

}

TEST(TreePreOrderingTest, RandomTree){
	using edge = loquat::edge<>;
	std::default_random_engine engine;
	for(const size_t n : { 2, 10, 50, 100 }){
		const auto graph =
			loquat::test::random_tree_generator<edge>(n)
				.generate(engine);
		const loquat::vertex_t root = n - 1;
		const auto actual = loquat::tree_pre_ordering(root, graph);
		const auto ett = loquat::euler_tour_technique(root, graph);
		for(loquat::vertex_t u = 0; u < n; ++u){
			EXPECT_LE(0u, actual[u]);
			EXPECT_LT(actual[u], n);
			for(loquat::vertex_t v = 0; v < n; ++v){
				EXPECT_EQ(
					compare(actual[u], actual[v]),
					compare(ett[u].in, ett[v].in));
			}
		}
	}
}

TEST(TreePostOrderingTest, RandomTree){
	using edge = loquat::edge<>;
	std::default_random_engine engine;
	for(const size_t n : { 2, 10, 50, 100 }){
		const auto graph =
			loquat::test::random_tree_generator<edge>(n)
				.generate(engine);
		const loquat::vertex_t root = n - 1;
		const auto actual = loquat::tree_post_ordering(root, graph);
		const auto ett = loquat::euler_tour_technique(root, graph);
		for(loquat::vertex_t u = 0; u < n; ++u){
			EXPECT_LE(0u, actual[u]);
			EXPECT_LT(actual[u], n);
			for(loquat::vertex_t v = 0; v < n; ++v){
				EXPECT_EQ(
					compare(actual[u], actual[v]),
					compare(ett[u].out, ett[v].out));
			}
		}
	}
}
