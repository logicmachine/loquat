#include <gtest/gtest.h>
#include <cstdint>
#include "loquat/misc/mo_algorithm.hpp"

namespace {

struct mo_bit_or_behavior {
	using value_type  = uint64_t;
	using state_type  = uint64_t;
	using result_type = uint64_t;
	state_type empty() const { return 0; }
	state_type add_head(value_type v, state_type s) const { return v | s; }
	state_type add_tail(state_type s, value_type v) const { return s | v; }
	state_type remove(value_type v, state_type s) const { return s & ~v; }
	result_type to_result(state_type s) const { return s; }
};

}

TEST(MoAlgorithmTest, DefaultConstructor){
	std::vector<std::pair<int, int>> queries;
	queries.emplace_back(0, 0);
	loquat::mo_algorithm<mo_bit_or_behavior> mo;
	const auto result = mo.query(queries.begin(), queries.end());
	EXPECT_EQ(result.size(), queries.size());
	EXPECT_EQ(result[0], 0ull);
}

TEST(MoAlgorithmTest, Query){
	const size_t n = 64;
	std::vector<uint64_t> v(n);
	for(size_t i = 0; i < n; ++i){ v[i] = (1ull << i); }
	std::vector<std::pair<int, int>> queries;
	for(size_t i = 0; i <= n; ++i){
		for(size_t j = i; j <= n; ++j){
			queries.emplace_back(i, j);
		}
	}
	loquat::mo_algorithm<mo_bit_or_behavior> mo(v.begin(), v.end());
	const auto result = mo.query(queries.begin(), queries.end());
	EXPECT_EQ(result.size(), queries.size());
	for(size_t i = 0; i < queries.size(); ++i){
		const size_t l = queries[i].first, r = queries[i].second;
		const uint64_t expected =
			 ((r == 64u) ? ~0ull : ((1ull << r) - 1)) &
			~((l == 64u) ? ~0ull : ((1ull << l) - 1));
		EXPECT_EQ(result[i], expected);
	}
}

