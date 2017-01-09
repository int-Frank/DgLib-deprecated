//! @file DgR2Vector3.h
//!
//! @author: Frank B. Hart
//! @date 4/10/2015
//!
//! Class declaration: Vector3

#ifndef DGVECTOR2D_H
#define DGVECTOR2D_H

#include "DgR3Vector4.h"
#include "DgMatrix.h"
#include "dgmath.h"

namespace Dg
{
  namespace R2
  {
    template<typename Real> class Vector3;
    template<typename Real> class Matrix33;

    //! Perpendicular dot product, assumes 2D vector
    template<typename Real>
    Real PerpDot(Vector3<Real> const &, Vector3<Real> const &);

    //! Returns a perpendicular vector.
    template<typename Real>
    Vector3<Real> Perpendicular(Vector3<Real> const & axis);

    //! Squared distance between two points.
    template<typename Real>
    Real SquaredDistance(Vector3<Real> const &, Vector3<Real> const &);

    //! Distance between two points.
    template<typename Real>
    Real Distance(Vector3<Real> const &, Vector3<Real> const &);

    //! @ingroup DgMath_types
    //!
    //! @class Vector3
    //!
    //! @brief Two dimensional homogeneous vector class [x, y, w].
    //!
    //! @author Frank Hart
    //! @date 4/10/2015
    template<typename Real>
    class Vector3 : public Matrix<1, 3, Real>
    {
      friend class Matrix33<Real>;

    public:

      //! Default constructor. Members not initialized.
      Vector3() {}

      //! Construct vector from coefficients
      Vector3(Real x, Real y, Real w);
      Vector3(R3::Vector4<Real> const &);
      ~Vector3() {}

      //! Copy constructor
      Vector3(Matrix<1, 3, Real> const & a_other) : Matrix<1, 3, Real>(a_other) {}

      //! Assignment
      Vector3& operator=(Matrix<1, 3, Real> const &);

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
      friend Real Dg::R2::SquaredDistance(Vector3<Real> const &, Vector3<Real> const &);

      //! Distance between two points.
      friend Real Dg::R2::Distance(Vector3<Real> const &, Vector3<Real> const &);

      //! Access element-x by value
      Real x() const { return m_V[0]; }

      //! Access element-y by value
      Real y() const { return m_V[1]; }

      //! Access element-w by value
      Real w() const { return m_V[2]; }

      //! Access element-x by reference
      Real & x() { return m_V[0]; }

      //! Access element-y by reference
      Real & y() { return m_V[1]; }

      //! Access element-w by reference
      Real & w() { return m_V[2]; }

    public:

      //! v = [0, 0, 1]
      static Vector3 Origin();

      //! v = [0, 0, 0]
      static Vector3 ZeroVector();

      //! v = [1, 0, 0]
      static Vector3 xAxis();

      //! v = [0, 1, 0]
      static Vector3 yAxis();

      //! v = [1, 1, 1]
      static Vector3 Ones();
    };


    //-------------------------------------------------------------------------------
    //	@	Vector3::Origin()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector3<Real> Vector3<Real>::Origin()
    {
      return Vector3(static_cast<Real>(0.0),
                     static_cast<Real>(0.0),
                     static_cast<Real>(1.0));
    }   // End:  Vector3::Origin()


    //-------------------------------------------------------------------------------
    //	@	Vector3::ZeroVector()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector3<Real> Vector3<Real>::ZeroVector()
    {
      return Vector3(static_cast<Real>(0.0),
                     static_cast<Real>(0.0),
                     static_cast<Real>(0.0));
    }   // End:  Vector3::ZeroVector()


    //-------------------------------------------------------------------------------
    //	@	Vector3::xAxis()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector3<Real> Vector3<Real>::xAxis()
    {
      return Vector3(static_cast<Real>(1.0),
                     static_cast<Real>(0.0),
                     static_cast<Real>(0.0));
    }   // End:  Vector3::xAxis()


    //-------------------------------------------------------------------------------
    //	@	Vector3::yAxis()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector3<Real> Vector3<Real>::yAxis()
    {
      return Vector3(static_cast<Real>(0.0),
                     static_cast<Real>(1.0),
                     static_cast<Real>(0.0));
    }   // End:  Vector3::yAxis()


    //-------------------------------------------------------------------------------
    //	@	Vector3::Ones()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector3<Real> Vector3<Real>::Ones()
    {
      return Vector3(static_cast<Real>(1.0),
                     static_cast<Real>(1.0),
                     static_cast<Real>(1.0));
    }   // End:  Vector3::Ones()


    //-------------------------------------------------------------------------------
    //  @	Vector3::operator=()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector3<Real>& Vector3<Real>::operator=(Matrix<1, 3, Real> const & a_other)
    {
      Matrix<1, 3, Real>::operator=(a_other);
      return *this;
    }   // End:  Vector3::operator=()


    //-------------------------------------------------------------------------------
    //	@	Vector3::Vector3()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector3<Real>::Vector3(Real a_x, Real a_y, Real a_w)
    {
      m_V[0] = a_x;
      m_V[1] = a_y;
      m_V[2] = a_w;
    }   // End:  Vector3::Vector3()


    //-------------------------------------------------------------------------------
    //	@	Vector3::Vector3()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector3<Real>::Vector3(R3::Vector4<Real> const & a_v)
    {
      m_V[0] = a_v.x();
      m_V[1] = a_v.y();
      m_V[2] = a_v.w();
    }   // End:  Vector3::Vector3()


    //-------------------------------------------------------------------------------
    //	@	Vector3::Set()
    //-------------------------------------------------------------------------------
    template<typename Real>
    void Vector3<Real>::Set(Real a_x, Real a_y, Real a_w)
    {
      m_V[0] = a_x;
      m_V[1] = a_y;
      m_V[2] = a_w;

    }   // End: Vector3::Set()

    //-------------------------------------------------------------------------------
    //	@	Vector3::Length()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Real Vector3<Real>::Length() const
    {
      return sqrt(m_V[0] * m_V[0] +
                  m_V[1] * m_V[1] +
                  m_V[2] * m_V[2]);

    }   // End:  Vector3::Length()


    //-------------------------------------------------------------------------------
    //	@	Vector3::LengthSquared()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Real Vector3<Real>::LengthSquared() const
    {
      return (m_V[0] * m_V[0] +
              m_V[1] * m_V[1] +
              m_V[2] * m_V[2]);

    }   // End:  Vector3::LengthSquared()


    //-------------------------------------------------------------------------------
    //	@	SquaredDistance()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Real SquaredDistance(Vector3<Real> const & a_p0,
      Vector3<Real> const & a_p1)
    {
      Real a = a_p0.m_V[0] - a_p1.m_V[0];
      Real b = a_p0.m_V[1] - a_p1.m_V[1];

      return a*a + b*b;

    } // End: SquaredDistance()


    //-------------------------------------------------------------------------------
    //	@	Distance()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Real Distance(Vector3<Real> const & a_p0,
      Vector3<Real> const & a_p1)
    {
      Real a = a_p0.m_V[0] - a_p1.m_V[0];
      Real b = a_p0.m_V[1] - a_p1.m_V[1];

      return sqrt(a*a + b*b);

    } // End: Distance()


    //-------------------------------------------------------------------------------
    //	@	Vector3::IsUnit()
    //-------------------------------------------------------------------------------
    template<typename Real>
    bool Vector3<Real>::IsUnit() const
    {
      return Dg::IsZero(static_cast<Real>(1.0) - LengthSquared());

    }   // End:  Vector3::IsUnit()


    //-------------------------------------------------------------------------------
    //	@	Vector3::Normalize()
    //-------------------------------------------------------------------------------
    template<typename Real>
    void Vector3<Real>::Normalize()
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

    }   // End:  Vector3::Normalize()


    //-------------------------------------------------------------------------------
    //	@	PerpDot()
    //--------------------------------------------------------------------------------
    template<typename Real>
    Real PerpDot(Vector3<Real> const & v1, Vector3<Real> const & v2)
    {
      return v1[0] * v2[1] - v1[1] * v2[0];

    }	//End: PerpDot()


    //--------------------------------------------------------------------------------
    //	@	Perpendicular()
    //--------------------------------------------------------------------------------
    template<typename Real>
    Vector3<Real> Perpendicular(Vector3<Real> const & a_vector)
    {
      return Vector3<Real>(-a_vector[1],
                           a_vector[0],
                           static_cast<Real>(0.0));

    }	//End: Perpendicular()
  }
}

#endif