#include <gtest/gtest.h>
#include <vector>
#include <random>
#include "loquat/misc/sparse_monotonic_list.hpp"

namespace {

static const int MAX_VALUE = 1000000000;

struct query {
	int key;
	int value;

	query(int key, int value)
		: key(key)
		, value(value)
	{ }
};

std::vector<query> generate_random_queries(int n, int q){
	std::default_random_engine engine;
	std::uniform_int_distribution<int> key_dist(0, n - 1);
	std::uniform_int_distribution<int> value_dist(0, MAX_VALUE);
	std::vector<query> ret;
	for(int i = 0; i < q; ++i){
		ret.emplace_back(key_dist(engine), value_dist(engine));
	}
	return ret;
}

}

TEST(SparseMonotonicListTest, Random){
	const int n = 50, m = 1000;
	const auto queries = generate_random_queries(n, m);
	std::vector<int> expect(n, -1);
	loquat::sparse_monotonic_list<int, int> actual;
	for(const auto& q : queries){
		const bool expect_flag = (expect[q.key] < q.value);
		for(int i = q.key; i < n; ++i){
			expect[i] = std::max(expect[i], q.value);
		}
		const bool actual_flag = actual.insert(q.key, q.value);
		EXPECT_EQ(actual_flag, expect_flag);
		auto it = actual.begin();
		for(int i = 0, last = -1; i < n; ++i){
			if(expect[i] > last){
				EXPECT_EQ(it->first, i);
				EXPECT_EQ(it->second, expect[i]);
				++it;
			}
			last = expect[i];
		}
		EXPECT_EQ(it, actual.end());
	}
}

