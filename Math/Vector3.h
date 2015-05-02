//! @file Vector3.h
//!
//! @author: James M. Van Verth, Lars M. Bishop, Frank B. Hart
//! @date 4/10/2015
//!
//! Class declaration: Vector3

#ifndef VECTOR3_H
#define VECTOR3_H

#include "SimpleRNG.h"
#include "dgmath.h"

namespace Dg
{
  template<typename Real> class Vector3;
  template<typename Real> class Matrix33;

  template<typename Real>
  Vector3<Real> operator*(Real, const Vector3<Real>&);

  template<typename Real>
  Real Dot(const Vector3<Real>&, const Vector3<Real>&);

  template<typename Real>
  Vector3<Real> Cross(const Vector3<Real>&, const Vector3<Real>&);

  template<typename Real>
  Vector3<Real> GetRandomVector();

  template<typename Real>
  void GetBasis(const Vector3<Real>& a0,
    const Vector3<Real>& a1,
    Vector3<Real>& x0,
    Vector3<Real>& x1,
    Vector3<Real>& x2);

  template<typename Real>
  Vector3<Real> Perpendicular(const Vector3<Real>&);

  template<typename Real>
  Vector3<Real> GetRandomOrthonormalVector(const Vector3<Real>&);

  template<typename Real>
  Vector3<Real> GetRandomVector(const Vector3<Real>& axis, Real angle);

  //! @ingroup Math_classes
  //!
  //! @class Vector3
  //!
  //! @brief Three dimensional homogeneous vector class [x, y, z].
  //!
  //! @author Frank Hart
  //! @date 4/10/2015
  template<typename Real>
  class Vector3
  {
    friend class Matrix33<Real>;

  public:

    //! Default constructor. Members not initialized.
    Vector3() {}

    Vector3(Real a_x, Real a_y, Real a_z) :
      m_x(a_x), m_y(a_y), m_z(a_z) {}
    ~Vector3() {}

    // copy operations
    Vector3(const Vector3&);
    Vector3& operator=(const Vector3&);

    //! Accessor member by index
    Real& operator[](unsigned int i)         { return (&m_x)[i]; }

    //! Accessor member by index
    Real operator[](unsigned int i) const    { return (&m_x)[i]; }

    bool operator== (const Vector3&) const;
    bool operator!= (const Vector3&) const;

    //! Determines if the vector is the zero vector within some tolerance.
    bool IsZero() const;

    //! Determines if the vector is the unit vector within some tolerance.
    bool IsUnit() const;

    Real x() const { return m_x; }
    Real y() const { return m_y; }
    Real z() const { return m_z; }
    Real& x()	   { return m_x; }
    Real& y()	   { return m_y; }
    Real& z()	   { return m_z; }

    inline void Set(Real a_x, Real a_y, Real a_z);

    //! Sets near-zero elements to 0
    void Clean();

    //! sets all elements to 0
    inline void Zero();

    void Normalize();

    // Arithmetic operations
    Vector3 operator+(const Vector3&) const;
    Vector3 operator-(const Vector3&) const;
    Vector3	operator*(Real) const;

    template<typename T>
    friend Vector3<T> operator*(T, const Vector3<T>&);

    Vector3	operator/(Real) const;
    Vector3 operator- () const;

    // Arithmetic updates
    Vector3& operator+=(const Vector3&);
    Vector3& operator-=(const Vector3&);
    Vector3& operator*=(Real);
    Vector3& operator/=(Real);

    template<typename T>
    friend T Dot(const Vector3<T>&, const Vector3<T>&);

    //! Assumes w = 0.
    template<typename T>
    friend Vector3<T> Cross(const Vector3<T>&, const Vector3<T>&);

    Real Length() const;
    Real LengthSquared() const;

    //Friend functions
    template<typename T>
    friend Vector3<T> operator* (const Vector3<T>&, const Matrix33<T>&);

    //! Returns a random unit vector.
    template<typename T>
    friend Vector3<T> GetRandomVector();

    //! Creates an orthogonal basis from two input vectors
    template<typename T>
    friend void GetBasis(const Vector3<T>& a0,
      const Vector3<T>& a1,
      Vector3<T>& x0,
      Vector3<T>& x1,
      Vector3<T>& x2);

    //! Returns a perpendicular vector.
    template<typename T>
    friend Vector3<T> Perpendicular(const Vector3<T>& axis);

    //! Returns a random orthonormal vector to an axis.
    //! @pre Input must be a unit vector.
    template<typename T>
    friend Vector3<T> GetRandomOrthonormalVector(const Vector3<T>& axis);

    //! Returns a random vector at an angle to an axis.
    //! @pre Input axis must be a unit vector.
    template<typename T>
    friend Vector3<T> GetRandomVector(const Vector3<T>& axis, T angle);

  private:
    Real m_x, m_y, m_z;
  };


  //-------------------------------------------------------------------------------
  //	@	Vector3::Vector3()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector3<Real>::Vector3(const Vector3<Real>& a_other) :
    m_x(a_other.m_x), m_y(a_other.m_y), m_z(a_other.m_z)
  {

  }   // End:  Vector3::Vector3()


  //-------------------------------------------------------------------------------
  //	@	Vector3::operator=()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector3<Real>& Vector3<Real>::operator=(const Vector3<Real>& a_other)
  {
    m_x = a_other.m_x;
    m_y = a_other.m_y;
    m_z = a_other.m_z;

    return *this;

  }   // End:  Vector3::operator=()

  //-------------------------------------------------------------------------------
  //	@	Vector3::Set()
  //-------------------------------------------------------------------------------
  template<typename Real>
  void Vector3<Real>::Set(Real a_x, Real a_y, Real a_z)
  {
    m_x = a_x;
    m_y = a_y;
    m_z = a_z;

  }   // End: Vector3::Set()


  //-------------------------------------------------------------------------------
  //	@	Vector3::Zero()
  //-------------------------------------------------------------------------------
  template<typename Real>
  inline void Vector3<Real>::Zero()
  {
    m_x = m_y = m_z = static_cast<Real>(0.0);

  }   // End: Vector3::Zero()

  //-------------------------------------------------------------------------------
  //	@	Vector3::Length()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Real Vector3<Real>::Length() const
  {
    return sqrt(m_x*m_x + m_y*m_y + m_z*m_z);

  }   // End:  Vector3::Length()


  //-------------------------------------------------------------------------------
  //	@	Vector3::LengthSquared()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Real Vector3<Real>::LengthSquared() const
  {
    return (m_x*m_x + m_y*m_y + m_z*m_z);

  }   // End:  Vector3::LengthSquared()


  //-------------------------------------------------------------------------------
  //	@	Vector3::operator==()
  //-------------------------------------------------------------------------------
  template<typename Real>
  bool Vector3<Real>::operator==(const Vector3<Real>& a_other) const
  {
    return (Dg::AreEqual(a_other.m_x, m_x)
      && Dg::AreEqual(a_other.m_y, m_y)
      && Dg::AreEqual(a_other.m_z, m_z));

  }   // End:  Vector3::operator==()


  //-------------------------------------------------------------------------------
  //	@	Vector3::operator!=()
  //-------------------------------------------------------------------------------
  template<typename Real>
  bool Vector3<Real>::operator!=(const Vector3<Real>& a_other) const
  {
    return !(Dg::AreEqual(a_other.m_x, m_x)
      && Dg::AreEqual(a_other.m_y, m_y)
      && Dg::AreEqual(a_other.m_z, m_z));

  }   // End:  Vector3::operator!=()


  //-------------------------------------------------------------------------------
  //	@	Vector3::IsZero()
  //-------------------------------------------------------------------------------
  template<typename Real>
  bool Vector3<Real>::IsZero() const
  {
    return Dg::IsZero(m_x*m_x + m_y*m_y + m_z*m_z);

  }   // End:  Vector3::IsZero()


  //-------------------------------------------------------------------------------
  //	@	Vector3::IsUnit()
  //-------------------------------------------------------------------------------
  template<typename Real>
  bool Vector3<Real>::IsUnit() const
  {
    return Dg::IsZero(static_cast<Real>(1.0) - (m_x*m_x + m_y*m_y + m_z*m_z));

  }   // End:  Vector3::IsUnit()


  //-------------------------------------------------------------------------------
  //	@	Vector3::Clean()
  //-------------------------------------------------------------------------------
  template<typename Real>
  void Vector3<Real>::Clean()
  {
    if (Dg::IsZero(m_x))
      m_x = static_cast<Real>(0.0);
    if (Dg::IsZero(m_y))
      m_y = static_cast<Real>(0.0);
    if (Dg::IsZero(m_z))
      m_z = static_cast<Real>(0.0);

  }   // End:  Vector3::Clean()


  //-------------------------------------------------------------------------------
  //	@	Vector3::Normalize()
  //-------------------------------------------------------------------------------
  template<typename Real>
  void Vector3<Real>::Normalize()
  {
    Real lengthsq = m_x*m_x + m_y*m_y + m_z*m_z;

    if (Dg::IsZero(lengthsq))
    {
      m_x = static_cast<Real>(1.0);
      m_y = static_cast<Real>(0.0);
      m_z = static_cast<Real>(0.0);
    }
    else
    {
      Real factor = static_cast<Real>(1.0) / sqrt(lengthsq);
      m_x *= factor;
      m_y *= factor;
      m_z *= factor;
    }

  }   // End:  Vector3::Normalize()


  //-------------------------------------------------------------------------------
  //	@	Vector3::operator+()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector3<Real> Vector3<Real>::operator+(const Vector3<Real>& a_other) const
  {
    return Vector3<Real>(m_x + a_other.m_x,
      m_y + a_other.m_y,
      m_z + a_other.m_z);

  }   // End:  Vector3::operator+()


  //-------------------------------------------------------------------------------
  //	@	Vector3::operator+=()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector3<Real>& Vector3<Real>::operator+=(const Vector3<Real>& a_other)
  {
    m_x += a_other.m_x;
    m_y += a_other.m_y;
    m_z += a_other.m_z;

    return *this;

  }   // End:  Vector3::operator+=()


  //-------------------------------------------------------------------------------
  //	@	Vector3::operator-()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector3<Real> Vector3<Real>::operator-(const Vector3<Real>& a_other) const
  {
    return Vector3<Real>(m_x - a_other.m_x,
      m_y - a_other.m_y,
      m_z - a_other.m_z);

  }   // End:  Vector3::operator-()


  //-------------------------------------------------------------------------------
  //	@	Vector3::operator-=()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector3<Real>& Vector3<Real>::operator-=(const Vector3<Real>& a_other)
  {
    m_x -= a_other.m_x;
    m_y -= a_other.m_y;
    m_z -= a_other.m_z;

    return *this;

  }   // End:  Vector3::operator-=()


  //-------------------------------------------------------------------------------
  //	@	Vector3::operator-()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Vector3<Real> Vector3<Real>::operator- () const
  {
    return Vector3<Real>(-m_x, -m_y, -m_z);

  }	//End: Vector3::operator-()


  //-------------------------------------------------------------------------------
  //	@	Vector3::operator*()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector3<Real> Vector3<Real>::operator*(Real a_scalar) const
  {
    return Vector3<Real>(a_scalar*m_x, a_scalar*m_y, a_scalar*m_z);

  }   // End:  operator*()


  //-------------------------------------------------------------------------------
  //	@	Vector3::operator*()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector3<Real> operator*(Real a_scalar, const Vector3<Real>& a_vector)
  {
    return Vector3<Real>(a_scalar * a_vector.m_x,
      a_scalar * a_vector.m_y,
      a_scalar * a_vector.m_z);

  }   // End:  operator*()


  //-------------------------------------------------------------------------------
  //	@	Vector3::operator*=()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector3<Real>& Vector3<Real>::operator*=(Real a_scalar)
  {
    m_x *= a_scalar;
    m_y *= a_scalar;
    m_z *= a_scalar;

    return *this;

  }   // End:  Vector3::operator*=()


  //-------------------------------------------------------------------------------
  //	@	Vector3::operator/=()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector3<Real> Vector3<Real>::operator/(Real a_scalar) const
  {
    return Vector3<Real>(m_x / a_scalar,
      m_y / a_scalar,
      m_z / a_scalar);

  }   // End:  operator/=()


  //-------------------------------------------------------------------------------
  //	@	Vector3::operator/=()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector3<Real>& Vector3<Real>::operator/=(Real a_scalar)
  {
    m_x /= a_scalar;
    m_y /= a_scalar;
    m_z /= a_scalar;

    return *this;

  }   // End:  Vector3::operator/=()


  //-------------------------------------------------------------------------------
  //	@	Dot()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Real Dot(const Vector3<Real>& a_v0, const Vector3<Real>& a_v1)
  {
    return (a_v0.m_x * a_v1.m_x +
      a_v0.m_y * a_v1.m_y +
      a_v0.m_z * a_v1.m_z);

  }   // End:  Dot()


  //-------------------------------------------------------------------------------
  //	@	Cross()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Vector3<Real> Cross(const Vector3<Real>& v1, const Vector3<Real>& v2)
  {
    return Vector3<Real>(v1.m_y*v2.m_z - v1.m_z*v2.m_y,
      v1.m_z*v2.m_x - v1.m_x*v2.m_z,
      v1.m_x*v2.m_y - v1.m_y*v2.m_x);

  }	//End: Cross()


  //--------------------------------------------------------------------------------
  //	@	GetBasis()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void GetBasis(const Vector3<Real>& a_a0,
    const Vector3<Real>& a_a1,
    Vector3<Real>& a_x0,
    Vector3<Real>& a_x1,
    Vector3<Real>& a_x2)
  {
    bool is_a1_zero = a_a1.IsZero();

    //Check for zero vectors, handle separately
    if (a_a0.IsZero())
    {
      //Both x0, x1 are zero vectors
      if (is_a1_zero)
      {
        a_x0 = Vector3<Real>(static_cast<Real>(1.0), static_cast<Real>(0.0), static_cast<Real>(0.0));
        a_x1 = Vector3<Real>(static_cast<Real>(0.0), static_cast<Real>(1.0), static_cast<Real>(0.0));
        a_x2 = Vector3<Real>(static_cast<Real>(0.0), static_cast<Real>(0.0), static_cast<Real>(1.0));

        return;
      }
      //x0 only is zero vector
      else
      {
        //Build the basis off a_a1
        a_x0 = a_a1;
        a_x0.Normalize();

        //Set x1
        a_x1 = Perpendicular(a_x0);

        //Find perpendicular vector to x0, x1.
        a_x2 = Cross(a_x0, a_x1);

        return;
      }
    }
    //x1 only is zero vector
    else if (is_a1_zero)
    {
      //Build the basis off a_a0
      a_x0 = a_a0;
      a_x0.Normalize();

      //Set x1
      a_x1 = Perpendicular(a_x0);

      //Find perpendicular vector to x0, x1.
      a_x2 = Cross(a_x0, a_x1);

      return;
    }

    //Assign x0
    a_x0 = a_a0;
    a_x0.Normalize();

    //Calculate x2
    a_x2 = Cross(a_x0, a_a1);

    //Test to see if a_a0 and a_a1 are parallel
    if (IsZero(a_x2.LengthSquared()))
    {
      //Find a perpendicular vector
      a_x1 = Perpendicular(a_x0);

      //Calculate x2
      a_x2 = Cross(a_x0, a_x1);

      return;
    }

    //Normalize x2
    a_x2.Normalize();

    //Calculate x1
    a_x1 = Cross(a_x2, a_x0);

  } //End: GetBasis()


  //--------------------------------------------------------------------------------
  //	@	Perpendicular()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Vector3<Real> Perpendicular(const Vector3<Real>& a_vector)
  {
    if (Dg::IsZero(a_vector.m_x))
    {
      return Vector3<Real>(static_cast<Real>(0.0),
        -a_vector.m_z,
        a_vector.m_y);
    }
    else
    {
      return Vector3<Real>(-a_vector.m_y,
        a_vector.m_x,
        static_cast<Real>(0.0));
    }

  }	//End: Perpendicular()


  //-------------------------------------------------------------------------------
  //		@	GetRandomVector()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector3<Real> GetRandomVector()
  {
    SimpleRNG generator;

    Real theta = generator.GetUniform<Real>(static_cast<Real>(0.0), static_cast<Real>(2.0) * static_cast<Real>(Dg::PI_d));
    Real phi = generator.GetUniform<Real>(static_cast<Real>(0.0), static_cast<Real>(Dg::PI_d));

    Real sinTheta = sin(theta);

    Real x = sinTheta * cos(phi);
    Real y = sinTheta * sin(phi);
    Real z = cos(theta);

    return Vector3<Real>(x, y, z);
  }	//End: GetRandomVector()


  //-------------------------------------------------------------------------------
  //		@ GetRandomOrthonormalVector()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector3<Real> GetRandomOrthonormalVector(const Vector3<Real>& a_axis)
  {
    SimpleRNG generator;

    //Get a random unit vector
    Vector3<Real> v(GetRandomVector<Real>());

    //Find the cross product, to find random orthogonal vector to the axis
    Vector3<Real> o(Cross(a_axis, v));

    if (o.IsZero())
      o = Perpendicular(a_axis);

    o.Normalize();

    return o;
  }	//End: GetRandomOrthonormalVector()


  //-------------------------------------------------------------------------------
  //		@ GetRandomVector()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector3<Real> GetRandomVector(const Vector3<Real>& a_axis, Real theta)
  {
    SimpleRNG generator;

    //Find random angle [0, theta]
    Real phi = generator.GetUniform<Real>(static_cast<Real>(0.0), theta);

    return (cos(phi) * a_axis + sin(phi) * GetRandomOrthonormalVector(a_axis));
  }	//End: GetRandomVector()

}


#endif