#include "TestHarness.h"
#include "dg_map_pod.h"
#include <vector>
#include <algorithm>

typedef std::pair<int, int>     mapPair;
typedef std::vector<mapPair>    vec;
typedef Dg::map_pod<int, int>     DgMap;

bool sortFcn(mapPair a, mapPair b)
{
  return a.first < b.first;
}

bool CheckState(vec v, DgMap m)
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
      || m.find(0, index)
      || m.set(0, 1))
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

  std::sort(v.begin(), v.end(), sortFcn);
  for (int i = 0; i < int(v.size()); ++i)
  {
    //Check data matches
    if (v[i].second != m[i]
      || m.query_key(i) != v[i].first)
    {
      return false;
    }

    //Check we can find data at the right index
    if (!m.find(v[i].first, index))
    {
      return false;
    }
    if (index != i)
    {
      return false;
    }

    //Check we can find data at the right index
    if (!m.find(v[i].first, index))
    {
      return false;
    }
    if (index != i)
    {
      return false;
    }

    //Check we cannot insert duplicate data
    if (m.insert(v[i].first, 1))
    {
      return false;
    }

    //Check we can set data
    if (!m.set(v[i].first, -1))
    {
      return false;
    }
    if (!m.set(v[i].first, v[i].second))
    {
      return false;
    }
  }

  //Check erasing elements.
  vec v1 = v;
  DgMap m1 = m;
  int eraseInd = 0;

  while (v1.size() != 0)
  {
    int ind = eraseInd % v1.size();
    
    m1.erase(v1[ind].first);

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

bool InsertAndCheck(vec & v, DgMap & m, int newK, int newT)
{
  v.push_back(mapPair(newK, newT));
  m.insert(newK, newT);

  if (!CheckState(v, m))
  {
    return false;
  }

  // Rebuild map
  DgMap newM = m;

  return CheckState(v, newM);
}

//--------------------------------------------------------------------------------
//	Dg::map_s
//--------------------------------------------------------------------------------
TEST(Stack_dg_map_p, creation_dg_map_p)
{
  DgMap m;
  m.resize(1);
  vec v;
  int index = 0;

  CHECK(m.size() == 0);
  CHECK(m.empty() == true);
  CHECK(m.max_size() == 1);
  CHECK(m.find(0, index) == false);
  CHECK(m.set(0, 1) == false);

  CHECK(InsertAndCheck(v, m, 3, 5456));
  CHECK(InsertAndCheck(v, m, 8, 5789));
  CHECK(InsertAndCheck(v, m, 1, 485));
  CHECK(InsertAndCheck(v, m, 9, 854890));
  CHECK(InsertAndCheck(v, m, 5, 3458));
  CHECK(InsertAndCheck(v, m, 2, 94567));
  CHECK(InsertAndCheck(v, m, 4, 348));
  CHECK(InsertAndCheck(v, m, 7, 757));
  CHECK(InsertAndCheck(v, m, 6, 346587));
  CHECK(InsertAndCheck(v, m, 0, 87));

}