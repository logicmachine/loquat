#include <gtest/gtest.h>
#include <functional>
#include <cstdint>
#include "loquat/container/segment_tree.hpp"

TEST(SegmentTreeTest, DefaultConstructor){
	loquat::segment_tree<int, std::plus<int>> st;
	EXPECT_EQ(0u, st.size());
	EXPECT_TRUE(st.begin() == st.end());
	EXPECT_EQ(st.begin(), st.cbegin());
	EXPECT_EQ(st.end(), st.cend());
}

TEST(SegmentTreeTest, ConstructWithSizeAndDefault){
	const size_t n = 31u;
	loquat::segment_tree<int, std::plus<int>> st(n, 1);
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
	loquat::segment_tree<int, std::plus<int>> st(init.begin(), init.end());
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
	loquat::segment_tree<int, std::plus<int>> st({ 0, 1, 2, 3, 4, 5 });
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
	loquat::segment_tree<uint64_t, std::bit_and<uint64_t>> st(n);
	for(size_t i = 0; i < n; ++i){
		const auto it = st.begin() + i;
		EXPECT_EQ(*it, 0u);
		st.update(i, ~(1ull << i));
		EXPECT_EQ(*it, ~(1ull << i));
	}
	for(size_t l = 0; l < n; ++l){
		EXPECT_EQ(st.query(l, l), uint64_t());
		for(size_t r = l + 1; r <= n; ++r){
			uint64_t expect = ~0ull;
			for(size_t k = l; k < r; ++k){ expect &= ~(1ull << k); }
			EXPECT_EQ(st.query(l, r), expect);
		}
	}
}


namespace {

int min_by_value(int x, int y){
	return std::min(x, y);
}

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
	{	// function object
		auto st = loquat::make_segment_tree<int>(n, std::plus<int>());
		increment_fill(st);
		EXPECT_TRUE(test_plus(st));
	}
	{	// int (*)(int, int)
		auto st = loquat::make_segment_tree<int>(n, min_by_value);
		increment_fill(st);
		EXPECT_TRUE(test_min(st));
	}
	{	// const int& (*)(const int&, const int&)
		auto st = loquat::make_segment_tree<int>(n, std::min<int>);
		increment_fill(st);
		EXPECT_TRUE(test_min(st));
	}
}

TEST(SegmentTreeTest, MakeSegmentTreeWithIteratorPair){
	const size_t n = 32;
	std::vector<int> init(n);
	for(size_t i = 0; i < n; ++i){ init[i] = i; }
	{	// function object
		auto st = loquat::make_segment_tree(
			init.begin(), init.end(), std::plus<int>());
		increment_fill(st);
		EXPECT_TRUE(test_plus(st));
	}
	{	// int (*)(int, int)
		auto st = loquat::make_segment_tree(
			init.begin(), init.end(), min_by_value);
		increment_fill(st);
		EXPECT_TRUE(test_min(st));
	}
	{	// const int& (*)(const int&, const int&)
		auto st = loquat::make_segment_tree(
			init.begin(), init.end(), std::min<int>);
		increment_fill(st);
		EXPECT_TRUE(test_min(st));
	}
}

