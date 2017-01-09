//! @file DgRay_general.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: Ray_general

#ifndef DGRAY_GENERAL_H
#define DGRAY_GENERAL_H

#include "DgGeometryTypes.h"
#include "dgmath.h"

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_types
    //!
    //! @class Ray_general
    //!
    //! A Ray_general is defined by a point of origin and direction extending infinitely in one
    //! direction. It is represented by a Point4 and a normalized Vector. This file
    //! also declares methods to test Rays against other geometric entities.
    //!
    //! Retrieved From: Essential Mathematics for Games and Interactive Applications SE
    //!
    //! @author: James M. Van Verth, Lars M. Bishop, Frank Hart
    //! @date 1/06/2016
    template<typename Real, int R>
    class Ray_general
    {
      typedef typename GeometryTypes<Real, R>::vec    vec;
      typedef typename GeometryTypes<Real, R>::matrix matrix;
      typedef typename GeometryTypes<Real, R>::vqs    vqs;

    public:
      //! Default constructor
      Ray_general();

      //! Construct from an origin and direction
      Ray_general(vec const & origin, vec const & direction);
      ~Ray_general() {}

      //! Copy constructor
      Ray_general(Ray_general const &);

      //! Assignment
      Ray_general& operator=(Ray_general const &);

      //! Get the origin of the ray
      vec const & Origin() const { return m_origin; }

      //! Get the direction of the ray
      vec const & Direction() const { return m_direction; }

      //! Get the origin and direction of the ray
      void Get(vec& origin, vec& direction) const;

      //! Comparison
      bool operator== (Ray_general const &) const;

      //! Comparison
      bool operator!= (Ray_general const &) const;

      //! Set ray from an origin and direction
      void Set(vec const & origin, vec const & direction);

      //! Transform the ray
      Ray_general GetTransformed(matrix const &) const;

      //! Transform the ray, assign to self
      Ray_general & TransformSelf(matrix const &);

      //! Transform the ray
      Ray_general GetTransformed(vqs const &) const;

      //! Transform the ray, assign to self
      Ray_general & TransformSelf(vqs const &);
    private:

      //Data members
      vec m_origin;
      vec m_direction;
    };


    //--------------------------------------------------------------------------------
    //	@ Ray_general::Ray_general()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Ray_general<Real, R>::Ray_general() : m_origin(vec::Origin())
      , m_direction(vec::xAxis())
    {
    }	//End: Ray_general::Ray_general()


      //--------------------------------------------------------------------------------
      //  @ Ray_general::Ray_general()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Ray_general<Real, R>::Ray_general(vec const & a_origin, vec const & a_direction)
    {
      Set(a_origin, a_direction);
    }	//End: Ray_general::Ray_general()


      //--------------------------------------------------------------------------------
      //  @ Ray_general::Ray_general()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Ray_general<Real, R>::Ray_general(Ray_general<Real, R> const & a_other) : m_origin(a_other.m_origin)
      , m_direction(a_other.m_direction)
    {
    }	//End: Ray_general::Ray_general()


      //--------------------------------------------------------------------------------
      //  @ Ray_general::operator=()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Ray_general<Real, R> & Ray_general<Real, R>::operator=(Ray_general<Real, R> const & a_other)
    {
      m_origin = a_other.m_origin;
      m_direction = a_other.m_direction;

      return *this;
    }	//End: Ray_general::operator=()


      //--------------------------------------------------------------------------------
      //  @ Ray_general::Get()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    void Ray_general<Real, R>::Get(vec & a_origin, vec & a_direction) const
    {
      a_origin = m_origin;
      a_direction = m_direction;
    }	//End: Ray_general::Get()


      //--------------------------------------------------------------------------------
      //  @ Ray_general::operator==()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    bool Ray_general<Real, R>::operator==(Ray_general<Real, R> const & a_other) const
    {
      return m_origin == a_other.m_origin && m_direction == a_other.m_direction;
    }	//End: Ray_general::operator==()


      //--------------------------------------------------------------------------------
      //  @ Ray_general::operator!=()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    bool Ray_general<Real, R>::operator!=(Ray_general<Real, R> const & a_other) const
    {
      return m_origin != a_other.m_origin || m_direction != a_other.m_direction;
    }	//End: Ray_general::operator!=()


      //--------------------------------------------------------------------------------
      //  @ Ray_general::Set()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    void Ray_general<Real, R>::Set(vec const & a_origin, vec const & a_direction)
    {
      //Assign
      m_origin = a_origin;
      m_origin.w() = static_cast<Real>(1.0);

      m_direction = a_direction;
      m_direction.w() = static_cast<Real>(0.0);

      m_direction.Normalize();

    }	//End: Ray_general::Set()


      //--------------------------------------------------------------------------------
      //	@	Ray_general::GetTransformed()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Ray_general<Real, R> Ray_general<Real, R>::GetTransformed(matrix const & a_mat) const
    {
      return Ray_general<Real, R>(m_origin * a_mat, m_direction * a_mat);
    }	//End: Ray_general::GetTransformed()


      //--------------------------------------------------------------------------------
      //	@	Ray_general::TransformSelf()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Ray_general<Real, R>& Ray_general<Real, R>::TransformSelf(matrix const & a_mat)
    {
      Set(m_origin * a_mat, m_direction * a_mat);
      return *this;
    }	//End: Ray_general::TransformSelf()


      //--------------------------------------------------------------------------------
      //	@	Ray_general::GetTransformed()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Ray_general<Real, R> Ray_general<Real, R>::GetTransformed(vqs const & a_vqs) const
    {
      return Ray_general<Real, R>(a_vqs.TransformPoint(m_origin), a_vqs.Rotate(m_direction));
    }	//End: Ray_general::GetTransformed()


      //--------------------------------------------------------------------------------
      //	@	Ray_general::TransformSelf()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Ray_general<Real, R>& Ray_general<Real, R>::TransformSelf(vqs const & a_vqs)
    {
      Set(a_vqs.TransformPoint(m_origin), a_vqs.Rotate(m_direction));
      return *this;
    }	//End: Ray_general::TransformSelf()
  }
}

#endif