//#include "TestHarness.h"
//#include "dg_pod_map.h"
//#include <vector>
//#include <algorithm>
//
//typedef std::pair<int, int>     mapPair;
//typedef std::vector<mapPair>    vec;
//typedef Dg::POD::map<int, int>  DgMap;
//
//bool sortFcn(mapPair a, mapPair b)
//{
//  return a.first < b.first;
//}
//
//bool CheckState(vec v, DgMap m)
//{
//  if (m.size() != v.size())
//  {
//    return false;
//  }
//
//  int index = 0;
//
//  //Check map state
//  if (v.size() == 0)
//  {
//    if (m.empty() == false
//      || m.find(0, index)
//      || m.set(0, 1))
//    {
//      return false;
//    }
//  }
//  else
//  {
//    if (m.empty() == true)
//    {
//      return false;
//    }
//  }
//
//  std::sort(v.begin(), v.end(), sortFcn);
//  for (int i = 0; i < int(v.size()); ++i)
//  {
//    //Check data matches
//    if (v[i].second != m[i]
//      || m.QueryKey(i) != v[i].first)
//    {
//      return false;
//    }
//
//    //Check we can find data at the right index
//    if (!m.find(v[i].first, index))
//    {
//      return false;
//    }
//    if (index != i)
//    {
//      return false;
//    }
//
//    //Check we can find data at the right index
//    if (!m.find(v[i].first, index, 0, 0xFFFFFFF))
//    {
//      return false;
//    }
//    if (index != i)
//    {
//      return false;
//    }
//
//    //Check we cannot insert duplicate data
//    if (m.insert(v[i].first, 1))
//    {
//      return false;
//    }
//
//    //Check we can set data
//    if (!m.set(v[i].first, -1))
//    {
//      return false;
//    }
//    if (!m.set(v[i].first, v[i].second))
//    {
//      return false;
//    }
//  }
//
//  return true;
//}
//
//bool InsertAndCheck(vec & v, DgMap & m, int newK, int newT)
//{
//  v.push_back(mapPair(newK, newT));
//  m.insert(newK, newT);
//
//  if (!CheckState(v, m))
//  {
//    return false;
//  }
//
//  // Rebuild map
//  DgMap newM = m;
//
//  return CheckState(v, newM);
//}
//
//bool RemoveAndCheck(vec & v, DgMap & m)
//{
//  return true;
//}
//
////--------------------------------------------------------------------------------
////	Dg::map_s
////--------------------------------------------------------------------------------
//TEST(Stack_dg_pod_map, creation_dg_pod_map)
//{
//  DgMap m;
//  vec v;
//  int index = 0;
//
//  CHECK(m.size() == 0);
//  CHECK(m.empty() == true);
//  CHECK(m.max_size() == DG_MAP_DEFAULT_SIZE);
//  CHECK(m.find(0, index) == false);
//  CHECK(m.set(0, 1) == false);
//
//  CHECK(InsertAndCheck(v, m, 3, 5456));
//  CHECK(InsertAndCheck(v, m, 8, 5789));
//  CHECK(InsertAndCheck(v, m, 1, 485));
//  CHECK(InsertAndCheck(v, m, 9, 854890));
//  CHECK(InsertAndCheck(v, m, 5, 3458));
//  CHECK(InsertAndCheck(v, m, 2, 94567));
//  CHECK(InsertAndCheck(v, m, 4, 348));
//  CHECK(InsertAndCheck(v, m, 7, 757));
//  CHECK(InsertAndCheck(v, m, 6, 346587));
//  CHECK(InsertAndCheck(v, m, 0, 87));
//
//}