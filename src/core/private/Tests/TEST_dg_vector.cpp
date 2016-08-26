#include "TestHarness.h"
#include "dg_vector.h"
#include <vector>

#include "NonPODTests.h"

typedef int t;
typedef std::vector<t>   vector;
typedef Dg::vector<t>    DgVec;



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


TEST(Stack_dg_vector_pod, creation_dg_vector_pod)
{
  DgVec       dglst;
  vector      lst;

  CHECK(dglst.size() == 0);
  CHECK(dglst.empty() == true);

  CHECK(CheckState(lst, dglst));

  t tMax = 20;
  for (t i = 0; i < tMax; ++i)
  {
    dglst.push_back(i);
    lst.push_back(i);
    CHECK(CheckState(lst, dglst));
  }

  lst.clear();
  dglst.clear();
  CHECK(CheckState(lst, dglst));

  for (t i = 0; i < tMax; ++i)
  {
    dglst.push_back(i);
    lst.push_back(i);
    CHECK(CheckState(lst, dglst));
  }

  for (t i = 0; i < tMax; ++i)
  {
    dglst.pop_back();
    lst.pop_back();
    CHECK(CheckState(lst, dglst));
  }

  for (t i = 0; i < tMax; ++i)
  {
    dglst.push_back(i);
    lst.push_back(i);
    CHECK(CheckState(lst, dglst));
  }

  DgVec newlst(dglst);
  DgVec newlst2 = dglst;
  CHECK(CheckState(lst, newlst));
  CHECK(CheckState(lst, newlst2));

  DgVec newlst3(666);
  CHECK(newlst3.size() == 0);
  newlst3 = dglst;
  CHECK(CheckState(lst, newlst3));
}