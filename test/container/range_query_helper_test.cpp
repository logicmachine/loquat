#include <gtest/gtest.h>
#include <algorithm>
#include "loquat/container/range_query_helper.hpp"
#include "loquat/utility/functional.hpp"

TEST(RangeQueryHelperTest, ImplicitIdentity){
	const auto behavior =
		loquat::make_range_query_behavior<int>(std::multiplies<int>());
	EXPECT_EQ(1, behavior.identity());
	EXPECT_EQ(6, behavior.merge(2, 3));
}

TEST(RangeQueryHelperTest, ExplicitIdentity){
	const auto behavior =
		loquat::make_range_query_behavior(loquat::min<int>(), 100);
	EXPECT_EQ(100, behavior.identity());
	EXPECT_EQ(  5, behavior.merge(5, 20));
}

