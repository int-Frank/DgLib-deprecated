//================================================================================
// @ Line4.h
// 
// Class: Line4
//
// A Line is defined by a point and m_direction extending infinitely in both
// m_directions. It is represented by a Vector<4, Real> and a normalized Vector<4, Real>. This file
// also declares methods to test Lines against other geometric entities.
//
// -------------------------------------------------------------------------------
//
// m_original Authors: James M. Van Verth, Lars M. Bishop
// Retrieved From: Essential Mathematics for Games and Interactive Applications SE
// On Date: 2013
//
// Modified by: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef LINE4_H
#define LINE4_H

#include "DgMatrix.h"
#include "DgVector.h"
#include "dgmath.h"

namespace Dg
{
  template<typename Real>
  class Line4
  {
  public:
    //constructor/destructor
    Line4();
    Line4(Matrix<1, 4, Real> const & m_origin, Matrix<1, 4, Real> const & m_direction);
    ~Line4() {}

    //copy operations
    Line4(Line4 const & other);
    Line4& operator=(Line4 const & other);

    //accessors
    Vector<4, Real> const & m_origin() const { return m_origin; }
    Vector<4, Real> const & m_direction() const { return m_direction; }
    void Get(Matrix<1, 4, Real>& O, Matrix<1, 4, Real>& D) const;

    //comparison
    bool operator== (Line4 const &) const;
    bool operator!= (Line4 const &) const;

    //manipulators
    void Set(Matrix<1, 4, Real> const & O, Matrix<1, 4, Real> const & D);
    void Clean() { m_origin.Clean(); m_direction.Clean(); }

    //Transformations do nothing by default
    void Transform(const VQS&)		{}
    void TransformQuick(const VQS&) {}

  private:

    //Data members
    Vector<4, Real> m_origin;
    Vector<4, Real> m_direction;
  };


  //--------------------------------------------------------------------------------
  //	@	Line4::Line4()
  //--------------------------------------------------------------------------------
  //		Constructor
  //--------------------------------------------------------------------------------
  template<typename Real>
  Line4<Real>::Line4()
  {
    m_origin.Zero();
    m_direction.Zero();
    m_direction[0] = static_cast<Real>(1.0);

  }	//End: Line4::Line4


  //--------------------------------------------------------------------------------
  //	@	Line4::Line4()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Line4<Real>::Line4(Matrix<1, 4, Real> const & a_origin, 
                     Matrix<1, 4, Real> const & a_direction) :
                     m_origin(a_origin),
                     m_direction(a_direction)
  {
    //Normalise direction
    m_direction.Normalize();

  }	//End: Line4::Line4()


  //--------------------------------------------------------------------------------
  //	@	Line4::Line4()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Line4<Real>::Line4(const Line4<Real>& a_other) :
    m_origin(a_other.m_origin),
    m_direction(a_other.m_direction)
  {
  }	//End: Line4::Line4()


  //--------------------------------------------------------------------------------
  //	@	Line4::operator=()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Line4<Real>& Line4<Real>::operator= (Line4<Real> const & a_other)
  {
    m_origin = a_other.m_origin;
    m_direction = a_other.m_direction;

    return *this;

  }	//End: Line4::operator=()


  //--------------------------------------------------------------------------------
  //	@	Line4::Get()
  //--------------------------------------------------------------------------------
  //		Accessor
  //--------------------------------------------------------------------------------
  template<typename Real>
  void Line4<Real>::Get(Matrix<1, 4, Real> const & a_origin,
                        Matrix<1, 4, Real> const & a_direction) const
  {
    a_origin = m_origin;
    a_direction = m_direction;

  }	//End: Line4::Get()


  //--------------------------------------------------------------------------------
  //	@	Line4::operator==()
  //--------------------------------------------------------------------------------
  template<typename Real>
  bool Line4<Real>::operator== (Line4<Real> const & a_line) const
  {
    return (a_line.m_origin == m_origin && a_line.m_direction == m_direction);

  }	//End: Line4::operator==()


  //--------------------------------------------------------------------------------
  //	@	Line4::operator!=()
  //--------------------------------------------------------------------------------
  //		Are two Line4's not equal?
  //--------------------------------------------------------------------------------
  bool Line4::operator!= (const Line4& L) const
  {
    return !(L.origin == origin && L.direction == direction);
  }	//End: Line4::operator!=()


  //--------------------------------------------------------------------------------
  //	@	Line4::ClosestPoint()
  //--------------------------------------------------------------------------------
  //		Returns closest point on a Line4 to a point.
  //--------------------------------------------------------------------------------
  Point4 Line4::ClosestPoint(const Point4& point) const
  {
    Vector4 w = point - origin;

    float vsq = Dot(direction, direction);
    float proj = Dot(w, direction);

    return origin + (proj / vsq)*direction;

  }	//End: Line4::ClosestPoint()


  //--------------------------------------------------------------------------------
  //	@	SqDistLineLine()
  //--------------------------------------------------------------------------------
  /*		Summary: Shortest distance (squared) between two lines
  --------------------------------------
  Post:
  Case - Lines not parallel: returns 0. sqDist set, u1 and u2 are set.
  Case - Lines parallel: returns 1. sqDist set, u1 and u2 are set with u1 = 0.
  --------------------------------------
  Param<L1>:	 input line
  Param<L2>:	 input line
  Param<sqDist>: squared distance between the lines
  Param<u1>:	 distance along L1 to the intersection or
  closest point to L2
  Param<u2>:	 distance along L2 to the intersection or
  closest point to L1
  */
  //--------------------------------------------------------------------------------
  uint8 SqDistLineLine(const Line4& L1, const Line4& L2,
    float& sqDist, float& u1, float& u2)
  {
    Point4 o1(L1.Origin());
    Point4 o2(L2.Origin());
    Vector4 d1(L1.Direction());
    Vector4 d2(L2.Direction());

    //compute intermediate parameters
    Vector4 w0(o1 - o2);
    float a = Dot(d1, d1);
    float b = Dot(d1, d2);
    float c = Dot(d2, d2);
    float d = Dot(d1, w0);
    float e = Dot(d2, w0);
    float denom = a*c - b*b;
    if (::IsZero(denom))
    {
      u1 = 0.0f;
      u2 = e / c;
      Vector4 wc(w0 - u2*d1);
      sqDist = wc.LengthSquared();
      return 1;
    }
    else
    {
      u1 = ((b*e - c*d) / denom);
      u2 = ((a*e - b*d) / denom);
      Vector4 wc(w0 + u1*d1 - u2*d2);
      sqDist = wc.LengthSquared();
      return 0;
    }
  }	//End: Line4::SqDistLineLine


  //--------------------------------------------------------------------------------
  //	@	SqDistLinePoint()
  //--------------------------------------------------------------------------------
  /*		Summary: Shortest distance (squared) between a point and a line
  --------------------------------------
  Post: returns 0. sqDist set, u set.
  --------------------------------------
  Param<L>:	 input line
  Param<P>:	 input point
  Param<sqDist>: squared distance from the point to the line
  Param<u>:	 distance along L to the closest point to P
  */
  //--------------------------------------------------------------------------------
  uint8 SqDistLinePoint(const Line4& L, const Point4& P, float& sqDist, float& u)
  {
    Vector4 dir(L.Direction());
    Vector4 w(P - L.Origin());

    float vsq = Dot(dir, dir);
    float proj = Dot(w, dir);
    u = proj / vsq;

    sqDist = w.LengthSquared() - u*proj;

    return 0;

  }	//End: SqDistLinePoint()


  //--------------------------------------------------------------------------------
  //	@	ClosestPointsLineLine()
  //--------------------------------------------------------------------------------
  /*		Summary: Finds closest points between two Lines
  --------------------------------------
  Post:
  Case Lines not parallel: returns 0. P1, P2 set.
  Case Lines parallel: returns 1. P1, P2 set with P1 = L1.origin.
  --------------------------------------
  Param<L1>: input line
  Param<L2>: input line
  Param<P1>: the point on L1 that is closest to L2
  Param<P2>: the point on L2 that is closest to L1
  */
  //--------------------------------------------------------------------------------
  uint8 ClosestPointsLineLine(const Line4& L1, const Line4& L2,
    Point4& P1, Point4& P2)
  {
    Point4 o1(L1.Origin());
    Point4 o2(L2.Origin());
    Vector4 d1(L1.Direction());
    Vector4 d2(L2.Direction());

    //compute intermediate parameters
    Vector4 w0(o1 - o2);
    float a = Dot(d1, d1);
    float b = Dot(d1, d2);
    float c = Dot(d2, d2);
    float d = Dot(d1, w0);
    float e = Dot(d2, w0);
    float denom = a*c - b*b;

    if (::IsZero(denom))
    {
      P1 = o1;
      P2 = o2 + (e / c)*d1;
      return 1;
    }
    else
    {
      P1 = o1 + ((b*e - c*d) / denom)*d1;
      P2 = o2 + ((a*e - b*d) / denom)*d2;
      return 0;
    }

  }	//End: ClosestPointsLineLine()

  //--------------------------------------------------------------------------------
  //	@	Line4::Set()
  //--------------------------------------------------------------------------------
  template<typename Real>
  inline void Line4::Set(Matrix<1, 4, Real> const & a_origin, 
                         Matrix<1, 4, Real> const & a_direction)
  {
    //Assign
    m_origin = a_m_origin;
    m_direction = a_m_direction;

    m_direction.Normalize();

  }	//End: Line4::Set()


  //--------------------------------------------------------------------------------
  //		Function Declarations
  //--------------------------------------------------------------------------------


  //--------------------------------------------------------------------------------
  //	@	SqDistLineLine()
  //--------------------------------------------------------------------------------
  /*		Summary: Shortest distance (squared) between two lines
  --------------------------------------
  Post:
  Case - Lines not parallel: returns 0. sqDist set, u1 and u2 are set.
  Case - Lines parallel: returns 1. sqDist set, u1 and u2 are set with u1 = 0.
  --------------------------------------
  Param<L1>:	 input line
  Param<L2>:	 input line
  Param<sqDist>: squared distance between the lines
  Param<u1>:	 distance along L1 to the intersection or
  closest point to L2
  Param<u2>:	 distance along L2 to the intersection or
  closest point to L1
  */
  //--------------------------------------------------------------------------------
  uint8 SqDistLineLine(const Line4&, const Line4&,
    float& sqDist, float& u1, float& u2);


  //--------------------------------------------------------------------------------
  //	@	SqDistLinePoint()
  //--------------------------------------------------------------------------------
  /*		Summary: Shortest distance (squared) between a point and a line
  --------------------------------------
  Post: returns 0. sqDist set, u set.
  --------------------------------------
  Param<L>:	 input line
  Param<P>:	 input point
  Param<sqDist>: squared distance from the point to the line
  Param<u>:	 distance along L to the closest point to P
  */
  //--------------------------------------------------------------------------------
  uint8 SqDistLinePoint(const Line4&, const Vector<4, Real>&, float& sqDist, float& u);


  //--------------------------------------------------------------------------------
  //	@	ClosestPointsLineLine()
  //--------------------------------------------------------------------------------
  /*		Summary: Finds closest points between two Lines
  --------------------------------------
  Post:
  Case Lines not parallel: returns 0. P1, P2 set.
  Case Lines parallel: returns 1. P1, P2 set with P1 = L1.m_origin.
  --------------------------------------
  Param<L1>: input line
  Param<L2>: input line
  Param<P1>: the point on L1 that is closest to L2
  Param<P2>: the point on L2 that is closest to L1
  */
  //--------------------------------------------------------------------------------
  uint8 ClosestPointsLineLine(const Line4&, const Line4&,
    Vector<4, Real>& point1, Vector<4, Real>& point2);


  //--------------------------------------------------------------------------------
  //	@	DistLineLine()
  //--------------------------------------------------------------------------------
  /*		Summary: Shortest distance between two lines
  --------------------------------------
  Post:
  Case - Lines not parallel: returns 0. dist set, u1 and u2 are set.
  Case - Lines parallel: returns 1. dist set, u1 and u2 are set with u1 = 0.
  --------------------------------------
  Param<L1>:	 input line
  Param<L2>:	 input line
  Param<dist>: distance between the lines
  Param<u1>:	 distance along L1 to the intersection or
  closest point to L2
  Param<u2>:	 distance along L2 to the intersection or
  closest point to L1
  */
  //--------------------------------------------------------------------------------
  inline uint8 DistLineLine(const Line4& line1, const Line4& line2,
    float& dist, float& u1, float& u2)
  {
    //Perform test
    uint8 result = SqDistLineLine(line1, line2, dist, u1, u2);

    //Root the distance
    DgSqrt(dist);

    return result;


  }	//End: DistLineLine()


  //--------------------------------------------------------------------------------
  //	@	DistLinePoint()
  //--------------------------------------------------------------------------------
  /*		Summary: Shortest distance between a point and a line
  --------------------------------------
  Post: returns 0. sqDist set, u set.
  --------------------------------------
  Param<L>:	 input line
  Param<P>:	 input point
  Param<dist>: squared distance from the point to the line
  Param<u>:	 distance along L to the closest point to P
  */
  //--------------------------------------------------------------------------------
  inline uint8 DistLinePoint(const Line4& line, const Vector<4, Real>& point,
    float& dist, float& u)
  {
    //Perform test
    uint8 result = SqDistLinePoint(line, point, dist, u);

    //Root the distance
    DgSqrt(dist);

    return result;

  }	//End: DistLinePoint()

}

#endif