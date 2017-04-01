#include <gtest/gtest.h>
#include <vector>
#include "loquat/math/modulus_utility.hpp"

static constexpr int MOD = 1000000007;
using mint = loquat::modulus_integer<MOD>;

TEST(ModulusUtilityTest, Factorial){
	EXPECT_EQ(loquat::modulus_factorial<MOD>(0), mint(1));
	mint last = 1;
	for(int i = 1; i < 100; ++i){
		last *= mint(i);
		if(i % 3 == 0 || i % 5 == 0){
			EXPECT_EQ(loquat::modulus_factorial<MOD>(i), last);
		}
	}
}

TEST(ModulusUtilityTest, InvFactorial){
	for(int i = 0; i < 100; ++i){
		if(i % 3 == 0 || i % 5 == 0){
			const auto f = loquat::modulus_factorial<MOD>(i);
			const auto inv_f = loquat::modulus_inv_factorial<MOD>(i);
			EXPECT_EQ(f * inv_f, mint(1));
		}
	}
}

TEST(ModulusUtilityTest, Combination){
	const int n = 100;
	std::vector<std::vector<mint>> triangle;
	for(int i = 0; i < n; ++i){
		std::vector<mint> next(i + 1);
		next[0] = next[i] = mint(1);
		for(int j = 1; j < i; ++j){
			next[j] = triangle[i - 1][j - 1] + triangle[i - 1][j];
		}
		triangle.emplace_back(std::move(next));
	}
	for(int i = 0; i < n; ++i){
		for(int j = 0; j <= i; ++j){
			EXPECT_EQ(loquat::modulus_combination<MOD>(i, j), triangle[i][j]);
		}
	}
}

TEST(ModulusUtilityTest, Multichoose){
	const int n = 100;
	std::vector<std::vector<mint>> matrix(n, std::vector<mint>(n));
	for(int i = 0; i < n; ++i){ matrix[i][1] = mint(i); }
	for(int j = 2; j < n; ++j){
		for(int i = 1; i < n; ++i){
			matrix[i][j] = matrix[i - 1][j] + matrix[i][j - 1];
		}
	}
	for(int i = 1; i < n; ++i){ matrix[i][0] = mint(1); }
	for(int i = 0; i < n; ++i){
		for(int j = 0; j < n; ++j){
			EXPECT_EQ(loquat::modulus_multichoose<MOD>(i, j), matrix[i][j]);
		}
	}
}

