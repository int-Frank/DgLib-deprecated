/*!
 * @file RNG.cpp
 *
 * @author Frank Hart
 * @date 11/01/2014
 *
 * class definitions: RNG
 */

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
  //		Use one value to set the seed
  //--------------------------------------------------------------------------------
  void SimpleRNG::SetSeed(unsigned int seed)
  {
    m_w = seed;

  }	//End: SimpleRNG::SetSeed()


  //--------------------------------------------------------------------------------
  //	@	SimpleRNG::SetSeed()
  //--------------------------------------------------------------------------------
  //		Use two values to set the seed
  //--------------------------------------------------------------------------------
  void SimpleRNG::SetSeed(unsigned int u, unsigned int v)
  {
    if (u != 0) m_w = u;
    if (v != 0) m_z = v;

  }	//End: SimpleRNG::SetSeed()


  //--------------------------------------------------------------------------------
  //	@	SimpleRNG::GetUint()
  //--------------------------------------------------------------------------------
  //		This is the heart of the generator.
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
  //		This is the heart of the generator.
  //--------------------------------------------------------------------------------
  unsigned int SimpleRNG::GetUint(unsigned int a, unsigned int b)
  {
    if (a > b)
    {
      return a;
    }
    else if (a == b)
    {
      return a;
    }


    unsigned int val = GetUint();
    unsigned int range = b - a;
    return a + (val % range);

  }	//End: SimpleRNG::GetUint()
}



