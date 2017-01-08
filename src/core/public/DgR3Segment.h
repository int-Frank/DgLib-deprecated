//! @file DgR3Segment.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: Segment

#ifndef DGR3SEGMENT_H
#define DGR3SEGMENT_H

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
    //! @class Segment
    //!
    //! A Linesgment is a line that connects two points. It is represented by a Point4 
    //! and a Vector4, the length of the vector being the distance between the points.
    //! This file also declares methods to test LineSegments against other 
    //! geometric entities.
    //!
    //! Retrieved From: Essential Mathematics for Games and Interactive Applications SE
    //!
    //! @author: James M. Van Verth, Lars M. Bishop, Frank Hart
    //! @date 29/05/2016
    template<typename Real>
    class Segment
    {
    public:
      //! Default constructor
      Segment();

      //! Construct from an origin and direction
      Segment(Vector4<Real> const & p0, Vector4<Real> const & p1);
      ~Segment() {}

      //! Copy constructor
      Segment(Segment const &);

      //! Assignment
      Segment& operator=(Segment const &);

      //! Get the origin of the line
      Vector4<Real> const & Origin() const { return m_origin; }

      //! Get the direction of the line
      Vector4<Real> const & Direction() const { return m_direction; }

      //! Get endpoint 0
      Vector4<Real> GetP0() const { return m_origin; }

      //! Get endpoint 1
      Vector4<Real> GetP1() const { return m_origin + m_direction; }

      //! Get the center of the line segment
      Vector4<Real> GetCenter() const { return m_origin + static_cast<Real>(0.5) * m_direction; }

      //! Get the endpoints of the line segment
      void Get(Vector4<Real>& a_p0, Vector4<Real>& a_p1) const;

      //! Comparison
      bool operator== (Segment const &) const;

      //! Comparison
      bool operator!= (Segment const &) const;

      //! Set line segment from endpoints
      void Set(Vector4<Real> const & p0, Vector4<Real> const & p1);

      //! Get the length of the line segment
      Real Length() const;

      //! Get the squared length of the line segment
      Real LengthSquared() const;

      //! Transform the line segment
      Segment GetTransformed(Matrix44<Real> const &) const;

      //! Transform the line segment, assign to self
      Segment & TransformSelf(Matrix44<Real> const &);

      //! Transform the line segment
      Segment GetTransformed(VQS<Real> const &) const;

      //! Transform the line segment, assign to self
      Segment & TransformSelf(VQS<Real> const &);
    private:

      //Data members
      Vector4<Real> m_origin;
      Vector4<Real> m_direction;
    };


    //--------------------------------------------------------------------------------
    //	@	Segment::Segment()
    //--------------------------------------------------------------------------------
    template<typename Real>
    Segment<Real>::Segment() : m_origin(Vector4<Real>::Origin())
      , m_direction(Vector4<Real>::xAxis())
    {
    }	//End: Segment::Segment()


      //--------------------------------------------------------------------------------
      //  @ Segment::Segment()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Segment<Real>::Segment(Vector4<Real> const & p0, Vector4<Real> const & p1)
    {
      Set(p0, p1);
    }	//End: Segment::Segment()


      //--------------------------------------------------------------------------------
      //  @ Segment::Segment()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Segment<Real>::Segment(Segment<Real> const & a_other) :
      m_origin(a_other.m_origin), m_direction(a_other.m_direction)
    {
    }	//End: Segment::Segment()


      //--------------------------------------------------------------------------------
      //  @ Segment::operator=()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Segment<Real> & Segment<Real>::operator=(Segment<Real> const & a_other)
    {
      m_origin = a_other.m_origin;
      m_direction = a_other.m_direction;

      return *this;
    }	//End: Segment::operator=()


      //--------------------------------------------------------------------------------
      //  @ Segment::Get()
      //--------------------------------------------------------------------------------
    template<typename Real>
    void Segment<Real>::Get(Vector4<Real> & a_p0, Vector4<Real> & a_p1) const
    {
      a_p0 = m_origin;
      a_p1 = m_origin + m_direction;
    }	//End: Segment::Get()


      //--------------------------------------------------------------------------------
      //  @ Segment::operator==()
      //--------------------------------------------------------------------------------
    template<typename Real>
    bool Segment<Real>::operator==(Segment<Real> const & a_other) const
    {
      return m_origin == a_other.m_origin && m_direction == a_other.m_direction;
    }	//End: Segment::operator==()


      //--------------------------------------------------------------------------------
      //  @ Segment::operator!=()
      //--------------------------------------------------------------------------------
    template<typename Real>
    bool Segment<Real>::operator!=(Segment<Real> const & a_other) const
    {
      return m_origin != a_other.m_origin || m_direction != a_other.m_direction;
    }	//End: Segment::operator!=()


      //--------------------------------------------------------------------------------
      //  @ Segment::Set()
      //--------------------------------------------------------------------------------
    template<typename Real>
    void Segment<Real>::Set(Vector4<Real> const & a_p0, Vector4<Real> const & a_p1)
    {
      //Assign
      m_origin = a_p0;
      m_origin.w() = static_cast<Real>(1.0);

      m_direction = a_p1 - a_p0;
      m_direction.w() = static_cast<Real>(0.0);

    }	//End: Segment::Set()


      //--------------------------------------------------------------------------------
      //  @ Segment::Length()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Real Segment<Real>::Length() const
    {
      return m_direction.Length();

    }	//End: Segment::Length()


      //--------------------------------------------------------------------------------
      //  @ Segment::LengthSquared()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Real Segment<Real>::LengthSquared() const
    {
      return m_direction.LengthSquared();

    }	//End: Segment::LengthSquared()


      //--------------------------------------------------------------------------------
      //	@	Segment::GetTransformed()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Segment<Real> Segment<Real>::GetTransformed(Matrix44<Real> const & a_mat) const
    {
      Vector4<Real> p0(m_origin * a_mat);
      Vector4<Real> p1(p0 + m_direction * a_mat);
      return Segment<Real>(p0, p1);
    }	//End: Segment::GetTransformed()


      //--------------------------------------------------------------------------------
      //	@	Segment::TransformSelf()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Segment<Real>& Segment<Real>::TransformSelf(Matrix44<Real> const & a_mat)
    {
      Vector4<Real> p0(m_origin * a_mat);
      Vector4<Real> p1(p0 + m_direction * a_mat);
      Set(p0, p1);
      return *this;
    }	//End: Segment::TransformSelf()


      //--------------------------------------------------------------------------------
      //	@	Segment::GetTransformed()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Segment<Real> Segment<Real>::GetTransformed(VQS<Real> const & a_vqs) const
    {
      Vector4<Real> p0(a_vqs.TransformPoint(m_origin));
      Vector4<Real> p1(p0 + a_vqs.TransformVector(m_direction));
      return Segment<Real>(p0, p1);
    }	//End: Segment::GetTransformed()


      //--------------------------------------------------------------------------------
      //	@	Segment::TransformSelf()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Segment<Real>& Segment<Real>::TransformSelf(VQS<Real> const & a_vqs)
    {
      Vector4<Real> p0(a_vqs.TransformPoint(m_origin));
      Vector4<Real> p1(p0 + a_vqs.TransformVector(m_direction));
      Set(p0, p1);
      return *this;
    }	//End: Segment::TransformSelf()
  }
}
#endif