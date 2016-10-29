#include <gtest/gtest.h>
#include "loquat/misc/binary_search.hpp"

TEST(BinarySearchTest, Integer){
	for(int l = -16; l <= 16; ++l){
		for(int r = l; r <= 16; ++r){
			for(int expect = l; expect <= r; ++expect){
				const auto actual = loquat::binary_search(
					l, r, [=](int x){ return x < expect; });
				EXPECT_EQ(expect, actual);
			}
		}
	}
}

TEST(BinarySearchTest, Floating){
	for(int li = -16; li <= 16; ++li){
		for(int ri = li; ri <= li; ++ri){
			const double l = li * 1.1, r = ri * 1.1;
			for(double expect = l; expect < r; expect += 0.2){
				const auto actual = loquat::binary_search(
					l, r, [=](double x){ return x < expect; });
				EXPECT_DOUBLE_EQ(expect, actual);
			}
			const auto min_actual = loquat::binary_search(
				l, r, [=](double){ return true; });
			EXPECT_DOUBLE_EQ(l, min_actual);
			const auto max_actual = loquat::binary_search(
				l, r, [=](double){ return false; });
			EXPECT_DOUBLE_EQ(r, max_actual);
		}
	}
}

