#include "TestHarness.h"
#include "DgHyperArray.h"

TEST(Stack_DgHyperArray, creation_DgHyperArray)
{
  size_t x = 2;
  size_t y = 3;
  size_t z = 4;

  Dg::HyperArray<int, 3> ary({{x, y, z}});

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
        ary(i, j, k) = counter;
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