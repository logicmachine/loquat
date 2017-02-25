#include <gtest/gtest.h>
#include <functional>
#include <cstdint>
#include "loquat/container/nazo_table.hpp"
#include "loquat/container/range_query_helper.hpp"

namespace {

struct test_plus_behavior {
	using value_type = int;
	value_type identity() const { return 0; }
	value_type merge(const value_type& a, const value_type& b) const {
		return a + b;
	}
};

struct test_and_behavior {
	using value_type = uint64_t;
	value_type identity() const { return ~0ull; }
	value_type merge(const value_type& a, const value_type& b) const {
		return a & b;
	}
};

}

TEST(NazoTableTest, DefaultConstructor){
	loquat::nazo_table<test_plus_behavior> nt;
	EXPECT_EQ(0u, nt.size());
}

TEST(NazoTableTest, ConstructWithIteratorPair){
	const size_t n = 33u;
	std::vector<int> init(n);
	std::iota(init.begin(), init.end(), 0);
	loquat::nazo_table<test_plus_behavior> nt(init.begin(), init.end());
	EXPECT_EQ(n, nt.size());
	for(size_t l = 0; l < n; ++l){
		for(size_t r = l; r < n; ++r){
			const int expect = static_cast<int>(
				r * (r - 1) / 2 - l * (l - 1) / 2);
			EXPECT_EQ(expect, nt.query(l, r)) << l << " " << r;
		}
	}
}

TEST(NazoTableTest, ConstructWithInitializerList){
	const size_t n = 6u;
	loquat::nazo_table<test_plus_behavior> nt = { 0, 1, 2, 3, 4, 5 };
	EXPECT_EQ(n, nt.size());
	for(size_t l = 0; l < n; ++l){
		for(size_t r = l + 1; r < n; ++r){
			const int expect = static_cast<int>(
				r * (r - 1) / 2 - l * (l - 1) / 2);
			EXPECT_EQ(expect, nt.query(l, r));
		}
	}
}


TEST(NazoTableTest, MakeNazoTableWithIteratorPair){
	const size_t n = 32u;
	std::vector<int> init(n);
	std::iota(init.begin(), init.end(), 0);
	auto nt = loquat::make_nazo_table(
		init.begin(), init.end(),
		loquat::make_range_query_behavior<int>(std::plus<int>()));
	EXPECT_EQ(n, nt.size());
	for(size_t l = 0; l < n; ++l){
		for(size_t r = l; r < n; ++r){
			const int expect = static_cast<int>(
				r * (r - 1) / 2 - l * (l - 1) / 2);
			EXPECT_EQ(expect, nt.query(l, r));
		}
	}
}

