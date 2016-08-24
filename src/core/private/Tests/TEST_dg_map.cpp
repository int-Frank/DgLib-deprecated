#include "TestHarness.h"
#include "dg_map.h"
#include <vector>
#include <algorithm>

//globals
int g_constructorCounter = 0;
int g_destructorCounter = 0;
int g_copyCounter = 0;

class testClass
{
public:

  testClass() { g_constructorCounter++; m_value = g_constructorCounter; }
  ~testClass(){ g_destructorCounter++; }

  testClass(const testClass& a_other)
  {
    g_copyCounter++;
    m_value = a_other.m_value;
  }

  testClass& operator=(const testClass& a_other)
  {
    m_value = a_other.m_value;
    g_copyCounter++;
    return *this;
  }

  bool operator==(testClass const & a_other) const { return m_value == a_other.m_value; }
  bool operator!=(testClass const & a_other) const { return m_value != a_other.m_value; }

private:

  int m_value;
};

typedef std::pair<int, testClass>  mapPair;
typedef std::vector<mapPair>       vec;
typedef Dg::map<int, testClass>    DgMap;

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
      || m.set(0, testClass()))
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
    if (m.insert(v[i].first, testClass()))
    {
      return false;
    }

    //Check we can set data
    if (!m.set(v[i].first, testClass()))
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

bool InsertAndCheck(vec & v, DgMap & m, int newK)
{
  testClass t;
  v.push_back(mapPair(newK, t));
  m.insert(newK, t);

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
TEST(Stack_dg_pod_map, creation_dg_pod_map)
{
  DgMap m;
  m.resize(1);
  vec v;
  int index = 0;

  CHECK(m.size() == 0);
  CHECK(m.empty() == true);
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