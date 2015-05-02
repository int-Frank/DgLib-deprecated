//! @file Vector4.h
//!
//! @author: Frank B. Hart
//! @date 4/10/2015
//!
//! Class declaration: Vector4

#ifndef VECTOR4_H
#define VECTOR4_H
 
#include "SimpleRNG.h"
#include "DgMatrix.h"
#include "dgmath.h"

namespace Dg
{
  template<typename Real> class Vector4;
  template<typename Real> class Matrix44;
  template<typename Real> class Plane4;
  template<typename Real> class Quaternion;
  template<typename Real> class VQS;

  template<typename Real>
  Vector4<Real> operator*(Vector4<Real> const &, VQS<Real> const &);

  template<typename Real>
  Real Dot(const Vector4<Real>&, const Vector4<Real>&);

  template<typename Real>
  Vector4<Real> Cross(const Vector4<Real>&, const Vector4<Real>&);

  //! Returns a random unit vector.
  template<typename Real>
 Vector4<Real> GetRandomVector();

  //! Creates an orthogonal basis from two input vectors
  template<typename Real>
  void GetBasis(const Vector4<Real>& a0,
               const Vector4<Real>& a1,
               Vector4<Real>& x0,
               Vector4<Real>& x1,
               Vector4<Real>& x2);

  //! Returns a perpendicular vector.
  template<typename Real>
  Vector4<Real> Perpendicular(const Vector4<Real>& axis);

  //! Returns a random orthonormal vector to an axis.
  //! @pre Input must be a unit vector.
  template<typename Real>
 Vector4<Real> GetRandomOrthonormalVector(const Vector4<Real>& axis);

  //! Returns a random vector at an angle to an axis.
  //! @pre Input axis must be a unit vector.
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
  class Vector4 : public Matrix<1, 4, Real>
  {
    friend class Plane4<Real>;
    friend class Matrix44<Real>;
    friend class Quaternion<Real>;
    friend class VQS<Real>;

  public:

    //! Default constructor. Members not initialized.
    Vector4() {}

    Vector4(Real a_x, Real a_y, Real a_z, Real a_w);
    ~Vector4() {}

    // copy operations
    Vector4(const Matrix<1, 4, Real>& a_other) : Matrix<1, 4, Real>(a_other) {}
    Vector4& operator=(const Matrix<1, 4, Real>&);

    //! Determines if the vector is the unit vector within some tolerance.
    bool IsUnit() const;

    Real x() const { return m_V[0]; }
    Real y() const { return m_V[1]; }
    Real z() const { return m_V[2]; }
    Real w() const { return m_V[3]; }
    Real& x()	   { return m_V[0]; }
    Real& y()	   { return m_V[1]; }
    Real& z()	   { return m_V[2]; }
    Real& w()	   { return m_V[3]; }

    //! Set elements
    void Set(Real a_x, Real a_y, Real a_z, Real a_w);

    //! Make unit vector
    void Normalize();  

    template<typename T>
    friend T Dot(const Vector4<T>&, const Vector4<T>&);

    //! Cross procudt, assumes w = 0.
    template<typename T>
    friend Vector4<T> Cross(const Vector4<T>&, const Vector4<T>&);

    Real Length() const;
    Real LengthSquared() const;

  };

  //-------------------------------------------------------------------------------
  //	@	Vector4::operator=()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real>& Vector4<Real>::operator=(const Matrix<1, 4, Real>& a_other)
  {
    Matrix<1, 4, Real>::operator=(a_other);
    return *this;
  }   // End:  Vector4::operator=()


  //-------------------------------------------------------------------------------
  //	@	Vector4::Vector4()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real>::Vector4(Real a_x, Real a_y, Real a_z, Real a_w)
  {
    m_V[0] = a_x;
    m_V[1] = a_y;
    m_V[2] = a_z;
    m_V[3] = a_w;
  }   // End:  Vector4::Vector4()

  //-------------------------------------------------------------------------------
  //	@	Vector4::Set()
  //-------------------------------------------------------------------------------
  template<typename Real>
  void Vector4<Real>::Set(Real a_x, Real a_y, Real a_z, Real a_w)
  {
    m_V[0] = a_x;
    m_V[1] = a_y;
    m_V[2] = a_z;
    m_V[3] = a_w;

  }   // End: Vector4::Set()

  //-------------------------------------------------------------------------------
  //	@	Vector4::Length()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Real Vector4<Real>::Length() const
  {
    return sqrt(m_V[0]*m_V[0] + m_V[1]*m_V[1] + m_V[2]*m_V[2] + m_V[3]*m_V[3]);

  }   // End:  Vector4::Length()


  //-------------------------------------------------------------------------------
  //	@	Vector4::LengthSquared()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Real Vector4<Real>::LengthSquared() const
  {
    return (m_V[0]*m_V[0] + m_V[1]*m_V[1] + m_V[2]*m_V[2] + m_V[3]*m_V[3]);

  }   // End:  Vector4::LengthSquared()


  //-------------------------------------------------------------------------------
  //	@	Vector4::IsUnit()
  //-------------------------------------------------------------------------------
  template<typename Real>
  bool Vector4<Real>::IsUnit() const
  {
    return Dg::IsZero(static_cast<Real>(1.0) - (m_V[0]*m_V[0] + m_V[1]*m_V[1] + m_V[2]*m_V[2] + m_V[3]*m_V[3]));

  }   // End:  Vector4::IsUnit()


  //-------------------------------------------------------------------------------
  //	@	Vector4::Normalize()
  //-------------------------------------------------------------------------------
  template<typename Real>
  void Vector4<Real>::Normalize()
  {
    Real lengthsq = m_V[0]*m_V[0] + m_V[1]*m_V[1] + m_V[2]*m_V[2] + m_V[3]*m_V[3];

    if (Dg::IsZero(lengthsq))
    {
      m_V[0] = static_cast<Real>(1.0);
      m_V[1] = static_cast<Real>(0.0);
      m_V[2] = static_cast<Real>(0.0);
      m_V[3] = static_cast<Real>(0.0);
    }
    else
    {
      Real factor = static_cast<Real>(1.0) / sqrt(lengthsq);
      m_V[0] *= factor;
      m_V[1] *= factor;
      m_V[2] *= factor;
      m_V[3] *= factor;
    }

  }   // End:  Vector4::Normalize()


  //-------------------------------------------------------------------------------
  //	@	Dot()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Real Dot(const Vector4<Real>& a_v0, const Vector4<Real>& a_v1)
  {
    return (a_v0.m_V[0] * a_v1.m_V[0] +
            a_v0.m_V[1] * a_v1.m_V[1] +
            a_v0.m_V[2] * a_v1.m_V[2] + 
            a_v0.m_V[3] * a_v1.m_V[3]);

  }   // End:  Dot()


  //-------------------------------------------------------------------------------
  //	@	Cross()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> Cross(const Vector4<Real>& v1, const Vector4<Real>& v2)
  {
    return Vector4<Real>(v1.m_V[1]*v2.m_V[2] - v1.m_V[2]*v2.m_V[1],
                         v1.m_V[2]*v2.m_V[0] - v1.m_V[0]*v2.m_V[2],
                         v1.m_V[0]*v2.m_V[1] - v1.m_V[1]*v2.m_V[0],
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