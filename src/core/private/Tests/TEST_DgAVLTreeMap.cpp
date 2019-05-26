#include <iostream>

#include "TestHarness.h"
#include "DgAVLTreeMap.h"


typedef Dg::AVLTreeMap<int, double> Map;

TEST(Stack_dg_AVLTreeMap, creation_dg_AVLTreeMap)
{
  Map map;

  int const nItems = 20;
  int keys[nItems] = {5, 12, 3, 19, 9, 7, 2, 20, 18, 15, 10, 11, 1, 4, 6, 8, 13, 14, 16, 17};

  for (int i = 0; i < nItems; i++)
  {
    map.insert(keys[i], 2 * keys[i]);
  }

  int value = 1;
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
    value++;
  }

  Map::const_iterator cit = map.cend();
  for (int i = 20; i > 0; i--)
  {
    cit--;
    CHECK(cit->first == i);
  }

  value = 1;
  for (Map::const_iterator cit = map.cbegin(); cit != map.cend(); cit++)
  {
    CHECK(cit->first == value);
    value++;
  }
}