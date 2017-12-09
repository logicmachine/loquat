#include <gtest/gtest.h>
#include "loquat/misc/sliding_minimum_query.hpp"

TEST(SlidingMinimumQueryTest, Normal){
	loquat::sliding_minimum_query<int> smq;
	EXPECT_TRUE(smq.empty());
	smq.push(10); // { 10 }
	EXPECT_EQ(smq.query(), 10);
	EXPECT_FALSE(smq.empty());
	smq.push(3);  // { 10, 3 }
	EXPECT_EQ(smq.query(), 3);
	EXPECT_FALSE(smq.empty());
	smq.push(-1); // { 10, 3, -1 }
	EXPECT_EQ(smq.query(), -1);
	EXPECT_FALSE(smq.empty());
	smq.pop();    // { 3, -1 }
	EXPECT_EQ(smq.query(), -1);
	EXPECT_FALSE(smq.empty());
	smq.push(4);  // { 3, -1, 4 }
	EXPECT_EQ(smq.query(), -1);
	EXPECT_FALSE(smq.empty());
	smq.push(-2); // { 3, -1, 4, -2 }
	EXPECT_EQ(smq.query(), -2);
	EXPECT_FALSE(smq.empty());
	smq.pop();    // { -1, 4, -2 }
	EXPECT_EQ(smq.query(), -2);
	EXPECT_FALSE(smq.empty());
	smq.push(2);  // { -1, 4, -2, 2 }
	EXPECT_EQ(smq.query(), -2);
	EXPECT_FALSE(smq.empty());
	smq.push(-1); // { -1, 4, -2, 2, -1 }
	EXPECT_EQ(smq.query(), -2);
	EXPECT_FALSE(smq.empty());
	smq.pop();    // { 4, -2, 2, -1 }
	EXPECT_EQ(smq.query(), -2);
	EXPECT_FALSE(smq.empty());
	smq.pop();    // { -2, 2, -1 }
	EXPECT_EQ(smq.query(), -2);
	EXPECT_FALSE(smq.empty());
	smq.pop();    // { 2, -1 }
	EXPECT_EQ(smq.query(), -1);
	EXPECT_FALSE(smq.empty());
	smq.pop();    // { -1 }
	EXPECT_EQ(smq.query(), -1);
	EXPECT_FALSE(smq.empty());
	smq.pop();    // { }
	EXPECT_TRUE(smq.empty());
	smq.push(8); // { 8 }
	EXPECT_EQ(smq.query(), 8);
	EXPECT_FALSE(smq.empty());
}
