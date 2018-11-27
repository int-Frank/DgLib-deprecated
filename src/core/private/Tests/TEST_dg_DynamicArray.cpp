#include "TestHarness.h"
#include "DgDynamicArray.h"
#include <vector>

#include "NonPODTests.h"

typedef int t;
typedef std::vector<t>         vector_pod;
typedef Dg::DynamicArray<t>    DgVec_pod;

class A
{
  int val;
public:
  A(int a_val): val(a_val){}
  ~A(){}
  bool operator==(A const & a_other) const {return val == a_other.val;}
  bool operator!=(A const & a_other) const {return val != a_other.val;}
};

typedef std::vector<A>         vector_nonpod;
typedef Dg::DynamicArray<A>    DgVec_nonpod;

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

//class Bone
//{
//public:
//
//  Bone(){}
//  ~Bone() {}
//
//private:
//
//  Dg::DynamicArray<Bone>  m_children;
//};

TEST(Stack_dg_vector_pod, creation_dg_vector_pod)
{
  DgVec_pod       dglst;
  vector_pod      lst;

  CHECK(DgVec_pod::IsPOD == true);

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

  t * pItem = Dg::Find(dglst, 3);
  CHECK(pItem != nullptr);

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

  DgVec_pod newlst(dglst);
  DgVec_pod newlst2 = dglst;
  CHECK(CheckState(lst, newlst));
  CHECK(CheckState(lst, newlst2));

  DgVec_pod newlst3(666);
  CHECK(newlst3.size() == 0);
  newlst3 = dglst;
  CHECK(CheckState(lst, newlst3));
}

TEST(Stack_dg_vector_nonpod, creation_dg_vector_nonpod)
{
  DgVec_nonpod       dglst;
  vector_nonpod      lst;

  CHECK(DgVec_nonpod::IsPOD == false);

  CHECK(dglst.size() == 0);
  CHECK(dglst.empty() == true);

  CHECK(CheckState(lst, dglst));

  t tMax = 20;
  for (t i = 0; i < tMax; ++i)
  {
    dglst.push_back(A(i));
    lst.push_back(A(i));
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
    dglst.push_back(A(i));
    lst.push_back(A(i));
    CHECK(CheckState(lst, dglst));
  }

  DgVec_nonpod newlst(dglst);
  DgVec_nonpod newlst2 = dglst;
  CHECK(CheckState(lst, newlst));
  CHECK(CheckState(lst, newlst2));

  DgVec_nonpod newlst3(666);
  CHECK(newlst3.size() == 0);
  newlst3 = dglst;
  CHECK(CheckState(lst, newlst3));
}