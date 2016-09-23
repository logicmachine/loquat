#include <gtest/gtest.h>
#include <algorithm>
#include "loquat/math/extended_gcd.hpp"

TEST(ExtendedGCDTest, Compute){
	const int numbers[] = {
		       0,         1,         5,         8,       19,
		      41,        43,       228,       251,      619,
		    1270,      2554,      5767,      8005,    23703,
		  510564,   1221169,   4385621,   6150522, 53198655,
		53441721, 114417332, 462356750, 596671574
	};
	for(int a : numbers){
		for(int b : numbers){
			const auto p = loquat::extended_gcd(a, b);
			const long long x = p.first, y = p.second;
			EXPECT_EQ(std::__gcd(a, b), a * x + b * y);
		}
	}
}

