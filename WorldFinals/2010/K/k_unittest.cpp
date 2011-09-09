#include "k.h"
#include <gtest/gtest.h>


TEST(isCircleInTriangleOrBox, Positive) {
	vector<cd> points;
	points.push_back(cd(0, 0));
	points.push_back(cd(1, 0));
	points.push_back(cd(1, 1));
	points.push_back(cd(0, 1));
	
	EXPECT_TRUE(isCircleInTriangleOrBox(points, cd(0.3, 0.3), 0.2));
	EXPECT_TRUE(isCircleInTriangleOrBox(points, cd(0.7, 0.3), 0.2));
	EXPECT_TRUE(isCircleInTriangleOrBox(points, cd(0.7, 0.7), 0.2));
	EXPECT_TRUE(isCircleInTriangleOrBox(points, cd(0.3, 0.7), 0.2));
}

TEST(isCircleInTriangleOrBox, NegativeCross) {
	vector<cd> points;
	points.push_back(cd(0, 0));
	points.push_back(cd(1, 0));
	points.push_back(cd(1, 1));
	points.push_back(cd(0, 1));
	
	EXPECT_FALSE(isCircleInTriangleOrBox(points, cd(0.1, 0.1), 0.2));
	EXPECT_FALSE(isCircleInTriangleOrBox(points, cd(0.9, 0.1), 0.2));
	EXPECT_FALSE(isCircleInTriangleOrBox(points, cd(0.9, 0.9), 0.2));
	EXPECT_FALSE(isCircleInTriangleOrBox(points, cd(0.1, 0.9), 0.2));
}
