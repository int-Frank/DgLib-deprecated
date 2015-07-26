//! @file ResourceKey.h
//!
//! @author: Frank B. Hart
//! @date 25/7/2015
//!
//! Class declaration: RKey

#ifndef RESOURCEKEY_H
#define RESOURCEKEY_H

#include <stdint.h>

namespace Dg
{
  //! A key associated with Resources.
  //! A key is a uint32_t.
  class RKey
  {
  public:

    RKey() : m_value(0) {}
    RKey(uint32_t a_value) : m_value(a_value) {}

    ~RKey() {}

    RKey(RKey const & a_other) : m_value(a_other.m_value) {}
    RKey & operator=(RKey const & a_other)
    {
      m_value = a_other.m_value;
      return *this;
    }

    bool operator==(RKey const & a_other) const { return m_value == a_other.m_value; }
    bool operator!=(RKey const & a_other) const { return m_value != a_other.m_value; }

    operator uint32_t() { return m_value; }

    //! Set a set of bits in the key
    template<unsigned Position, unsigned Length>
    void SetBitSet(uint32_t a_value)
    {
      static_assert((Position + Length) < 33, "Values out of bounds.");

      unsigned mask = 0;
      for (unsigned i = 0; i < Length; ++i)
      {
        mask <<= 1;
        mask |= 1;
      }
      a_value &= mask;
      mask <<= Position;
      mask = ~mask;
      m_value &= mask;
      m_value |= (a_value << Position);
    }

    //! Get a set of bits within the mask
    template<unsigned Position, unsigned Length>
    uint32_t GetBitSet() 
    { 
      static_assert((Position + Length) < 33, "Values out of bounds.");

      uint32_t result = m_value;
      result >>= Position;

      unsigned mask = 0;
      for (unsigned i = 0; i < Length; ++i)
      {
        mask <<= 1;
        mask |= 1;
      }

      return result & mask;
    }

    //! Is this a valid key?
    bool IsValid() { return m_value != 0; }

  private:

    uint32_t m_value;
  };
}

#endif