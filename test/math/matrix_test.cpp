#include <gtest/gtest.h>
#include "loquat/math/matrix.hpp"

using matrix = loquat::matrix<int>;

TEST(MatrixTest, Constructors){
	const matrix a(2, 2);
	EXPECT_EQ(a.rows(), 2u);
	EXPECT_EQ(a.columns(), 2u);
	EXPECT_EQ(a(0, 0), 0);
	EXPECT_EQ(a(0, 1), 0);
	EXPECT_EQ(a(1, 0), 0);
	EXPECT_EQ(a(1, 1), 0);

	const matrix b(2, 2, 10);
	EXPECT_EQ(b.rows(), 2u);
	EXPECT_EQ(b.columns(), 2u);
	EXPECT_EQ(b(0, 0), 10);
	EXPECT_EQ(b(0, 1), 10);
	EXPECT_EQ(b(1, 0), 10);
	EXPECT_EQ(b(1, 1), 10);

	const matrix c;
	EXPECT_EQ(c.rows(), 0u);
	EXPECT_EQ(c.columns(), 0u);
}


TEST(MatrixTest, Comparators){
	const matrix a(2, 2, { 0, 1, 2, 3 });
	const matrix b(2, 2, { 0, 1, 0, 3 });
	const matrix c(2, 2, { 0, 1, 2, 3 });
	EXPECT_FALSE(a == b);
	EXPECT_TRUE (a == c);
	EXPECT_TRUE (a != b);
	EXPECT_FALSE(a != c);
}


TEST(MatrixTest, ArithmeticUnaryOperations){
	const matrix a(2, 3, { 1, 2, 3,  4, 5, 6 });
	EXPECT_EQ(+a, a);
	EXPECT_EQ(-a, matrix(2, 3, { -1, -2, -3,  -4, -5, -6 }));
}


TEST(MatrixTest, ArithmeticBinaryOperations){
	const matrix a(2, 2, { 1, 2,  3, 4 });
	const matrix b(2, 2, { 2, 3,  4, 5 });
	const matrix c(3, 2, { 2, 3,  4, 5,  6, 7 });

	auto plus = a; plus += b;
	EXPECT_EQ(plus, matrix(2, 2, { 3, 5,  7, 9 }));

	auto minus = a; minus -= b;
	EXPECT_EQ(minus, matrix(2, 2, { -1, -1,  -1, -1 }));

	const matrix ca(3, 2, {
		2 * 1 + 3 * 3,  2 * 2 + 3 * 4,
		4 * 1 + 5 * 3,  4 * 2 + 5 * 4,
		6 * 1 + 7 * 3,  6 * 2 + 7 * 4
	});
	EXPECT_EQ(c * a, ca);

	const matrix ab(2, 2, {
		1 * 2 + 2 * 4,  1 * 3 + 2 * 5,
		3 * 2 + 4 * 4,  3 * 3 + 4 * 5
	});
	auto mult = a; mult *= b;
	EXPECT_EQ(mult, ab);

	auto smat = a; smat *= 2;
	EXPECT_EQ(smat,  matrix(2, 2, { 2, 4,  6, 8 }));
	EXPECT_EQ(2 * a, matrix(2, 2, { 2, 4,  6, 8 }));
}


TEST(MatrixTest, BitwiseUnaryOperations){
	const matrix a(2, 3, { 1, 2, 3,  4, 5, 6 });
	EXPECT_EQ(~a, matrix(2, 3, { ~1, ~2, ~3,  ~4, ~5, ~6 }));
}


TEST(MatrixTest, BitwiseBinaryOperations){
	const matrix a(2, 2, { 1, 2,  3, 4 });
	const matrix b(2, 2, { 2, 3,  4, 5 });

	auto bit_and = a; bit_and &= b;
	EXPECT_EQ(bit_and, matrix(2, 2, { 1 & 2, 2 & 3,  3 & 4, 4 & 5 }));

	auto bit_or = a; bit_or |= b;
	EXPECT_EQ(bit_or, matrix(2, 2, { 1 | 2, 2 | 3,  3 | 4, 4 | 5 }));

	auto bit_xor = a; bit_xor ^= b;
	EXPECT_EQ(bit_xor, matrix(2, 2, { 1 ^ 2, 2 ^ 3,  3 ^ 4, 4 ^ 5 }));
}

