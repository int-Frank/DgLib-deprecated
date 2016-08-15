#ifndef DGFIXEDPOINT_H
#define DGFIXEDPOINT_H

#include <stdint.h>
#include <limits>

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
      typedef Error_promote_type_not_specialized_for_this_type PromoteType;
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
  }

  //! I: base int type.
  //! F: number of fraction bits.
  template<typename I, uint8_t F>
  class FixedPoint
  {
    static_assert(impl::QueryFPType<I, F>::valid, "Invalid template argument. Check input type and number of fractional bits.");
    
    /// Grant the fixed_point template access to private members. Types with
    /// different template parameters are different types and without this
    /// declaration they do not have access to private members.
    friend class Dg::FixedPoint;

    /// Grant the numeric_limits specialization for this fixed_point class 
    /// access to private members.
    friend class std::numeric_limits<Dg::FixedPoint<I, F> >;

  public:

    /// The base type of this fixed_point class.
    typedef I base_type;

    FixedPoint() : m_val(0) {}
    ~FixedPoint() {}

    //! Construct from floating point
    FixedPoint(float a_val)       : m_val(static_cast<I>(a_val * static_cast<float>(Power2<F>::value) + (a_val >= 0.0f ? 0.5f : -0.5f))) {}
    FixedPoint(double a_val)      : m_val(static_cast<I>(a_val * static_cast<double>(Power2<F>::value) + (a_val >= 0.0 ? 0.5 : -0.5))) {}
    FixedPoint(long double a_val) : m_val(static_cast<I>(a_val * static_cast<long double>(Power2<F>::value) + (a_val >= 0.0L ? 0.5L : -0.5L))) {}

    //! Constructor from int types.
    FixedPoint(bool a_val)     : m_val((F >= sizeof(I) * CHAR_BIT)  ? 0 : static_cast<I>(a_val) << F) {}
    FixedPoint(int8_t a_val)   : m_val((F >= sizeof(I) * CHAR_BIT)  ? 0 : static_cast<I>(a_val) << F) {}
    FixedPoint(uint8_t a_val)  : m_val((F >= sizeof(I) * CHAR_BIT)  ? 0 : static_cast<I>(a_val) << F) {}
    FixedPoint(int16_t a_val)  : m_val((F >= sizeof(I) * CHAR_BIT)  ? 0 : static_cast<I>(a_val) << F) {}
    FixedPoint(uint16_t a_val) : m_val((F >= sizeof(I) * CHAR_BIT)  ? 0 : static_cast<I>(a_val) << F) {}
    FixedPoint(int32_t a_val)  : m_val((F >= sizeof(I) * CHAR_BIT)  ? 0 : static_cast<I>(a_val) << F) {}
    FixedPoint(uint32_t a_val) : m_val((F >= sizeof(I) * CHAR_BIT)  ? 0 : static_cast<I>(a_val) << F) {}
    FixedPoint(int64_t a_val)  : m_val((F >= sizeof(I) * CHAR_BIT)  ? 0 : static_cast<I>(a_val) << F) {}
    FixedPoint(uint64_t a_val) : m_val((F >= sizeof(I) * CHAR_BIT)  ? 0 : static_cast<I>(a_val) << F) {}

    //! Constructor from another fixed point type.
    template<typename I2, uint8_t F2>
    explicit operator FixedPoint<I2, F2>() const;

    FixedPoint(FixedPoint const &);
    FixedPoint & operator=(FixedPoint const &);

    //! Floating point conversion.
    operator float()        const { return static_cast<float>(m_val) / Power2<F>::value; }
    operator double()       const { return static_cast<double>(m_val) / Power2<F>::value; }
    operator long double()  const { return static_cast<long double>(m_val) / Power2<F>::value; }

    //! Integer conversion.
    operator bool()         const { return (F >= sizeof(I) * CHAR_BIT)  ? 0 : static_cast<bool>(m_val >> F); }
    operator int8_t()       const { return (F >= sizeof(I) * CHAR_BIT)  ? 0 : static_cast<int8_t>(m_val >> F); }
    operator uint8_t()      const { return (F >= sizeof(I) * CHAR_BIT)  ? 0 : static_cast<uint8_t>(m_val >> F); }
    operator int16_t()      const { return (F >= sizeof(I) * CHAR_BIT)  ? 0 : static_cast<int16_t>(m_val >> F); }
    operator uint16_t()     const { return (F >= sizeof(I) * CHAR_BIT)  ? 0 : static_cast<uint16_t>(m_val >> F); }
    operator int32_t()      const { return (F >= sizeof(I) * CHAR_BIT)  ? 0 : static_cast<int32_t>(m_val >> F); }
    operator uint32_t()     const { return (F >= sizeof(I) * CHAR_BIT)  ? 0 : static_cast<uint32_t>(m_val >> F); }
    operator int64_t()      const { return (F >= sizeof(I) * CHAR_BIT)  ? 0 : static_cast<int64_t>(m_val >> F); }
    operator uint64_t()     const { return (F >= sizeof(I) * CHAR_BIT)  ? 0 : static_cast<uint64_t>(m_val >> F); }

    bool operator==(FixedPoint const & a_other) const {return m_val == a_other.m_val;}
    bool operator!=(FixedPoint const & a_other) const {return m_val != a_other.m_val;}
    bool operator> (FixedPoint const & a_other) const {return m_val > a_other.m_val;}
    bool operator< (FixedPoint const & a_other) const {return m_val < a_other.m_val;}
    bool operator>=(FixedPoint const & a_other) const {return m_val >= a_other.m_val;}
    bool operator<=(FixedPoint const & a_other) const {return m_val <= a_other.m_val;}            

    bool operator !() const { return m_val == 0; }
    FixedPoint operator-() const;

    FixedPoint operator++() const;
    FixedPoint operator--() const;

    FixedPoint operator<<(FixedPoint) const;
    FixedPoint operator>>(FixedPoint) const;
    FixedPoint operator<<=(FixedPoint) const;
    FixedPoint operator>>=(FixedPoint) const;

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
    friend FixedPoint floor(FixedPoint);
    friend FixedPoint mod(FixedPoint, FixedPoint);
    friend FixedPoint modf(FixedPoint, FixedPoint *);
    friend FixedPoint exp(FixedPoint);
    friend FixedPoint log(FixedPoint);
    friend FixedPoint cos(FixedPoint);
    friend FixedPoint sin(FixedPoint);
    friend FixedPoint sqrt(FixedPoint);

  private:

    FixedPoint(I a_val, bool DUMMY) : m_val(a_val) {}

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
    //! We use this table to get around the compiler throwing a warning 
    //! when bit shifting by a potentially negative number.
    static uint32_t const Shfts[32] =
    {
      0,  1,  2,  3,  4,  5,  6,  7,
      8,  9,  10, 11, 12, 13, 14, 15,
      16, 17, 18, 19, 20, 21, 22, 23,
      24, 25, 26, 27, 28, 29, 30, 31
    };

    if (sizeof(I2) >= sizeof(I))
    {
      if (F2 > F)
      {
        int diff = F2 - F;
        return FixedPoint<I2, F2>((diff >= sizeof(I2) * CHAR_BIT) ? 0 : static_cast<I2>(m_val) << Shfts[diff], true);
      }
      else
      {
        int diff = F - F2;
        return FixedPoint<I2, F2>((diff >= sizeof(I2) * CHAR_BIT) ? 0 : static_cast<I2>(m_val) >> Shfts[diff], true);
      }
    }
    else
    {
      if (F2 > F)
      {
        int diff = F2 - F;
        return FixedPoint<I2, F2>(static_cast<I2>((diff >= sizeof(I2) * CHAR_BIT) ? 0 : m_val << Shfts[diff]), true);
      }
      else
      {
        int diff = F - F2;
        return FixedPoint<I2, F2>(static_cast<I2>((diff >= sizeof(I2) * CHAR_BIT) ? 0 : m_val >> Shfts[diff]), true);
      }
    }
  }

  template<typename I, uint8_t F>
  FixedPoint<I, F> FixedPoint<I, F>::operator-() const
  {
    //Negating an unsigned FixedPoint has undefined behaviour.
    return FixedPoint<I, F>(-m_val, true);
  }

  template<typename I, uint8_t F>
  FixedPoint<I, F> FixedPoint<I, F>::operator+(FixedPoint<I, F> a_rhs) const
  {
    return FixedPoint<I, F>(m_val + a_rhs.m_val, true);
  }


  template<typename I, uint8_t F>
  FixedPoint<I, F> FixedPoint<I, F>::operator-(FixedPoint<I, F> a_rhs) const
  {
    return FixedPoint<I, F>(m_val - a_rhs.m_val, true);
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
        ), true
      );
  }

  template<typename I, uint8_t F>
  FixedPoint<I, F> FixedPoint<I, F>::operator/(FixedPoint<I, F> a_rhs) const
  {
    return FixedPoint<I, F>
      (
        static_cast<I>
        (
          (
            (static_cast<typename impl::QueryFPType<I, F>::PromoteType>(m_val) << F) / a_rhs.m_val
          )
        ), true
      );
  }

  template<typename I, uint8_t F>
  FixedPoint<I, F> & FixedPoint<I, F>::operator+=(FixedPoint<I, F> a_rhs)
  {
    m_val += a_rhs.m_val;
    return *this;
  }


  template<typename I, uint8_t F>
  FixedPoint<I, F> & FixedPoint<I, F>::operator-=(FixedPoint<I, F> a_rhs)
  {
    m_val -= a_rhs.m_val;
    return *this;
  }


  template<typename I, uint8_t F>
  FixedPoint<I, F> & FixedPoint<I, F>::operator*=(FixedPoint<I, F> a_rhs)
  {
    m_val = static_cast<I>
            (
              (
                static_cast<typename impl::QueryFPType<I, F>::PromoteType>(m_val) * a_rhs.m_val
              ) >> F
            );
    return *this;
  }
  
  template<typename I, uint8_t F>
  FixedPoint<I, F> & FixedPoint<I, F>::operator/=(FixedPoint<I, F> a_rhs)
  {
    m_val = static_cast<I>
            (
              (
                (static_cast<typename impl::QueryFPType<I, F>::PromoteType>(m_val) << F)/ a_rhs.m_val
              )
            );
    return *this;
  }
}



// TODO
template<typename S, typename I, uint8_t F>
S & operator >> (S & a_stream, Dg::FixedPoint<I, F> & a_val)
{
  return a_stream;
}

// TODO
template<typename S, typename I, uint8_t F>
S & operator << (S & a_stream, Dg::FixedPoint<I, F> a_val)
{
  return a_stream;
}


/******************************************************************************/
/*                                                                            */
/* numeric_limits<Dg::FixedPoint<I, F> >                                */
/*                                                                            */
/******************************************************************************/
namespace std
{
  template<typename I, uint8_t F>
  class numeric_limits<Dg::FixedPoint<I, F> >
  {
  public:
    /// The FixedPoint type. This numeric_limits specialization is specialized
    /// for this type.
    typedef Dg::FixedPoint<I, F> fp_type;

    /// Tests whether a type allows denormalized values.
    //!
    //! An enumeration value of type const float_denorm_style, indicating 
    //! whether the type allows denormalized values. The FixedPoint class does
    //! not have denormalized values.
    //!
    //! The member is always set to denorm_absent.
    static const float_denorm_style has_denorm = denorm_absent;

    /// Tests whether loss of accuracy is detected as a denormalization loss 
    /// rather than as an inexact result.
    //!
    //! The FixedPoint class does not have denormalized values.
    //!
    //! The member is always set to false.
    static const bool has_denorm_loss = false;

    /// Tests whether a type has a representation for positive infinity.
    //!
    //! The FixedPoint class does not have a representation for positive
    //! infinity.
    //!
    //! The member is always set to false.
    static const bool has_infinity = false;

    /// Tests whether a type has a representation for a quiet not a number 
    /// (NAN), which is nonsignaling.
    //!
    //! The FixedPoint class does not have a quiet NAN.
    //!
    //! The member is always set to false.
    static const bool has_quiet_NaN = false;

    /// Tests whether a type has a representation for signaling not a number 
    //! (NAN).
    //!
    //! The FixedPoint class does not have a signaling NAN.
    //!
    //! The member is always set to false.
    static const bool has_signaling_NaN = false;

    /// Tests if the set of values that a type may represent is finite.
    //!
    //! The FixedPoint type has a bounded set of representable values.
    //!
    //! The member is always set to true.
    static const bool is_bounded = true;

    /// Tests if the calculations done on a type are free of rounding errors.
    //!
    //! The FixedPoint type is considered exact.
    //!
    //! The member is always set to true.
    static const bool is_exact = true;

    /// Tests if a type conforms to IEC 559 standards.
    //!
    //! The FixedPoint type does not conform to IEC 559 standards.
    //!
    //! The member is always set to false.
    static const bool is_iec559 = false;

    /// Tests if a type has an integer representation.
    //!
    //! The FixedPoint type behaves like a real number and thus has not
    //! integer representation.
    //!
    //! The member is always set to false.
    static const bool is_integer = false;

    /// Tests if a type has a modulo representation.
    //!
    //! A modulo representation is a representation where all results are 
    //! reduced modulo some value. The FixedPoint class does not have a
    //! modulo representation.
    //!
    //! The member is always set to false.
    static const bool is_modulo = false;

    /// Tests if a type has a signed representation.
    //!
    //! The member stores true for a type that has a signed representation, 
    //! which is the case for all FixedPoint types with a signed base type.
    //! Otherwise it stores false.
    static const bool is_signed = std::numeric_limits<typename fp_type::base_type>::is_signed;

    /// Tests if a type has an explicit specialization defined in the template 
    /// class numeric_limits.
    //!
    //! The FixedPoint class has an explicit specialization.
    //!
    //! The member is always set to true.
    static const bool is_specialized = true;

    /// Tests whether a type can determine that a value is too small to 
    /// represent as a normalized value before rounding it.
    //!
    //! Types that can detect tinyness were included as an option with IEC 559 
    //! floating-point representations and its implementation can affect some 
    //! results.
    //!
    //! The member is always set to false.
    static const bool tinyness_before = false;

    /// Tests whether trapping that reports on arithmetic exceptions is 
    //! implemented for a type.
    //!
    //! The member is always set to false.
    static const bool traps = false;

    /// Returns a value that describes the various methods that an 
    /// implementation can choose for rounding a real value to an integer 
    /// value.
    //!
    //! The member is always set to round_toward_zero.
    static const float_round_style round_style = round_toward_zero;

    /// Returns the number of radix digits that the type can represent without 
    /// loss of precision.
    //!
    //! The member stores the number of radix digits that the type can represent 
    //! without change.
    //!
    //! The member is set to the template parameter I (number of integer bits).
    static const int digits = I;

    /// Returns the number of decimal digits that the type can represent without 
    /// loss of precision.
    //!
    //! The member is set to the number of decimal digits that the type can 
    //! represent.
    static const int digits10 = (int)(digits * 301. / 1000. + .5);

    static const int max_exponent = 0;
    static const int max_exponent10 = 0;
    static const int min_exponent = 0;
    static const int min_exponent10 = 0;
    static const int radix = 0;

    /// The minimum value of this type.
    //!
    //! /return The minimum value representable with this type.
    static fp_type(min)()
    {
      fp_type minimum;
      minimum.m_val = (std::numeric_limits<typename fp_type::base_type>::min)();
      return minimum;
    }

    /// The maximum value of this type.
    //!
    //! /return The maximum value representable with this type.
    static fp_type(max)()
    {
      fp_type maximum;
      maximum.m_val = (std::numeric_limits<typename fp_type::base_type>::max)();
      return maximum;
    }

    /// The function returns the difference between 1 and the smallest value 
    /// greater than 1 that is representable for the data type.
    //!
    //! /return The smallest effective increment from 1.0.
    static fp_type epsilon()
    {
      fp_type epsilon;
      epsilon.m_val = 1;
      return epsilon;
    }

    /// Returns the maximum rounding error for the type.
    //!
    //! The maximum rounding error for the FixedPoint type is 0.5.
    //!
    //! /return Always returns 0.5.
    static fp_type round_error()
    {
      return (fp_type)(0.5);
    }

    static fp_type denorm_min()
    {
      return (fp_type)(0);
    }

    static fp_type infinity()
    {
      return (fp_type)(0);
    }

    static fp_type quiet_NaN()
    {
      return (fp_type)(0);
    }

    static fp_type signaling_NaN()
    {
      return (fp_type)(0);
    }
  };

}

#endif