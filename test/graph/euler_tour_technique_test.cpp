#include <gtest/gtest.h>
#include <unordered_set>
#include "loquat/graph/edge.hpp"
#include "loquat/graph/euler_tour_technique.hpp"
#include "random_graph_generator.hpp"

namespace {

enum class relationship {
	none,
	ancestor,
	descendant
};

template <typename EdgeType>
static void build_relationships(
	std::vector<std::vector<relationship>>& result,
	std::unordered_set<loquat::vertex_t>& ancestors,
	loquat::vertex_t u,
	loquat::vertex_t p,
	const loquat::adjacency_list<EdgeType>& g)
{
	for(const auto& a : ancestors){
		result[u][a] = relationship::ancestor;
		result[a][u] = relationship::descendant;
	}
	ancestors.insert(u);
	for(const auto& e : g[u]){
		if(e.to == p){ continue; }
		build_relationships(result, ancestors, e.to, u, g);
	}
	ancestors.erase(u);
}

}

TEST(EulerTourTechniqueTest, RandomTree){
	using edge = loquat::edge<>;
	std::default_random_engine engine;
	for(const size_t n : { 2, 10, 50, 100 }){
		const auto graph =
			loquat::test::random_tree_generator<edge>(n)
				.generate(engine);
		const loquat::vertex_t root = n - 1;
		const auto actual = loquat::euler_tour_technique(root, graph);
		std::vector<std::vector<relationship>> relationships(
			n, std::vector<relationship>(n, relationship::none));
		std::unordered_set<loquat::vertex_t> ancestors;
		build_relationships(
			relationships, ancestors, root, root, graph);
		std::unordered_set<size_t> done;
		for(loquat::vertex_t u = 0; u < n; ++u){
			EXPECT_LE(0u, actual[u].in);
			EXPECT_LT(actual[u].in, actual[u].out);
			EXPECT_LT(actual[u].out, 2 * n);
			EXPECT_EQ(done.end(), done.find(actual[u].in));
			done.insert(actual[u].in);
			EXPECT_EQ(done.end(), done.find(actual[u].out));
			done.insert(actual[u].out);
			for(loquat::vertex_t v = 0; v < n; ++v){
				if(u == v){ continue; }
				const auto rs = relationships[u][v];
				if(rs == relationship::ancestor){
					EXPECT_LT(actual[v].in, actual[u].in);
					EXPECT_LT(actual[u].out, actual[v].out);
				}else if(rs == relationship::descendant){
					EXPECT_LT(actual[u].in, actual[v].in);
					EXPECT_LT(actual[v].out, actual[u].out);
				}else{
					EXPECT_TRUE(
						actual[v].out < actual[u].in ||
						actual[u].out < actual[v].in);
				}
			}
		}
	}
}

