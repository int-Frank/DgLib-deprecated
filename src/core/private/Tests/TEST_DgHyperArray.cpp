#include "TestHarness.h"
#include "DgHyperArray.h"

TEST(Stack_DgHyperArray, creation_DgHyperArray)
{
  size_t x = 2;
  size_t y = 3;
  size_t z = 4;

  Dg::HyperArray<int, 3> ary({x, y, z});

  //Dynamic length checks
  CHECK(ary.length(0) == x);
  CHECK(ary.length(1) == y);
  CHECK(ary.length(2) == z);
  CHECK(ary.length_noChecks(0) == x);
  CHECK(ary.length_noChecks(1) == y);
  CHECK(ary.length_noChecks(2) == z);

  try {ary.length(5); CHECK(false);} catch (...){}
  try {ary.length(-1); CHECK(false);} catch (...){}
  try {ary.length(17); CHECK(false);} catch (...){}

  //Compile length checks
  CHECK(ary.length<0>() == x);
  CHECK(ary.length<1>() == y);
  CHECK(ary.length<2>() == z);

  size_t counter = 0;
  for (size_t i = 0; i < x; i++)
  {
    for (size_t j = 0; j < y; j++)
    {
      for (size_t k = 0; k < z; k++)
      {
        ary(i, j, k) = int(counter);
        counter++;
      }
    }
  }

  counter = 0;
  for (size_t i = 0; i < x; i++)
  {
    for (size_t j = 0; j < y; j++)
    {
      for (size_t k = 0; k < z; k++)
      {
        CHECK(ary(i, j, k) == counter);
        counter++;
      }
    }
  }

  counter = 0;
  for (size_t i = 0; i < x; i++)
  {
    for (size_t j = 0; j < y; j++)
    {
      for (size_t k = 0; k < z; k++)
      {
        CHECK(ary.at(i, j, k) == counter);
        counter++;
      }
    }
  }

  Dg::HyperArray<int, 3> ary1(ary);
  Dg::HyperArray<int, 3> ary2 = ary;

  counter = 0;
  for (size_t i = 0; i < x; i++)
  {
    for (size_t j = 0; j < y; j++)
    {
      for (size_t k = 0; k < z; k++)
      {
        CHECK(ary1.at(i, j, k) == counter);
        counter++;
      }
    }
  }

  counter = 0;
  for (size_t i = 0; i < x; i++)
  {
    for (size_t j = 0; j < y; j++)
    {
      for (size_t k = 0; k < z; k++)
      {
        CHECK(ary2.at(i, j, k) == counter);
        counter++;
      }
    }
  }
}



TEST(Stack_DgHyperArrayCompare, creation_DgHyperArrayCompare)
{
  Dg::HyperArray<int, 3> ary({3, 3, 3});

  ary(0, 0, 0) = 1; ary(0, 0, 1) = 2; ary(0, 0, 2) = 3;
  ary(0, 1, 0) = 4; ary(0, 1, 1) = 5; ary(0, 1, 2) = 6;
  ary(0, 2, 0) = 7; ary(0, 2, 1) = 8; ary(0, 2, 2) = 9;

  ary(1, 0, 0) = 1; ary(1, 0, 1) = 2; ary(1, 0, 2) = 3;
  ary(1, 1, 0) = 4; ary(1, 1, 1) = 5; ary(1, 1, 2) = 6;
  ary(1, 2, 0) = 7; ary(1, 2, 1) = 8; ary(1, 2, 2) = 9;

  ary(2, 0, 0) = 1; ary(2, 0, 1) = 2; ary(2, 0, 2) = 3;
  ary(2, 1, 0) = 4; ary(2, 1, 1) = 7; ary(2, 1, 2) = 6;
  ary(2, 2, 0) = 7; ary(2, 2, 1) = 8; ary(2, 2, 2) = 9;

  CHECK(ary.compare<0>({}, {}) == true);

  CHECK(ary.compare<1>({0}, {1}) == true);
  CHECK(ary.compare<1>({0}, {2}) == false);
  CHECK(ary.compare<1>({1}, {2}) == false);

  CHECK(ary.compare<2>({0, 0}, {1, 0}) == true);
  CHECK(ary.compare<2>({0, 0}, {2, 0}) == true);
  CHECK(ary.compare<2>({1, 1}, {2, 1}) == false);

  CHECK(ary.compare<3>({0, 0, 0}, {1, 0, 0}) == true);
  CHECK(ary.compare<3>({1, 1, 1}, {0, 1, 1}) == true);
  CHECK(ary.compare<3>({2, 1, 1}, {1, 2, 0}) == true);

  CHECK(ary.compare<3>({1, 0, 1}, {1, 0, 2}) == false);
  CHECK(ary.compare<3>({2, 2, 2}, {1, 2, 1}) == false);
  CHECK(ary.compare<3>({2, 1, 2}, {2, 1, 1}) == false);

  //Fill
  ary.fill(0);
  for (size_t i = 0; i < 3; i++)
  {
    for (size_t j = 0; j < 3; j++)
    {
      for (size_t k = 0; k < 3; k++)
      {
        CHECK(ary(i, j, k) == 0);
      }
    }
  }

  ary.fill<1>(1, {1});
  for (size_t j = 0; j < 3; j++)
  {
    for (size_t k = 0; k < 3; k++)
    {
      CHECK(ary(1, j, k) == 1);
    }
  }

  ary.fill<2>(2, {2, 1});
  for (size_t k = 0; k < 3; k++)
  {
    CHECK(ary(2, 1, k) == 2);
  }

  ary.fill<3>(3, {2, 2, 2});
  CHECK(ary(2, 2, 2) == 3);
}