#include <gtest/gtest.h>
#include <cmath>
#include "loquat/math/identity.hpp"

TEST(IdentityTest, Plus){
	EXPECT_EQ(0,   loquat::identity<std::plus<int>>::value());
	EXPECT_EQ(0u,  loquat::identity<std::plus<unsigned long long>>::value());
	EXPECT_EQ(0.0, loquat::identity<std::plus<double>>::value());
	EXPECT_EQ("",  loquat::identity<std::plus<std::string>>::value());
}

TEST(IdentityTest, Multiplies){
	EXPECT_EQ(1,   loquat::identity<std::multiplies<long>>::value());
	EXPECT_EQ(1u,  loquat::identity<std::multiplies<unsigned int>>::value());
	EXPECT_EQ(1.0, loquat::identity<std::multiplies<float>>::value());
}

TEST(IdentityTest, Bitwise){
	EXPECT_EQ(~0ll,  loquat::identity<std::bit_and<long long>>::value());
	EXPECT_EQ(0xffu, loquat::identity<std::bit_and<unsigned char>>::value());
	EXPECT_EQ(0u,    loquat::identity<std::bit_or<unsigned int>>::value());
	EXPECT_EQ(0u,    loquat::identity<std::bit_xor<unsigned int>>::value());
}

TEST(IdentityTest, MinMax){
	EXPECT_EQ(
		std::numeric_limits<int>::max(),
		loquat::identity<loquat::min<int>>::value());
	EXPECT_EQ(
		std::numeric_limits<unsigned long long>::max(),
		loquat::identity<loquat::min<unsigned long long>>::value());
	EXPECT_TRUE(
		std::isinf(loquat::identity<loquat::min<double>>::value()) &&
		loquat::identity<loquat::min<double>>::value() > 0.0);
	EXPECT_EQ(
		std::numeric_limits<unsigned int>::min(),
		loquat::identity<loquat::max<unsigned int>>::value());
	EXPECT_EQ(
		std::numeric_limits<long long>::min(),
		loquat::identity<loquat::max<long long>>::value());
	EXPECT_TRUE(
		std::isinf(loquat::identity<loquat::max<float>>::value()) &&
		loquat::identity<loquat::max<float>>::value() < 0.0f);
}

