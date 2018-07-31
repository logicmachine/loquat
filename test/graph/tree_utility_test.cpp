#include <gtest/gtest.h>
#include "loquat/graph/edge.hpp"
#include "loquat/graph/tree_utility.hpp"
#include "random_graph_generator.hpp"

namespace {

template <typename EdgeType>
std::pair<bool, size_t> verify_compute_subtree_sizes(
	loquat::vertex_t u,
	loquat::vertex_t p,
	const loquat::adjacency_list<EdgeType>& graph,
	const std::vector<size_t>& actual)
{
	bool status = true;
	size_t expect = 1;
	for(const auto& e : graph[u]){
		if(e.to == p){ continue; }
		const auto t = verify_compute_subtree_sizes(e.to, u, graph, actual);
		expect += t.second;
		if(!t.first){ status = false; }
	}
	if(expect != actual[u]){ status = false; }
	return std::make_pair(status, expect);
}

}

TEST(TreeUtilityTest, ComputeSubtreeSizesRandom){
	using edge = loquat::edge<>;
	std::default_random_engine engine;
	for(const size_t n : { 2, 10, 50, 100 }){
		const auto graph =
			loquat::test::random_tree_generator<edge>(n)
				.generate(engine);
		const auto actual = loquat::compute_subtree_sizes(graph, 0);
		EXPECT_TRUE(verify_compute_subtree_sizes(0, -1, graph, actual).first);
	}
}

