#include <gtest/gtest.h>
#include <random>
#include "loquat/misc/range_manager.hpp"

namespace {

struct query {
	bool is_add;
	size_t left;
	size_t right;

	query(bool is_add, size_t left, size_t right)
		: is_add(is_add)
		, left(left)
		, right(right)
	{ }
};

std::vector<query> generate_random_queries(size_t n, size_t q){
	std::default_random_engine engine;
	std::uniform_int_distribution<int> flag_dist(0, 1);
	std::uniform_int_distribution<size_t> index_dist(0, n - 1);
	std::vector<query> ret;
	for(size_t i = 0; i < q; ++i){
		const bool is_add = flag_dist(engine);
		size_t l = index_dist(engine);
		size_t r = index_dist(engine);
		if(r < l){ std::swap(l, r); }
		ret.emplace_back(is_add, l, r + 1);
	}
	return ret;
}

}

TEST(RangeManagerTest, Random){
	const size_t n = 30, m = 1000;
	const auto queries = generate_random_queries(n, m);
	loquat::range_manager<size_t> rman;
	std::vector<size_t> expect(n), actual(n), timestamps(n);
	size_t now = 1;
	for(const auto& q : queries){
		auto verify = [&]() -> bool {
			for(size_t i = 1; i < n; ++i){
				if(!actual[i] || !actual[i - 1]){ continue; }
				if(timestamps[i] != timestamps[i - 1]){ return false; }
			}
			return true;
		};
		auto insert = [&](size_t l, size_t r){
			for(size_t i = l; i < r; ++i){
				EXPECT_FALSE(actual[i]);
				actual[i] = true;
				timestamps[i] = now;
			}
			EXPECT_TRUE(verify());
			++now;
		};
		auto erase = [&](size_t l, size_t r){
			for(size_t i = l; i < r; ++i){
				EXPECT_TRUE(actual[i]);
				actual[i] = false;
			}
			EXPECT_TRUE(verify());
		};
		const size_t l = q.left, r = q.right;
		if(q.is_add){
			for(size_t i = l; i < r; ++i){ expect[i] = true; }
			rman.add(l, r, insert, erase);
		}else{
			for(size_t i = l; i < r; ++i){ expect[i] = false; }
			rman.remove(l, r, insert, erase);
		}
		EXPECT_EQ(actual, expect);
	}
}

