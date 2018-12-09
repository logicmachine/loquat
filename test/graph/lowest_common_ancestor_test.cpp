#include <gtest/gtest.h>
#include <unordered_set>
#include "loquat/graph/edge.hpp"
#include "loquat/graph/adjacency_list.hpp"
#include "loquat/graph/breadth_first_search.hpp"
#include "loquat/graph/lowest_common_ancestor.hpp"
#include "random_graph_generator.hpp"

TEST(LowestCommonAncestorTest, Random){
	using edge = loquat::edge<>;
	std::default_random_engine engine;
	for(const size_t n : { 2, 10, 50, 100 }){
		const auto graph =
			loquat::test::random_tree_generator<edge>(n)
				.generate(engine);
		const loquat::vertex_t root = 0;
		const loquat::lowest_common_ancestor lca(graph, root);
		std::vector<loquat::vertex_t> parents(n, n);
		loquat::breadth_first_search(
			graph, root,
			[&parents](loquat::vertex_t u, const edge& e){
				parents[e.to] = u;
			});
		for(loquat::vertex_t a = 0; a < n; ++a){
			for(loquat::vertex_t b = 0; b < n; ++b){
				const auto actual = lca.query(a, b);
				std::unordered_set<loquat::vertex_t> a_set;
				loquat::vertex_t a_cur = a;
				while(a_cur != actual){
					ASSERT_NE(a_cur, n);
					a_set.insert(a_cur);
					a_cur = parents[a_cur];
				}
				loquat::vertex_t b_cur = b;
				while(b_cur != actual){
					ASSERT_NE(b_cur, n);
					ASSERT_TRUE(a_set.find(b_cur) == a_set.end());
					b_cur = parents[b_cur];
				}
			}
		}
	}
}
