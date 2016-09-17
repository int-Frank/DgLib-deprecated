//! @file DgVector2.h
//!
//! @author: Frank B. Hart
//! @date 16/09/2016
//!
//! Class declaration: Vector2

#ifndef DGVECTOR2_H
#define DGVECTOR2_H

#include "DgMatrix.h"
#include "dgmath.h"

namespace Dg
{
  template<typename Real> class Vector2;

  //! VQS transform
  template<typename Real>
  Vector2<Real> operator*(Vector2<Real> const &, VQS<Real> const &);

  //! PerpDot product
  template<typename Real>
  Real PerpDot(Vector2<Real> const &, Vector2<Real> const &);

  //! Returns a perpendicular vector.
  template<typename Real>
  Vector2<Real> Perpendicular(Vector2<Real> const & axis);

  //! Squared distance between two points.
  template<typename Real>
  Real SquaredDistance(Vector2<Real> const &, Vector2<Real> const &);

  //! Distance between two points.
  template<typename Real>
  Real Distance(Vector2<Real> const &, Vector2<Real> const &);

  //! @ingroup DgMath_types
  //!
  //! @class Vector2
  //!
  //! @brief Two dimensional vector class [x, y].
  //!
  //! @author Frank Hart
  //! @date 4/10/2015
  template<typename Real>
  class Vector2 : public Matrix<1, 2, Real>
  {
  public:

    //! Default constructor. Members not initialized.
    Vector2() {}

    //! Construct vector from coefficients
    Vector2(Real x, Real y);
    ~Vector2() {}

    //! Copy constructor
    Vector2(Matrix<1, 2, Real> const & a_other) : Matrix<1, 2, Real>(a_other) {}

    //! Assignment
    Vector2& operator=(Matrix<1, 2, Real> const &);

    //! Determines if the vector is the unit vector within some tolerance.
    bool IsUnit() const;

    //! Set elements
    void Set(Real x, Real y);

    //! Make unit vector
    void Normalize();

    //! Vector length
    Real Length() const;

    //! Squared length
    Real LengthSquared() const;

    //! Squared distance between two points.
    friend Real Dg::SquaredDistance(Vector2<Real> const &, Vector2<Real> const &);

    //! Distance between two points.
    friend Real Dg::Distance(Vector2<Real> const &, Vector2<Real> const &);

    //! Access element-x by value
    Real x() const { return m_V[0]; }

    //! Access element-y by value
    Real y() const { return m_V[1]; }

    //! Access element-x by reference
    Real & x() { return m_V[0]; }

    //! Access element-y by reference
    Real & y() { return m_V[1]; }

  public:

    //! v = [0, 0, 0, 1]
    static Vector2 Origin();

    //! v = [0, 0, 0, 0]
    static Vector2 ZeroVector();

    //! v = [1, 0, 0, 0]
    static Vector2 xAxis();

    //! v = [0, 1, 0, 0]
    static Vector2 yAxis();

    //! v = [1, 1, 1, 1]
    static Vector2 Ones();
  };


  //-------------------------------------------------------------------------------
  //	@	Vector2::Origin()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector2<Real> Vector2<Real>::Origin()
  {
    return Vector2(static_cast<Real>(0.0),
                   static_cast<Real>(0.0));
  }   // End:  Vector2::Origin()


  //-------------------------------------------------------------------------------
  //	@	Vector2::ZeroVector()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector2<Real> Vector2<Real>::ZeroVector()
  {
    return Vector2(static_cast<Real>(0.0),
                   static_cast<Real>(0.0));
  }   // End:  Vector2::ZeroVector()


  //-------------------------------------------------------------------------------
  //	@	Vector2::xAxis()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector2<Real> Vector2<Real>::xAxis()
  {
    return Vector2(static_cast<Real>(1.0),
                   static_cast<Real>(0.0));
  }   // End:  Vector2::xAxis()


  //-------------------------------------------------------------------------------
  //	@	Vector2::yAxis()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector2<Real> Vector2<Real>::yAxis()
  {
    return Vector2(static_cast<Real>(0.0),
                   static_cast<Real>(1.0));
  }   // End:  Vector2::yAxis()


  //-------------------------------------------------------------------------------
  //	@	Vector2::Ones()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector2<Real> Vector2<Real>::Ones()
  {
    return Vector2(static_cast<Real>(1.0),
                   static_cast<Real>(1.0));
  }   // End:  Vector2::Ones()


  //-------------------------------------------------------------------------------
  //	@	Vector2::operator=()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector2<Real>& Vector2<Real>::operator=(Matrix<1, 2, Real> const & a_other)
  {
    Matrix<1, 2, Real>::operator=(a_other);
    return *this;
  }   // End:  Vector2::operator=()


  //-------------------------------------------------------------------------------
  //	@	Vector2::Vector2()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector2<Real>::Vector2(Real a_x, Real a_y)
  {
    m_V[0] = a_x;
    m_V[1] = a_y;
  }   // End:  Vector2::Vector2()

  //-------------------------------------------------------------------------------
  //	@	Vector2::Set()
  //-------------------------------------------------------------------------------
  template<typename Real>
  void Vector2<Real>::Set(Real a_x, Real a_y)
  {
    m_V[0] = a_x;
    m_V[1] = a_y;
  }   // End: Vector2::Set()

  //-------------------------------------------------------------------------------
  //	@	Vector2::Length()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Real Vector2<Real>::Length() const
  {
    return sqrt(m_V[0] * m_V[0] +
                m_V[1] * m_V[1]);

  }   // End:  Vector2::Length()


  //-------------------------------------------------------------------------------
  //	@	Vector2::LengthSquared()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Real Vector2<Real>::LengthSquared() const
  {
    return (m_V[0] * m_V[0] +
            m_V[1] * m_V[1]);

  }   // End:  Vector2::LengthSquared()


  //-------------------------------------------------------------------------------
  //	@	SquaredDistance()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Real SquaredDistance(Vector2<Real> const & a_p0,
                       Vector2<Real> const & a_p1)
  {
    Real a = a_p0.m_V[0] - a_p1.m_V[0];
    Real b = a_p0.m_V[1] - a_p1.m_V[1];

    return a*a + b*b;

  } // End: SquaredDistance()


  //-------------------------------------------------------------------------------
  //	@	Distance()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Real Distance(Vector2<Real> const & a_p0,
    Vector2<Real> const & a_p1)
  {
    Real a = a_p0.m_V[0] - a_p1.m_V[0];
    Real b = a_p0.m_V[1] - a_p1.m_V[1];

    return sqrt(a*a + b*b);

  } // End: Distance()


  //-------------------------------------------------------------------------------
  //	@	Vector2::IsUnit()
  //-------------------------------------------------------------------------------
  template<typename Real>
  bool Vector2<Real>::IsUnit() const
  {
    return Dg::IsZero(static_cast<Real>(1.0) - LengthSquared());

  }   // End:  Vector2::IsUnit()


  //-------------------------------------------------------------------------------
  //	@	Vector2::Normalize()
  //-------------------------------------------------------------------------------
  template<typename Real>
  void Vector2<Real>::Normalize()
  {
    Real lengthsq = LengthSquared();

    if (Dg::IsZero(lengthsq))
    {
      m_V[0] = static_cast<Real>(1.0);
      m_V[1] = static_cast<Real>(0.0);
    }
    else
    {
      Real factor = static_cast<Real>(1.0) / sqrt(lengthsq);
      m_V[0] *= factor;
      m_V[1] *= factor;
    }

  }   // End:  Vector2::Normalize()


  //-------------------------------------------------------------------------------
  //	@	PerpDot()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Real PerpDot(Vector2<Real> const & v1, Vector2<Real> const & v2)
  {
    return v1[0] * v2[1] - v1[1] * v2[0];
  }	//End: PerpDot()


  //--------------------------------------------------------------------------------
  //	@	Perpendicular()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Vector2<Real> Perpendicular(Vector2<Real> const & a_vector)
  {
    return Vector2<Real>(-a_vector.m_V[1], a_vector.m_V[0]);
  }	//End: Perpendicular()
}


#endif