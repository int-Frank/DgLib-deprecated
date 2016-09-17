//! @file DgVector4.h
//!
//! @author: Frank B. Hart
//! @date 4/10/2015
//!
//! Class declaration: Vector4

#ifndef DGVECTOR4_H
#define DGVECTOR4_H

#include "DgMatrix.h"
#include "dgmath.h"

namespace Dg
{
  template<typename Real> class Vector4;
  template<typename Real> class Matrix44;
  template<typename Real> class Plane;
  template<typename Real> class Quaternion;
  template<typename Real> class VQS;

  //! VQS transform
  template<typename Real>
  Vector4<Real> operator*(Vector4<Real> const &, VQS<Real> const &);

  //! Cross product
  template<typename Real>
  Vector4<Real> Cross(Vector4<Real> const &, Vector4<Real> const &);

  //! Creates an orthogonal basis from two input vectors. 
  //!   - The vector 'A_in' is considered the principle axis.
  //!   - If A_in is a zero vector, A_out will be set to Vector4<Real>::xAxis()
  //!   - If A_in and B_in are parallel, or B_in is a Zero vector, B_out will be chosen such that it is orthogonal to A_in
  //!   - Finally, C_out is created from A_out X B_out.
  template<typename Real>
  void GetBasis(Vector4<Real> const & A_in,
                Vector4<Real> const & B_in,
                Vector4<Real>& A_out,
                Vector4<Real>& B_out,
                Vector4<Real>& C_out);

  //! Returns a perpendicular vector.
  template<typename Real>
  Vector4<Real> Perpendicular(Vector4<Real> const & axis);

  //! Squared distance between two points.
  template<typename Real>
  Real SquaredDistance(Vector4<Real> const &, Vector4<Real> const &);

  //! Distance between two points.
  template<typename Real>
  Real Distance(Vector4<Real> const &, Vector4<Real> const &);

  //! @ingroup DgMath_types
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
    friend class Plane<Real>;
    friend class Matrix44<Real>;
    friend class Quaternion<Real>;
    friend class VQS<Real>;

  public:

    //! Default constructor. Members not initialized.
    Vector4() {}

    //! Construct vector from coefficients
    Vector4(Real x, Real y, Real z, Real w);
    ~Vector4() {}

    //! Copy constructor
    Vector4(Matrix<1, 4, Real> const & a_other) : Matrix<1, 4, Real>(a_other) {}

    //! Assignment
    Vector4& operator=(Matrix<1, 4, Real> const &);

    //! Determines if the vector is the unit vector within some tolerance.
    bool IsUnit() const;

    //! Set elements
    void Set(Real x, Real y, Real z, Real w);

    //! Make unit vector
    void Normalize();

    //! Vector length
    Real Length() const;

    //! Squared length
    Real LengthSquared() const;

    //! Squared distance between two points.
    friend Real Dg::SquaredDistance(Vector4<Real> const &, Vector4<Real> const &);

    //! Distance between two points.
    friend Real Dg::Distance(Vector4<Real> const &, Vector4<Real> const &);

    //! Access element-x by value
    Real x() const { return m_V[0]; }

    //! Access element-y by value
    Real y() const { return m_V[1]; }

    //! Access element-z by value
    Real z() const { return m_V[2]; }

    //! Access element-w by value
    Real w() const { return m_V[3]; }

    //! Access element-x by reference
    Real & x() { return m_V[0]; }

    //! Access element-y by reference
    Real & y() { return m_V[1]; }

    //! Access element-z by reference
    Real & z() { return m_V[2]; }

    //! Access element-w by reference
    Real & w() { return m_V[3]; }

  public:

    //! v = [0, 0, 0, 1]
    static Vector4 Origin();

    //! v = [0, 0, 0, 0]
    static Vector4 ZeroVector();

    //! v = [1, 0, 0, 0]
    static Vector4 xAxis();

    //! v = [0, 1, 0, 0]
    static Vector4 yAxis();

    //! v = [0, 0, 1, 0]
    static Vector4 zAxis();

    //! v = [1, 1, 1, 1]
    static Vector4 Ones();
  };


  //-------------------------------------------------------------------------------
  //	@	Vector4::Origin()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> Vector4<Real>::Origin()
  {
    return Vector4(static_cast<Real>(0.0),
      static_cast<Real>(0.0),
      static_cast<Real>(0.0),
      static_cast<Real>(1.0));
  }   // End:  Vector4::Origin()


  //-------------------------------------------------------------------------------
  //	@	Vector4::ZeroVector()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> Vector4<Real>::ZeroVector()
  {
    return Vector4(static_cast<Real>(0.0),
                   static_cast<Real>(0.0), 
                   static_cast<Real>(0.0), 
                   static_cast<Real>(0.0));
  }   // End:  Vector4::ZeroVector()


  //-------------------------------------------------------------------------------
  //	@	Vector4::xAxis()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> Vector4<Real>::xAxis()
  {
    return Vector4(static_cast<Real>(1.0),
                   static_cast<Real>(0.0),
                   static_cast<Real>(0.0),
                   static_cast<Real>(0.0));
  }   // End:  Vector4::xAxis()


  //-------------------------------------------------------------------------------
  //	@	Vector4::yAxis()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> Vector4<Real>::yAxis()
  {
    return Vector4(static_cast<Real>(0.0),
                   static_cast<Real>(1.0),
                   static_cast<Real>(0.0),
                   static_cast<Real>(0.0));
  }   // End:  Vector4::yAxis()


  //-------------------------------------------------------------------------------
  //	@	Vector4::zAxis()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> Vector4<Real>::zAxis()
  {
    return Vector4(static_cast<Real>(0.0),
                   static_cast<Real>(0.0),
                   static_cast<Real>(1.0),
                   static_cast<Real>(0.0));
  }   // End:  Vector4::zAxis()


  //-------------------------------------------------------------------------------
  //	@	Vector4::Ones()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> Vector4<Real>::Ones()
  {
    return Vector4(static_cast<Real>(1.0),
                   static_cast<Real>(1.0),
                   static_cast<Real>(1.0),
                   static_cast<Real>(1.0));
  }   // End:  Vector4::Ones()


  //-------------------------------------------------------------------------------
  //	@	Vector4::operator=()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real>& Vector4<Real>::operator=(Matrix<1, 4, Real> const & a_other)
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
    return sqrt(m_V[0] * m_V[0] + 
                m_V[1] * m_V[1] + 
                m_V[2] * m_V[2] + 
                m_V[3] * m_V[3]);

  }   // End:  Vector4::Length()


  //-------------------------------------------------------------------------------
  //	@	Vector4::LengthSquared()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Real Vector4<Real>::LengthSquared() const
  {
    return (m_V[0] * m_V[0] + 
            m_V[1] * m_V[1] + 
            m_V[2] * m_V[2] + 
            m_V[3] * m_V[3]);

  }   // End:  Vector4::LengthSquared()


  //-------------------------------------------------------------------------------
  //	@	SquaredDistance()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Real SquaredDistance(Vector4<Real> const & a_p0,
                       Vector4<Real> const & a_p1)
  {
    Real a = a_p0.m_V[0] - a_p1.m_V[0];
    Real b = a_p0.m_V[1] - a_p1.m_V[1];
    Real c = a_p0.m_V[2] - a_p1.m_V[2];

    return a*a + b*b + c*c;

  } // End: SquaredDistance()


  //-------------------------------------------------------------------------------
  //	@	Distance()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Real Distance(Vector4<Real> const & a_p0,
                Vector4<Real> const & a_p1)
  {
    Real a = a_p0.m_V[0] - a_p1.m_V[0];
    Real b = a_p0.m_V[1] - a_p1.m_V[1];
    Real c = a_p0.m_V[2] - a_p1.m_V[2];

    return sqrt(a*a + b*b + c*c);

  } // End: Distance()


  //-------------------------------------------------------------------------------
  //	@	Vector4::IsUnit()
  //-------------------------------------------------------------------------------
  template<typename Real>
  bool Vector4<Real>::IsUnit() const
  {
    return Dg::IsZero(static_cast<Real>(1.0) - LengthSquared());

  }   // End:  Vector4::IsUnit()


  //-------------------------------------------------------------------------------
  //	@	Vector4::Normalize()
  //-------------------------------------------------------------------------------
  template<typename Real>
  void Vector4<Real>::Normalize()
  {
    Real lengthsq = LengthSquared();

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
  //	@	Cross()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> Cross(Vector4<Real> const & v1, Vector4<Real> const & v2)
  {
    Vector4<Real> result;
    result[0] = v1[1] * v2[2] - v1[2] * v2[1];
    result[1] = v1[2] * v2[0] - v1[0] * v2[2];
    result[2] = v1[0] * v2[1] - v1[1] * v2[0];
    result[3] = static_cast<Real>(0.0);

    return result;

  }	//End: Cross()


  //--------------------------------------------------------------------------------
  //	@	GetBasis()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void GetBasis(Vector4<Real> const & a_A_in,
                Vector4<Real> const & a_B_in,
                Vector4<Real>& a_A_out,
                Vector4<Real>& a_B_out,
                Vector4<Real>& a_C_out)
  {
    if (a_A_in.IsZero())
    {
      a_A_out = Vector4<Real>::xAxis();
    }
    else
    {
      a_A_out = a_A_in;
      a_A_out.Normalize();
    }

    a_C_out = Cross(a_A_out, a_B_in);

    //Test to see if A_in and B_in are parallel
    if (IsZero(a_C_out.LengthSquared()))
    {
      a_B_out = Perpendicular(a_A_out);
      a_C_out = Cross(a_A_out, a_B_out);
    }
    else
    {
      a_C_out.Normalize();
      a_B_out = Cross(a_C_out, a_A_out);
    }
  } //End: GetBasis()


  //--------------------------------------------------------------------------------
  //	@	Perpendicular()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> Perpendicular(Vector4<Real> const & a_vector)
  {
    Vector4<Real> result;

    if (!Dg::IsZero(a_vector[0]) || !Dg::IsZero(a_vector[1]))
    {
      result[0] = -a_vector[1];
      result[1] = a_vector[0];
      result[2] = static_cast<Real>(0.0);
      result[3] = static_cast<Real>(0.0);
    }
    else
    {
      result[0] = -a_vector[2];
      result[1] = static_cast<Real>(0.0);
      result[2] = a_vector[0];
      result[3] = static_cast<Real>(0.0);
    }

    result.Normalize();
    return result;
  }	//End: Perpendicular()
}


#endif