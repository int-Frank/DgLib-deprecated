//! @file RNG.cpp
//!
//! @author Frank Hart
//! @date 4/8/2015
//!
//! Class definitions: RNG

#include "DgRNG.h"

namespace Dg
{

	//--------------------------------------------------------------------------------
	//	@	Statics
	//--------------------------------------------------------------------------------
	unsigned int RNG::m_w = 521288629;
	unsigned int RNG::m_z = 362436069;


	//--------------------------------------------------------------------------------
	//	@	RNG::SetSeed()
	//--------------------------------------------------------------------------------
	void RNG::SetSeed(unsigned int seed)
	{
		m_w = seed;

	}	//End: RNG::SetSeed()


		//--------------------------------------------------------------------------------
		//	@	RNG::SetSeed()
		//--------------------------------------------------------------------------------
	void RNG::SetSeed(unsigned int u, unsigned int v)
	{
		if (u != 0) m_w = u;
		if (v != 0) m_z = v;

	}	//End: RNG::SetSeed()


		//--------------------------------------------------------------------------------
		//	@	RNG::GetUint()
		//--------------------------------------------------------------------------------
	unsigned int RNG::GetUint()
	{
		m_z = 36969 * (m_z & 65535) + (m_z >> 16);
		m_w = 18000 * (m_w & 65535) + (m_w >> 16);
		return (m_z << 16) + m_w;

	}	//End: RNG::GetUint()

  bool RNG::FlipACoin()
  {
    return (GetUint() & 1);
  }

		//--------------------------------------------------------------------------------
		//	@	RNG::GetUint()
		//--------------------------------------------------------------------------------
	unsigned int RNG::GetUint(unsigned int a, unsigned int b)
	{
		if (a >= b)
		{
			return a;
		}


		unsigned int val = GetUint();
		unsigned int range = b - a;
		return a + (val % range);

	}	//End: RNG::GetUint()

}