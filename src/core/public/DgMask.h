//! @file DgMask.h
//!
//! @author: Frank B. Hart
//! @date 29/06/2018
//!
//! Class declaration: Mask

#ifndef DGMASK_H
#define DGMASK_H

#include <cstring>
#include <stdint.h>
#include <limits.h>

namespace Dg
{
  //! @ingroup DgUtility_types
  //!
  //! @class Mask
  //!
  //! 
  //!
  //! @author Frank Hart
  //! @date 29/06/2016
  template<size_t Size>
  class Mask
  {
    typedef uint8_t DataType;
    static const size_t s_DataBits = sizeof(DataType) * CHAR_BIT;
    static const size_t s_arraySize = (Size + (s_DataBits - 1)) / s_DataBits;
  public:

    Mask()
      : m_data {}
    {}

    ~Mask() {}

    Mask(Mask const & a_other)
    {
      memcpy(m_data, a_other.m_data, s_arraySize);
    }

    Mask & operator=(Mask const & a_other)
    {
      if (this != &a_other)
      {
        memcpy(m_data, a_other.m_data, s_arraySize);
      }

      return *this
    }

    bool operator==(Mask const & a_other) const
    {
      return memcmp(m_data, a_other.m_data, s_arraySize) == 0;
    }

    bool operator!=(Mask const & a_other) const
    {
      return memcmp(m_data, a_other.m_data, s_arraySize) != 0;
    }

    void SetAllOn()
    {
      memset(m_data, -1, s_arraySize);
    }

    void SetAllOff()
    {
      memset(m_data, 0, s_arraySize);
    }

    void SetOn(size_t a_index)
    {
      size_t arrayIndex = a_index / s_DataBits;
      size_t bitIndex = a_index % s_DataBits;
      DataType bitMask = static_cast<DataType>(1) << bitIndex;

      m_data[arrayIndex] |= bitMask;
    }

    void SetOff(size_t a_index)
    {
      size_t arrayIndex = a_index / s_DataBits;
      size_t bitIndex = a_index % s_DataBits;
      DataType bitMask = ~static_cast<DataType>(1) << bitIndex;

      m_data[arrayIndex] &= bitMask;
    }

    bool IsOn(int a_index) const
    {
      size_t arrayIndex = a_index / s_DataBits;
      size_t bitIndex = a_index % s_DataBits;
      DataType bitMask = static_cast<DataType>(1) << bitIndex;

      return (m_data[arrayIndex] & bitMask) != 0;
    }

  private:

    DataType m_data[s_arraySize];
  };
}

#endif