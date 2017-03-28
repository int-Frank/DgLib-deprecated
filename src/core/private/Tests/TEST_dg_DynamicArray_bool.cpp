#include "TestHarness.h"
#include "DgDynamicArray.h"
#include <vector>

#include "NonPODTests.h"

typedef std::vector<bool>         vector;
typedef Dg::DynamicArray<bool>    DgVec;



bool CheckState(vector & a_vec, DgVec & a_dgVec)
{
  if (a_vec.size() != a_dgVec.size())
  {
    return false;
  }

  if (a_vec.empty() != a_dgVec.empty())
  {
    return false;
  }

  for (size_t i = 0; i < a_dgVec.size(); ++i)
  {
    if (a_vec[i] != a_dgVec[i])
    {
      return false;
    }
  }

  if (a_vec.size() != 0)
  {
    if (a_vec.back() != a_dgVec.back())
    {
      return false;
    }
  }

  return true;
}


TEST(Stack_DgVector_bool, creation_DgVector_bool)
{
  DgVec       dgVec;
  vector      stdVec;

  CHECK(dgVec.size() == 0);
  CHECK(dgVec.empty() == true);

  CHECK(CheckState(stdVec, dgVec));

  int tMax = 256;
  for (int i = 0; i < tMax; ++i)
  {
    dgVec.push_back(i % 3 == 0);
    stdVec.push_back(i % 3 == 0);
    CHECK(CheckState(stdVec, dgVec));
  }

  stdVec.clear();
  dgVec.clear();
  CHECK(CheckState(stdVec, dgVec));

  for (int i = 0; i < tMax; ++i)
  {
    dgVec.push_back(i % 3 == 0);
    stdVec.push_back(i % 3 == 0);
    CHECK(CheckState(stdVec, dgVec));
  }

  for (int i = 0; i < tMax; ++i)
  {
    dgVec.push_back(i % 3 == 0);
    stdVec.push_back(i % 3 == 0);
    CHECK(CheckState(stdVec, dgVec));
  }

  for (int i = 0; i < tMax; ++i)
  {
    dgVec.push_back(i % 3 == 0);
    stdVec.push_back(i % 3 == 0);
    CHECK(CheckState(stdVec, dgVec));
  }

  DgVec newlst(dgVec);
  DgVec newlst2 = dgVec;
  CHECK(CheckState(stdVec, newlst));
  CHECK(CheckState(stdVec, newlst2));

  DgVec newlst3(666);
  CHECK(newlst3.size() == 0);
  newlst3 = dgVec;
  CHECK(CheckState(stdVec, newlst3));
}