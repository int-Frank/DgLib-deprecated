#include <list>
#include <iostream>

#include "TestHarness.h"
#include "Dg_HashTable.h"

typedef unsigned Key;

int g_constructors = 0;
int g_copyConstructors = 0;
int g_assignments = 0;
int g_destructors = 0;

class C
{
public:

  C() : m(0) { g_constructors++; }
  C(int a) : m(a) { g_constructors++; }
  ~C() { g_destructors++; }
  C(C const & a) : m(a.m) { g_copyConstructors++; }
  C & operator=(C const & a) 
  { 
    m = a.m;
    g_assignments++; 
    return *this;
  }

  bool operator==(C const & a) const { return a.m == m; }
  bool operator!=(C const & a) const { return a.m != m; }

  int m;
};

int HaveSameItems(Dg::HashTable<Key, C, true> ht0, Dg::HashTable<Key, C, true> ht1)
{
  if (ht0.size() != ht1.size())
  {
    return __LINE__;
  }

  Dg::HashTable<Key, C, true>::iterator it0 = ht0.begin();
  Dg::HashTable<Key, C, true>::iterator it1 = ht1.begin();
  while (it0 != ht0.end())
  {
    if (*it0 != *it1)
    {
      return __LINE__;
    }
    it0++;
    it1++;
  }
  return 0;
}

int Compare(Dg::HashTable<Key, C, true> a_ht, std::list<C> a_list)
{
  //Check sizes
  if (a_ht.size() != a_list.size())
  {
    return __LINE__;
  }

  //Have same items?
  std::list<C>::iterator l_it = a_list.begin();
  while (l_it != a_list.end())
  {
    if (!a_ht.at(l_it->m))
    {
      return __LINE__;
    }
  }

  //Make sure elments match, use interator++
  std::list<C> newList(a_list);
  Dg::HashTable<Key, C, true>::iterator it = a_ht.begin();
  while (it != a_ht.end())
  {
    std::list<C>::iterator l_it = std::find(newList.begin(), newList.end(), *it);
    if (l_it == newList.end())
    {
      return __LINE__;
    }
    newList.erase(l_it);
    it++;
  }
  if (newList.size() != 0)
  {
    return __LINE__;
  }

  //Make sure elments match, use iterator--
  newList = a_list;
  it = a_ht.end();
  while (--it != a_ht.end())
  {
    std::list<C>::iterator l_it = std::find(newList.begin(), newList.end(), *it);
    if (l_it == newList.end())
    {
      return __LINE__;
    }
    newList.erase(l_it);
  }
  if (newList.size() != 0)
  {
    return __LINE__;
  }

  //Make sure elments match, use const_interator++
  newList = a_list;
  Dg::HashTable<Key, C, true>::const_iterator cit = a_ht.cbegin();
  while (cit != a_ht.cend())
  {
    std::list<C>::iterator l_it = std::find(newList.begin(), newList.end(), *it);
    if (l_it == newList.end())
    {
      return __LINE__;
    }
    newList.erase(l_it);
    it++;
  }
  if (newList.size() != 0)
  {
    return __LINE__;
  }

  //Make sure elments match, use const_iterator--
  newList = a_list;
  cit = a_ht.cend();
  while (--cit != a_ht.cend())
  {
    std::list<C>::iterator l_it = std::find(newList.begin(), newList.end(), *it);
    if (l_it == newList.end())
    {
      return __LINE__;
    }
    newList.erase(l_it);
  }
  if (newList.size() != 0)
  {
    return __LINE__;
  }

  //Construction
  Dg::HashTable<Key, C, true> newHT(a_ht);
  if (newHT.PoolSlotsWasted()) return __LINE__;
  int res = HaveSameItems(a_ht, newHT);
  if (res != 0) return res;

  newHT = a_ht;
  if (newHT.PoolSlotsWasted()) return __LINE__;
  res = HaveSameItems(a_ht, newHT);
  if (res != 0) return res;

  Dg::HashTable<Key, C, true> moved(std::move(newHT));
  if (moved.PoolSlotsWasted()) return __LINE__;
  res = HaveSameItems(a_ht, moved);
  if (res != 0) return res;
  res = HaveSameItems(a_ht, newHT);
  if (res == 0) return res;

  newHT = moved;
  if (newHT.PoolSlotsWasted()) return __LINE__;
  moved = std::move(newHT);
  if (moved.PoolSlotsWasted()) return __LINE__;
  res = HaveSameItems(a_ht, moved);
  if (res != 0) return res;
  res = HaveSameItems(a_ht, newHT);
  if (res == 0) return res;

  return 0;
}

int AddNewItem(Key k, Dg::HashTable<Key, C> & a_ht, std::list<C> & a_list)
{
  if (a_ht.at(k))
  {
    return __LINE__;
  }

  float oldLF = a_ht.load_factor();
  float nItems = float(a_ht.size());
  float oldBucketCount = float(a_ht.bucket_count());
  float maxLF = float(a_ht.max_load_factor());
  size_t oldSize = a_ht.size();
  bool shouldRehash = ((nItems + 1.0f) / oldBucketCount) > maxLF;

  //Insert dummy
  a_ht.insert(k, C(-1));
  if (a_ht.PoolSlotsWasted()) return __LINE__;

  if (a_ht.size() != oldSize + 1) return __LINE__;
  if (!a_ht.at(k)) return __LINE__;
  if (a_ht.at(k)->m != -1) return __LINE__;
  if (a_ht.insert_no_overwrite(k, C(k))->m != -1) return __LINE__;
  if (a_ht.PoolSlotsWasted()) return __LINE__;

  a_ht.erase(k);
  if (a_ht.size() != oldSize) return __LINE__;
  if (a_ht.at(k)) return __LINE__;

  //Insert real
  a_ht.insert(k, C(k));
  if (!a_ht.at(k)) return __LINE__;
  if (a_ht.at(k)->m != k) return __LINE__;
  if (a_ht.PoolSlotsWasted()) return __LINE__;
  if (shouldRehash && a_ht.bucket_count() <= oldBucketCount) return __LINE__;

  a_list.push_back(C(k));
  return 0;
}

TEST(Stack_dg_HashTable_pod, creation_dg_HashTable_pod)
{
  Dg::HashTable<Key, C, true> ht;
  std::list<C> lst;
  int failLine = 0;
  for (int i = 0; i < 100; ++i)
  {
    failLine = AddNewItem(i, ht, lst);
    std::cout << "\n\n" << ht;
    if (failLine)
    {
      std::cout << "\n\nFAIL: " << failLine;
    }
    CHECK(failLine == 0);
  }
  failLine = Compare(ht, lst);
  if (failLine)
  {
    std::cout << "\n\nFAIL: " << failLine;
  }
  CHECK(failLine == 0);
}