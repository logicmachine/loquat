#include <gtest/gtest.h>
#include "loquat/graph/edge.hpp"

TEST(EdgeTest, Simple){
	using edge = loquat::edge<>;
	edge e(10u);
	EXPECT_EQ(10u, e.to);
}

TEST(EdgeTest, Weight){
	using edge = loquat::edge<loquat::edge_param::weight<int>>;
	edge e(1u, 2);
	EXPECT_EQ(1u, e.to);
	EXPECT_EQ(2, e.weight);
}

TEST(EdgeTest, Capacity){
	using edge = loquat::edge<loquat::edge_param::capacity<double>>;
	edge e(2u, 3.5);
	EXPECT_EQ(2u, e.to);
	EXPECT_EQ(3.5, e.capacity);
}

TEST(EdgeTest, MultipleParameters){
	using edge = loquat::edge<
		loquat::edge_param::weight<int>,
		loquat::edge_param::capacity<int>>;
	edge e(3u, 2, 1);
	EXPECT_EQ(3u, e.to);
	EXPECT_EQ(2, e.weight);
	EXPECT_EQ(1, e.capacity);
}

TEST(EdgeTest, DefaultConstruct){
	using edge = loquat::edge<
		loquat::edge_param::weight<double>,
		loquat::edge_param::capacity<int>>;
	edge e;
	EXPECT_EQ(0u,  e.to);
	EXPECT_EQ(0.0, e.weight);
	EXPECT_EQ(0,   e.capacity);
}

