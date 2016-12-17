//! @file DgVector2D.h
//!
//! @author: Frank B. Hart
//! @date 4/10/2015
//!
//! Class declaration: Vector2D

#ifndef DGVECTOR2D_H
#define DGVECTOR2D_H

#include "DgVector4.h"
#include "DgMatrix.h"
#include "dgmath.h"

namespace Dg
{
  template<typename Real> class Vector2D;
  template<typename Real> class Matrix33;

  //! VQS transform
  template<typename Real>
  Vector2D<Real> operator*(Vector2D<Real> const &, VQS<Real> const &);

  //! Perpendicular dot product, assumes 2D vector
  template<typename Real>
  Real PerpDot(Vector2D<Real> const &, Vector2D<Real> const &);

  //! Returns a perpendicular vector.
  template<typename Real>
  Vector2D<Real> Perpendicular(Vector2D<Real> const & axis);

  //! Squared distance between two points.
  template<typename Real>
  Real SquaredDistance(Vector2D<Real> const &, Vector2D<Real> const &);

  //! Distance between two points.
  template<typename Real>
  Real Distance(Vector2D<Real> const &, Vector2D<Real> const &);

  //! @ingroup DgMath_types
  //!
  //! @class Vector2D
  //!
  //! @brief Two dimensional homogeneous vector class [x, y, w].
  //!
  //! @author Frank Hart
  //! @date 4/10/2015
  template<typename Real>
  class Vector2D : public Matrix<1, 3, Real>
  {
    friend class Matrix33<Real>;

  public:

    //! Default constructor. Members not initialized.
    Vector2D() {}

    //! Construct vector from coefficients
    Vector2D(Real x, Real y, Real w);
    Vector2D(Vector4<Real> const &);
    ~Vector2D() {}

    //! Copy constructor
    Vector2D(Matrix<1, 3, Real> const & a_other) : Matrix<1, 3, Real>(a_other) {}

    //! Assignment
    Vector2D& operator=(Matrix<1, 3, Real> const &);

    //! Determines if the vector is the unit vector within some tolerance.
    bool IsUnit() const;

    //! Set elements
    void Set(Real x, Real y, Real w);

    //! Make unit vector
    void Normalize();

    //! Vector length
    Real Length() const;

    //! Squared length
    Real LengthSquared() const;

    //! Squared distance between two points.
    friend Real Dg::SquaredDistance(Vector2D<Real> const &, Vector2D<Real> const &);

    //! Distance between two points.
    friend Real Dg::Distance(Vector2D<Real> const &, Vector2D<Real> const &);

    //! Access element-x by value
    Real x() const { return m_V[0]; }

    //! Access element-y by value
    Real y() const { return m_V[1]; }

    //! Access element-w by value
    Real w() const { return m_V[3]; }

    //! Access element-x by reference
    Real & x() { return m_V[0]; }

    //! Access element-y by reference
    Real & y() { return m_V[1]; }

    //! Access element-w by reference
    Real & w() { return m_V[3]; }

  public:

    //! v = [0, 0, 0, 1]
    static Vector2D Origin();

    //! v = [0, 0, 0, 0]
    static Vector2D ZeroVector();

    //! v = [1, 0, 0, 0]
    static Vector2D xAxis();

    //! v = [0, 1, 0, 0]
    static Vector2D yAxis();

    //! v = [1, 1, 1, 1]
    static Vector2D Ones();
  };


  //-------------------------------------------------------------------------------
  //	@	Vector2D::Origin()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector2D<Real> Vector2D<Real>::Origin()
  {
    return Vector2D(static_cast<Real>(0.0),
                   static_cast<Real>(0.0),
                   static_cast<Real>(1.0));
  }   // End:  Vector2D::Origin()


  //-------------------------------------------------------------------------------
  //	@	Vector2D::ZeroVector()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector2D<Real> Vector2D<Real>::ZeroVector()
  {
    return Vector2D(static_cast<Real>(0.0),
                   static_cast<Real>(0.0),
                   static_cast<Real>(0.0));
  }   // End:  Vector2D::ZeroVector()


  //-------------------------------------------------------------------------------
  //	@	Vector2D::xAxis()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector2D<Real> Vector2D<Real>::xAxis()
  {
    return Vector2D(static_cast<Real>(1.0),
                   static_cast<Real>(0.0),
                   static_cast<Real>(0.0));
  }   // End:  Vector2D::xAxis()


  //-------------------------------------------------------------------------------
  //	@	Vector2D::yAxis()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector2D<Real> Vector2D<Real>::yAxis()
  {
    return Vector2D(static_cast<Real>(0.0),
                   static_cast<Real>(1.0),
                   static_cast<Real>(0.0));
  }   // End:  Vector2D::yAxis()


  //-------------------------------------------------------------------------------
  //	@	Vector2D::Ones()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector2D<Real> Vector2D<Real>::Ones()
  {
    return Vector2D(static_cast<Real>(1.0),
                   static_cast<Real>(1.0),
                   static_cast<Real>(1.0));
  }   // End:  Vector2D::Ones()


  //-------------------------------------------------------------------------------
  //	@	Vector2D::operator=()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector2D<Real>& Vector2D<Real>::operator=(Matrix<1, 3, Real> const & a_other)
  {
    Matrix<1, 3, Real>::operator=(a_other);
    return *this;
  }   // End:  Vector2D::operator=()


  //-------------------------------------------------------------------------------
  //	@	Vector2D::Vector2D()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector2D<Real>::Vector2D(Real a_x, Real a_y, Real a_w)
  {
    m_V[0] = a_x;
    m_V[1] = a_y;
    m_V[3] = a_w;
  }   // End:  Vector2D::Vector2D()

  //-------------------------------------------------------------------------------
  //	@	Vector2D::Set()
  //-------------------------------------------------------------------------------
  template<typename Real>
  void Vector2D<Real>::Set(Real a_x, Real a_y, Real a_w)
  {
    m_V[0] = a_x;
    m_V[1] = a_y;
    m_V[3] = a_w;

  }   // End: Vector2D::Set()

  //-------------------------------------------------------------------------------
  //	@	Vector2D::Length()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Real Vector2D<Real>::Length() const
  {
    return sqrt(m_V[0] * m_V[0] + 
                m_V[1] * m_V[1] + 
                m_V[2] * m_V[2]);

  }   // End:  Vector2D::Length()


  //-------------------------------------------------------------------------------
  //	@	Vector2D::LengthSquared()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Real Vector2D<Real>::LengthSquared() const
  {
    return (m_V[0] * m_V[0] + 
            m_V[1] * m_V[1] + 
            m_V[2] * m_V[2]);

  }   // End:  Vector2D::LengthSquared()


  //-------------------------------------------------------------------------------
  //	@	SquaredDistance()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Real SquaredDistance(Vector2D<Real> const & a_p0,
                       Vector2D<Real> const & a_p1)
  {
    Real a = a_p0.m_V[0] - a_p1.m_V[0];
    Real b = a_p0.m_V[1] - a_p1.m_V[1];

    return a*a + b*b;

  } // End: SquaredDistance()


  //-------------------------------------------------------------------------------
  //	@	Distance()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Real Distance(Vector2D<Real> const & a_p0,
                Vector2D<Real> const & a_p1)
  {
    Real a = a_p0.m_V[0] - a_p1.m_V[0];
    Real b = a_p0.m_V[1] - a_p1.m_V[1];

    return sqrt(a*a + b*b);

  } // End: Distance()


  //-------------------------------------------------------------------------------
  //	@	Vector2D::IsUnit()
  //-------------------------------------------------------------------------------
  template<typename Real>
  bool Vector2D<Real>::IsUnit() const
  {
    return Dg::IsZero(static_cast<Real>(1.0) - LengthSquared());

  }   // End:  Vector2D::IsUnit()


  //-------------------------------------------------------------------------------
  //	@	Vector2D::Normalize()
  //-------------------------------------------------------------------------------
  template<typename Real>
  void Vector2D<Real>::Normalize()
  {
    Real lengthsq = LengthSquared();

    if (Dg::IsZero(lengthsq))
    {
      m_V[0] = static_cast<Real>(1.0);
      m_V[1] = static_cast<Real>(0.0);
      m_V[2] = static_cast<Real>(0.0);
    }
    else
    {
      Real factor = static_cast<Real>(1.0) / sqrt(lengthsq);
      m_V[0] *= factor;
      m_V[1] *= factor;
      m_V[2] *= factor;
    }

  }   // End:  Vector2D::Normalize()


  //-------------------------------------------------------------------------------
  //	@	PerpDot()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Real PerpDot(Vector2D<Real> const & v1, Vector2D<Real> const & v2)
  {
    return v1[0] * v2[1] - v1[1] * v2[0];

  }	//End: PerpDot()


  //--------------------------------------------------------------------------------
  //	@	Perpendicular()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Vector2D<Real> Perpendicular(Vector2D<Real> const & a_vector)
  {
    return Vector2D(-a_vector[1]
                 , a_vector[0]
                 , static_cast<Real>(0.0));

  }	//End: Perpendicular()
}


#endif