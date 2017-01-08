//! @file DgR3Line.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: Line

#ifndef DGR3LINE_H
#define DGR3LINE_H

#include "DgR3Vector4.h"
#include "DgR3Matrix44.h"
#include "DgR3VQS.h"
#include "dgmath.h"

namespace Dg
{
  namespace R3
  {
    //! @ingroup DgMath_types
    //!
    //! @class Line
    //!
    //! A Line is defined by a point and direction extending infinitely in both
    //! directions. It is represented by a point and a normalized vector. This file
    //! also declares methods to test Lines against other geometric entities.
    //!
    //! Retrieved From: Essential Mathematics for Games and Interactive Applications SE
    //!
    //! @author: James M. Van Verth, Lars M. Bishop, Frank Hart
    //! @date 29/05/2016
    template<typename Real>
    class Line
    {
    public:
      //! Default constructor
      Line();

      //! Construct from an origin and direction
      Line(Vector4<Real> const & origin, Vector4<Real> const & direction);
      ~Line() {}

      //! Copy constructor
      Line(Line const &);

      //! Assignment
      Line& operator=(Line const &);

      //! Get the origin of the line
      Vector4<Real> const & Origin() const { return m_origin; }

      //! Get the direction of the line
      Vector4<Real> const & Direction() const { return m_direction; }

      //! Get the origin and direction of the line
      void Get(Vector4<Real>& origin, Vector4<Real>& direction) const;

      //! Comparison
      bool operator== (Line const &) const;

      //! Comparison
      bool operator!= (Line const &) const;

      //! Set line from an origin and direction
      void Set(Vector4<Real> const & origin, Vector4<Real> const & direction);

      //! Transform the line
      Line GetTransformed(Matrix44<Real> const &) const;

      //! Transform the line, assign to self
      Line & TransformSelf(Matrix44<Real> const &);

      //! Transform the line
      Line GetTransformed(VQS<Real> const &) const;

      //! Transform the line, assign to self
      Line & TransformSelf(VQS<Real> const &);

    private:

      //Data members
      Vector4<Real> m_origin;
      Vector4<Real> m_direction;
    };


    //--------------------------------------------------------------------------------
    //	@	Line::Line()
    //--------------------------------------------------------------------------------
    template<typename Real>
    Line<Real>::Line() : m_origin(Vector4<Real>::Origin())
      , m_direction(Vector4<Real>::xAxis())
    {
    }	//End: Line::Line()


    //--------------------------------------------------------------------------------
    //	@	Line::Line()
    //--------------------------------------------------------------------------------
    template<typename Real>
    Line<Real>::Line(Vector4<Real> const & a_origin, Vector4<Real> const & a_direction)
    {
      Set(a_origin, a_direction);
    }	//End: Line::Line()


    //--------------------------------------------------------------------------------
    //	@	Line::Line()
    //--------------------------------------------------------------------------------
    template<typename Real>
    Line<Real>::Line(Line<Real> const & a_other) : m_origin(a_other.m_origin)
      , m_direction(a_other.m_direction)
    {
    }	//End: Line::Line()


    //--------------------------------------------------------------------------------
    //	@	Line::operator=()
    //--------------------------------------------------------------------------------
    template<typename Real>
    Line<Real> & Line<Real>::operator=(Line<Real> const & a_other)
    {
      m_origin = a_other.m_origin;
      m_direction = a_other.m_direction;

      return *this;
    }	//End: Line::operator=()


    //--------------------------------------------------------------------------------
    //	@	Line::Get()
    //--------------------------------------------------------------------------------
    template<typename Real>
    void Line<Real>::Get(Vector4<Real> & a_origin, Vector4<Real> & a_direction) const
    {
      a_origin = m_origin;
      a_direction = m_direction;
    }	//End: Line::Get()


    //--------------------------------------------------------------------------------
    //	@	Line::operator==()
    //--------------------------------------------------------------------------------
    template<typename Real>
    bool Line<Real>::operator==(Line<Real> const & a_other) const
    {
      return m_origin == a_other.m_origin && m_direction == a_other.m_direction;
    }	//End: Line::operator==()


    //--------------------------------------------------------------------------------
    //	@	Line::operator!=()
    //--------------------------------------------------------------------------------
    template<typename Real>
    bool Line<Real>::operator!=(Line<Real> const & a_other) const
    {
      return m_origin != a_other.m_origin || m_direction != a_other.m_direction;
    }	//End: Line::operator!=()


    //--------------------------------------------------------------------------------
    //	@	Line::Set()
    //--------------------------------------------------------------------------------
    template<typename Real>
    void Line<Real>::Set(Vector4<Real> const & a_origin, Vector4<Real> const & a_direction)
    {
      //Assign
      m_origin = a_origin;
      m_origin.w() = static_cast<Real>(1.0);

      m_direction = a_direction;
      m_direction.w() = static_cast<Real>(0.0);

      m_direction.Normalize();

    }	//End: Line::Set()


    //--------------------------------------------------------------------------------
    //	@	Line::GetTransformed()
    //--------------------------------------------------------------------------------
    template<typename Real>
    Line<Real> Line<Real>::GetTransformed(Matrix44<Real> const & a_mat) const
    {
      return Line<Real>(m_origin * a_mat, m_direction * a_mat);
    }	//End: Line::GetTransformed()


    //--------------------------------------------------------------------------------
    //	@	Line::TransformSelf()
    //--------------------------------------------------------------------------------
    template<typename Real>
    Line<Real>& Line<Real>::TransformSelf(Matrix44<Real> const & a_mat)
    {
      Set(m_origin * a_mat, m_direction * a_mat);
      return *this;
    }	//End: Line::TransformSelf()


    //--------------------------------------------------------------------------------
    //	@	Line::GetTransformed()
    //--------------------------------------------------------------------------------
    template<typename Real>
    Line<Real> Line<Real>::GetTransformed(VQS<Real> const & a_vqs) const
    {
      return Line<Real>(a_vqs.TransformPoint(m_origin), a_vqs.Rotate(m_direction));
    }	//End: Line::GetTransformed()


    //--------------------------------------------------------------------------------
    //	@	Line::TransformSelf()
    //--------------------------------------------------------------------------------
    template<typename Real>
    Line<Real>& Line<Real>::TransformSelf(VQS<Real> const & a_vqs)
    {
      Set(a_vqs.TransformPoint(m_origin), a_vqs.Rotate(m_direction));
      return *this;
    }	//End: Line::TransformSelf()
  }
}

#endif