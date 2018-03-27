#include <gtest/gtest.h>
#include "loquat/math/rational.hpp"

using rational = loquat::rational;
using value_type = typename rational::value_type;

static ::testing::AssertionResult check_value(
	const rational& r, value_type nume, value_type deno)
{
	if(r.nume() != nume || r.deno() != deno){
		return ::testing::AssertionFailure()
			<< r << " does not equal to " << nume << "/" << deno;
	}else{
		return ::testing::AssertionSuccess();
	}
}

TEST(RationalTest, Constructors){
	EXPECT_TRUE(check_value(rational(),       0, 1));
	EXPECT_TRUE(check_value(rational(3),      3, 1));
	EXPECT_TRUE(check_value(rational(8, 2),   4, 1));
	EXPECT_TRUE(check_value(rational(-6, 4), -3, 2));
	EXPECT_TRUE(check_value(rational(7, -2), -7, 2));
}

TEST(RationalTest, UnaryOperations){
	EXPECT_TRUE(check_value(+rational(5, 3),  5, 3));
	EXPECT_TRUE(check_value(-rational(5, 3), -5, 3));
}

TEST(RationalTest, Comparators){
	const rational values[] = {
		rational(-9, 2),
		rational(-4, 1),
		rational(-5, 3),
		rational(-2, 5),
		rational( 0, 1),
		rational( 3, 7),
		rational( 4, 3)
	};
	const int n = sizeof(values) / sizeof(*values);
	for(int i = 0; i < n; ++i){
		const auto& a = values[i];
		for(int j = 0; j < n; ++j){
			const auto& b = values[j];
			EXPECT_EQ(a == b, i == j);
			EXPECT_EQ(a != b, i != j);
			EXPECT_EQ(a <  b, i <  j);
			EXPECT_EQ(a <= b, i <= j);
			EXPECT_EQ(a >  b, i >  j);
			EXPECT_EQ(a >= b, i >= j);
		}
	}
}

TEST(RationalTest, BinaryOperations){
	rational x(3, 5);
	x += rational(4, 3);
	EXPECT_TRUE(check_value(x, 29, 15));
	x -= rational(3, 5);
	EXPECT_TRUE(check_value(x, 4, 3));
	x *= rational(-3, 2);
	EXPECT_TRUE(check_value(x, -2, 1));
	x /= rational(5, 3);
	EXPECT_TRUE(check_value(x, -6, 5));
}

