
#include <gtest/gtest.h>

#include "Vector2f.h"

TEST(Vector2fTest, VectorAddition) {
	Vector2f v1(0, 0);
	Vector2f v2(1, 2);
	Vector2f v3(3, 4);

	Vector2f result = v1 + v1;
	EXPECT_FLOAT_EQ(0.0f, result.x);
	EXPECT_FLOAT_EQ(0.0f, result.y);

	result = v2 + v1;
	EXPECT_FLOAT_EQ(v2.x, result.x);
	EXPECT_FLOAT_EQ(v2.y, result.y);

	result = v2 + v3;
	EXPECT_FLOAT_EQ(4.0f, result.x);
	EXPECT_FLOAT_EQ(6.0f, result.y);
}