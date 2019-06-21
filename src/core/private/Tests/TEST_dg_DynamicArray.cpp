#include "TestHarness.h"
#include "DgDynamicArray.h"
#include <vector>

#include "NonPODTests.h"

template<typename T>
bool CheckState(std::vector<T> & a_vec, Dg::DynamicArray<T> & a_dgVec)
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


TEST(Stack_dg_DynamicArray, creation_dg_DynamicArray)
{
  std::vector<int>       lst;
  Dg::DynamicArray<int>  dglst;

  CHECK(dglst.size() == 0);
  CHECK(dglst.empty() == true);

  CHECK(CheckState(lst, dglst));

  int tMax = 20;
  for (int i = 0; i < tMax; ++i)
  {
    dglst.push_back(i);
    lst.push_back(i);
    CHECK(CheckState(lst, dglst));
  }

  lst.clear();
  dglst.clear();
  CHECK(CheckState(lst, dglst));

  for (int i = 0; i < tMax; ++i)
  {
    dglst.push_back(i);
    lst.push_back(i);
    CHECK(CheckState(lst, dglst));
  }

  for (int i = 0; i < tMax; ++i)
  {
    dglst.pop_back();
    lst.pop_back();
    CHECK(CheckState(lst, dglst));
  }

  for (int i = 0; i < tMax; ++i)
  {
    dglst.push_back(i);
    lst.push_back(i);
    CHECK(CheckState(lst, dglst));
  }

  Dg::DynamicArray<int> newlst(dglst);
  Dg::DynamicArray<int> newlst2 = dglst;
  CHECK(CheckState(lst, newlst));
  CHECK(CheckState(lst, newlst2));

  Dg::DynamicArray<int> newlst3(666);
  CHECK(newlst3.size() == 0);
  newlst3 = dglst;
  CHECK(CheckState(lst, newlst3));
}