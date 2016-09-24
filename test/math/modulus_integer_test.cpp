#include <gtest/gtest.h>
#include "loquat/math/modulus_integer.hpp"

static constexpr int MOD = 1000000007;
using mint = loquat::modulus_integer<MOD>;

TEST(ModulusIntegerTest, Constructors){
	EXPECT_EQ(*mint(), 0);
	EXPECT_EQ(*mint(100), 100);
	EXPECT_EQ(*mint(10 + MOD + MOD), 10);
	EXPECT_EQ(*mint(-5), MOD - 5);
}

TEST(ModulusIntegerTest, UnaryOperations){
	EXPECT_EQ(*(+mint(100)), 100);
	EXPECT_EQ(*(-mint(100)), MOD - 100);
	EXPECT_EQ(*(-mint(0)), 0);
}

TEST(ModulusIntegerTest, Comparators){
	const int numbers[] = { 0, 100, MOD - 1, MOD, MOD + 100, MOD + 101 };
	for(const int a : numbers){
		for(const int b : numbers){
			const int ma = a % MOD, mb = b % MOD;
			EXPECT_EQ(ma == mb, mint(a) == mint(b));
			EXPECT_EQ(ma != mb, mint(a) != mint(b));
			EXPECT_EQ(ma <  mb, mint(a) <  mint(b));
			EXPECT_EQ(ma <= mb, mint(a) <= mint(b));
			EXPECT_EQ(ma >  mb, mint(a) >  mint(b));
			EXPECT_EQ(ma >= mb, mint(a) >= mint(b));
		}
	}
}

TEST(ModulusIntegerTest, BinaryOperations){
	mint x(200);
	x += mint(MOD - 100);
	EXPECT_EQ(*x, 100);
	x -= mint(200);
	EXPECT_EQ(*x, MOD - 100);
	mint y(65536);
	y *= mint(100000);
	EXPECT_EQ(*y, 6553600000ll % MOD);
	mint z(6142);
	z /= mint(100);
	EXPECT_EQ(*(z * mint(100)), 6142);
}

TEST(ModulusIntegerTest, Increments){
	mint x(MOD - 3);
	EXPECT_EQ(*(++x), MOD - 2);
	EXPECT_EQ(*x,     MOD - 2);
	EXPECT_EQ(*(x++), MOD - 2);
	EXPECT_EQ(*x,     MOD - 1);
	EXPECT_EQ(*(++x), 0);
}

TEST(ModulusIntegerTest, Decrements){
	mint x(2);
	EXPECT_EQ(*(--x), 1);
	EXPECT_EQ(*x,     1);
	EXPECT_EQ(*(x--), 1);
	EXPECT_EQ(*x,     0);
	EXPECT_EQ(*(--x), MOD - 1);
}

