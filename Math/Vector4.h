//! @file Vector4.h
//!
//! @author: James M. Van Verth, Lars M. Bishop, Frank B. Hart
//! @date 4/10/2015
//!
//! Class declaration: Vector4

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
  template<typename Real> class VQS;

  template<typename Real>
  Vector4<Real> operator*(Real, const Vector4<Real>&);

  template<typename Real>
  Vector4<Real> operator*(Vector4<Real> const &, VQS<Real> const &);

  template<typename Real>
  Real Dot(const Vector4<Real>&, const Vector4<Real>&);

  template<typename Real>
  Vector4<Real> Cross(const Vector4<Real>&, const Vector4<Real>&);

  template<typename Real>
  Vector4<Real> GetRandomVector();

  template<typename Real>
  void GetBasis(const Vector4<Real>& a0,
                const Vector4<Real>& a1,
                Vector4<Real>& x0, 
                Vector4<Real>& x1, 
                Vector4<Real>& x2);

  template<typename Real>
  Vector4<Real> Perpendicular(const Vector4<Real>&);

  template<typename Real>
  Vector4<Real> GetRandomOrthonormalVector(const Vector4<Real>&);

  template<typename Real>
  Vector4<Real> GetRandomVector(const Vector4<Real>& axis, Real angle);

  //! @ingroup Math_classes
  //!
  //! @class Vector4
  //!
  //! @brief Three dimensional homogeneous vector class [x, y, z, w].
  //!
  //! @author Frank Hart
  //! @date 4/10/2015
  template<typename Real>
  class Vector4
  {
    friend class Plane4<Real>;
    friend class Matrix44<Real>;
    friend class Quaternion<Real>;
    friend class VQS<Real>;

  public:

    //! Default constructor. Members not initialized.
    Vector4() {}

    Vector4(Real a_x, Real a_y, Real a_z, Real a_w) :
      m_x(a_x), m_y(a_y), m_z(a_z), m_w(a_w) {}
    ~Vector4() {}

    // copy operations
    Vector4(const Vector4&);
    Vector4& operator=(const Vector4&);

    //! Accessor member by index
    Real& operator[](unsigned int i)         { return (&m_x)[i]; }

    //! Accessor member by index
    Real operator[](unsigned int i) const    { return (&m_x)[i]; }

    bool operator== (const Vector4&) const;
    bool operator!= (const Vector4&) const;

    //! Determines if the vector is the zero vector within some tolerance.
    bool IsZero() const;

    //! Determines if the vector is the unit vector within some tolerance.
    bool IsUnit() const;

    Real x() const { return m_x; }
    Real y() const { return m_y; }
    Real z() const { return m_z; }
    Real w() const { return m_w; }
    Real& x()	   { return m_x; }
    Real& y()	   { return m_y; }
    Real& z()	   { return m_z; }
    Real& w()	   { return m_w; }

    inline void Set(Real a_x, Real a_y, Real a_z, Real a_w);

    //! Sets near-zero elements to 0
    void Clean();      

    //! sets all elements to 0
    inline void Zero(); 

    void Normalize();  

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

    template<typename T>
    friend T Dot(const Vector4<T>&, const Vector4<T>&);

    //! Assumes w = 0.
    template<typename T>
    friend Vector4<T> Cross(const Vector4<T>&, const Vector4<T>&);

    Real Length() const;
    Real LengthSquared() const;

    //Friend functions
    template<typename T>
    friend Vector4<T> operator* (const Vector4<T>&, const Matrix44<T>&);

    template<typename T>
    friend Vector4<T> operator* (Vector4<T> const &, VQS<T> const &);

    //! Returns a random unit vector.
    template<typename T>
    friend Vector4<T> GetRandomVector();

    //! Creates an orthogonal basis from two input vectors
    template<typename T>
    friend void GetBasis(const Vector4<T>& a0, 
                         const Vector4<T>& a1,
                         Vector4<T>& x0, 
                         Vector4<T>& x1, 
                         Vector4<T>& x2);

    //! Returns a perpendicular vector.
    template<typename T>
    friend Vector4<T> Perpendicular(const Vector4<T>& axis);

    //! Returns a random orthonormal vector to an axis.
    //! @pre Input must be a unit vector.
    template<typename T>
    friend Vector4<T> GetRandomOrthonormalVector(const Vector4<T>& axis);

    //! Returns a random vector at an angle to an axis.
    //! @pre Input axis must be a unit vector.
    template<typename T>
    friend Vector4<T> GetRandomVector(const Vector4<T>& axis, T angle);

  private:
    Real m_x, m_y, m_z, m_w;
  };


  //-------------------------------------------------------------------------------
  //	@	Vector4::Vector4()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real>::Vector4(const Vector4<Real>& a_other) :
    m_x(a_other.m_x), m_y(a_other.m_y), m_z(a_other.m_z), m_w(a_other.m_w)
  {

  }   // End:  Vector4::Vector4()


  //-------------------------------------------------------------------------------
  //	@	Vector4::operator=()
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
  template<typename Real>
  inline void Vector4<Real>::Zero()
  {
    m_x = m_y = m_z = m_w = static_cast<Real>(0.0);

  }   // End: Vector4::Zero()

  //-------------------------------------------------------------------------------
  //	@	Vector4::Length()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Real Vector4<Real>::Length() const
  {
    return sqrt(m_x*m_x + m_y*m_y + m_z*m_z + m_w*m_w);

  }   // End:  Vector4::Length()


  //-------------------------------------------------------------------------------
  //	@	Vector4::LengthSquared()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Real Vector4<Real>::LengthSquared() const
  {
    return (m_x*m_x + m_y*m_y + m_z*m_z + m_w*m_w);

  }   // End:  Vector4::LengthSquared()


  //-------------------------------------------------------------------------------
  //	@	Vector4::operator==()
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
  template<typename Real>
  bool Vector4<Real>::IsZero() const
  {
    return Dg::IsZero(m_x*m_x + m_y*m_y + m_z*m_z + m_w*m_w);

  }   // End:  Vector4::IsZero()


  //-------------------------------------------------------------------------------
  //	@	Vector4::IsUnit()
  //-------------------------------------------------------------------------------
  template<typename Real>
  bool Vector4<Real>::IsUnit() const
  {
    return Dg::IsZero(static_cast<Real>(1.0) - (m_x*m_x + m_y*m_y + m_z*m_z + m_w*m_w));

  }   // End:  Vector4::IsUnit()


  //-------------------------------------------------------------------------------
  //	@	Vector4::Clean()
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
  template<typename Real>
  Vector4<Real> Vector4<Real>::operator- () const
  {
    return Vector4<Real>(-m_x, -m_y, -m_z, -m_w);

  }	//End: Vector3::operator-()


  //-------------------------------------------------------------------------------
  //	@	Vector4::operator*()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> Vector4<Real>::operator*(Real a_scalar) const
  {
    return Vector4<Real>(a_scalar*m_x, a_scalar*m_y, a_scalar*m_z, a_scalar*m_w);

  }   // End:  operator*()


  //-------------------------------------------------------------------------------
  //	@	Vector4::operator*()
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
  template<typename Real>
  Vector4<Real> Cross(const Vector4<Real>& v1, const Vector4<Real>& v2)
  {
    return Vector4<Real>(v1.m_y*v2.m_z - v1.m_z*v2.m_y,
                         v1.m_z*v2.m_x - v1.m_x*v2.m_z,
                         v1.m_x*v2.m_y - v1.m_y*v2.m_x,
                         static_cast<Real>(0.0));

  }	//End: Cross()


  //--------------------------------------------------------------------------------
  //	@	GetBasis()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void GetBasis(const Vector4<Real>& a_a0,
                const Vector4<Real>& a_a1,
                Vector4<Real>& a_x0,
                Vector4<Real>& a_x1,
                Vector4<Real>& a_x2)
  {
    bool is_a1_zero = a_a1.IsZero();

    //Check for zero vectors, handle separately
    if (a_a0.IsZero())
    {
      //Both x0, x1 are zero vectors
      if (is_a1_zero)
      {
        a_x0 = Vector4<Real>(static_cast<Real>(1.0), static_cast<Real>(0.0), static_cast<Real>(0.0), static_cast<Real>(0.0));
        a_x1 = Vector4<Real>(static_cast<Real>(0.0), static_cast<Real>(1.0), static_cast<Real>(0.0), static_cast<Real>(0.0));
        a_x2 = Vector4<Real>(static_cast<Real>(0.0), static_cast<Real>(0.0), static_cast<Real>(1.0), static_cast<Real>(0.0));

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
  Vector4<Real> Perpendicular(const Vector4<Real>& a_vector)
  {
    if (Dg::IsZero(a_vector.m_x))
    {
      return Vector4<Real>(static_cast<Real>(0.0), 
                          -a_vector.m_z, 
                           a_vector.m_y, 
                           static_cast<Real>(0.0));
    }
    else
    {
      return Vector4<Real>(-a_vector.m_y, 
                            a_vector.m_x, 
                            static_cast<Real>(0.0), 
                            static_cast<Real>(0.0));
    }

  }	//End: Perpendicular()


  //-------------------------------------------------------------------------------
  //		@	GetRandomVector()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> GetRandomVector()
  {
    SimpleRNG generator;

    Real theta = generator.GetUniform<Real>(static_cast<Real>(0.0), static_cast<Real>(2.0) * static_cast<Real>(Dg::PI_d));
    Real phi = generator.GetUniform<Real>(static_cast<Real>(0.0), static_cast<Real>(Dg::PI_d));

    Real sinTheta = sin(theta);

    Real x = sinTheta * cos(phi);
    Real y = sinTheta * sin(phi);
    Real z = cos(theta);

    return Vector4<Real>(x, y, z, static_cast<Real>(0.0));
  }	//End: GetRandomVector()


  //-------------------------------------------------------------------------------
  //		@ GetRandomOrthonormalVector()
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
  }	//End: GetRandomOrthonormalVector()


  //-------------------------------------------------------------------------------
  //		@ GetRandomVector()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> GetRandomVector(const Vector4<Real>& a_axis, Real theta)
  {
    SimpleRNG generator;

    //Find random angle [0, theta]
    Real phi = generator.GetUniform<Real>(static_cast<Real>(0.0), theta);

    return (cos(phi) * a_axis + sin(phi) * GetRandomOrthonormalVector(a_axis));
  }	//End: GetRandomVector()

}


#endif