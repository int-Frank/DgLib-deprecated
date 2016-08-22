#ifndef DGERRORHANDLER_H
#define DGERRORHANDLER_H

#define EXPAND(x) x

#define DG_ASSERT_2(val, message) __DG_ASSERT(val, message, __LINE__, __FILE__, __func__)
#define DG_ASSERT_1(val) __DG_ASSERT(val, "No message", __LINE__, __FILE__, __func__)

#define GET_MACRO(_1,_2,NAME,...) NAME
#define DG_ASSERT(...) EXPAND(GET_MACRO(__VA_ARGS__, DG_ASSERT_2, DG_ASSERT_1)(__VA_ARGS__))


//! @ingroup DgUtility_functions
//!
//! Writes output to crash file and aborts program.
//!
//! @param[in] val Value to be tested (0 fails)
//! @param[in] message Optional message to write.
void __DG_ASSERT(int val 
               , char const * message
               , int lineNumber
               , char const * fileName
               , char const * functionName);

#endif