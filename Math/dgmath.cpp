//! @file dgmath.cpp
//!
//! @author Frank Hart
//! @date 4/8/2015
//! 
//! Definitions from dgmath.cpp

#include "dgmath.h"

namespace Dg
{
	namespace impl
	{
		static uint32_t const TAB32[32] = {
		  0, 9, 1, 10, 13, 21, 2, 29,
		  11, 14, 16, 18, 22, 25, 3, 30,
		  8, 12, 20, 28, 15, 17, 24, 7,
		  19, 27, 23, 6, 26, 5, 4, 31 };
	}


	//--------------------------------------------------------------------------------
	//	@	Log2()
	//--------------------------------------------------------------------------------
	uint32_t Log2(uint32_t input)
	{
		input |= (input >> 1);
		input |= (input >> 2);
		input |= (input >> 4);
		input |= (input >> 8);
		input |= (input >> 16);
		return impl::TAB32[(input * 0x07C4ACDD) >> 27];

	}	//End: Log2()


	//--------------------------------------------------------------------------------
	//	@	FloorPower2()
	//--------------------------------------------------------------------------------
	uint32_t FloorPower2(uint32_t input)
	{
		input |= (input >> 1);
		input |= (input >> 2);
		input |= (input >> 4);
		input |= (input >> 8);
		input |= (input >> 16);
		input ^= (input >> 1);
		return input;

	}	//End: FloorPower2()


	//--------------------------------------------------------------------------------
	//	@	NextPower2()
	//--------------------------------------------------------------------------------
	uint32_t NextPower2(uint32_t input)
	{
		--input;
		input |= (input >> 1);
		input |= (input >> 2);
		input |= (input >> 4);
		input |= (input >> 8);
		input |= (input >> 16);
		++input;
		return input;

	}	//End: NextPower2()
}
