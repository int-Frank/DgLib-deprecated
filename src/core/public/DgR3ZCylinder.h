//! @file DgR3AACylinder.h
//!
//! @author: Frank B. Hart
//! @date 22/04/2019
//!
//! Class declaration: ZCylinder

#ifndef DGR3ZCYLINDER_H
#define DGR3ZCYLINDER_H

#include "DgR3Vector.h"
#include "DgR2Disk.h"

//TODO Queries
namespace Dg
{
  namespace R3
  {
    //! @ingroup DgMath_types
    //!
    //! @class ZCylinder
    //!
    //! @author: Frank Hart
    //! @date 22/04/2019
    template<typename Real>
    class ZCylinder
    {
    private:

    public:
      //! Default constructor.
      ZCylinder()
        : m_bottomCentre(Vector<Real>::Origin())
        , m_height(Real(1))
        , m_radius(Real(1))
      {}

      ~ZCylinder()
      {}

      //! Copy constructor
      ZCylinder(ZCylinder const & a_other)
        : m_bottomCentre(a_other.m_bottomCentre)
        , m_height(a_other.m_height)
        , m_radius(a_other.m_radius)
      {}

      //! Assignment
      ZCylinder& operator= (ZCylinder const & a_other)
      {
        m_bottomCentre = a_other.m_bottomCentre;
        m_height = a_other.m_height;
        m_radius = a_other.m_radius;

        return *this;
      }

      R2::Disk<Real> GetDisk2D() const;
      Real GetHeight() const {return m_height;}
      Real Getradius() const {return m_radius;}

    private:
      Vector<Real>  m_bottomCentre;
      Real          m_height;
      Real          m_radius;
    };
  }
}

#endif