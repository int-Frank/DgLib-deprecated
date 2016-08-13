#ifndef DGFIXEDPOINT_H
#define DGFIXEDPOINT_H

#include <stdint.h>

namespace Dg
{
  namespace impl
  {
    struct Error_promote_type_not_specialized_for_this_type
    { 
    };

    template<typename I, uint8_t F>
    struct QueryFPType 
    { 
      static bool const valid = false;
      static int const nIntegerBits = 0;
      typedef Error_promote_type_not_specialized_for_this_type promoteType;
    };

    template<uint8_t F>
    struct QueryFPType<int8_t, F>
    {
      static bool const valid = (F <= 7);
      static int const nIntegerBits = 7 - F;
      typedef int16_t PromoteType;
    };

    template<uint8_t F>
    struct QueryFPType<uint8_t, F>
    {
      static bool const valid = (F <= 8);
      static int const nIntegerBits = 8 - F;
      typedef uint16_t PromoteType;
    };

    template<uint8_t F>
    struct QueryFPType<int16_t, F>
    {
      static bool const valid = (F <= 15);
      static int const nIntegerBits = 15 - F;
      typedef int32_t PromoteType;
    };

    template<uint8_t F>
    struct QueryFPType<uint16_t, F>
    {
      static bool const valid = (F <= 16);
      static int const nIntegerBits = 16 - F;
      typedef uint32_t PromoteType;
    };

    template<uint8_t F>
    struct QueryFPType<int32_t, F>
    {
      static bool const valid = (F <= 31);
      static int const nIntegerBits = 31 - F;
      typedef int64_t PromoteType;
    };

    template<uint8_t F>
    struct QueryFPType<uint32_t, F>
    {
      static bool const valid = (F <= 32);
      static int const nIntegerBits = 32 - F;
      typedef uint64_t PromoteType;
    };

    uint32_t const fBitMasks[32] =
    {
      0x1, 0x3, 0x7, 0xF,
      0x1F, 0x3F, 0x7F, 0xFF,
      0x1FF, 0x3FF, 0x7FF, 0xFFF,
      0x1FFF, 0x3FFF, 0x7FFF, 0xFFFF,
      0x1FFFF, 0x3FFFF, 0x7FFFF, 0xFFFFF,
      0x1FFFFF, 0x3FFFFF, 0x7FFFFF, 0xFFFFFF,
      0x1FFFFFF, 0x3FFFFFF, 0x7FFFFFF, 0xFFFFFFF,
      0x1FFFFFFF, 0x3FFFFFFF, 0x7FFFFFFF, 0xFFFFFFFF
    };

    //! We use this table to get around the compiler throwing a warning 
    //! when bit shifting by a potentially negative number.
    uint32_t const Shfts[32] =
    {
      0,  1,  2,  3,  4,  5,  6,  7,
      8,  9,  10, 11, 12, 13, 14, 15,
      16, 17, 18, 19, 20, 21, 22, 23,
      24, 25, 26, 27, 28, 29, 30, 31
    };

  }


  template<typename I, uint8_t F>
  class FixedPoint
  {
    static_assert(impl::QueryFPType<I, F>::valid, "Invalid template argument. Check input type and number of fractional bits.");
    
  public:

    FixedPoint() : m_val(0) {}
    FixedPoint(I a_val) : m_val(a_val) {}

    //! Construct from floating point
    FixedPoint(float a_val)       { SetFromFloatingPoint<float>(a_val); }
    FixedPoint(double a_val)      { SetFromFloatingPoint<double>(a_val); }
    FixedPoint(long double a_val) { SetFromFloatingPoint<long double>(a_val); }

    template<typename I2, uint8_t F2>
    explicit operator FixedPoint<I2, F2>() const;

    operator float()       const { return static_cast<float>(m_val) / Power2<F>::value; }
    operator double()      const { return static_cast<double>(m_val) / Power2<F>::value; }
    operator long double() const { return static_cast<long double>(m_val) / Power2<F>::value; }

    operator I() const { return m_val; }

    ~FixedPoint() {}
    FixedPoint(FixedPoint const &);
    FixedPoint & operator=(FixedPoint const &);

    bool operator==(FixedPoint const & a_other) const {return m_val == a_other.m_val;}
    bool operator!=(FixedPoint const & a_other) const {return m_val != a_other.m_val;}
    bool operator> (FixedPoint const & a_other) const {return m_val > a_other.m_val;}
    bool operator< (FixedPoint const & a_other) const {return m_val < a_other.m_val;}
    bool operator>=(FixedPoint const & a_other) const {return m_val >= a_other.m_val;}
    bool operator<=(FixedPoint const & a_other) const {return m_val <= a_other.m_val;}            

    bool operator !() const { return m_val == 0; }
    FixedPoint operator-() const;

    FixedPoint operator+(FixedPoint) const;
    FixedPoint operator-(FixedPoint) const;
    FixedPoint operator*(FixedPoint) const;
    FixedPoint operator/(FixedPoint) const;

    FixedPoint & operator+=(FixedPoint);
    FixedPoint & operator-=(FixedPoint);
    FixedPoint & operator*=(FixedPoint);
    FixedPoint & operator/=(FixedPoint);

    friend FixedPoint abs(FixedPoint);
    friend FixedPoint ceil(FixedPoint);
    friend FixedPoint mod(FixedPoint, FixedPoint);
    friend FixedPoint exp(FixedPoint);
    friend FixedPoint cos(FixedPoint);
    friend FixedPoint sin(FixedPoint);
    friend FixedPoint sqrt(FixedPoint);

  private:

    template<int E>
    struct Power2
    {
      static uint64_t const value = Power2<E - 1>::value * 2;
    };

    template<>
    struct Power2<0>
    {
      static uint64_t const value = 1;
    };

    template<typename T>
    void SetFromFloatingPoint(T);

  private:
    I       m_val;
  };


  template<typename I, uint8_t F>
  FixedPoint<I, F>::FixedPoint(FixedPoint<I, F> const & a_other)
    : m_val(a_other.m_val)
  {

  }


  template<typename I, uint8_t F>
  FixedPoint<I, F> & FixedPoint<I, F>::operator=(FixedPoint<I, F> const & a_other)
  {
    m_val = a_other.m_val;
    return *this;
  }

  
  template<typename I, uint8_t F>
  template<typename I2, uint8_t F2>
  FixedPoint<I, F>::operator FixedPoint<I2, F2>() const
  {
    I2 intPart = static_cast<I2>(m_val >> F);
    I ifracPart = m_val & static_cast<I>(impl::fBitMasks[F]);
    I2 i2fracPart;
    if (F2 > F)
    {
      i2fracPart = static_cast<I2>(ifracPart) << impl::Shfts[F2 - F];
    }
    else
    {
      i2fracPart = static_cast<I2>(ifracPart >> impl::Shfts[F - F2]);
    }
    return FixedPoint<I2, F2>((intPart << F2) | i2fracPart);
  }

  template<typename I, uint8_t F>
  template<typename T>
  void FixedPoint<I, F>::SetFromFloatingPoint(T a_fval)
  {
    m_val = static_cast<I>(a_fval * static_cast<T>(Power2<F>::value) + (a_fval >= static_cast<T>(0.0) ? static_cast<T>(0.5) : static_cast<T>(-0.5)));
  }


  template<typename I, uint8_t F>
  FixedPoint<I, F> FixedPoint<I, F>::operator+(FixedPoint<I, F> a_rhs) const
  {
    return FixedPoint<I, F>(m_val + a_rhs.m_val);
  }


  template<typename I, uint8_t F>
  FixedPoint<I, F> FixedPoint<I, F>::operator-(FixedPoint<I, F> a_rhs) const
  {
    return FixedPoint<I, F>(m_val - a_rhs.m_val);
  }


  template<typename I, uint8_t F>
  FixedPoint<I, F> FixedPoint<I, F>::operator*(FixedPoint<I, F> a_rhs) const
  {
    return FixedPoint<I, F>
      (
        static_cast<I>
        (
          (
            static_cast<typename impl::QueryFPType<I, F>::PromoteType>(m_val) * a_rhs.m_val
          ) >> F
        )
      );
  }
}


#endif