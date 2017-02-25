#include <gtest/gtest.h>
#include "loquat/math/static_matrix.hpp"

using matrix2x2 = loquat::static_matrix<int, 2, 2>;
using matrix2x3 = loquat::static_matrix<int, 2, 3>;
using matrix3x2 = loquat::static_matrix<int, 3, 2>;

TEST(StaticMatrixTest, Constructors){
	const matrix2x2 a;
	EXPECT_EQ(a(0, 0), 0);
	EXPECT_EQ(a(0, 1), 0);
	EXPECT_EQ(a(1, 0), 0);
	EXPECT_EQ(a(1, 1), 0);

	const matrix2x2 b(10);
	EXPECT_EQ(b(0, 0), 10);
	EXPECT_EQ(b(0, 1), 10);
	EXPECT_EQ(b(1, 0), 10);
	EXPECT_EQ(b(1, 1), 10);

	const matrix2x2 c({ 1, 2,  3, 4 });
	EXPECT_EQ(c(0, 0), 1);
	EXPECT_EQ(c(0, 1), 2);
	EXPECT_EQ(c(1, 0), 3);
	EXPECT_EQ(c(1, 1), 4);

	EXPECT_THROW(matrix2x2({ 1, 2 }), std::logic_error);

	const matrix2x2 d =
		loquat::identity<std::multiplies<matrix2x2>>::value();
	EXPECT_EQ(d(0, 0), 1);
	EXPECT_EQ(d(0, 1), 0);
	EXPECT_EQ(d(1, 0), 0);
	EXPECT_EQ(d(1, 1), 1);
}


TEST(StaticMatrixTest, Comparators){
	const matrix2x2 a({ 0, 1, 2, 3 });
	const matrix2x2 b({ 0, 1, 0, 3 });
	const matrix2x2 c({ 0, 1, 2, 3 });
	EXPECT_FALSE(a == b);
	EXPECT_TRUE (a == c);
	EXPECT_TRUE (a != b);
	EXPECT_FALSE(a != c);
}


TEST(StaticMatrixTest, ArithmeticUnaryOperations){
	const matrix2x3 a({ 1, 2, 3,  4, 5, 6 });
	EXPECT_EQ(+a, a);
	EXPECT_EQ(-a, matrix2x3({ -1, -2, -3,  -4, -5, -6 }));
}


TEST(StaticMatrixTest, ArithmeticBinaryOperations){
	const matrix2x2 a({ 1, 2,  3, 4 });
	const matrix2x2 b({ 2, 3,  4, 5 });
	const matrix3x2 c({ 2, 3,  4, 5,  6, 7 });

	auto plus = a; plus += b;
	EXPECT_EQ(plus, matrix2x2({ 3, 5,  7, 9 }));

	auto minus = a; minus -= b;
	EXPECT_EQ(minus, matrix2x2({ -1, -1,  -1, -1 }));

	const matrix3x2 ca({
		2 * 1 + 3 * 3,  2 * 2 + 3 * 4,
		4 * 1 + 5 * 3,  4 * 2 + 5 * 4,
		6 * 1 + 7 * 3,  6 * 2 + 7 * 4
	});
	EXPECT_EQ(c * a, ca);

	const matrix2x2 ab({
		1 * 2 + 2 * 4,  1 * 3 + 2 * 5,
		3 * 2 + 4 * 4,  3 * 3 + 4 * 5
	});
	auto mult = a; mult *= b;
	EXPECT_EQ(mult, ab);

	auto smat = a; smat *= 2;
	EXPECT_EQ(smat,  matrix2x2({ 2, 4,  6, 8 }));
	EXPECT_EQ(2 * a, matrix2x2({ 2, 4,  6, 8 }));
}


TEST(StaticMatrixTest, BitwiseUnaryOperations){
	const matrix2x3 a({ 1, 2, 3,  4, 5, 6 });
	EXPECT_EQ(~a, matrix2x3({ ~1, ~2, ~3,  ~4, ~5, ~6 }));
}


TEST(StaticMatrixTest, BitwiseBinaryOperations){
	const matrix2x2 a({ 1, 2,  3, 4 });
	const matrix2x2 b({ 2, 3,  4, 5 });

	auto bit_and = a; bit_and &= b;
	EXPECT_EQ(bit_and, matrix2x2({ 1 & 2, 2 & 3,  3 & 4, 4 & 5 }));

	auto bit_or = a; bit_or |= b;
	EXPECT_EQ(bit_or, matrix2x2({ 1 | 2, 2 | 3,  3 | 4, 4 | 5 }));

	auto bit_xor = a; bit_xor ^= b;
	EXPECT_EQ(bit_xor, matrix2x2({ 1 ^ 2, 2 ^ 3,  3 ^ 4, 4 ^ 5 }));
}

