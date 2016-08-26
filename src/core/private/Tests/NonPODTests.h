#pragma once


class TestClass
{
public:

  TestClass() { s_constructors++; }
  ~TestClass() { s_destructors++; }

  TestClass(TestClass const & a) { s_copyConsts++; }
  TestClass & operator=(TestClass const & a) { s_assignments++; }

  TestClass(TestClass && a) { s_moveConsts++; }
  TestClass & operator=(TestClass && a) { s_moveAssigns++; }

  static void Reset() 
  {
    s_constructors = 0;
    s_destructors = 0;
    s_copyConsts = 0;
    s_assignments = 0;
    s_moveConsts = 0;
    s_moveAssigns = 0;
  }

  static int s_constructors;
  static int s_destructors;
  static int s_copyConsts;
  static int s_assignments;
  static int s_moveConsts;
  static int s_moveAssigns;
};

