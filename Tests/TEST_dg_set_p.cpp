#include "TestHarness.h"
#include "dg_set_pod.h"
#include <vector>
#include <algorithm>

typedef std::vector<int>   vec;
typedef Dg::set_pod<int>     DgSet;

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

bool InsertAndCheck(vec & v, DgSet & m, int newT)
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

}