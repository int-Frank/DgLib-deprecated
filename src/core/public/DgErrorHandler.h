//! @file DgErrorHandler.h
//!
//! @author Frank Hart
//! @date 22/08/2016

//TODO update this to later version
#ifndef DGERRORHANDLER_H
#define DGERRORHANDLER_H

//! Macro for calling __DG_ASSERT()
#define DG_ASSERT(...) EXPAND(GET_MACRO(__VA_ARGS__, DG_ASSERT_2, DG_ASSERT_1)(__VA_ARGS__))


//impl...
#define EXPAND(x) x

#define DG_ASSERT_2(val, message) Dg::impl::__DG_ASSERT(val, message, __LINE__, __FILE__, __func__)
#define DG_ASSERT_1(val) Dg::impl::__DG_ASSERT(val, "No message", __LINE__, __FILE__, __func__)

#define GET_MACRO(_1,_2,NAME,...) NAME

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgUtility_functions
    //!
    //! Writes output to a crash file and aborts program.
    //!
    //! @param[in] val Value to be tested (0 fails)
    //! @param[in] message Optional message to write.
    //! @param[in] lineNumber Line number supplied by macro
    //! @param[in] fileName File name supplied by macro
    //! @param[in] functionName Function name supplied by macro
    void __DG_ASSERT(bool val
                   , char const * message
                   , int lineNumber
                   , char const * fileName
                   , char const * functionName);
  }
}

#endif