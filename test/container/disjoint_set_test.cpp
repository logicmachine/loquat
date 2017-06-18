#include <gtest/gtest.h>
#include <functional>
#include <cstdint>
#include "loquat/container/disjoint_set.hpp"

TEST(DisjointSetTest, DefaultConstructor){
	loquat::disjoint_set ds;
	EXPECT_EQ(0u, ds.size());
}

TEST(DisjointSetTest, ConstructWithSize){
	const size_t n = 31;
	loquat::disjoint_set ds(n);
	for(size_t i = 0; i < n; ++i){
		for(size_t j = 0; j < n; ++j){
			EXPECT_EQ(i == j, ds.same(i, j));
		}
	}
}

TEST(DisjoinSetTest, QueryAndModify){
	std::default_random_engine engine;
	for(const size_t n : { 3, 16, 127 }){
		loquat::disjoint_set ds(n);
		std::vector<size_t> naive(n);
		std::iota(naive.begin(), naive.end(), 0);
		std::uniform_int_distribution<int> type_dist(0, 1);
		std::uniform_int_distribution<size_t> index_dist(0, n - 1);
		for(size_t iter = 0; iter < n; ++iter){
			const int type = type_dist(engine);
			const size_t i = index_dist(engine);
			const size_t j = index_dist(engine);
			if(type == 0){
				ds.unite(i, j);
				const size_t k = naive[j];
				for(auto& x : naive){
					if(x == k){ x = naive[i]; }
				}
			}else{
				EXPECT_EQ(naive[i] == naive[j], ds.same(i, j));
			}
		}
	}
}

