#include <gtest/gtest.h>
#include "loquat/math/infinity.hpp"

TEST(InfinityTest, IntegralType){
	const auto maxval = std::numeric_limits<int>::max();
	const auto minval = std::numeric_limits<int>::min();
	EXPECT_TRUE (loquat::is_positive_infinity(maxval));
	EXPECT_FALSE(loquat::is_positive_infinity(maxval - 1));
	EXPECT_FALSE(loquat::is_positive_infinity(minval));
	EXPECT_FALSE(loquat::is_negative_infinity(maxval));
	EXPECT_TRUE (loquat::is_negative_infinity(minval));
	EXPECT_FALSE(loquat::is_negative_infinity(minval + 1));
	EXPECT_TRUE(
		loquat::is_positive_infinity(loquat::positive_infinity<int>()));
	EXPECT_FALSE(
		loquat::is_positive_infinity(loquat::negative_infinity<int>()));
	EXPECT_FALSE(
		loquat::is_negative_infinity(loquat::positive_infinity<int>()));
	EXPECT_TRUE(
		loquat::is_negative_infinity(loquat::negative_infinity<int>()));
}

TEST(InfinityTest, FloatingPointType){
	const auto maxval = std::numeric_limits<double>::max();
	const auto minval = std::numeric_limits<double>::min();
	EXPECT_FALSE(loquat::is_positive_infinity(maxval));
	EXPECT_FALSE(loquat::is_positive_infinity(minval));
	EXPECT_FALSE(loquat::is_negative_infinity(maxval));
	EXPECT_FALSE(loquat::is_negative_infinity(minval));
	EXPECT_TRUE(
		loquat::is_positive_infinity(loquat::positive_infinity<double>()));
	EXPECT_FALSE(
		loquat::is_positive_infinity(loquat::negative_infinity<double>()));
	EXPECT_FALSE(
		loquat::is_negative_infinity(loquat::positive_infinity<double>()));
	EXPECT_TRUE(
		loquat::is_negative_infinity(loquat::negative_infinity<double>()));
}

