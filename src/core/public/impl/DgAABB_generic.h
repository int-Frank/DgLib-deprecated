//! @file DgAABB_generic.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: AABB_generic

#ifndef DGAABB_GENERIC_H
#define DGAABB_GENERIC_H

#include "DgVector_generic.h"
#include "dgmath.h"

//TODO Queries
namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_types
    //!
    //! @class AABB_generic
    //!
    //! Axis Aligned Bounding Box.
    //!
    //! @author: Frank Hart
    //! @date 20/06/2016
    template<typename Real, int R>
    class AABB_generic
    {
    public:
      //! Default constructor.
      AABB_generic()
        : m_center(Vector_generic<Real, R>::Origin())
        , m_halfLengths{ static_cast<Real>(0.5), static_cast<Real>(0.5) }
      {}

      //! Constructor.
      AABB_generic(Vector_generic<Real, R> const a_center, Real const * a_hl)
        : m_center(a_center)
      {
        for (int i = 0; i < R; ++i)
        {
          m_halfLengths[i] = a_hl[i];
        }
      }

      //! Construct AABB_generic from origin and radius
      ~AABB_generic() {}

      //! Copy constructor
      AABB_generic(AABB_generic const & a_other)
        : m_center(a_other.m_center)
      {
        for (int i = 0; i < R; ++i)
        {
          m_halfLengths[i] = a_other.m_halfLengths[i];
        }
      }

      //! Assignment
      AABB_generic& operator= (AABB_generic const & a_other)
      {
        m_center = a_other.m_center;

        for (int i = 0; i < R; ++i)
        {
          m_halfLengths[i] = a_other.m_halfLengths[i];
        }

        return *this;
      }

      //! Get the AABB_generic half lengths.
      void GetHalfLengths(Real * a_out) const
      {
        for (int i = 0; i < R; ++i)
        {
          a_out[i] = m_halfLengths[i];
        }
      }

      //! Get the AABB_generic center.
      Vector_generic<Real, R> const & GetCenter() const { return m_center; }

      //! Set the AABB_generic half lengths.
      void SetHalfLengths(Real const * a_data)
      {
        for (int i = 0; i < R; ++i)
        {
          m_halfLengths[i] = a_data[i];
        }
      }

      //! Set the AABB_generic center.
      void SetCenter(Vector_generic<Real, R> const & a_center)
      {
        m_center = a_center;
      }

    private:
      Vector_generic<Real, R>     m_center;
      Real                        m_halfLengths[R]; //[x, y, (z)]
    };
  }
}

#endif