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

    template<typename T>
    struct Promote
    {
      #ifdef _MSC_VER
			typedef Error_promote_type_not_specialized_for_this_type type;
			#endif
    };

    template<> struct Promote<uint8_t>  { uint16_t value; };
    template<> struct Promote<int8_t>   { int16_t  value; };
    template<> struct Promote<uint16_t> { uint32_t value; };
    template<> struct Promote<int16_t>  { int32_t  value; };
    template<> struct Promote<uint32_t> { uint64_t value; };
    template<> struct Promote<int32_t>  { int64_t  value; };

    enum class FPBaseType
    {
      sint8,
      uint8,
      sint16,
      uint16,
      sint32,
      uint32,
      INVALID_TYPE
    };

    template<typename T, uint8_t F>
    struct QueryFPType 
    { 
      static FPBaseType const fpBaseType = FPBaseType::INVALID_TYPE;
      static bool const valid = false;
      static int const nIntegerBits = 0;
    };

    template<uint8_t F>
    struct QueryFPType<int8_t, F>
    {
      static FPBaseType const fpBaseType = FPBaseType::sint8;
      static bool const valid = (F <= 7);
      static int const nIntegerBits = 7 - F;
    };

    template<uint8_t F>
    struct QueryFPType<uint8_t, F>
    {
      static FPBaseType const fpBaseType = FPBaseType::uint8;
      static bool const valid = (F <= 8);
      static int const nIntegerBits = 8 - F;
    };

    template<uint8_t F>
    struct QueryFPType<int16_t, F>
    {
      static FPBaseType const fpBaseType = FPBaseType::sint16;
      static bool const valid = (F <= 15);
      static int const nIntegerBits = 15 - F;
    };

    template<uint8_t F>
    struct QueryFPType<uint16_t, F>
    {
      static FPBaseType const fpBaseType = FPBaseType::uint16;
      static bool const valid = (F <= 16);
      static int const nIntegerBits = 16 - F;
    };

    template<uint8_t F>
    struct QueryFPType<int32_t, F>
    {
      static FPBaseType const fpBaseType = FPBaseType::sint32;
      static bool const valid = (F <= 31);
      static int const nIntegerBits = 31 - F;
    };

    template<uint8_t F>
    struct QueryFPType<uint32_t, F>
    {
      static FPBaseType const fpBaseType = FPBaseType::uint32;
      static bool const valid = (F <= 32);
      static int const nIntegerBits = 32 - F;
    };
  }


  template<typename I, uint8_t F>
  class FixedPoint
  {
    static_assert(impl::QueryFPType<I, F>::valid, "Invalid template argument. Check input type and number of fractional bits.");
    
  public:

    FixedPoint() : m_val(0) {}

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
    void SetBase(I a_val) { m_val = a_val; }

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
    I ifracPart = m_val & ((1 << F) - 1);
    I2 i2fracPart;
    if (F2 > F)
    {
      i2fracPart = static_cast<I2>(ifracPart << (F2 - F));
    }
    else
    {
      i2fracPart = static_cast<I2>(ifracPart >> (F - F2));
    }
    FixedPoint<I2, F2> result;
    result.SetBase((intPart << F2) | i2fracPart);
    return result;
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
    return m_val + a_rhs;
  }


  template<typename I, uint8_t F>
  FixedPoint<I, F> FixedPoint<I, F>::operator-(FixedPoint<I, F> a_rhs) const
  {
    return m_val - a_rhs;
  }


  //template<typename I, uint8_t F>
  //FixedPoint<I, F> FixedPoint<I, F>::operator*(FixedPoint<I, F> a_rhs) const
  //{
  //  impl::Promote<I> result;
  //}
}


#endif