//================================================================================
// @ Vector3.h
// 
// Class: Vector3
//
// 3 dimensional homogeneous vector class.
//
// -------------------------------------------------------------------------------
//
// Original Authors: James M. Van Verth, Lars M. Bishop
// Retrieved From: Essential Mathematics for Games and Interactive Applications SE
// On Date: 2013
//
// Modified by: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef Vector3_H
#define Vector3_H

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

  //--------------------------------------------------------------------------------
  //	@	Vector3
  //--------------------------------------------------------------------------------
  template<typename Real>
  class Vector3
  {
    friend class Matrix33<Real>;
  public:

    // constructor/destructor
    Vector3() {}
    Vector3(Real a_x, Real a_y, Real a_z) :
      m_x(a_x), m_y(a_y), m_z(a_z) {}
    ~Vector3() {}

    // copy operations
    Vector3(const Vector3&);
    Vector3& operator=(const Vector3&);

    // accessors
    Real& operator[](unsigned int i)         { return (&m_x)[i]; }
    Real operator[](unsigned int i) const    { return (&m_x)[i]; }

    // Comparison.
    bool operator== (const Vector3&) const;
    bool operator!= (const Vector3&) const;
    bool IsZero() const;
    bool IsUnit() const;

    // Coordinate access.
    Real x() const { return m_x; }
    Real y() const { return m_y; }
    Real z() const { return m_z; }
    Real w() const { return m_w; }
    Real& x()	   { return m_x; }
    Real& y()	   { return m_y; }
    Real& z()	   { return m_z; }
    Real& w()	   { return m_w; }

    // manipulators
    inline void Set(Real a_x, Real a_y, Real a_z);
    void Clean();       // sets near-zero elements to 0
    inline void Zero(); // sets all elements to 0
    void Normalize();   // sets to unit vector

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

    // dot, cross product
    template<typename T>
    friend T Dot(const Vector3<T>&, const Vector3<T>&);

    template<typename T>
    friend Vector3<T> Cross(const Vector3<T>&, const Vector3<T>&);

    //other 
    Real Length() const;
    Real LengthSquared() const;

    //Friend functions
    friend Vector3<Real> operator* (const Vector3<Real>&, const Matrix33<Real>&);

  private:
    Real m_x, m_y, m_z, m_w;
  };

  //-------------------------------------------------------------------------------
  //-- Inlines --------------------------------------------------------------------
  //-------------------------------------------------------------------------------

  //-------------------------------------------------------------------------------
  //	@	Vector3::Vector3()
  //-------------------------------------------------------------------------------
  //		Copy constructor
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector3<Real>::Vector3(const Vector3<Real>& a_other) :
    m_x(a_other.m_x), m_y(a_other.m_y), m_z(a_other.m_z)
  {

  }   // End:  Vector3::Vector3()


  //-------------------------------------------------------------------------------
  //	@	Vector3::operator=()
  //-------------------------------------------------------------------------------
  //		Assignment operator
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
  // Set vector elements
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
  // Zero all elements
  //-------------------------------------------------------------------------------
  template<typename Real>
  inline void Vector3<Real>::Zero()
  {
    m_x = m_y = m_z = static_cast<Real>(0.0);

  }   // End: Vector3::Zero()

  //-------------------------------------------------------------------------------
  //	@	Vector3::Length()
  //-------------------------------------------------------------------------------
  //		Vector length
  //-------------------------------------------------------------------------------
  template<typename Real>
  Real Vector3<Real>::Length() const
  {
    return sqrt(m_x*m_x + m_y*m_y + m_z*m_z);

  }   // End:  Vector3::Length()


  //-------------------------------------------------------------------------------
  //	@	Vector3::LengthSquared()
  //-------------------------------------------------------------------------------
  //		Vector length squared (avoids square root)
  //-------------------------------------------------------------------------------
  template<typename Real>
  Real Vector3<Real>::LengthSquared() const
  {
    return (m_x*m_x + m_y*m_y + m_z*m_z);

  }   // End:  Vector3::LengthSquared()


  //-------------------------------------------------------------------------------
  //	@	Vector3::operator==()
  //-------------------------------------------------------------------------------
  //		Comparison operator
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
  //		Comparison operator
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
  //		Check for zero vector
  //-------------------------------------------------------------------------------
  template<typename Real>
  bool Vector3<Real>::IsZero() const
  {
    return Dg::IsZero(m_x*m_x + m_y*m_y + m_z*m_z);

  }   // End:  Vector3::IsZero()


  //-------------------------------------------------------------------------------
  //	@	Vector3::IsUnit()
  //-------------------------------------------------------------------------------
  //		Check for unit vector
  //-------------------------------------------------------------------------------
  template<typename Real>
  bool Vector3<Real>::IsUnit() const
  {
    return Dg::IsZero(static_cast<Real>(1.0) - (m_x*m_x + m_y*m_y + m_z*m_z));

  }   // End:  Vector3::IsUnit()


  //-------------------------------------------------------------------------------
  //	@	Vector3::Clean()
  //-------------------------------------------------------------------------------
  //		Set elements close to zero equal to zero
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
  //		Set to unit vector
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
  //		Add vector to self and return
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
  //		Add vector to self, store in self
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
  //		Subtract vector from self and return
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
  //		Subtract vector from self, store in self
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
  //		Negate self and return; w is unchanged
  //--------------------------------------------------------------------------------
  template<typename Real>
  Vector3<Real> Vector3<Real>::operator- () const
  {
    return Vector3<Real>(-m_x, -m_y, -m_z);

  }	//End: Vector3::operator-()


  //-------------------------------------------------------------------------------
  //	@	Vector3::operator*()
  //-------------------------------------------------------------------------------
  //		Scalar multiplication
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector3<Real> Vector3<Real>::operator*(Real a_scalar) const
  {
    return Vector3<Real>(a_scalar*m_x, a_scalar*m_y, a_scalar*m_z);

  }   // End:  operator*()


  //-------------------------------------------------------------------------------
  //	@	Vector3::operator*()
  //-------------------------------------------------------------------------------
  //		Scalar multiplication
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
  //		Scalar multiplication by self
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
  //		Scalar division
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
  //		Scalar division by self
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
  //		Dot product friend operator
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
  //		Cross product: treats input as vectors, returns vector (w=0)
  //--------------------------------------------------------------------------------
  template<typename Real>
  Vector3<Real> Cross(const Vector3<Real>& v1, const Vector3<Real>& v2)
  {
    return Vector3<Real>(v1.m_y*v2.m_z - v1.m_z*v2.m_y,
      v1.m_z*v2.m_x - v1.m_x*v2.m_z,
      v1.m_x*v2.m_y - v1.m_y*v2.m_x);

  }	//End: Cross()


  //--------------------------------------------------------------------------------
  //	@	Vector3::Perpendicular()
  //--------------------------------------------------------------------------------
  //		Returns a perpendicular vector
  //--------------------------------------------------------------------------------
  template<typename Real>
  Vector3<Real> Perpendicular(const Vector3<Real>& a_vector)
  {
    if (Dg::IsZero(a_vector.z()))
    {
      return Vector3<Real>(-a_vector.y(),
        a_vector.x(),
        static_cast<Real>(0.0));
    }
    else
    {
      return Vector3<Real>(static_cast<Real>(0.0),
        -a_vector.z(),
        a_vector.y());
    }

  }	//End: Vector3::Perpendicular()


  //-------------------------------------------------------------------------------
  //		Returns a random unit vector
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector3<Real> GetRandomVector()
  {
    SimpleRNG generator;

    Real theta = generator.GetUniform<Real>(static_cast<Real>(0.0), static_cast<Real>(2.0) * static_cast<Real>(Dg::PI));
    Real phi = generator.GetUniform<Real>(static_cast<Real>(0.0), static_cast<Real>(Dg::PI));

    Real sinTheta = sin(theta);

    Real x = sinTheta * cos(phi);
    Real y = sinTheta * sin(phi);
    Real z = cos(theta);

    return Vector3<Real>(x, y, z);
  }


  //-------------------------------------------------------------------------------
  //		Returns a random orthonormal vector.
  //		axis must be a unit vector.
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
  }


  //-------------------------------------------------------------------------------
  //		Returns a random unit vector at angle theta from the axis.
  //		axis must be a unit vector.
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector3<Real> GetRandomConeVector(const Vector3<Real>& a_axis, Real theta)
  {
    SimpleRNG generator;

    //Find random angle [0, theta]
    Real phi = generator.GetUniform<Real>(static_cast<Real>(0.0), theta);

    return (cos(phi) * a_axis + sin(phi) * GetRandomOrthonormalVector(a_axis));
  }

}


#endif