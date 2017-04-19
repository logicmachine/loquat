#include <gtest/gtest.h>
#include <functional>
#include <cstdint>
#include "loquat/container/lazy_segment_tree.hpp"

namespace {

struct test_behavior {
	using value_type    = int;
	using modifier_type = int;

	value_type identity_value() const { return 0; }

	modifier_type identity_modifier() const { return 0; }

	std::pair<modifier_type, modifier_type>
	split_modifier(
		const modifier_type& m,
		size_t k) const
	{
		if(m == 0){
			return std::pair<int, int>(0, 0);
		}else{
			return std::pair<int, int>(m, m + k);
		}
	}

	modifier_type merge_modifier(
		const modifier_type& a,
		const modifier_type& b) const
	{
		return a + b;
	}

	value_type merge_value(
		const value_type& a,
		const value_type& b) const
	{
		return a + b;
	}

	value_type modify(
		size_t n,
		const value_type& v,
		const modifier_type& m) const
	{
		if(m == 0){ return v; }
		return v + (m * n) + (n * (n - 1) / 2);
	}
};

}

TEST(LazySegmentTreeTest, DefaultConstructor){
	loquat::lazy_segment_tree<test_behavior> st;
	EXPECT_EQ(0u, st.size());
}

TEST(LazySegmentTreeTest, ConstructWithSize){
	const size_t n = 31u;
	loquat::lazy_segment_tree<test_behavior> st(n);
	for(size_t l = 0; l < n; ++l){
		for(size_t r = l + 1; r < n; ++r){
			EXPECT_EQ(st.query(l, r), 0);
		}
	}
}

TEST(LazySegmentTreeTest, ConstructWithIteratorPair){
	const size_t n = 33u;
	std::vector<int> init(n);
	std::iota(init.begin(), init.end(), 0);
	loquat::lazy_segment_tree<test_behavior> st(init.begin(), init.end());
	EXPECT_EQ(st.size(), n);
	for(size_t l = 0; l < n; ++l){
		for(size_t r = l + 1; r < n; ++r){
			const int expect = static_cast<int>(
				r * (r - 1) / 2 - l * (l - 1) / 2);
			EXPECT_EQ(st.query(l, r), expect);
		}
	}
}

TEST(LazySegmentTreeTest, RandomQueryAndUpdate){
	std::default_random_engine engine;
	for(const size_t n : { 24, 31, 32, 37, 53, 60 }){
		std::uniform_int_distribution<int> type_dist(0, 1);
		std::uniform_int_distribution<int> index_dist(0, n - 1);
		std::uniform_int_distribution<int> modify_dist(1, 10);
		std::vector<int> naive(n);
		loquat::lazy_segment_tree<test_behavior> st(n);
		for(size_t iter = 0; iter < n * n; ++iter){
			const int type = type_dist(engine);
			size_t l = index_dist(engine);
			size_t r = index_dist(engine);
			if(r > l){ std::swap(l, r); }
			if(type == 0){
				// query
				int expect = 0;
				for(size_t i = l; i <= r; ++i){ expect += naive[i]; }
				const int actual = st.query(l, r + 1);
				EXPECT_EQ(actual, expect);
			}else{
				// modify
				const auto modifier = modify_dist(engine);
				for(size_t i = l; i <= r; ++i){
					naive[i] += modifier + (i - l);
				}
				st.modify(l, r + 1, modifier);
			}
		}
	}
}

