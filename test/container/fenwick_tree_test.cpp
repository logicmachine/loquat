#include <gtest/gtest.h>
#include <functional>
#include <cstdint>
#include "loquat/container/fenwick_tree.hpp"

TEST(FenwickTreeTest, DefaultConstructor){
	loquat::fenwick_tree<int> ft;
	EXPECT_EQ(0u, ft.size());
}

TEST(FenwickTreeTest, ConstructWithSizeAndDefault){
	const size_t n = 31;
	loquat::fenwick_tree<int> ft(n, 1);
	for(size_t i = 0; i <= n; ++i){
		EXPECT_EQ(static_cast<int>(i), ft.query(i));
	}
}

TEST(FenwickTreeTest, QueryAndModify){
	const size_t n = 64;
	loquat::fenwick_tree<uint64_t, std::bit_and<uint64_t>> ft(n, ~0ull);
	for(size_t i = 0; i < n; ++i){
		ft.modify(i, ~(1ull << i));
	}
	EXPECT_EQ(uint64_t(), ft.query(0));
	for(size_t i = 1; i < n; ++i){
		EXPECT_EQ(~((1ull << (i & 63u)) - 1ull), ft.query(i));
	}
}

