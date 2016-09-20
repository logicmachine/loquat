#include <gtest/gtest.h>
#include <cstdint>
#include "loquat/container/segment_tree.hpp"

TEST(SegmentTreeTest, QueryAndUpdate){
	const size_t n = 64;
	loquat::segment_tree<uint64_t, std::bit_and<uint64_t>> st(n);
	for(size_t i = 0; i < n; ++i){ st.update(i, ~(1ull << i)); }
	for(size_t l = 0; l < n; ++l){
		for(size_t r = l + 1; r <= n; ++r){
			uint64_t expect = ~0ull;
			for(size_t k = l; k < r; ++k){ expect &= ~(1ull << k); }
			EXPECT_EQ(st.query(l, r), expect);
		}
	}
}

