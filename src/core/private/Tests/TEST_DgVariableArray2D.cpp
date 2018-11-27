#include "TestHarness.h"
#include "DgVariableArray2D.h"

TEST(Stack_DgVariableArray2D, creation_DgVariableArray2D)
{
  Dg::VariableArray2D<int> arr;

  int x0[3] = {0, 1, 2};
  int x1[2] = {3, 4};
  int x2[4] = {5, 6, 7, 8};
  int x3[1] = {9};
  int x4[5] = {10, 11, 12, 13, 14};

  arr.push_back(x0, 3);
  arr.push_back(x1, 2);
  arr.push_back(x2, 4);
  arr.push_back(x3, 1);
  arr.push_back(x4, 5);

  CHECK(arr(0, 0) == 0);
  CHECK(arr(0, 1) == 1);
  CHECK(arr(0, 2) == 2);
  CHECK(arr(1, 0) == 3);
  CHECK(arr(1, 1) == 4);
  CHECK(arr(2, 0) == 5);
  CHECK(arr(2, 1) == 6);
  CHECK(arr(2, 2) == 7);
  CHECK(arr(2, 3) == 8);
  CHECK(arr(3, 0) == 9);
  CHECK(arr(4, 0) == 10);
  CHECK(arr(4, 1) == 11);
  CHECK(arr(4, 2) == 12);
  CHECK(arr(4, 3) == 13);
  CHECK(arr(4, 4) == 14);
}