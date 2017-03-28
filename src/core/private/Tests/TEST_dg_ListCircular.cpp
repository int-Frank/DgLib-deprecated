#include "TestHarness.h"
#include "DgCircularDoublyLinkedList.h"

typedef int t;
typedef Dg::CircularDoublyLinkedList<t>        DgList;

TEST(Stack_dg_ListCircular, creation_dg_ListCircular)
{
  DgList list;

  for (int i = 0; i < 17; ++i)
  {
    list.push_back(i);
  }

  auto it = list.head();
  for (int i = 0; i < 8; ++i)
  {
    ++it;
  }

  for (int i = 0; i < 3; ++i)
  {
    it = list.erase(it);
  }

  it++; 
  it++;

  list.insert(it, -1);

  DgList list2(list);
  DgList list3 = list;

  //list.Print();
  //list2.Print();
  //list3.Print();
}