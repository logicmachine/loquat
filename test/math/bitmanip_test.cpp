#include <gtest/gtest.h>
#include "loquat/math/bitmanip.hpp"

TEST(BitmanipTest, Ctz){
	EXPECT_EQ(loquat::bitmanip::ctz(uint32_t(0x33cc0000u)), 18u);
	EXPECT_EQ(loquat::bitmanip::ctz( int32_t(0x33cc0000u)), 18u);
	EXPECT_EQ(loquat::bitmanip::ctz(uint64_t(0x33cc000000000000ull)), 50u);
	EXPECT_EQ(loquat::bitmanip::ctz( int64_t(0x33cc000000000000ull)), 50u);
}

TEST(BitmanipTest, Clz){
	EXPECT_EQ(loquat::bitmanip::clz(uint32_t(0x33cc0000u)), 2u);
	EXPECT_EQ(loquat::bitmanip::clz( int32_t(0x33cc0000u)), 2u);
	EXPECT_EQ(loquat::bitmanip::clz(uint64_t(0x33cc000000000000ull)), 2u);
	EXPECT_EQ(loquat::bitmanip::clz( int64_t(0x33cc000000000000ull)), 2u);
	EXPECT_EQ(loquat::bitmanip::clz(uint64_t(0x00000000000033ccull)), 50u);
	EXPECT_EQ(loquat::bitmanip::clz( int64_t(0x00000000000033ccull)), 50u);
}

TEST(BitmanipTest, Popcount){
	EXPECT_EQ(loquat::bitmanip::popcount(uint32_t(0x137f137fu)), 20u);
	EXPECT_EQ(loquat::bitmanip::popcount( int32_t(0xf731f731u)), 20u);
	EXPECT_EQ(loquat::bitmanip::popcount(uint64_t(0x137f137f137f137full)), 40u);
	EXPECT_EQ(loquat::bitmanip::popcount( int64_t(0xf731f731f731f731ull)), 40u);
}

TEST(BitmanipTest, Clp2){
	EXPECT_EQ(loquat::bitmanip::clp2(uint32_t(0x0000ffffu)), uint32_t(0x00010000u));
	EXPECT_EQ(loquat::bitmanip::clp2(uint32_t(0x00010000u)), uint32_t(0x00010000u));
	EXPECT_EQ(loquat::bitmanip::clp2(uint32_t(0x00010001u)), uint32_t(0x00020000u));
	EXPECT_EQ(loquat::bitmanip::clp2(uint64_t(0x0000ffffffffffffull)), uint64_t(0x0001000000000000ull));
	EXPECT_EQ(loquat::bitmanip::clp2(uint64_t(0x0001000000000000ull)), uint64_t(0x0001000000000000ull));
	EXPECT_EQ(loquat::bitmanip::clp2(uint64_t(0x0001000000000001ull)), uint64_t(0x0002000000000000ull));
}

TEST(BitmanipTest, Flp2){
	EXPECT_EQ(loquat::bitmanip::flp2(uint32_t(0x0000ffffu)), uint32_t(0x00008000u));
	EXPECT_EQ(loquat::bitmanip::flp2(uint32_t(0x00010000u)), uint32_t(0x00010000u));
	EXPECT_EQ(loquat::bitmanip::flp2(uint32_t(0x00010001u)), uint32_t(0x00010000u));
	EXPECT_EQ(loquat::bitmanip::flp2(uint64_t(0x0000ffffffffffffull)), uint64_t(0x0000800000000000ull));
	EXPECT_EQ(loquat::bitmanip::flp2(uint64_t(0x0001000000000000ull)), uint64_t(0x0001000000000000ull));
	EXPECT_EQ(loquat::bitmanip::flp2(uint64_t(0x0001000000000001ull)), uint64_t(0x0001000000000000ull));
}

