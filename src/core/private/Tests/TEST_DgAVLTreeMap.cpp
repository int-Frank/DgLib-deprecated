#include <iostream>
#include <set>

#include "TestHarness.h"

#define DEBUG
#define BRK do{char t(0); std::cin >> t;}while(false)
#include "DgAVLTreeMap.h"

typedef Dg::AVLTreeMap<int, int> Map;

bool AreEqual(Map const & m0, Map const & m1)
{
  if (m0.size() != m1.size())
    return false;

  Map::const_iterator m0_cit = m0.cbegin();
  Map::const_iterator m1_cit = m1.cbegin();
  for (; m0_cit != m0.cend(); m0_cit++, m1_cit++)
  {/*
    int v0 = m0_cit->first;
    int v1 = m1_cit->first;
    int v0s = m0_cit->second;
    int v1s = m1_cit->second;
*/
    if (m0_cit->first != m1_cit->first)
      return false;
    if (m0_cit->second != m1_cit->second)
      return false;
  }
  return true;
}

TEST(Stack_dg_AVLTreeMap, creation_dg_AVLTreeMap)
{
  Map map, map2;

  int const nItems = 20;
  int keys[nItems] = {5, 12, 3, 19, 9, 7, 2, 20, 18, 15, 10, 11, 1, 4, 6, 8, 13, 14, 16, 17};

  //----------------------------------------------------------------------------------------
  //Insertion
  //----------------------------------------------------------------------------------------
  for (int i = 0; i < nItems; i++)
  {
    map.insert(keys[i], 2 * keys[i]);
  }

  for (int i = 0; i < nItems; i++)
  {
    map2[keys[i]] = 2 * keys[i];
  }

  CHECK(AreEqual(map, map2));

  //----------------------------------------------------------------------------------------
  //Copy
  //----------------------------------------------------------------------------------------
  Map map3(map);
  CHECK(AreEqual(map, map3));
  Map map4 = map3;
  CHECK(AreEqual(map3, map4));
  
  //----------------------------------------------------------------------------------------
  //Iterating
  //----------------------------------------------------------------------------------------
  int value = 1;
  for (auto kv : map3)
  {
    CHECK(kv.first == value);
    CHECK(kv.second == 2 * value);
    value++;
  }

  value = 1;
  for (auto kv : map)
  {
    CHECK(kv.first == value);
    CHECK(kv.second == 2 * value);
    value++;
  }

  Map::iterator it = map.end();
  for (int i = 20; i > 0; i--)
  {
    it--;
    CHECK(it->first == i);
    CHECK(it->second == 2*i);
    CHECK((*it).first == i);
    CHECK((*it).second == 2*i);
  }

  value = 1;
  for (Map::iterator it = map.begin(); it != map.end(); it++)
  {
    CHECK(it->first == value);
    CHECK(it->second == 2*value);
    CHECK((*it).first == value);
    CHECK((*it).second == 2*value);
    value++;
  }

  Map::const_iterator cit = map.cend();
  for (int i = 20; i > 0; i--)
  {
    cit--;
    CHECK(cit->first == i);
    CHECK(cit->second == 2*i);
    CHECK((*cit).first == i);
    CHECK((*cit).second == 2*i);
  }

  value = 1;
  for (Map::const_iterator cit = map.cbegin(); cit != map.cend(); cit++)
  {
    CHECK(cit->first == value);
    CHECK(cit->second == 2*value);
    CHECK((*cit).first == value);
    CHECK((*cit).second == 2*value);
    value++;
  }

  for (Map::iterator_rand itr = map.begin_rand(); itr != map.end_rand(); itr++)
  {
    Map::const_iterator_rand citr(itr);
    std::cout << itr->first << ", "; 
  }
  std::cout << '\n';
  for (Map::const_iterator_rand citr = map.cbegin_rand(); citr != map.cend_rand(); citr++)
  {
    std::cout << citr->first << ", "; 
  }
  std::cout << '\n';

  //----------------------------------------------------------------------------------------
  //Utility
  //----------------------------------------------------------------------------------------
  CHECK(map.size() == 20);
  CHECK(!map.empty());

  //----------------------------------------------------------------------------------------
  //Accessing/searching
  //----------------------------------------------------------------------------------------
  Map const & crmap(map);

  cit = crmap.find(4);
  CHECK(cit != crmap.cend());
  CHECK(cit->second == 8);

  cit = crmap.find(-2);
  CHECK(cit == map.cend());

  cit = crmap.find(514);
  CHECK(cit == crmap.cend());

  //Non-const tests...
  it = map.find(4);
  CHECK(it != map.end());
  CHECK(it->second == 8);

  it = map.find(-2);
  CHECK(it == map.end());

  it = map.find(514);
  CHECK(it == map.end());

  //----------------------------------------------------------------------------------------
  //Modifying
  //----------------------------------------------------------------------------------------
  for (int i = 0; i < nItems; i++)
  {
    map[keys[i]] = 3 * keys[i];
  }
  value = 1;
  for (auto kv : map)
  {
    CHECK(kv.first == value);
    CHECK(kv.second == 3 * value);
    value++;
  }

  for (int i = 0; i < nItems; i++)
  {
    map.at(keys[i]) = 4 * keys[i];
  }
  value = 1;
  for (auto kv : map)
  {
    CHECK(kv.first == value);
    CHECK(kv.second == 4 * value);
    value++;
  }

  for (int i = 0; i < nItems; i++)
  {
    CHECK(crmap.at(keys[i]) == 4 * keys[i]);
  }
  
  //----------------------------------------------------------------------------------------
  //Erasing
  //----------------------------------------------------------------------------------------
  map.clear();

  for (int i = 0; i < nItems; i++)
  {
    map[keys[i]] = 2 * keys[i];
  }

  map2 = map;
  for (int i = nItems; i >= 1; i--)
  {
    map.erase(i);

    int value = 1;
    for (auto kv : map)
    {
      CHECK(kv.first == value);
      CHECK(kv.second == 2 * value);
      value++;
    }
  }

  map = map2;
  for (int i = 1; i <= nItems; i++)
  {
    map.erase(i);

    int value = i + 1;
    for (auto kv : map)
    {
      CHECK(kv.first == value);
      CHECK(kv.second == 2 * value);
      value++;
    }
  }
  
  map = map2;
  std::set<int> items;
  for (int i = 1; i <= 20; i++)
    items.insert(i);

  for (int i = 0; i < nItems; i++)
  {
    map.erase(keys[i]);
    items.erase(keys[i]);
    std::set<int>::iterator sit = items.begin();

    for (auto kv : map)
    {
      CHECK(kv.first == *sit);
      CHECK(kv.second == 2 * *sit);
      sit++;
    }
  }

  map = map2;
  it = map.begin();
  for (int i = 1; i < nItems; i++)
  {
    it = map.erase(it);
    CHECK(it->first == i+1);
  }

  it = map.erase(it);
  CHECK(it == map.end());

  map = map2;
  it = map.end();
  for (int i = nItems; i > 0; i--)
  {
    it--;
    it = map.erase(it);
    CHECK(it == map.end());
  }

  map = map2;
  it = map.begin();
  for (int i = 0; i < 5; i++)
    it++;
  //Erase 6
  it = map.erase(it);
  int val = it->first;
  CHECK(it->first == 7);

  for (int i = 0; i < 2; i++)
    it++;

  //Erase 9
  it = map.erase(it);
  CHECK(it->first == 10);

  map.Print();
  BRK;

  it--;
  //Erase 8
  it = map.erase(it);
  CHECK(it->first == 10);

  map = map2;
  it = map.begin();
  for (int i = 0; i < 8; i++)
    it++;
  //Erase 9 (root)
  it = map.erase(it);
  CHECK(it->first == 10);

  map = map2;
  items.clear();
  for (int i = 1; i <= 20; i++)
    items.insert(i);

  //Check deletion of each element only
  for (int i = 0; i < nItems; i++)
  {
    items.erase(keys[i]);
    map.erase(keys[i]);
    std::set<int>::iterator sit = items.begin();
    for (auto kv : map)
    {
      CHECK(kv.first == *sit);
      CHECK(kv.second == 2 * *sit);
      sit++;
    }
    map = map2;
    items.insert(keys[i]);
  }

  //Check deletion of each element only via iterator
  map = map2;
  for (int i = 0; i < nItems; i++)
  {
    it = map.begin();
    for (int j = 0; j < keys[i] - 1; j++)
      it++;

    items.erase(keys[i]);
    it = map.erase(it);

    if (keys[i] == 20)
    {
      CHECK(it == map.end());
    }
    else
    {
      CHECK(it->first == keys[i] + 1);
    }

    std::set<int>::iterator sit = items.begin();
    for (auto kv : map)
    {
      CHECK(kv.first == *sit);
      CHECK(kv.second == 2 * *sit);
      sit++;
    }
    map = map2;
    items.insert(keys[i]);
  }

  //----------------------------------------------------------------------------------------
  //Clearing
  //----------------------------------------------------------------------------------------
  map.clear();
  CHECK(map.empty());
}