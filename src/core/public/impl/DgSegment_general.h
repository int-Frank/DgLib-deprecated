//! @file DgSegment.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: Segment_general

#ifndef DGR2SEGMENT2D_H
#define DGR2SEGMENT2D_H

#include "DgGeometryTypes.h"
#include "dgmath.h"

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_types
    //!
    //! @class Segment_general
    //!
    //! A Linesgment is a line that connects two points. It is represented by a Point4 
    //! and a Vector, the length of the vector being the distance between the points.
    //! This file also declares methods to test LineSegments against other 
    //! geometric entities.
    //!
    //! Retrieved From: Essential Mathematics for Games and Interactive Applications SE
    //!
    //! @author: James M. Van Verth, Lars M. Bishop, Frank Hart
    //! @date 29/05/2016
    template<typename Real, int R>
    class Segment_general
    {
      typedef typename GeometryTypes<Real, R>::vec    vec;
      typedef typename GeometryTypes<Real, R>::matrix matrix;
      typedef typename GeometryTypes<Real, R>::vqs    vqs;

    public:
      //! Default constructor
      Segment_general();

      //! Construct from an origin and direction
      Segment_general(vec const & p0, vec const & p1);
      ~Segment_general() {}

      //! Copy constructor
      Segment_general(Segment_general const &);

      //! Assignment
      Segment_general& operator=(Segment_general const &);

      //! Get the origin of the line
      vec const & Origin() const { return m_origin; }

      //! Get the direction of the line
      vec const & Direction() const { return m_direction; }

      //! Get endpoint 0
      vec GetP0() const { return m_origin; }

      //! Get endpoint 1
      vec GetP1() const { return m_origin + m_direction; }

      //! Get the center of the line segment
      vec GetCenter() const { return m_origin + static_cast<Real>(0.5) * m_direction; }

      //! Get the endpoints of the line segment
      void Get(vec& a_p0, vec& a_p1) const;

      //! Comparison
      bool operator== (Segment_general const &) const;

      //! Comparison
      bool operator!= (Segment_general const &) const;

      //! Set line segment from endpoints
      void Set(vec const & p0, vec const & p1);

      //! Get the length of the line segment
      Real Length() const;

      //! Get the squared length of the line segment
      Real LengthSquared() const;

      //! Transform the line segment
      Segment_general GetTransformed(matrix const &) const;

      //! Transform the line segment, assign to self
      Segment_general & TransformSelf(matrix const &);

      //! Transform the line segment
      Segment_general GetTransformed(vqs const &) const;

      //! Transform the line segment, assign to self
      Segment_general & TransformSelf(vqs const &);
    private:

      //Data members
      vec m_origin;
      vec m_direction;
    };


    //--------------------------------------------------------------------------------
    //	@	Segment_general::Segment_general()
    //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Segment_general<Real, R>::Segment_general()
      : m_origin(vec::Origin())
      , m_direction(vec::xAxis())
    {
    }	//End: Segment_general::Segment_general()


      //--------------------------------------------------------------------------------
      //  @ Segment_general::Segment_general()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Segment_general<Real, R>::Segment_general(vec const & p0, vec const & p1)
    {
      Set(p0, p1);
    }	//End: Segment_general::Segment_general()


      //--------------------------------------------------------------------------------
      //  @ Segment_general::Segment_general()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Segment_general<Real, R>::Segment_general(Segment_general<Real, R> const & a_other)
      : m_origin(a_other.m_origin)
      , m_direction(a_other.m_direction)
    {
    }	//End: Segment_general::Segment_general()


      //--------------------------------------------------------------------------------
      //  @ Segment_general::operator=()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Segment_general<Real, R> & Segment_general<Real, R>::operator=(Segment_general<Real, R> const & a_other)
    {
      m_origin = a_other.m_origin;
      m_direction = a_other.m_direction;

      return *this;
    }	//End: Segment_general::operator=()


      //--------------------------------------------------------------------------------
      //  @ Segment_general::Get()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    void Segment_general<Real, R>::Get(vec & a_p0, vec & a_p1) const
    {
      a_p0 = m_origin;
      a_p1 = m_origin + m_direction;
    }	//End: Segment_general::Get()


      //--------------------------------------------------------------------------------
      //  @ Segment_general::operator==()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    bool Segment_general<Real, R>::operator==(Segment_general<Real, R> const & a_other) const
    {
      return m_origin == a_other.m_origin && m_direction == a_other.m_direction;
    }	//End: Segment_general::operator==()


      //--------------------------------------------------------------------------------
      //  @ Segment_general::operator!=()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    bool Segment_general<Real, R>::operator!=(Segment_general<Real, R> const & a_other) const
    {
      return m_origin != a_other.m_origin || m_direction != a_other.m_direction;
    }	//End: Segment_general::operator!=()


      //--------------------------------------------------------------------------------
      //  @ Segment_general::Set()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    void Segment_general<Real, R>::Set(vec const & a_p0, vec const & a_p1)
    {
      //Assign
      m_origin = a_p0;
      m_origin.w() = static_cast<Real>(1.0);

      m_direction = a_p1 - a_p0;
      m_direction.w() = static_cast<Real>(0.0);

    }	//End: Segment_general::Set()


      //--------------------------------------------------------------------------------
      //  @ Segment_general::Length()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Real Segment_general<Real, R>::Length() const
    {
      return m_direction.Length();

    }	//End: Segment_general::Length()


      //--------------------------------------------------------------------------------
      //  @ Segment_general::LengthSquared()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Real Segment_general<Real, R>::LengthSquared() const
    {
      return m_direction.LengthSquared();

    }	//End: Segment_general::LengthSquared()


      //--------------------------------------------------------------------------------
      //	@	Segment_general::GetTransformed()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Segment_general<Real, R> Segment_general<Real, R>::GetTransformed(matrix const & a_mat) const
    {
      vec p0(m_origin * a_mat);
      vec p1(p0 + m_direction * a_mat);
      return Segment_general<Real, R>(p0, p1);
    }	//End: Segment_general::GetTransformed()


      //--------------------------------------------------------------------------------
      //	@	Segment_general::TransformSelf()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Segment_general<Real, R>& Segment_general<Real, R>::TransformSelf(matrix const & a_mat)
    {
      vec p0(m_origin * a_mat);
      vec p1(p0 + m_direction * a_mat);
      Set(p0, p1);
      return *this;
    }	//End: Segment_general::TransformSelf()


      //--------------------------------------------------------------------------------
      //	@	Segment::GetTransformed()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Segment_general<Real, R> Segment_general<Real, R>::GetTransformed(vqs const & a_vqs) const
    {
      vec p0(a_vqs.TransformPoint(m_origin));
      vec p1(p0 + a_vqs.TransformVector(m_direction));
      return Segment_general<Real, R>(p0, p1);
    }	//End: Segment::GetTransformed()


      //--------------------------------------------------------------------------------
      //	@	Segment::TransformSelf()
      //--------------------------------------------------------------------------------
    template<typename Real, int R>
    Segment_general<Real, R>& Segment_general<Real, R>::TransformSelf(vqs const & a_vqs)
    {
      vec p0(a_vqs.TransformPoint(m_origin));
      vec p1(p0 + a_vqs.TransformVector(m_direction));
      Set(p0, p1);
      return *this;
    }	//End: Segment::TransformSelf()
  }
}
#endif