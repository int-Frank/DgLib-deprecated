//! @file DgR3Ray.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: Ray

#ifndef DGR3RAY_H
#define DGR3RAY_H

#include "DgR3Vector4.h"
#include "DgR3Plane.h"
#include "DgR3Line.h"
#include "DgR3Matrix44.h"
#include "DgR3VQS.h"
#include "dgmath.h"

namespace Dg
{
  namespace R3
  {
    //! @ingroup DgMath_types
    //!
    //! @class Ray
    //!
    //! A Ray is defined by a point of origin and direction extending infinitely in one
    //! direction. It is represented by a Point4 and a normalized Vector4. This file
    //! also declares methods to test Rays against other geometric entities.
    //!
    //! Retrieved From: Essential Mathematics for Games and Interactive Applications SE
    //!
    //! @author: James M. Van Verth, Lars M. Bishop, Frank Hart
    //! @date 1/06/2016
    template<typename Real>
    class Ray
    {
    public:
      //! Default constructor
      Ray();

      //! Construct from an origin and direction
      Ray(Vector4<Real> const & origin, Vector4<Real> const & direction);
      ~Ray() {}

      //! Copy constructor
      Ray(Ray const &);

      //! Assignment
      Ray& operator=(Ray const &);

      //! Get the origin of the ray
      Vector4<Real> const & Origin() const { return m_origin; }

      //! Get the direction of the ray
      Vector4<Real> const & Direction() const { return m_direction; }

      //! Get the origin and direction of the ray
      void Get(Vector4<Real>& origin, Vector4<Real>& direction) const;

      //! Comparison
      bool operator== (Ray const &) const;

      //! Comparison
      bool operator!= (Ray const &) const;

      //! Set ray from an origin and direction
      void Set(Vector4<Real> const & origin, Vector4<Real> const & direction);

      //! Transform the ray
      Ray GetTransformed(Matrix44<Real> const &) const;

      //! Transform the ray, assign to self
      Ray & TransformSelf(Matrix44<Real> const &);

      //! Transform the ray
      Ray GetTransformed(VQS<Real> const &) const;

      //! Transform the ray, assign to self
      Ray & TransformSelf(VQS<Real> const &);
    private:

      //Data members
      Vector4<Real> m_origin;
      Vector4<Real> m_direction;
    };


    //--------------------------------------------------------------------------------
    //	@ Ray::Ray()
    //--------------------------------------------------------------------------------
    template<typename Real>
    Ray<Real>::Ray() : m_origin(Vector4<Real>::Origin())
      , m_direction(Vector4<Real>::xAxis())
    {
    }	//End: Ray::Ray()


      //--------------------------------------------------------------------------------
      //  @ Ray::Ray()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Ray<Real>::Ray(Vector4<Real> const & a_origin, Vector4<Real> const & a_direction)
    {
      Set(a_origin, a_direction);
    }	//End: Ray::Ray()


      //--------------------------------------------------------------------------------
      //  @ Ray::Ray()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Ray<Real>::Ray(Ray<Real> const & a_other) : m_origin(a_other.m_origin)
      , m_direction(a_other.m_direction)
    {
    }	//End: Ray::Ray()


      //--------------------------------------------------------------------------------
      //  @ Ray::operator=()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Ray<Real> & Ray<Real>::operator=(Ray<Real> const & a_other)
    {
      m_origin = a_other.m_origin;
      m_direction = a_other.m_direction;

      return *this;
    }	//End: Ray::operator=()


      //--------------------------------------------------------------------------------
      //  @ Ray::Get()
      //--------------------------------------------------------------------------------
    template<typename Real>
    void Ray<Real>::Get(Vector4<Real> & a_origin, Vector4<Real> & a_direction) const
    {
      a_origin = m_origin;
      a_direction = m_direction;
    }	//End: Ray::Get()


      //--------------------------------------------------------------------------------
      //  @ Ray::operator==()
      //--------------------------------------------------------------------------------
    template<typename Real>
    bool Ray<Real>::operator==(Ray<Real> const & a_other) const
    {
      return m_origin == a_other.m_origin && m_direction == a_other.m_direction;
    }	//End: Ray::operator==()


      //--------------------------------------------------------------------------------
      //  @ Ray::operator!=()
      //--------------------------------------------------------------------------------
    template<typename Real>
    bool Ray<Real>::operator!=(Ray<Real> const & a_other) const
    {
      return m_origin != a_other.m_origin || m_direction != a_other.m_direction;
    }	//End: Ray::operator!=()


      //--------------------------------------------------------------------------------
      //  @ Ray::Set()
      //--------------------------------------------------------------------------------
    template<typename Real>
    void Ray<Real>::Set(Vector4<Real> const & a_origin, Vector4<Real> const & a_direction)
    {
      //Assign
      m_origin = a_origin;
      m_origin.w() = static_cast<Real>(1.0);

      m_direction = a_direction;
      m_direction.w() = static_cast<Real>(0.0);

      m_direction.Normalize();

    }	//End: Ray::Set()


      //--------------------------------------------------------------------------------
      //	@	Ray::GetTransformed()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Ray<Real> Ray<Real>::GetTransformed(Matrix44<Real> const & a_mat) const
    {
      return Ray<Real>(m_origin * a_mat, m_direction * a_mat);
    }	//End: Ray::GetTransformed()


      //--------------------------------------------------------------------------------
      //	@	Ray::TransformSelf()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Ray<Real>& Ray<Real>::TransformSelf(Matrix44<Real> const & a_mat)
    {
      Set(m_origin * a_mat, m_direction * a_mat);
      return *this;
    }	//End: Ray::TransformSelf()


      //--------------------------------------------------------------------------------
      //	@	Ray::GetTransformed()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Ray<Real> Ray<Real>::GetTransformed(VQS<Real> const & a_vqs) const
    {
      return Ray<Real>(a_vqs.TransformPoint(m_origin), a_vqs.Rotate(m_direction));
    }	//End: Ray::GetTransformed()


      //--------------------------------------------------------------------------------
      //	@	Ray::TransformSelf()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Ray<Real>& Ray<Real>::TransformSelf(VQS<Real> const & a_vqs)
    {
      Set(a_vqs.TransformPoint(m_origin), a_vqs.Rotate(m_direction));
      return *this;
    }	//End: Ray::TransformSelf()
  }
}

#endif