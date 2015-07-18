//#include "TestHarness.h"
//#include "dg_map_s.h"
//#include <vector>
//
//
//struct testClass
//{
//  static int s_counter;
//
//  int m_id;
//
//  static void Init() { s_counter = 0; }
//
//  testClass() { m_id = s_counter; s_counter++; printf("%d constructed!\n", m_id); }
//  ~testClass(){ printf("%d destroyed!\n", m_id); }
//
//  testClass(const testClass& a_other)
//  {
//    m_id = s_counter; s_counter++;
//    printf("%d constructed from %d\n", m_id, a_other.m_id);
//  }
//
//  testClass& operator=(const testClass& a_other)
//  {
//    m_id = s_counter; s_counter++;
//    printf("%d assigned from %d\n", m_id, a_other.m_id);
//    return *this;
//  }
//};
//
//int testClass::s_counter = 0;
//
////--------------------------------------------------------------------------------
////	Dg::map_s
////--------------------------------------------------------------------------------
////TEST(Stack_dg_map_s, creation_dg_map_s)
////{
////  Dg::map<int, testClass> m;
////
////  testClass t0;
////  m.insert(t0, t0.m_id);
////
////  testClass t1;
////  m.insert(t1, t1.m_id);
////
////  testClass t2;
////  m.insert(t2, t2.m_id);
////
////  m.erase(t1.m_id);
////  m.erase(t0.m_id);
////  m.erase(t2.m_id);
////  m.erase(t1.m_id);
////
////  m.insert(t0, t0.m_id);
////  m.insert(t1, t1.m_id);
////  m.insert(t2, t2.m_id);
////
////}