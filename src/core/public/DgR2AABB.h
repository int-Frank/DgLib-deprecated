//! @file DgR3AABB.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: AABB

#ifndef DGR3AABB_H
#define DGR3AABB_H

#include "DgR2Vector.h"
#include "dgmath.h"

//TODO Queries
namespace Dg
{
  namespace R2
  {
    //! @ingroup DgMath_types
    //!
    //! @class AABB
    //!
    //! Axis Aligned Bounding Box.
    //!
    //! @author: Frank Hart
    //! @date 20/06/2016
    template<typename Real>
    class AABB
    {
    public:
      //! Default constructor.
      AABB()
        : m_center(Vector<Real>::Origin())
        , m_halfLengths{ static_cast<Real>(0.5), static_cast<Real>(0.5) }
      {}

      //! Constructor.
      AABB(Vector<Real> const a_center, Real a_xl, Real a_yl)
        : m_center(a_center)
        , m_halfLengths{ a_xl, a_yl }
      {}

      //! Construct AABB from origin and radius
      ~AABB() {}

      //! Copy constructor
      AABB(AABB const & a_other)
        : m_center(a_other.m_center)
        , m_halfLengths{ a_other.m_halfLengths[0], a_other.m_halfLengths[1] }
      {}

      //! Assignment
      AABB& operator= (AABB const & a_other)
      {
        m_center = a_other.m_center;

        m_halfLengths[0] = a_other.m_halfLengths[0];
        m_halfLengths[1] = a_other.m_halfLengths[1];

        return *this;
      }

      //! Get the AABB half lengths.
      void GetHalfLengths(Real & a_xl,
                          Real & a_yl) const
      {
        a_xl = m_halfLengths[0];
        a_yl = m_halfLengths[1];
      }

      //! Get the AABB center.
      Vector<Real> const & GetCenter() const { return m_center; }

      //! Set the AABB half lengths.
      void SetHalfLengths(Real a_xl, Real a_yl)
      {
        m_halfLengths[0] = a_xl;
        m_halfLengths[1] = a_yl;
      }

      //! Set the AABB center.
      void SetCenter(Vector<Real> const & a_center)
      {
        m_center = a_center;
      }

    private:
      Vector<Real>    m_center;
      Real            m_halfLengths[2]; //[x, y]
    };
  }
}

#endif