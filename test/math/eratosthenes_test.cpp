#include <gtest/gtest.h>
#include "loquat/math/eratosthenes.hpp"

namespace {

const std::vector<int> expected = {
	2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47,
	53, 59, 61, 67, 71, 73, 79, 83, 89, 97
};

}

TEST(EratosthenesTest, GenerateEmpty){
	EXPECT_TRUE(loquat::eratosthenes(0).empty());
	EXPECT_TRUE(loquat::eratosthenes(1).empty());
}

TEST(EratosthenesTest, Generate){
	const auto actual = loquat::eratosthenes(100);
	EXPECT_EQ(actual, expected);
}

TEST(EratosthenesTest, Sieve){
	std::vector<bool> sieve(100);
	loquat::eratosthenes(sieve);
	for(int i = 0; i < 100; ++i){
		const bool expect =
			std::binary_search(expected.begin(), expected.end(), i);
		EXPECT_EQ(sieve[i], expect);
	}
}

