#include "a.h"
#include <gtest/gtest.h>


TEST(a, a) {
}

TEST (toString1D, normal) {
  vector<int> v;
  v.push_back(1);
  EXPECT_EQ(" 1\n", toString1D(v));
  v.push_back(1);
  EXPECT_EQ(" 1 1\n", toString1D(v));
}

TEST (toString2D, normal) {
  vector<vector<int> > v;

  v.push_back(vector<int>());
  v[0].push_back(1);
  EXPECT_EQ(" 1\n", toString2D(v));
  
  v[0].push_back(1);
  EXPECT_EQ(" 1 1\n", toString2D(v));
  
  v.push_back(vector<int>());
  v[1].push_back(1);
  v[1].push_back(1);
  EXPECT_EQ(" 1 1\n 1 1\n", toString2D(v));
}

TEST (toString3D, normal) {
  vector<vector<vector<int> > > v;

  v.push_back(vector<vector<int> >());
  v[0].push_back(vector<int>());
  v[0][0].push_back(1);
  EXPECT_EQ(" 1\n", toString3D(v));
  
  v[0].push_back(vector<int>());
  v[0][1].push_back(1);
  EXPECT_EQ(" 1\n 1\n", toString3D(v));
  v.push_back(vector<vector<int> >());
  v[1].push_back(vector<int>());
  v[1][0].push_back(1);
  v[1].push_back(vector<int>());
  v[1][1].push_back(1);
  EXPECT_EQ(" 1\n 1\n\n 1\n 1\n", toString3D(v));
}
