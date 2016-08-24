#include "TestHarness.h"
#include "dg_set.h"
#include <vector>
#include <algorithm>

typedef int                Int;
typedef std::vector<Int>   vec;
typedef Dg::set<Int>       DgSet;

bool CheckState(vec v, DgSet m)
{
  if (m.size() != v.size())
  {
    return false;
  }

  int index = 0;

  //Check map state
  if (v.size() == 0)
  {
    if (m.empty() == false
      || m.find(0, index))
    {
      return false;
    }
  }
  else
  {
    if (m.empty() == true)
    {
      return false;
    }
  }

  std::sort(v.begin(), v.end());
  for (int i = 0; i < int(v.size()); ++i)
  {
    //Check data matches
    if (v[i] != m[i])
    {
      return false;
    }

    //Check we can find data at the right index
    if (!m.find(v[i], index))
    {
      return false;
    }
    if (index != i)
    {
      return false;
    }

    //Check we can find data at the right index
    if (!m.find(v[i], index))
    {
      return false;
    }
    if (index != i)
    {
      return false;
    }

    //Check we cannot insert duplicate data
    if (m.insert_unique(v[i]))
    {
      return false;
    }
  }

  //Check erasing elements.
  vec v1 = v;
  DgSet m1 = m;
  int eraseInd = 0;

  while (v1.size() != 0)
  {
    int ind = eraseInd % v1.size();

    m1.erase(v1[ind]);

    auto it = v1.begin();
    for (int i = 0; i < ind; ++i)
    {
      it++;
    }
    v1.erase(it);

    if (!CheckState(v1, m1))
    {
      return false;
    }

  }

  return true;
}

bool InsertAndCheck(vec & v, DgSet & m, Int newT)
{
  if (!m.insert_unique(newT))
  {
    return true;
  }

  v.push_back(newT);

  if (!CheckState(v, m))
  {
    return false;
  }

  // Rebuild map
  DgSet newM = m;

  return CheckState(v, newM);
}

bool CheckEraseAll(int num)
{
  DgSet m;
  vec v;

  //Item in within the set
  m.insert(1);
  v.push_back(1);
  m.insert(3);
  v.push_back(3);
  for (int i = 0; i < num; ++i)
  {
    m.insert(2);
  }
  m.erase_all(2);
  if (!CheckState(v, m)) return false;

  //Item at the beginning of the set
  m.clear();
  v.clear();
  m.insert(3);
  v.push_back(3);
  for (int i = 0; i < num; ++i)
  {
    m.insert(2);
  }
  m.erase_all(2);
  if (!CheckState(v, m)) return false;

  //Item at the end of the set
  m.clear();
  v.clear();
  m.insert(1);
  v.push_back(1);
  for (int i = 0; i < num; ++i)
  {
    m.insert(2);
  }
  m.erase_all(2);
  if (!CheckState(v, m)) return false;

  //Only the item is in the set
  m.clear();
  v.clear();
  for (int i = 0; i < num; ++i)
  {
    m.insert(2);
  }
  m.erase_all(2);
  if (!CheckState(v, m)) return false;

  return true;
}

//--------------------------------------------------------------------------------
//	Dg::map_s
//--------------------------------------------------------------------------------
TEST(Stack_dg_set_p, creation_dg_set_p)
{
  DgSet m;
  m.resize(1);
  vec v;
  int index = 0;

  CHECK(m.size() == 0);
  CHECK(m.empty() == true);
  CHECK(m.max_size() == 1);
  CHECK(m.find(0, index) == false);

  CHECK(InsertAndCheck(v, m, 3));
  CHECK(InsertAndCheck(v, m, 8));
  CHECK(InsertAndCheck(v, m, 1));
  CHECK(InsertAndCheck(v, m, 9));
  CHECK(InsertAndCheck(v, m, 5));
  CHECK(InsertAndCheck(v, m, 2));
  CHECK(InsertAndCheck(v, m, 4));
  CHECK(InsertAndCheck(v, m, 7));
  CHECK(InsertAndCheck(v, m, 6));
  CHECK(InsertAndCheck(v, m, 0));

  CHECK(CheckEraseAll(1));
  CHECK(CheckEraseAll(2));
  CHECK(CheckEraseAll(3));

}