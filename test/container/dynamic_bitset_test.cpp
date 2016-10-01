#include <gtest/gtest.h>
#include "loquat/container/dynamic_bitset.hpp"

TEST(DynamicBitsetTest, DefaultConstruct){
	loquat::dynamic_bitset bs;
	EXPECT_TRUE(bs.empty());
	EXPECT_EQ(bs.size(), 0u);
}

TEST(DynamicBitsetTest, SizedConstruct){
	const std::tuple<size_t, bool> test_cases[] = {
		std::make_tuple(128, false),
		std::make_tuple(81,  true)
	};
	for(const auto &test_case : test_cases){
		const auto n = std::get<0>(test_case);
		const auto v = std::get<1>(test_case);
		loquat::dynamic_bitset bs(n, v);
		EXPECT_FALSE(bs.empty());
		EXPECT_EQ(bs.size(), n);
		for(size_t i = 0; i < n; ++i){
			EXPECT_EQ(bs.test(i), v);
		}
	}
}

TEST(DynamicBitsetTest, CopyConstruct){
	for(const size_t n : { 81, 128 }){
		loquat::dynamic_bitset src(n, true);
		loquat::dynamic_bitset dst(src);
		EXPECT_EQ(dst.size(), src.size());
		for(size_t i = 0; i < n; ++i){
			EXPECT_EQ(dst.test(i), src.test(i));
		}
	}
}

TEST(DynamicBitsetTest, MoveConstruct){
	for(const size_t n : { 81, 128 }){
		loquat::dynamic_bitset src(n, true);
		for(size_t i = 0; i < n; ++i){
			EXPECT_TRUE(src.test(i));
		}
		loquat::dynamic_bitset dst(std::move(src));
		EXPECT_EQ(dst.size(), n);
		EXPECT_EQ(src.size(), 0u);
		for(size_t i = 0; i < n; ++i){
			EXPECT_TRUE(dst.test(i));
		}
	}
}

TEST(DynamicBitsetTest, CopySubstitution){
	for(const size_t n : { 81, 128 }){
		loquat::dynamic_bitset src(n, true);
		loquat::dynamic_bitset dst;
		EXPECT_EQ(dst.size(), 0u);
		dst = src;
		EXPECT_EQ(dst.size(), src.size());
		for(size_t i = 0; i < n; ++i){
			EXPECT_EQ(dst.test(i), src.test(i));
		}
	}
}

TEST(DynamicBitsetTest, MoveSubstitution){
	for(const size_t n : { 81, 128 }){
		loquat::dynamic_bitset src(n, true);
		loquat::dynamic_bitset dst;
		EXPECT_EQ(dst.size(), 0u);
		dst = std::move(src);
		EXPECT_EQ(dst.size(), n);
		EXPECT_EQ(src.size(), 0u);
		for(size_t i = 0; i < n; ++i){
			EXPECT_TRUE(dst.test(i));
		}
	}
}

TEST(DynamicBitsetTest, OperatorEquals){
	for(const size_t n : { 81, 128 }){
		const size_t s1 = 3;
		loquat::dynamic_bitset a(n);
		for(size_t i = 0; i < n; ++i){
			if(i % s1 == 0){ a.set(i); }
		}
		loquat::dynamic_bitset b(a);
		EXPECT_TRUE (a == b);
		EXPECT_FALSE(a != b);
		b.flip(0);
		EXPECT_FALSE(a == b);
		EXPECT_TRUE (a != b);
	}
}

TEST(DynamicBitsetTest, OperatorAnd){
	for(const size_t n : { 81, 128 }){
		const size_t s1 = 3, s2 = 5;
		loquat::dynamic_bitset bs1(n), bs2(n);
		EXPECT_THROW(bs1 & loquat::dynamic_bitset(), std::logic_error);
		for(size_t i = 0; i < n; ++i){
			if(i % s1 == 0){ bs1.set(i); }
			if(i % s2 == 0){ bs2.set(i); }
		}
		const auto bs3 = bs1 & bs2;
		EXPECT_EQ(bs3.size(), n);
		for(size_t i = 0; i < n; ++i){
			EXPECT_EQ(bs3.test(i), (i % s1 == 0) && (i % s2 == 0));
		}
	}
}

TEST(DynamicBitsetTest, OperatorOr){
	for(const size_t n : { 81, 128 }){
		const size_t s1 = 3, s2 = 5;
		loquat::dynamic_bitset bs1(n), bs2(n);
		EXPECT_THROW(bs1 | loquat::dynamic_bitset(), std::logic_error);
		for(size_t i = 0; i < n; ++i){
			if(i % s1 == 0){ bs1.set(i); }
			if(i % s2 == 0){ bs2.set(i); }
		}
		const auto bs3 = bs1 | bs2;
		EXPECT_EQ(bs3.size(), n);
		for(size_t i = 0; i < n; ++i){
			EXPECT_EQ(bs3.test(i), (i % s1 == 0) || (i % s2 == 0));
		}
	}
}

TEST(DynamicBitsetTest, OperatorXor){
	for(const size_t n : { 81, 128 }){
		const size_t s1 = 3, s2 = 5;
		loquat::dynamic_bitset bs1(n), bs2(n);
		EXPECT_THROW(bs1 ^ loquat::dynamic_bitset(), std::logic_error);
		for(size_t i = 0; i < n; ++i){
			if(i % s1 == 0){ bs1.set(i); }
			if(i % s2 == 0){ bs2.set(i); }
		}
		const auto bs3 = bs1 ^ bs2;
		EXPECT_EQ(bs3.size(), n);
		for(size_t i = 0; i < n; ++i){
			EXPECT_EQ(bs3.test(i), (i % s1 == 0) != (i % s2 == 0));
		}
	}
}

TEST(DynamicBitsetTest, OperatorNot){
	for(const size_t n : { 81, 128 }){
		const size_t s1 = 3;
		loquat::dynamic_bitset src(n);
		for(size_t i = 0; i < n; ++i){
			if(i % s1 == 0){ src.set(i); }
		}
		const auto dst = ~src;
		EXPECT_EQ(dst.size(), n);
		for(size_t i = 0; i < n; ++i){
			EXPECT_EQ(dst.test(i), !(i % s1 == 0));
		}
	}
}

TEST(DynamicBitsetTest, OperatorShiftLeft){
	const std::tuple<size_t, size_t> test_cases[] = {
		std::make_tuple(81,  17),
		std::make_tuple(81,  64),
		std::make_tuple(128, 17),
		std::make_tuple(128, 64),
	};
	for(const auto &test_case : test_cases){
		const auto n = std::get<0>(test_case);
		const auto shift = std::get<1>(test_case);
		const size_t s1 = 3;
		loquat::dynamic_bitset src(n);
		for(size_t i = 0; i < n; ++i){
			if(i % s1 == 0){ src.set(i); }
		}
		const auto dst = src << shift;
		EXPECT_EQ(dst.size(), n);
		for(size_t i = 0; i < n; ++i){
			EXPECT_EQ(dst.test(i), (i >= shift) && ((i - shift) % s1 == 0));
		}
	}
}

TEST(DynamicBitsetTest, OperatorShiftRight){
	const std::tuple<size_t, size_t> test_cases[] = {
		std::make_tuple(81,  17),
		std::make_tuple(81,  64),
		std::make_tuple(128, 17),
		std::make_tuple(128, 64),
	};
	for(const auto &test_case : test_cases){
		const auto n = std::get<0>(test_case);
		const auto shift = std::get<1>(test_case);
		const size_t s1 = 3;
		loquat::dynamic_bitset src(n);
		for(size_t i = 0; i < n; ++i){
			if(i % s1 == 0){ src.set(i); }
		}
		const auto dst = src >> shift;
		EXPECT_EQ(dst.size(), n);
		for(size_t i = 0; i < n; ++i){
			EXPECT_EQ(dst.test(i), (i + shift < n) && ((i + shift) % s1 == 0));
		}
	}
}

TEST(DynamicBitsetTest, Set){
	const size_t n = 81;
	loquat::dynamic_bitset bs(n);
	loquat::dynamic_bitset set_all(bs);
	for(size_t i = 0; i < n; ++i){
		bs.set(i);
		EXPECT_TRUE(bs.test(i));
	}
	set_all.set();
	EXPECT_EQ(set_all, bs);
}

TEST(DynamicBitsetTest, Reset){
	const size_t n = 81;
	loquat::dynamic_bitset bs(n, true);
	loquat::dynamic_bitset reset_all(bs);
	for(size_t i = 0; i < n; ++i){
		bs.reset(i);
		EXPECT_FALSE(bs.test(i));
	}
	reset_all.reset();
	EXPECT_EQ(reset_all, bs);
}

TEST(DynamicBitsetTest, Flip){
	const size_t n = 81, s1 = 3;
	loquat::dynamic_bitset bs(n);
	for(size_t i = 0; i < n; ++i){
		if(i % s1 == 0){ bs.set(i); }
	}
	loquat::dynamic_bitset flip_all(bs);
	for(size_t i = 0; i < n; ++i){
		bs.flip(i);
		EXPECT_EQ(bs.test(i), (i % s1 != 0));
	}
	flip_all.flip();
	EXPECT_EQ(flip_all, bs);
}

TEST(DynamicBitsetTest, OperatorSubscript){
	const size_t n = 81, s1 = 3, s2 = 5;
	loquat::dynamic_bitset bs(n);
	for(size_t i = 0; i < n; ++i){
		if(i % s1 == 0){ bs.set(i); }
	}
	for(size_t i = 0; i < n; ++i){
		EXPECT_EQ(bs[i], bs.test(i));
		bs[i] = (i % s2 == 0);
	}
	for(size_t i = 0; i < n; ++i){
		EXPECT_EQ(bs.test(i), (i % s2 == 0));
	}
	for(size_t i = 0; i < n; ++i){
		bs[i] |= (i % s1 == 0);
		EXPECT_EQ(bs.test(i), (i % s1 == 0) || (i % s2 == 0));
		bs[i] &= (i % s1 == 0);
		EXPECT_EQ(bs.test(i), (i % s1 == 0));
		bs[i] ^= (i % s2 == 0);
		EXPECT_EQ(bs.test(i), (i % s1 == 0) != (i % s2 == 0));
	}
}

TEST(DynamicBitsetTest, Count){
	const size_t n = 81, s1 = 3;
	loquat::dynamic_bitset bs(n);
	for(size_t i = 0; i < n; ++i){
		if(i % s1 == 0){ bs.set(i); }
	}
	EXPECT_EQ(bs.count(), (n + s1 - 1) / s1);
}

