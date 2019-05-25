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

  for (auto kv : map)
  {
    std::cout << kv.first << ", ";
  }
}