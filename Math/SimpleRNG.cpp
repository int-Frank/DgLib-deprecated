//! @file SimpleRNG.cpp
//!
//! @author Frank Hart
//! @date 4/8/2015
//!
//! Class definitions: SimpleRNG

#include "SimpleRNG.h"

namespace Dg
{

  //--------------------------------------------------------------------------------
  //	@	Statics
  //--------------------------------------------------------------------------------
  unsigned int SimpleRNG::m_w = 521288629;
  unsigned int SimpleRNG::m_z = 362436069;


  //--------------------------------------------------------------------------------
  //	@	SimpleRNG::SetSeed()
  //--------------------------------------------------------------------------------
  void SimpleRNG::SetSeed(unsigned int seed)
  {
    m_w = seed;

  }	//End: SimpleRNG::SetSeed()


  //--------------------------------------------------------------------------------
  //	@	SimpleRNG::SetSeed()
  //--------------------------------------------------------------------------------
  void SimpleRNG::SetSeed(unsigned int u, unsigned int v)
  {
    if (u != 0) m_w = u;
    if (v != 0) m_z = v;

  }	//End: SimpleRNG::SetSeed()


  //--------------------------------------------------------------------------------
  //	@	SimpleRNG::GetUint()
  //--------------------------------------------------------------------------------
  unsigned int SimpleRNG::GetUint()
  {
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;

  }	//End: SimpleRNG::GetUint()


  //--------------------------------------------------------------------------------
  //	@	SimpleRNG::GetUint()
  //--------------------------------------------------------------------------------
  unsigned int SimpleRNG::GetUint(unsigned int a, unsigned int b)
  {
    if (a >= b)
    {
      return a;
    }


    unsigned int val = GetUint();
    unsigned int range = b - a;
    return a + (val % range);

  }	//End: SimpleRNG::GetUint()

  }