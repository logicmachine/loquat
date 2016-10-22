#include <gtest/gtest.h>
#include "loquat/graph/edge.hpp"
#include "loquat/graph/adjacency_list.hpp"

using edge = loquat::edge<loquat::edge_param::weight<int>>;

TEST(AdjacencyListTest, DefaultConstruct){
	loquat::adjacency_list<edge> g;
	EXPECT_EQ(0u, g.size());
}

TEST(AdjacencyListTest, AddEdge){
	loquat::adjacency_list<edge> g(3);
	EXPECT_EQ(3u, g.size());
	g.add_edge(0, 1, 10);
	g.add_edge(1, 0, 11);
	g.add_edge(1, edge(2, 12));
	EXPECT_EQ(1u, g[0].size());
	EXPECT_EQ(1u, g[0][0].to);
	EXPECT_EQ(10, g[0][0].weight);
	EXPECT_EQ(2u, g[1].size());
	EXPECT_EQ(0u, g[1][0].to);
	EXPECT_EQ(11, g[1][0].weight);
	EXPECT_EQ(2u, g[1][1].to);
	EXPECT_EQ(12, g[1][1].weight);
}

