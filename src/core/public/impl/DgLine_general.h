//! @file DgLine_general.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: Line_general

#ifndef DGLINE_GENERAL_H
#define DGLINE_GENERAL_H

#include "DgGeometryTypes.h"
#include "dgmath.h"

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_types
    //!
    //! @class Line_general
    //!
    //! A Line_general is defined by a point and direction extending infinitely in both
    //! directions. It is represented by a point and a normalized vector. This file
    //! also declares methods to test Lines against other geometric entities.
    //!
    //! Retrieved From: Essential Mathematics for Games and Interactive Applications SE
    //!
    //! @author: James M. Van Verth, Lars M. Bishop, Frank Hart
    //! @date 29/05/2016
    template<typename Real, int R>
    class Line_general
    {
      typedef typename GeometryTypes<Real, R>::vec    vec;
      typedef typename GeometryTypes<Real, R>::matrix matrix;
      typedef typename GeometryTypes<Real, R>::vqs    vqs;
    
    public:
      //! Default constructor
      Line_general();

      //! Construct from an origin and direction
      Line_general(vec const & origin, vec const & direction);
      ~Line_general() {}

      //! Copy constructor
      Line_general(Line_general const &);

      //! Assignment
      Line_general& operator=(Line_general const &);

      //! Get the origin of the line
      vec const & Origin() const { return m_origin; }

      //! Get the direction of the line
      vec const & Direction() const { return m_direction; }

      //! Get the origin and direction of the line
      void Get(vec& origin, vec& direction) const;

      //! Comparison
      bool operator== (Line_general const &) const;

      //! Comparison
      bool operator!= (Line_general const &) const;

      //! Set line from an origin and direction
      void Set(vec const & origin, vec const & direction);

      //! Transform the line
      Line_general GetTransformed(matrix const &) const;

      //! Transform the line, assign to self
      Line_general & TransformSelf(matrix const &);

      //! Transform the line
      Line_general GetTransformed(vqs const &) const;

      //! Transform the line, assign to self
      Line_general & TransformSelf(vqs const &);

    private:

      //Data members
      vec m_origin;
      vec m_direction;
    };


    //--------------------------------------------------------------------------------
    //	@	Line_general::Line_general()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Line_general<Real, R>::Line_general() 
      : m_origin(vec::Origin())
      , m_direction(vec::xAxis())
    {
    }	//End: Line_general::Line_general()


      //--------------------------------------------------------------------------------
      //	@	Line_general::Line_general()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Line_general<Real, R>::Line_general(vec const & a_origin, vec const & a_direction)
    {
      Set(a_origin, a_direction);
    }	//End: Line_general::Line_general()


      //--------------------------------------------------------------------------------
      //	@	Line_general::Line_general()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Line_general<Real, R>::Line_general(Line_general<Real, R> const & a_other) 
      : m_origin(a_other.m_origin)
      , m_direction(a_other.m_direction)
    {
    }	//End: Line_general::Line_general()


      //--------------------------------------------------------------------------------
      //	@	Line_general::operator=()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Line_general<Real, R> & Line_general<Real, R>::operator=(Line_general<Real, R> const & a_other)
    {
      m_origin = a_other.m_origin;
      m_direction = a_other.m_direction;

      return *this;
    }	//End: Line_general::operator=()


      //--------------------------------------------------------------------------------
      //	@	Line_general::Get()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    void Line_general<Real, R>::Get(vec & a_origin, vec & a_direction) const
    {
      a_origin = m_origin;
      a_direction = m_direction;
    }	//End: Line_general::Get()


      //--------------------------------------------------------------------------------
      //	@	Line_general::operator==()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    bool Line_general<Real, R>::operator==(Line_general<Real, R> const & a_other) const
    {
      return m_origin == a_other.m_origin && m_direction == a_other.m_direction;
    }	//End: Line_general::operator==()


      //--------------------------------------------------------------------------------
      //	@	Line_general::operator!=()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    bool Line_general<Real, R>::operator!=(Line_general<Real, R> const & a_other) const
    {
      return m_origin != a_other.m_origin || m_direction != a_other.m_direction;
    }	//End: Line_general::operator!=()


      //--------------------------------------------------------------------------------
      //	@	Line_general::Set()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    void Line_general<Real, R>::Set(vec const & a_origin, vec const & a_direction)
    {
      //Assign
      m_origin = a_origin;
      m_origin.w() = static_cast<Real>(1.0);

      m_direction = a_direction;
      m_direction.w() = static_cast<Real>(0.0);

      m_direction.Normalize();

    }	//End: Line_general::Set()


      //--------------------------------------------------------------------------------
      //	@	Line_general::GetTransformed()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Line_general<Real, R> Line_general<Real, R>::GetTransformed(matrix const & a_mat) const
    {
      return Line_general<Real, R>(m_origin * a_mat, m_direction * a_mat);
    }	//End: Line_general::GetTransformed()


      //--------------------------------------------------------------------------------
      //	@	Line_general::TransformSelf()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Line_general<Real, R>& Line_general<Real, R>::TransformSelf(matrix const & a_mat)
    {
      Set(m_origin * a_mat, m_direction * a_mat);
      return *this;
    }	//End: Line_general::TransformSelf()


      //--------------------------------------------------------------------------------
      //	@	Line_general::GetTransformed()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Line_general<Real, R> Line_general<Real, R>::GetTransformed(vqs const & a_vqs) const
    {
      return Line_general<Real, R>(a_vqs.TransformPoint(m_origin), a_vqs.Rotate(m_direction));
    }	//End: Line_general::GetTransformed()


      //--------------------------------------------------------------------------------
      //	@	Line_general::TransformSelf()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Line_general<Real, R>& Line_general<Real, R>::TransformSelf(vqs const & a_vqs)
    {
      Set(a_vqs.TransformPoint(m_origin), a_vqs.Rotate(m_direction));
      return *this;
    }	//End: Line_general::TransformSelf()
  }
}

#endif