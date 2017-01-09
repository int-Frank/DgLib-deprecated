//! @file DgR2Vector.h
//!
//! @author: Frank B. Hart
//! @date 4/10/2015
//!
//! Class declaration: Vector

#ifndef DGVECTOR2D_H
#define DGVECTOR2D_H

#include "DgR3Vector.h"
#include "DgMatrix.h"
#include "dgmath.h"

namespace Dg
{
  namespace R2
  {
    template<typename Real> class Vector;
    template<typename Real> class Matrix;

    //! Perpendicular dot product, assumes 2D vector
    template<typename Real>
    Real PerpDot(Vector<Real> const &, Vector<Real> const &);

    //! Returns a perpendicular vector.
    template<typename Real>
    Vector<Real> Perpendicular(Vector<Real> const & axis);

    //! Squared distance between two points.
    template<typename Real>
    Real SquaredDistance(Vector<Real> const &, Vector<Real> const &);

    //! Distance between two points.
    template<typename Real>
    Real Distance(Vector<Real> const &, Vector<Real> const &);

    //! @ingroup DgMath_types
    //!
    //! @class Vector
    //!
    //! @brief Two dimensional homogeneous vector class [x, y, w].
    //!
    //! @author Frank Hart
    //! @date 4/10/2015
    template<typename Real>
    class Vector : public Dg::Matrix<1, 3, Real>
    {
      friend class Matrix<Real>;

    public:

      //! Default constructor. Members not initialized.
      Vector() {}

      //! Construct vector from coefficients
      Vector(Real x, Real y, Real w);
      Vector(R3::Vector<Real> const &);
      ~Vector() {}

      //! Copy constructor
      Vector(Dg::Matrix<1, 3, Real> const & a_other) : Dg::Matrix<1, 3, Real>(a_other) {}

      //! Assignment
      Vector& operator=(Matrix<1, 3, Real> const &);

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
      friend Real Dg::R2::SquaredDistance(Vector<Real> const &, Vector<Real> const &);

      //! Distance between two points.
      friend Real Dg::R2::Distance(Vector<Real> const &, Vector<Real> const &);

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
      static Vector Origin();

      //! v = [0, 0, 0]
      static Vector ZeroVector();

      //! v = [1, 0, 0]
      static Vector xAxis();

      //! v = [0, 1, 0]
      static Vector yAxis();

      //! v = [1, 1, 1]
      static Vector Ones();
    };


    //-------------------------------------------------------------------------------
    //	@	Vector::Origin()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector<Real> Vector<Real>::Origin()
    {
      return Vector(static_cast<Real>(0.0),
                     static_cast<Real>(0.0),
                     static_cast<Real>(1.0));
    }   // End:  Vector::Origin()


    //-------------------------------------------------------------------------------
    //	@	Vector::ZeroVector()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector<Real> Vector<Real>::ZeroVector()
    {
      return Vector(static_cast<Real>(0.0),
                     static_cast<Real>(0.0),
                     static_cast<Real>(0.0));
    }   // End:  Vector::ZeroVector()


    //-------------------------------------------------------------------------------
    //	@	Vector::xAxis()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector<Real> Vector<Real>::xAxis()
    {
      return Vector(static_cast<Real>(1.0),
                     static_cast<Real>(0.0),
                     static_cast<Real>(0.0));
    }   // End:  Vector::xAxis()


    //-------------------------------------------------------------------------------
    //	@	Vector::yAxis()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector<Real> Vector<Real>::yAxis()
    {
      return Vector(static_cast<Real>(0.0),
                     static_cast<Real>(1.0),
                     static_cast<Real>(0.0));
    }   // End:  Vector::yAxis()


    //-------------------------------------------------------------------------------
    //	@	Vector::Ones()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector<Real> Vector<Real>::Ones()
    {
      return Vector(static_cast<Real>(1.0),
                     static_cast<Real>(1.0),
                     static_cast<Real>(1.0));
    }   // End:  Vector::Ones()


    //-------------------------------------------------------------------------------
    //  @	Vector::operator=()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector<Real>& Vector<Real>::operator=(Dg::Matrix<1, 3, Real> const & a_other)
    {
      Matrix<1, 3, Real>::operator=(a_other);
      return *this;
    }   // End:  Vector::operator=()


    //-------------------------------------------------------------------------------
    //	@	Vector::Vector()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector<Real>::Vector(Real a_x, Real a_y, Real a_w)
    {
      m_V[0] = a_x;
      m_V[1] = a_y;
      m_V[2] = a_w;
    }   // End:  Vector::Vector()


    //-------------------------------------------------------------------------------
    //	@	Vector::Vector()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector<Real>::Vector(R3::Vector<Real> const & a_v)
    {
      m_V[0] = a_v.x();
      m_V[1] = a_v.y();
      m_V[2] = a_v.w();
    }   // End:  Vector::Vector()


    //-------------------------------------------------------------------------------
    //	@	Vector::Set()
    //-------------------------------------------------------------------------------
    template<typename Real>
    void Vector<Real>::Set(Real a_x, Real a_y, Real a_w)
    {
      m_V[0] = a_x;
      m_V[1] = a_y;
      m_V[2] = a_w;

    }   // End: Vector::Set()

    //-------------------------------------------------------------------------------
    //	@	Vector::Length()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Real Vector<Real>::Length() const
    {
      return sqrt(m_V[0] * m_V[0] +
                  m_V[1] * m_V[1] +
                  m_V[2] * m_V[2]);

    }   // End:  Vector::Length()


    //-------------------------------------------------------------------------------
    //	@	Vector::LengthSquared()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Real Vector<Real>::LengthSquared() const
    {
      return (m_V[0] * m_V[0] +
              m_V[1] * m_V[1] +
              m_V[2] * m_V[2]);

    }   // End:  Vector::LengthSquared()


    //-------------------------------------------------------------------------------
    //	@	SquaredDistance()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Real SquaredDistance(Vector<Real> const & a_p0,
      Vector<Real> const & a_p1)
    {
      Real a = a_p0.m_V[0] - a_p1.m_V[0];
      Real b = a_p0.m_V[1] - a_p1.m_V[1];

      return a*a + b*b;

    } // End: SquaredDistance()


    //-------------------------------------------------------------------------------
    //	@	Distance()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Real Distance(Vector<Real> const & a_p0,
      Vector<Real> const & a_p1)
    {
      Real a = a_p0.m_V[0] - a_p1.m_V[0];
      Real b = a_p0.m_V[1] - a_p1.m_V[1];

      return sqrt(a*a + b*b);

    } // End: Distance()


    //-------------------------------------------------------------------------------
    //	@	Vector::IsUnit()
    //-------------------------------------------------------------------------------
    template<typename Real>
    bool Vector<Real>::IsUnit() const
    {
      return Dg::IsZero(static_cast<Real>(1.0) - LengthSquared());

    }   // End:  Vector::IsUnit()


    //-------------------------------------------------------------------------------
    //	@	Vector::Normalize()
    //-------------------------------------------------------------------------------
    template<typename Real>
    void Vector<Real>::Normalize()
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

    }   // End:  Vector::Normalize()


    //-------------------------------------------------------------------------------
    //	@	PerpDot()
    //--------------------------------------------------------------------------------
    template<typename Real>
    Real PerpDot(Vector<Real> const & v1, Vector<Real> const & v2)
    {
      return v1[0] * v2[1] - v1[1] * v2[0];

    }	//End: PerpDot()


    //--------------------------------------------------------------------------------
    //	@	Perpendicular()
    //--------------------------------------------------------------------------------
    template<typename Real>
    Vector<Real> Perpendicular(Vector<Real> const & a_vector)
    {
      return Vector<Real>(-a_vector[1],
                           a_vector[0],
                           static_cast<Real>(0.0));

    }	//End: Perpendicular()
  }
}

#endif