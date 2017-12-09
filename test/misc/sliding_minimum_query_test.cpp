#include <gtest/gtest.h>
#include "loquat/misc/sliding_minimum_query.hpp"

TEST(SlidingMinimumQueryTest, Normal){
	loquat::sliding_minimum_query<int> smq;
	smq.push(10); // { 10 }
	EXPECT_EQ(smq.query(), 10);
	smq.push(3);  // { 10, 3 }
	EXPECT_EQ(smq.query(), 3);
	smq.push(-1); // { 10, 3, -1 }
	EXPECT_EQ(smq.query(), -1);
	smq.pop();    // { 3, -1 }
	EXPECT_EQ(smq.query(), -1);
	smq.push(4);  // { 3, -1, 4 }
	EXPECT_EQ(smq.query(), -1);
	smq.push(-2); // { 3, -1, 4, -2 }
	EXPECT_EQ(smq.query(), -2);
	smq.pop();    // { -1, 4, -2 }
	EXPECT_EQ(smq.query(), -2);
	smq.push(2);  // { -1, 4, -2, 2 }
	EXPECT_EQ(smq.query(), -2);
	smq.push(-1); // { -1, 4, -2, 2, -1 }
	EXPECT_EQ(smq.query(), -2);
	smq.pop();    // { 4, -2, 2, -1 }
	EXPECT_EQ(smq.query(), -2);
	smq.pop();    // { -2, 2, -1 }
	EXPECT_EQ(smq.query(), -2);
	smq.pop();    // { 2, -1 }
	EXPECT_EQ(smq.query(), -1);
}
