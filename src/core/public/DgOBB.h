//! @file DgOBB.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: OBB

#ifndef DGOBB_H
#define DGOBB_H

#include "DgVector4.h"
#include "DgVQS.h"
#include "dgmath.h"
#include "DgRNG.h"

//TODO Queries
namespace Dg
{

  //! @ingroup DgMath_types
  //!
  //! @class OBB
  //!
  //! Orientated Bounding Box. A 3D rectangular prism that can be rotated and
  //! transformed. It is defined by an orientation (basis), center and length
  //! of the three sides. Typically used as a bounding geometry.
  //!
  //! @author: Frank Hart
  //! @date 20/06/2016
  template<typename Real>
  class OBB
  {
  public:
    //! Default constructor.
    OBB() : m_basis {Vector4<Real>::xAxis(), Vector4<Real>::yAxis(), Vector4<Real>::zAxis() }
          , m_center(Vector4<Real>::Origin())
          , m_halfLengths{ static_cast<Real>(1.0), static_cast<Real>(1.0), static_cast<Real>(1.0)}
    {}

    //! Constructor.
    OBB(Vector4<Real> const & forward
      , Vector4<Real> const & left
      , Vector4<Real> const & up
      , Real lf, Real ll, Real lu
      , Vector4<Real> const & center) 
        : m_basis {forward, left, up}
        , m_center(center)
        , m_halfLengths{lf, ll, lu}
    {}

    //! Construct OBB from origin and radius
    ~OBB() {}

    //! Copy constructor
    OBB(OBB const & a_other)
      : m_basis { a_other.m_basis[0], a_other.m_basis[1], a_other.m_basis[2] }
      , m_center(a_other.m_center)
      , m_halfLengths{ a_other.m_halfLengths[0], a_other.m_halfLengths[1], a_other.m_halfLengths[2]}
    {}

    //! Assignment
    OBB& operator= (OBB const & a_other)
    {
      m_basis[0] = a_other.m_basis[0];
      m_basis[1] = a_other.m_basis[1];
      m_basis[2] = a_other.m_basis[2];

      m_center = a_other.m_center;

      m_halfLengths[0] = a_other.m_halfLengths[0];
      m_halfLengths[1] = a_other.m_halfLengths[1];
      m_halfLengths[2] = a_other.m_halfLengths[2];
    }

    //! Get the OBB basis vectors.
    void GetBasis(Vector4<Real> & a_forward, 
                  Vector4<Real> & a_left, 
                  Vector4<Real> & a_up) const
    {
      a_forward = m_basis[0];
      a_left    = m_basis[1];
      a_up      = m_basis[2];
    }

    //! Get the OBB half lengths.
    void GetHalfLengths(Real & a_forward
                      , Real & a_left
                      , Real & a_up) const
    {
      a_forward = m_halfLengths[0];
      a_left    = m_halfLengths[1];
      a_up      = m_halfLengths[2];
    }

    //! Get the OBB center.
    Vector4<Real> const & GetCenter() const { return m_center; }

    //! Set the OBB basis vectors. It is up to the user to ensure
    //! this is a valid orthogonal basis.
    void SetBasis(Vector4<Real> const & a_forward
                , Vector4<Real> const & a_left
                , Vector4<Real> const & a_up)
    {
      m_basis[0] = a_forward;
      m_basis[1] = a_left;
      m_basis[2] = a_up;
    }

    //! Set the OBB half lengths.
    void SetHalfLengths(Real a_lf, Real a_ll, Real a_lu)
    {
      m_halfLengths[0] = a_lf;
      m_halfLengths[1] = a_ll;
      m_halfLengths[2] = a_lu;
    }

    //! Set the OBB center.
    void SetCenter(Vector4<Real> const & a_center)
    {
      m_center = a_center;
    }

    //! Transform the OBB
    OBB GetTransformed(VQS<Real> const & a_vqs) const
    {
      return OBB
      (
        a_vqs.Rotate(m_basis[0]),
        a_vqs.Rotate(m_basis[1]),
        a_vqs.Rotate(m_basis[2]),
        m_halfLengths[0],
        m_halfLengths[1],
        m_halfLengths[2],
        a_vqs.TransformPoint(m_center)
      );
    }

    //! Transform the OBB, assign to self
    OBB & TransformSelf(VQS<Real> const & a_vqs)
    {
      a_vqs.RotateSelf(m_basis[0]);
      a_vqs.RotateSelf(m_basis[1]);
      a_vqs.RotateSelf(m_basis[2]);
      a_vqs.TransformPointSelf(m_center);
    }

    //! Get a random point inside the OBB
    Vector4<Real> GetRandomPointInside() const
    {
      RNG rng;
      return Vector4<Real>
        (
          m_center + rng.GetUniform(-m_halfLengths[0], m_halfLengths[0]) * m_basis[0],
          m_center + rng.GetUniform(-m_halfLengths[1], m_halfLengths[1]) * m_basis[1],
          m_center + rng.GetUniform(-m_halfLengths[2], m_halfLengths[2]) * m_basis[2],
          static_cast<Real>(1.0)
        )
    }

  private:
    Vector4<Real>   m_basis[3];       //[f, l, u]
    Vector4<Real>   m_center;
    Real            m_halfLengths[3]; //[f, l, u]
  };


}

#endif