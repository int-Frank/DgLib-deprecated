//! @file DgTypes.h
//!
//! @author Frank Hart
//! @date 3/07/2016

#ifndef DGTYPES_H
#define DGTYPES_H

#define IS_FLAGGED(x, y) static_cast<bool>(x & (1 << y))

namespace Dg
{
  //! @ingroup DgUtility_constants
  //! Error codes
  enum class ErrorCode
  {
    None = 0,
    Failure,
    Undefined,
    OutOfBounds,
    Duplicate,
    FailedToOpenFile,
    BadInput
  };
}

#endif