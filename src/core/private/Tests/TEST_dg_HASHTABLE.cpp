//#include <list>
//#include <iostream>
//
//#include "TestHarness.h"
//#include "DgOpenHashTable.h"
//
//typedef unsigned Key;
//
//int g_constructors = 0;
//int g_copyConstructors = 0;
//int g_assignments = 0;
//int g_destructors = 0;
//
//class C
//{
//public:
//
//  C() : m(0) { g_constructors++; }
//  C(int a) : m(a) { g_constructors++; }
//  ~C() { g_destructors++; }
//  C(C const & a) : m(a.m) { g_copyConstructors++; }
//  C & operator=(C const & a) 
//  { 
//    m = a.m;
//    g_assignments++; 
//    return *this;
//  }
//
//  bool operator==(C const & a) const { return a.m == m; }
//  bool operator!=(C const & a) const { return a.m != m; }
//
//  int m;
//};
//
//int HaveSameItems(Dg::OpenHashTable<Key, C> ht0, Dg::OpenHashTable<Key, C> ht1)
//{
//  if (ht0.Size() != ht1.Size())
//  {
//    return __LINE__;
//  }
//
//  Dg::OpenHashTable<Key, C>::iterator it0 = ht0.Begin();
//  Dg::OpenHashTable<Key, C>::iterator it1 = ht1.Begin();
//  while (it0 != ht0.End())
//  {
//    if (*it0 != *it1)
//    {
//      return __LINE__;
//    }
//    it0++;
//    it1++;
//  }
//  return 0;
//}
//
//
//int Compare(Dg::OpenHashTable<Key, C> a_ht, std::list<C> a_list)
//{
//  //Check sizes
//  if (a_ht.Size() != a_list.size())
//  {
//    return __LINE__;
//  }
//
//  //Have same items?
//  std::list<C>::iterator l_it = a_list.begin();
//  for (l_it; l_it != a_list.end(); ++l_it)
//  {
//    if (!a_ht.At(l_it->m))
//    {
//      return __LINE__;
//    }
//  }
//
//  //Make sure elments match, use interator++
//  std::list<C> newList(a_list);
//  Dg::OpenHashTable<Key, C>::iterator it = a_ht.Begin();
//  for(it; it != a_ht.End(); it++)
//  {
//    std::list<C>::iterator l_it = std::find(newList.begin(), newList.end(), *it);
//    if (l_it == newList.end())
//    {
//      return __LINE__;
//    }
//    newList.erase(l_it);
//  }
//  if (newList.size() != 0)
//  {
//    return __LINE__;
//  }
//
//  //Make sure elments match, use iterator--
//  newList = a_list;
//  it = a_ht.End();
//  it--;
//  for (it; it != a_ht.End(); it--)
//  {
//    std::list<C>::iterator l_it = std::find(newList.begin(), newList.end(), *it);
//    if (l_it == newList.end())
//    {
//      return __LINE__;
//    }
//    newList.erase(l_it);
//  }
//  if (newList.size() != 0)
//  {
//    return __LINE__;
//  }
//
//  //Make sure elments match, use const_interator++
//  newList = a_list;
//  Dg::OpenHashTable<Key, C>::const_iterator cit = a_ht.CBegin();
//  for (cit; cit != a_ht.CEnd(); cit++)
//  {
//    std::list<C>::iterator l_it = std::find(newList.begin(), newList.end(), *cit);
//    if (l_it == newList.end())
//    {
//      return __LINE__;
//    }
//    newList.erase(l_it);
//  }
//  if (newList.size() != 0)
//  {
//    return __LINE__;
//  }
//
//  //Make sure elments match, use const_iterator--
//  newList = a_list;
//  cit = a_ht.CEnd();
//  for (cit; --cit != a_ht.CEnd() ;)
//  {
//    std::list<C>::iterator l_it = std::find(newList.begin(), newList.end(), *cit);
//    if (l_it == newList.end())
//    {
//      return __LINE__;
//    }
//    newList.erase(l_it);
//  }
//  if (newList.size() != 0)
//  {
//    return __LINE__;
//  }
//
//  //Construction
//  Dg::OpenHashTable<Key, C> newHT(a_ht);
//  if (newHT.PoolSlotsWasted()) return __LINE__;
//  int res = HaveSameItems(a_ht, newHT);
//  if (res != 0) return res;
//
//  newHT = a_ht;
//  if (newHT.PoolSlotsWasted()) return __LINE__;
//  res = HaveSameItems(a_ht, newHT);
//  if (res != 0) return res;
//
//  Dg::OpenHashTable<Key, C> moved(std::move(newHT));
//  if (moved.PoolSlotsWasted()) return __LINE__;
//  res = HaveSameItems(a_ht, moved);
//  if (res != 0) return res;
//  res = HaveSameItems(a_ht, newHT);
//  if (res == 0) return res;
//
//  newHT = moved;
//  if (newHT.PoolSlotsWasted()) return __LINE__;
//  moved = std::move(newHT);
//  if (moved.PoolSlotsWasted()) return __LINE__;
//  res = HaveSameItems(a_ht, moved);
//  if (res != 0) return res;
//  res = HaveSameItems(a_ht, newHT);
//  if (res == 0) return res;
//
//  return 0;
//}
//
//int AddNewItem(Key k, Dg::OpenHashTable<Key, C> & a_ht, std::list<C> & a_list)
//{
//  if (a_ht.At(k))
//  {
//    return __LINE__;
//  }
//
//  float oldLF = a_ht.LoadFactor();
//  float nItems = float(a_ht.Size());
//  float oldBucketCount = float(a_ht.BucketCount());
//  float maxLF = float(a_ht.MaxLoadFactor());
//  size_t oldSize = a_ht.Size();
//  bool shouldRehash = ((nItems + 1.0f) / oldBucketCount) > maxLF;
//
//  //Insert dummy
//  a_ht.Insert(k, C(-1));
//  if (a_ht.PoolSlotsWasted()) return __LINE__;
//
//  if (a_ht.Size() != oldSize + 1) return __LINE__;
//  if (!a_ht.At(k)) return __LINE__;
//  if (a_ht.At(k)->m != -1) return __LINE__;
//  if (a_ht.InsertNoOverwrite(k, C(k))->m != -1) return __LINE__;
//  if (a_ht.PoolSlotsWasted()) return __LINE__;
//
//  a_ht.Erase(k);
//  if (a_ht.Size() != oldSize) return __LINE__;
//  if (a_ht.At(k)) return __LINE__;
//
//  //Insert real
//  a_ht.Insert(k, C(k));
//  if (!a_ht.At(k)) return __LINE__;
//  if (a_ht.At(k)->m != k) return __LINE__;
//  if (a_ht.PoolSlotsWasted()) return __LINE__;
//  if (shouldRehash && a_ht.BucketCount() <= oldBucketCount) return __LINE__;
//
//  a_list.push_back(C(k));
//  return 0;
//}
//
//TEST(Stack_dg_HashTable, creation_dg_HashTable)
//{
//  Dg::OpenHashTable<Key, C> ht;
//  std::list<C> lst;
//  int failLine = 0;
//  for (int i = 0; i < 100; ++i)
//  {
//    failLine = AddNewItem(i, ht, lst);
//    CHECK(failLine == 0);
//  }
//  failLine = Compare(ht, lst);
//  CHECK(failLine == 0);
//}