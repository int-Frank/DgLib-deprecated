#include <iostream>

#include "TestHarness.h"
#include "DgCircularDoublyLinkedList.h"

typedef int t;
typedef Dg::CircularDoublyLinkedList<t>        DgCList;

bool AreSame(DgCList const & a_l0, DgCList const & a_l1)
{
  if (a_l0.size() != a_l1.size())
    return false;

  auto it0 = a_l0.chead();
  auto it1 = a_l1.chead();
  do
  {
    if (*it0 != *it1)
      return false;
    it0++;
    it1++;
  } while (it0 != a_l0.chead());
  return true;
}

void Print(DgCList const & a_l)
{
  auto it = a_l.chead();
  do
  {
    std::cout << *it << ", ";
    it++;
  } while (it != a_l.chead());
  std::cout << " (head) " << *it << ", ";
  std::cout << " (head--) " << *(--a_l.chead()) << '\n';
}

#define BRK do{char t(0); std::cin >> t;}while(false)

TEST(Stack_dg_ListCircular, creation_dg_ListCircular)
{
  DgCList list, list2;

  for (int i = 0; i < 17; ++i)
    list.push_back(i);

  list2 = list;
  list.clear();

  for (int i = 0; i < 17; ++i)
    list.push_back(i);

  CHECK(AreSame(list, list2));

  auto it = list.head();
  for (int i = 0; i < 8; ++i)
    ++it;

  for (int i = 0; i < 3; ++i)
    it = list.erase(it);

  it++; 
  ++it;

  list.insert(it, -1);

  list2 = list;
  DgCList list3(list2);
  CHECK(AreSame(list, list2));
  CHECK(AreSame(list2, list3));
  CHECK(AreSame(list, list3));
}