#include <list>
#include <iostream>

#include "TestHarness.h"
#include "DgOpenHashTable.h"

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

int HaveSameItems(Dg::OpenHashTable<Key, C> ht0, Dg::OpenHashTable<Key, C> ht1)
{
  if (ht0.size() != ht1.size())
  {
    return __LINE__;
  }

  Dg::OpenHashTable<Key, C>::iterator it0 = ht0.begin();
  Dg::OpenHashTable<Key, C>::iterator it1 = ht1.begin();
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


int Compare(Dg::OpenHashTable<Key, C> a_ht, std::list<C> a_list)
{
  //Check sizes
  if (a_ht.size() != a_list.size())
  {
    return __LINE__;
  }

  //Have same items?
  std::list<C>::iterator l_it = a_list.begin();
  for (l_it; l_it != a_list.end(); ++l_it)
  {
    if (!a_ht.at(l_it->m))
    {
      return __LINE__;
    }
  }

  //Make sure elments match, use interator++
  std::list<C> newList(a_list);
  Dg::OpenHashTable<Key, C>::iterator it = a_ht.begin();
  for(it; it != a_ht.end(); it++)
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

  //Make sure elments match, use iterator--
  newList = a_list;
  it = a_ht.end();
  it--;
  for (it; it != a_ht.end(); it--)
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
  Dg::OpenHashTable<Key, C>::const_iterator cit = a_ht.cbegin();
  for (cit; cit != a_ht.cend(); cit++)
  {
    std::list<C>::iterator l_it = std::find(newList.begin(), newList.end(), *cit);
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

  //Make sure elments match, use const_iterator--
  newList = a_list;
  cit = a_ht.cend();
  for (cit; --cit != a_ht.cend() ;)
  {
    std::list<C>::iterator l_it = std::find(newList.begin(), newList.end(), *cit);
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
  Dg::OpenHashTable<Key, C> newHT(a_ht);
  if (newHT.PoolSlotsWasted()) return __LINE__;
  int res = HaveSameItems(a_ht, newHT);
  if (res != 0) return res;

  newHT = a_ht;
  if (newHT.PoolSlotsWasted()) return __LINE__;
  res = HaveSameItems(a_ht, newHT);
  if (res != 0) return res;

  Dg::OpenHashTable<Key, C> moved(std::move(newHT));
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

int AddNewItem(Key k, Dg::OpenHashTable<Key, C> & a_ht, std::list<C> & a_list)
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

TEST(Stack_dg_HashTable, creation_dg_HashTable)
{
  Dg::OpenHashTable<Key, C> ht;
  std::list<C> lst;
  int failLine = 0;
  for (int i = 0; i < 100; ++i)
  {
    failLine = AddNewItem(i, ht, lst);
    CHECK(failLine == 0);
  }
  failLine = Compare(ht, lst);
  CHECK(failLine == 0);
}