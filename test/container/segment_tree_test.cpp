#include <gtest/gtest.h>
#include <functional>
#include <cstdint>
#include "loquat/container/segment_tree.hpp"
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

TEST(SegmentTreeTest, DefaultConstructor){
	loquat::segment_tree<test_plus_behavior> st;
	EXPECT_EQ(0u, st.size());
	EXPECT_TRUE(st.begin() == st.end());
	EXPECT_EQ(st.begin(), st.cbegin());
	EXPECT_EQ(st.end(), st.cend());
}

TEST(SegmentTreeTest, ConstructWithSizeAndDefault){
	const size_t n = 31u;
	loquat::segment_tree<test_plus_behavior> st(n, 1);
	EXPECT_EQ(n, st.size());
	for(size_t l = 0; l < n; ++l){
		EXPECT_EQ(*(st.begin() + l), 1);
		for(size_t r = l + 1; r < n; ++r){
			EXPECT_EQ(st.query(l, r), static_cast<int>(r - l));
		}
	}
}

TEST(SegmentTreeTest, ConstructWithIteratorPair){
	const size_t n = 33u;
	std::vector<int> init(n);
	std::iota(init.begin(), init.end(), 0);
	loquat::segment_tree<test_plus_behavior> st(init.begin(), init.end());
	EXPECT_EQ(st.size(), n);
	EXPECT_TRUE(std::equal(st.begin(), st.end(), init.begin()));
	for(size_t l = 0; l < n; ++l){
		EXPECT_EQ(st[l], static_cast<int>(l));
		for(size_t r = l + 1; r < n; ++r){
			const int expect = static_cast<int>(
				r * (r - 1) / 2 - l * (l - 1) / 2);
			EXPECT_EQ(st.query(l, r), expect);
		}
	}
}

TEST(SegmentTreeTest, ConstructWithInitializerList){
	const size_t n = 6u;
	loquat::segment_tree<test_plus_behavior> st = { 0, 1, 2, 3, 4, 5 };
	EXPECT_EQ(st.size(), n);
	for(size_t l = 0; l < n; ++l){
		for(size_t r = l + 1; r < n; ++r){
			const int expect = static_cast<int>(
				r * (r - 1) / 2 - l * (l - 1) / 2);
			EXPECT_EQ(st.query(l, r), expect);
		}
	}
}

TEST(SegmentTreeTest, QueryAndUpdate){
	const size_t n = 64;
	loquat::segment_tree<test_and_behavior> st(n);
	for(size_t i = 0; i < n; ++i){
		const auto it = st.begin() + i;
		EXPECT_EQ(*it, 0u);
		st.update(i, ~(1ull << i));
		EXPECT_EQ(*it, ~(1ull << i));
	}
	for(size_t l = 0; l < n; ++l){
		EXPECT_EQ(st.query(l, l), ~0ull);
		for(size_t r = l + 1; r <= n; ++r){
			uint64_t expect = ~0ull;
			for(size_t k = l; k < r; ++k){ expect &= ~(1ull << k); }
			EXPECT_EQ(st.query(l, r), expect);
		}
	}
}

TEST(SegmentTreeTest, PartitionRight){
	const size_t n = 64;
	loquat::segment_tree<test_plus_behavior> st(n);
	for(size_t i = 0; i < n; ++i){
		st.update(i, 1);
	}
	for(size_t l = 0; l < n; ++l){
		for(size_t r = l; r <= n; ++r){
			const int k = static_cast<int>(r - l);
			const int expect = r;
			const int actual = st.partition_right(
				l, [&](int x){ return x < k; });
			EXPECT_EQ(actual, expect);
		}
	}
}

TEST(SegmentTreeTest, PartitionRightOverrun){
	const size_t n = 64;
	loquat::segment_tree<test_plus_behavior> st(n);
	for(size_t i = 0; i < n; ++i){
		st.update(i, 1);
	}
	EXPECT_THROW(
		st.partition_right(0, [&](int){ return true; }),
		loquat::no_solution_error);
}

TEST(SegmentTreeTest, PartitionRightNotPow2){
	const size_t n = 0x25;
	loquat::segment_tree<test_plus_behavior> st(n);
	for(size_t i = 0; i < n; ++i){
		st.update(i, 1);
	}
	for(size_t l = 0; l < n; ++l){
		for(size_t r = l; r <= n; ++r){
			const int k = static_cast<int>(r - l);
			const int expect = r;
			const int actual = st.partition_right(
				l, [&](int x){ return x < k; });
			EXPECT_EQ(actual, expect);
		}
	}
}

TEST(SegmentTreeTest, PartitionLeft){
	const size_t n = 64;
	loquat::segment_tree<test_plus_behavior> st(n);
	for(size_t i = 0; i < n; ++i){
		st.update(i, 1);
	}
	for(size_t l = 0; l < n; ++l){
		for(size_t r = l; r <= n; ++r){
			const int k = static_cast<int>(r - l);
			const int expect = l;
			const int actual = st.partition_left(
				r, [&](int x){ return x < k; });
			EXPECT_EQ(actual, expect);
		}
	}
}

TEST(SegmentTreeTest, PartitionLeftOverrun){
	const size_t n = 64;
	loquat::segment_tree<test_plus_behavior> st(n);
	for(size_t i = 0; i < n; ++i){
		st.update(i, 1);
	}
	EXPECT_THROW(
		st.partition_left(0, [](int){ return true; }),
		loquat::no_solution_error);
	EXPECT_THROW(
		st.partition_left(n, [](int){ return true; }),
		loquat::no_solution_error);
}

TEST(SegmentTreeTest, PartitionLeftNotPow2){
	const size_t n = 0x25;
	loquat::segment_tree<test_plus_behavior> st(n);
	for(size_t i = 0; i < n; ++i){
		st.update(i, 1);
	}
	for(size_t l = 0; l < n; ++l){
		for(size_t r = l; r <= n; ++r){
			const int k = static_cast<int>(r - l);
			const int expect = l;
			const int actual = st.partition_left(
				r, [&](int x){ return x < k; });
			EXPECT_EQ(actual, expect);
		}
	}
}


namespace {

template <typename T>
void increment_fill(T& st){
	const size_t n = st.size();
	for(size_t i = 0; i < n; ++i){
		st.update(i, static_cast<int>(i));
	}
}

template <typename T>
bool test_plus(T& st){
	const auto n = st.size();
	for(size_t i = 0; i < n; ++i){
		for(size_t j = i + 1; j < n; ++j){
			size_t sum = 0;
			for(size_t k = i; k < j; ++k){ sum += k; }
			if(st.query(i, j) != static_cast<int>(sum)){ return false; }
		}
	}
	return true;
}

template <typename T>
bool test_min(T& st){
	const auto n = st.size();
	for(size_t i = 0; i < n; ++i){
		for(size_t j = i + 1; j < n; ++j){
			if(st.query(i, j) != static_cast<int>(i)){ return false; }
		}
	}
	return true;
}

}

TEST(SegmentTreeTest, MakeSegmentTreeWithSize){
	const size_t n = 32;
	auto st = loquat::make_segment_tree(
		n, loquat::make_range_query_behavior<int>(std::plus<int>()));
	increment_fill(st);
	EXPECT_TRUE(test_plus(st));
}

TEST(SegmentTreeTest, MakeSegmentTreeWithIteratorPair){
	const size_t n = 32;
	std::vector<int> init(n);
	for(size_t i = 0; i < n; ++i){ init[i] = i; }
	auto st = loquat::make_segment_tree(
		init.begin(), init.end(),
		loquat::make_range_query_behavior<int>(std::plus<int>()));
	increment_fill(st);
	EXPECT_TRUE(test_plus(st));
}

