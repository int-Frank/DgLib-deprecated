//================================================================================
// @ Vector4.h
// 
// Class: Vector4
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

#ifndef VECTOR4_H
#define VECTOR4_H
 
#include "SimpleRNG.h"
#include "dgmath.h"

namespace Dg
{
  template<typename Real> class Vector4;
  template<typename Real> class Matrix44;
  template<typename Real> class Plane4;
  template<typename Real> class Quaternion;

  template<typename Real>
  Vector4<Real> operator*(Real, const Vector4<Real>&);

  template<typename Real>
  Real Dot(const Vector4<Real>&, const Vector4<Real>&);

  template<typename Real>
  Vector4<Real> Cross(const Vector4<Real>&, const Vector4<Real>&);

  //--------------------------------------------------------------------------------
  //	@	Vector4
  //--------------------------------------------------------------------------------
  template<typename Real>
  class Vector4
  {
    friend class Plane4<Real>;
    friend class Matrix44<Real>;
    friend class Quaternion<Real>;
    //friend class VQS<Real>;
  public:

    // constructor/destructor
    Vector4() {}
    Vector4(Real a_x, Real a_y, Real a_z, Real a_w) :
      m_x(a_x), m_y(a_y), m_z(a_z), m_w(a_w) {}
    ~Vector4() {}

    // copy operations
    Vector4(const Vector4&);
    Vector4& operator=(const Vector4&);

    // accessors
    Real& operator[](unsigned int i)         { return (&m_x)[i]; }
    Real operator[](unsigned int i) const    { return (&m_x)[i]; }

    // Comparison.
    bool operator== (const Vector4&) const;
    bool operator!= (const Vector4&) const;
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
    inline void Set(Real a_x, Real a_y, Real a_z, Real a_w);
    void Clean();       // sets near-zero elements to 0
    inline void Zero(); // sets all elements to 0
    void Normalize();   // sets to unit vector

    // Arithmetic operations
    Vector4 operator+(const Vector4&) const;
    Vector4 operator-(const Vector4&) const;
    Vector4	operator*(Real) const;

    template<typename T>
    friend Vector4<T> operator*(T, const Vector4<T>&);

    Vector4	operator/(Real) const;
    Vector4 operator- () const;

    // Arithmetic updates
    Vector4& operator+=(const Vector4&);
    Vector4& operator-=(const Vector4&);
    Vector4& operator*=(Real);
    Vector4& operator/=(Real);

    // dot, cross product
    template<typename T>
    friend T Dot(const Vector4<T>&, const Vector4<T>&);

    template<typename T>
    friend Vector4<T> Cross(const Vector4<T>&, const Vector4<T>&);

    //other 
    Real Length() const;
    Real LengthSquared() const;

    //Friend functions
    friend Vector4<Real> operator* (const Vector4<Real>&, const Matrix44<Real>&);

  private:
    Real m_x, m_y, m_z, m_w;
  };

  //-------------------------------------------------------------------------------
  //-- Inlines --------------------------------------------------------------------
  //-------------------------------------------------------------------------------

  //-------------------------------------------------------------------------------
  //	@	Vector4::Vector4()
  //-------------------------------------------------------------------------------
  //		Copy constructor
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real>::Vector4(const Vector4<Real>& a_other) :
    m_x(a_other.m_x), m_y(a_other.m_y), m_z(a_other.m_z), m_w(a_other.m_w)
  {

  }   // End:  Vector4::Vector4()


  //-------------------------------------------------------------------------------
  //	@	Vector4::operator=()
  //-------------------------------------------------------------------------------
  //		Assignment operator
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real>& Vector4<Real>::operator=(const Vector4<Real>& a_other)
  {
    m_x = a_other.m_x;
    m_y = a_other.m_y;
    m_z = a_other.m_z;
    m_w = a_other.m_w;

    return *this;

  }   // End:  Vector4::operator=()

  //-------------------------------------------------------------------------------
  //	@	Vector4::Set()
  //-------------------------------------------------------------------------------
  // Set vector elements
  //-------------------------------------------------------------------------------
  template<typename Real>
  void Vector4<Real>::Set(Real a_x, Real a_y, Real a_z, Real a_w)
  {
    m_x = a_x;
    m_y = a_y;
    m_z = a_z;
    m_w = a_w;

  }   // End: Vector4::Set()


  //-------------------------------------------------------------------------------
  //	@	Vector4::Zero()
  //-------------------------------------------------------------------------------
  // Zero all elements
  //-------------------------------------------------------------------------------
  template<typename Real>
  inline void Vector4<Real>::Zero()
  {
    m_x = m_y = m_z = m_w = static_cast<Real>(0.0);

  }   // End: Vector4::Zero()

  //-------------------------------------------------------------------------------
  //	@	Vector4::Length()
  //-------------------------------------------------------------------------------
  //		Vector length
  //-------------------------------------------------------------------------------
  template<typename Real>
  Real Vector4<Real>::Length() const
  {
    return sqrt(m_x*m_x + m_y*m_y + m_z*m_z + m_w*m_w);

  }   // End:  Vector4::Length()


  //-------------------------------------------------------------------------------
  //	@	Vector4::LengthSquared()
  //-------------------------------------------------------------------------------
  //		Vector length squared (avoids square root)
  //-------------------------------------------------------------------------------
  template<typename Real>
  Real Vector4<Real>::LengthSquared() const
  {
    return (m_x*m_x + m_y*m_y + m_z*m_z + m_w*m_w);

  }   // End:  Vector4::LengthSquared()


  //-------------------------------------------------------------------------------
  //	@	Vector4::operator==()
  //-------------------------------------------------------------------------------
  //		Comparison operator
  //-------------------------------------------------------------------------------
  template<typename Real>
  bool Vector4<Real>::operator==(const Vector4<Real>& a_other) const
  {
    return (Dg::AreEqual(a_other.m_x, m_x)
            && Dg::AreEqual(a_other.m_y, m_y)
            && Dg::AreEqual(a_other.m_z, m_z)
            && Dg::AreEqual(a_other.m_w, m_w));

  }   // End:  Vector4::operator==()


  //-------------------------------------------------------------------------------
  //	@	Vector4::operator!=()
  //-------------------------------------------------------------------------------
  //		Comparison operator
  //-------------------------------------------------------------------------------
  template<typename Real>
  bool Vector4<Real>::operator!=(const Vector4<Real>& a_other) const
  {
    return !(Dg::AreEqual(a_other.m_x, m_x)
      && Dg::AreEqual(a_other.m_y, m_y)
      && Dg::AreEqual(a_other.m_z, m_z)
      && Dg::AreEqual(a_other.m_w, m_w));

  }   // End:  Vector4::operator!=()


  //-------------------------------------------------------------------------------
  //	@	Vector4::IsZero()
  //-------------------------------------------------------------------------------
  //		Check for zero vector
  //-------------------------------------------------------------------------------
  template<typename Real>
  bool Vector4<Real>::IsZero() const
  {
    return Dg::IsZero(m_x*m_x + m_y*m_y + m_z*m_z + m_w*m_w);

  }   // End:  Vector4::IsZero()


  //-------------------------------------------------------------------------------
  //	@	Vector4::IsUnit()
  //-------------------------------------------------------------------------------
  //		Check for unit vector
  //-------------------------------------------------------------------------------
  template<typename Real>
  bool Vector4<Real>::IsUnit() const
  {
    return Dg::IsZero(static_cast<Real>(1.0) - (m_x*m_x + m_y*m_y + m_z*m_z + m_w*m_w));

  }   // End:  Vector4::IsUnit()


  //-------------------------------------------------------------------------------
  //	@	Vector4::Clean()
  //-------------------------------------------------------------------------------
  //		Set elements close to zero equal to zero
  //-------------------------------------------------------------------------------
  template<typename Real>
  void Vector4<Real>::Clean()
  {
    if (Dg::IsZero(m_x))
      m_x = static_cast<Real>(0.0);
    if (Dg::IsZero(m_y))
      m_y = static_cast<Real>(0.0);
    if (Dg::IsZero(m_z))
      m_z = static_cast<Real>(0.0);
    if (Dg::IsZero(m_w))
      m_w = static_cast<Real>(0.0);

  }   // End:  Vector4::Clean()


  //-------------------------------------------------------------------------------
  //	@	Vector4::Normalize()
  //-------------------------------------------------------------------------------
  //		Set to unit vector
  //-------------------------------------------------------------------------------
  template<typename Real>
  void Vector4<Real>::Normalize()
  {
    Real lengthsq = m_x*m_x + m_y*m_y + m_z*m_z + m_w*m_w;

    if (Dg::IsZero(lengthsq))
    {
      m_x = static_cast<Real>(1.0);
      m_y = static_cast<Real>(0.0);
      m_z = static_cast<Real>(0.0);
      m_w = static_cast<Real>(0.0);
    }
    else
    {
      Real factor = static_cast<Real>(1.0) / sqrt(lengthsq);
      m_x *= factor;
      m_y *= factor;
      m_z *= factor;
      m_w *= factor;
    }

  }   // End:  Vector4::Normalize()


  //-------------------------------------------------------------------------------
  //	@	Vector4::operator+()
  //-------------------------------------------------------------------------------
  //		Add vector to self and return
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> Vector4<Real>::operator+(const Vector4<Real>& a_other) const
  {
    return Vector4<Real>(m_x + a_other.m_x,
                         m_y + a_other.m_y, 
                         m_z + a_other.m_z,
                         m_w + a_other.m_w);

  }   // End:  Vector4::operator+()


  //-------------------------------------------------------------------------------
  //	@	Vector4::operator+=()
  //-------------------------------------------------------------------------------
  //		Add vector to self, store in self
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real>& Vector4<Real>::operator+=(const Vector4<Real>& a_other)
  {
    m_x += a_other.m_x;
    m_y += a_other.m_y;
    m_z += a_other.m_z;
    m_w += a_other.m_w;

    return *this;

  }   // End:  Vector4::operator+=()


  //-------------------------------------------------------------------------------
  //	@	Vector4::operator-()
  //-------------------------------------------------------------------------------
  //		Subtract vector from self and return
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> Vector4<Real>::operator-(const Vector4<Real>& a_other) const
  {
    return Vector4<Real>(m_x - a_other.m_x,
                         m_y - a_other.m_y, 
                         m_z - a_other.m_z,
                         m_w - a_other.m_w);

  }   // End:  Vector4::operator-()


  //-------------------------------------------------------------------------------
  //	@	Vector4::operator-=()
  //-------------------------------------------------------------------------------
  //		Subtract vector from self, store in self
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real>& Vector4<Real>::operator-=(const Vector4<Real>& a_other)
  {
    m_x -= a_other.m_x;
    m_y -= a_other.m_y;
    m_z -= a_other.m_z;
    m_w -= a_other.m_w;

    return *this;

  }   // End:  Vector4::operator-=()


  //-------------------------------------------------------------------------------
  //	@	Vector4::operator-()
  //--------------------------------------------------------------------------------
  //		Negate self and return; w is unchanged
  //--------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> Vector4<Real>::operator- () const
  {
    return Vector4<Real>(-m_x, -m_y, -m_z, -m_w);

  }	//End: Vector3::operator-()


  //-------------------------------------------------------------------------------
  //	@	Vector4::operator*()
  //-------------------------------------------------------------------------------
  //		Scalar multiplication
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> Vector4<Real>::operator*(Real a_scalar) const
  {
    return Vector4<Real>(a_scalar*m_x, a_scalar*m_y, a_scalar*m_z, a_scalar*m_w);

  }   // End:  operator*()


  //-------------------------------------------------------------------------------
  //	@	Vector4::operator*()
  //-------------------------------------------------------------------------------
  //		Scalar multiplication
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> operator*(Real a_scalar, const Vector4<Real>& a_vector)
  {
    return Vector4<Real>(a_scalar * a_vector.m_x,
                         a_scalar * a_vector.m_y,
                         a_scalar * a_vector.m_z,
                         a_scalar * a_vector.m_w);

  }   // End:  operator*()


  //-------------------------------------------------------------------------------
  //	@	Vector4::operator*=()
  //-------------------------------------------------------------------------------
  //		Scalar multiplication by self
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real>& Vector4<Real>::operator*=(Real a_scalar)
  {
    m_x *= a_scalar;
    m_y *= a_scalar;
    m_z *= a_scalar;
    m_w *= a_scalar;

    return *this;

  }   // End:  Vector4::operator*=()


  //-------------------------------------------------------------------------------
  //	@	Vector4::operator/=()
  //-------------------------------------------------------------------------------
  //		Scalar division
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> Vector4<Real>::operator/(Real a_scalar) const
  {
    return Vector4<Real>(m_x / a_scalar,
                         m_y / a_scalar,
                         m_z / a_scalar,
                         m_w / a_scalar);

  }   // End:  operator/=()


  //-------------------------------------------------------------------------------
  //	@	Vector4::operator/=()
  //-------------------------------------------------------------------------------
  //		Scalar division by self
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real>& Vector4<Real>::operator/=(Real a_scalar)
  {
    m_x /= a_scalar;
    m_y /= a_scalar;
    m_z /= a_scalar;
    m_w /= a_scalar;

    return *this;

  }   // End:  Vector4::operator/=()


  //-------------------------------------------------------------------------------
  //	@	Dot()
  //-------------------------------------------------------------------------------
  //		Dot product friend operator
  //-------------------------------------------------------------------------------
  template<typename Real>
  Real Dot(const Vector4<Real>& a_v0, const Vector4<Real>& a_v1)
  {
    return (a_v0.m_x * a_v1.m_x +
            a_v0.m_y * a_v1.m_y +
            a_v0.m_z * a_v1.m_z + 
            a_v0.m_w * a_v1.m_w);

  }   // End:  Dot()


  //-------------------------------------------------------------------------------
  //	@	Cross()
  //--------------------------------------------------------------------------------
  //		Cross product: treats input as vectors, returns vector (w=0)
  //--------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> Cross(const Vector4<Real>& v1, const Vector4<Real>& v2)
  {
    return Vector4<Real>(v1.m_y*v2.m_z - v1.m_z*v2.m_y,
                         v1.m_z*v2.m_x - v1.m_x*v2.m_z,
                         v1.m_x*v2.m_y - v1.m_y*v2.m_x,
                         static_cast<Real>(0.0));

  }	//End: Cross()


  //--------------------------------------------------------------------------------
  //	@	Vector4::Perpendicular()
  //--------------------------------------------------------------------------------
  //		Returns a perpendicular vector
  //--------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> Perpendicular(const Vector4<Real>& a_vector)
  {
    if (Dg::IsZero(a_vector.x()))
    {
      return Vector4<Real>(static_cast<Real>(0.0), 
                          -a_vector.z(), 
                           a_vector.y(), 
                           static_cast<Real>(0.0));
    }
    else
    {
      return Vector4<Real>(-a_vector.y(), 
                            a_vector.x(), 
                            static_cast<Real>(0.0), 
                            static_cast<Real>(0.0));
    }

  }	//End: Vector4::Perpendicular()


  //-------------------------------------------------------------------------------
  //		Returns a random unit vector
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> GetRandomVector()
  {
    SimpleRNG generator;

    Real theta = generator.GetUniform<Real>(static_cast<Real>(0.0), static_cast<Real>(Dg::TWOPI));
    Real phi = generator.GetUniform<Real>(static_cast<Real>(0.0), static_cast<Real>(Dg::PI));

    Real sinTheta = sin(theta);

    Real x = sinTheta * cos(phi);
    Real y = sinTheta * sin(phi);
    Real z = cos(theta);

    return Vector4<Real>(x, y, z, static_cast<Real>(0.0));
  }


  //-------------------------------------------------------------------------------
  //		Returns a random orthonormal vector.
  //		axis must be a unit vector.
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> GetRandomOrthonormalVector(const Vector4<Real>& a_axis)
  {
    SimpleRNG generator;

    //Get a random unit vector
    Vector4<Real> v(GetRandomVector<Real>());

    //Find the cross product, to find random orthogonal vector to the axis
    Vector4<Real> o(Cross(a_axis, v));

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
  Vector4<Real> GetRandomConeVector(const Vector4<Real>& a_axis, Real theta)
  {
    SimpleRNG generator;

    //Find random angle [0, theta]
    Real phi = generator.GetUniform<Real>(static_cast<Real>(0.0), theta);

    return (cos(phi) * a_axis + sin(phi) * GetRandomOrthonormalVector(a_axis));
  }

}


#endif