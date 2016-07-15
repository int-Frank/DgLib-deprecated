#include "TestHarness.h"
#include "dg_list_pod.h"
#include <list>
#include <algorithm>

typedef int t;
typedef std::list<t>       list;
typedef Dg::list_pod<t>    DgList;

bool CheckState(list & a_list, DgList & a_dgList)
{
  if (a_list.size() != a_dgList.size())
  {
    return false;
  }

  DgList::const_iterator cdgit = a_dgList.cbegin();
  list::const_iterator clit = a_list.cbegin();
  for (clit, cdgit; clit != a_list.cend(); ++clit, ++cdgit)
  {
    if (*clit != *cdgit) return false;
  }

  DgList::iterator dgit = a_dgList.begin();
  list::iterator lit = a_list.begin();
  for (lit, dgit; lit != a_list.end(); ++lit, ++dgit)
  {
    if (*lit != *dgit) return false;
  }

  if (a_list.size() > 0)
  {
    lit = a_list.end(); --lit;
    dgit = a_dgList.end(); --dgit;
    if (*lit != *dgit) return false;

    clit = a_list.cend(); --clit;
    cdgit = a_dgList.cend(); --cdgit;
    if (*clit != *cdgit) return false;

    if (a_list.back() != a_dgList.back()) return false;
    if (a_list.front() != a_dgList.front()) return false;
  }


  return true;
}


//--------------------------------------------------------------------------------
//	Dg::map_s
//--------------------------------------------------------------------------------
TEST(Stack_dg_list_pod, creation_dg_list_pod)
{
  DgList    dglst;
  list      lst;

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

  for (t i = 0; i < tMax; ++i)
  {
    dglst.pop_back();
    lst.pop_back();
    CHECK(CheckState(lst, dglst));
  }

  for (t i = 0; i < tMax; ++i)
  {
    dglst.push_front(i);
    lst.push_front(i);
    CHECK(CheckState(lst, dglst));
  }

  for (t i = 0; i < tMax; ++i)
  {
    dglst.pop_front();
    lst.pop_front();
    CHECK(CheckState(lst, dglst));
  }

  for (t i = 0; i < tMax; ++i)
  {
    dglst.push_front(i);
    lst.push_front(i);
  }

  list::iterator lit = lst.begin();
  DgList::iterator dgit = dglst.begin();

  lit = lst.insert(lit, 2); 
  dgit = dglst.insert(dgit, 2);
  CHECK(*lit == *dgit);

  lit++; ++lit;
  dgit++; ++dgit;
  lit = lst.insert(lit, 23);
  dgit = dglst.insert(dgit, 23);
  CHECK(*lit == *dgit);

  lit++; ++lit;
  dgit++; ++dgit;
  lit = lst.erase(lit);
  dgit = dglst.erase(dgit);
  CHECK(*lit == *dgit);

  lit++; ++lit;
  dgit++; ++dgit;
  lit = lst.erase(lit);
  dgit = dglst.erase(dgit);
  CHECK(*lit == *dgit);

  lit--; --lit;
  dgit--; --dgit;
  CHECK(*lit == *dgit);
  CHECK(CheckState(lst, dglst));

  lst.clear();
  dglst.clear();
  CHECK(CheckState(lst, dglst));

  for (t i = 0; i < tMax; ++i)
  {
    dglst.push_back(i);
    lst.push_back(i);
    CHECK(CheckState(lst, dglst));
  }

  DgList newlst(dglst);
  DgList newlst2 = dglst;
  CHECK(CheckState(lst, newlst));
  CHECK(CheckState(lst, newlst2));

  DgList newlst3(666);
  CHECK(newlst3.max_size() == 666);
  CHECK(newlst3.size() == 0);
  newlst3 = dglst;
  CHECK(CheckState(lst, newlst3));
}