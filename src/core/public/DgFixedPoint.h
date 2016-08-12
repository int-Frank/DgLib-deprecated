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

    template<> struct Promote<uint8_t> { uint16_t type; };
    template<> struct Promote<int8_t> { int16_t type; };
    template<> struct Promote<uint16_t> { uint32_t type; };
    template<> struct Promote<int16_t> { int32_t type; };
    template<> struct Promote<uint32_t> { uint64_t type; };
    template<> struct Promote<int32_t> { int64_t type; };
  }


  template<typename I, uint8_t FBits>
  class FixedPoint
  {
  public:

    FixedPoint() : m_val(0) {}

    //! Construct from floating point
    template<typename F>
    FixedPoint(F);

    template<typename I2, uint8_t FBits2>
    explicit operator FixedPoint<I2, FBits2>();

    ~FixedPoint() {}
    FixedPoint(FixedPoint const &);
    FixedPoint & operator=(FixedPoint const &);

    bool operator==(FixedPoint const &) const;
    bool operator!=(FixedPoint const &) const;
    bool operator>(FixedPoint const &) const;
    bool operator<(FixedPoint const &) const;
    bool operator>=(FixedPoint const &) const;
    bool operator<=(FixedPoint const &) const;

    FixedPoint operator+(FixedPoint);
    FixedPoint operator-(FixedPoint);
    FixedPoint operator*(FixedPoint);
    FixedPoint operator/(FixedPoint);

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

  private:
    I       m_val;
  };

  template<typename I, uint8_t FBits>
  template<typename F>
  FixedPoint<I, FBits>::FixedPoint(F a_fval)
    : m_val(static_cast<I>(a_fval * static_cast<F>(Power2<FBits>::value) + (a_fval >= static_cast<F>(0.0) ? static_cast<F>(0.5) : static_cast<F>(-0.5))))
  {

  }

}


#endif