//! @file DgFrustum.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: Frustum

#ifndef DGOBB_H
#define DGOBB_H

#include "DgVector4.h"
#include "DgLine.h"
#include "DgPlane.h"
#include "DgVQS.h"
#include "dgmath.h"

//TODO Queries
namespace Dg
{
  //! Data used for intersection testing.
  template<typename Real>
  struct FrustumSpaceData
  {
    Plane<Real>     planes[4];    //[t, b, l, r]
    Vector4<Real>   center;
    Line<Real>      edges[4];    //[tl, tr, bl, br]
    Vector4<Real>   vertices[8]; //[ftl, ftr, fbl, fbr, ntl, ntr, nbl, nbr ]
  };

  //! Data used for intersection testing.
  template<typename Real>
  struct FrustumData
  {
    enum
    {
      //Used to calculate the quadrant
      Top               = 0,
      Bottom            = 4,
      Far               = 0,
      Near              = 2,
      Left              = 0,
      Right             = 1,

      //Vertices
      TopFarLeft        = (Top | Far | Left),         // 0
      TopFarRight       = (Top | Far | Right),        // 1
      TopNearLeft       = (Top | Near | Left),        // 2
      TopNearRight      = (Top | Near | Right),       // 3
      BottomFarLeft     = (Bottom | Far | Left),      // 4
      BottomFarRight    = (Bottom | Far | Right),     // 5
      BottomNearLeft    = (Bottom | Near | Left),     // 6
      BottomNearRight   = (Bottom | Near | Right),    // 7

      //Edges
      TopFar            = 0,
      TopNear           = 1,
      TopLeft           = 2,
      TopRight          = 3,
      BottomFar         = 4,
      BottomNear        = 5,
      BottomLeft        = 6,
      BottomRight       = 7,
      FarLeft           = 8,
      FarRight          = 9,
      NearLeft          = 10,
      NearRight         = 11,

      //Planes
      TopPlane          = 0,
      BottomPlane       = 1,
      FarPlane          = 2,
      NearPlane         = 3,
      LeftPlane         = 4,
      RightPlane        = 5
    };

    //The relevant edges touching each vertex.
    static int const VEdges[8][3];

    //The relevant planes touching each vertex.
    static int const VPlanes[8][3];

    Plane<Real>     planes[6];    //[t, b, f, n, l, r]
    Vector4<Real>   center;
    Vector4<Real>   origin;
    Vector4<Real>   basis[3];     //[f, l, u]
    Line<Real>      edges[12];    //as per enum
    Vector4<Real>   vertices[8];  //as per enum
  };

  template<typename Real>
  int const FrustumData<Real>::VEdges[8][3] =
  {
    { TopFar,     TopLeft,     FarLeft },    //TopFarLeft
    { TopFar,     TopRight,    FarRight },   //TopFarRight    
    { TopNear,    TopLeft,     NearLeft },   //TopNearLeft    
    { TopNear,    TopRight,    NearRight },  //TopNearRight   
    { BottomFar,  BottomLeft,  FarLeft },    //BottomFarLeft  
    { BottomFar,  BottomRight, FarRight },   //BottomFarRight 
    { BottomNear, BottomLeft,  NearLeft },   //BottomNearLeft 
    { BottomNear, BottomRight, NearRight }   //BottomNearRight
  };

  template<typename Real>
  int const FrustumData<Real>::VPlanes[8][3] =
  {
    { TopPlane,    FarPlane,   LeftPlane },   //TopFarLeft
    { TopPlane,    FarPlane,   RightPlane },  //TopFarRight    
    { TopPlane,    NearPlane,  LeftPlane },   //TopNearLeft    
    { TopPlane,    NearPlane,  RightPlane },  //TopNearRight   
    { BottomPlane, FarPlane,   LeftPlane },   //BottomFarLeft  
    { BottomPlane, FarPlane,   RightPlane },  //BottomFarRight 
    { BottomPlane, NearPlane , LeftPlane },   //BottomNearLeft 
    { BottomPlane, NearPlane , RightPlane }   //BottomNearRight
  };

  //! @ingroup DgMath_types
  //!
  //! @class Frustum
  //!
  //! @author: Frank Hart
  //! @date 20/06/2016
  template<typename Real>
  class Frustum
  {
  private:

  public:
    //! Default constructor.
    Frustum()
      : m_basis{ Vector4<Real>::xAxis(), Vector4<Real>::yAxis(), Vector4<Real>::zAxis() }
      , m_origin(Vector4<Real>::Origin())
      , m_near(static_cast<Real>(0.1))
      , m_far(static_cast<Real>(100.0))
      , m_nearHalfLengths{ static_cast<Real>(1.0), static_cast<Real>(1.0)}
    {}

    Frustum(Vector4<Real> const & forward
      , Vector4<Real> const & left
      , Vector4<Real> const & up
      , Vector4<Real> const & origin
      , Real nearL, Real farL, Real verL, Real horL)
      : m_basis{forward, left, up}
      , m_nearHalfLengths{verL, farL}
      , m_near(nearL)
      , m_far(farL)
    {}

    //! Construct Frustum from origin and radius
    ~Frustum() {}

    //! Copy constructor
    Frustum(Frustum const & a_other)
      : m_basis{a_other.m_basis[0], a_other.m_basis[1], a_other.m_basis[2]}
      , m_origin(a_other.m_origin)
      , m_near(a_other.m_near)
      , m_far(a_other.m_far)
      , m_nearHalfLengths{a_other.m_nearHalfLengths[0], a_other.m_nearHalfLengths[1] }
    {}

    //! Assignment
    Frustum& operator= (Frustum const & a_other)
    {
      m_basis[0] = a_other.m_basis[0];
      m_basis[1] = a_other.m_basis[1];
      m_basis[2] = a_other.m_basis[2];

      m_near = a_other.m_near;
      m_far = a_other.m_far;
      m_origin = a_other.m_origin;

      m_nearHalfLengths[0] = a_other.m_nearHalfLengths[0];
      m_nearHalfLengths[1] = a_other.m_nearHalfLengths[1];
    }

    //! Get the Frustum center.
    void GetData(FrustumData<Real> & a_data) const
    {
      a_data.basis[0] = m_basis[0];
      a_data.basis[1] = m_basis[1];
      a_data.basis[2] = m_basis[2];

      a_data.origin = m_origin;

      a_data.center = m_origin + (m_near + (m_far - m_near) / static_cast<Real>(2.0)) * m_basis[0];

      Real eV = m_nearHalfLengths[0] / m_near;
      Real eH = m_nearHalfLengths[1] / m_near;
      a_data.planes[0].Set(-m_basis[0], m_origin + m_far * m_basis[0]);
      a_data.planes[1].Set(m_basis[0], m_origin + m_near * m_basis[0]);
      a_data.planes[2].Set(-m_basis[2] + (eV)* m_basis[0], m_origin);
      a_data.planes[3].Set(m_basis[2] + (eV)* m_basis[0], m_origin);
      a_data.planes[4].Set(-m_basis[1] + (eH)* m_basis[0], m_origin);
      a_data.planes[5].Set(m_basis[1] + (eH)* m_basis[0], m_origin);

      //TODO These vertices and edges are wrong
      Real farRatio = m_far / m_near;
      a_data.vertices[FrustumData<Real>::TopFarLeft] = m_origin
        + m_far * m_basis[0] 
        + m_nearHalfLengths[0] * farRatio * m_basis[2]
        + m_nearHalfLengths[1] * farRatio * m_basis[1];

      a_data.vertices[FrustumData<Real>::TopFarRight] = m_origin
        + m_far * m_basis[0]
        + m_nearHalfLengths[0] * farRatio * m_basis[2]
        - m_nearHalfLengths[1] * farRatio * m_basis[1];

      a_data.vertices[FrustumData<Real>::TopNearLeft] = m_origin
        + m_far * m_basis[0]
        - m_nearHalfLengths[0] * farRatio * m_basis[2]
        + m_nearHalfLengths[1] * farRatio * m_basis[1];

      a_data.vertices[FrustumData<Real>::TopNearRight] = m_origin
        + m_far * m_basis[0]
        - m_nearHalfLengths[0] * farRatio * m_basis[2]
        - m_nearHalfLengths[1] * farRatio * m_basis[1];

      a_data.vertices[FrustumData<Real>::BottomFarLeft] = m_origin
        + m_far * m_basis[0]
        + m_nearHalfLengths[0] * m_basis[2]
        + m_nearHalfLengths[1] * m_basis[1];

      a_data.vertices[FrustumData<Real>::BottomFarRight] = m_origin
        + m_far * m_basis[0]
        + m_nearHalfLengths[0] * m_basis[2]
        - m_nearHalfLengths[1] * m_basis[1];

      a_data.vertices[FrustumData<Real>::BottomNearLeft] = m_origin
        + m_far * m_basis[0]
        - m_nearHalfLengths[0] * m_basis[2]
        + m_nearHalfLengths[1] * m_basis[1];

      a_data.vertices[FrustumData<Real>::BottomNearRight] = m_origin
        + m_far * m_basis[0]
        - m_nearHalfLengths[0] * m_basis[2]
        - m_nearHalfLengths[1] * m_basis[1];

      //[ft, fb, fl, fr, nt, nb, nl, nr, tl, tr, bl, br]
      a_data.edges[FrustumData<Real>::TopFar] = Line<Real>(a_data.vertices[0], -m_basis[1]);
      a_data.edges[FrustumData<Real>::TopNear] = Line<Real>(a_data.vertices[2], -m_basis[1]);
      a_data.edges[FrustumData<Real>::TopLeft] = Line<Real>(a_data.vertices[0], -m_basis[2]);
      a_data.edges[FrustumData<Real>::TopRight] = Line<Real>(a_data.vertices[1], -m_basis[2]);
      a_data.edges[FrustumData<Real>::BottomFar] = Line<Real>(a_data.vertices[4], -m_basis[1]);
      a_data.edges[FrustumData<Real>::BottomNear] = Line<Real>(a_data.vertices[6], -m_basis[1]);
      a_data.edges[FrustumData<Real>::BottomLeft] = Line<Real>(a_data.vertices[4], -m_basis[2]);
      a_data.edges[FrustumData<Real>::BottomRight] = Line<Real>(a_data.vertices[5], -m_basis[2]);
      a_data.edges[FrustumData<Real>::FarLeft] = Line<Real>(a_data.vertices[4], a_data.vertices[0] - a_data.vertices[4]);
      a_data.edges[FrustumData<Real>::FarRight] = Line<Real>(a_data.vertices[5], a_data.vertices[1] - a_data.vertices[5]);
      a_data.edges[FrustumData<Real>::NearLeft] = Line<Real>(a_data.vertices[6], a_data.vertices[2] - a_data.vertices[6]);
      a_data.edges[FrustumData<Real>::NearRight] = Line<Real>(a_data.vertices[7], a_data.vertices[3] - a_data.vertices[7]);
    }

    //! Transform the Frustum
    Frustum GetTransformed(VQS<Real> const & a_vqs) const
    {
      return Frustum
      (
        a_vqs.Rotate(m_basis[0]),
        a_vqs.Rotate(m_basis[1]),
        a_vqs.Rotate(m_basis[2]),
        a_vqs.TransformPoint(m_origin),
        a_vqs.Scale(m_near),
        a_vqs.Scale(m_far),
        a_vqs.Scale(m_nearHalfLengths[0]),
        a_vqs.Scale(m_nearHalfLengths[1]),
      )
    }

    //! Transform the Frustum, assign to self
    Frustum & TransformSelf(VQS<Real> const & a_vqs)
    {
      a_vqs.RotateSelf(m_basis[0]);
      a_vqs.RotateSelf(m_basis[1]);
      a_vqs.RotateSelf(m_basis[2]);
      a_vqs.TransformPointSelf(m_origin);
      a_vqs.ScaleSelf(m_near);
      a_vqs.ScaleSelf(m_far);
      a_vqs.ScaleSelf(m_nearHalfLengths[0]);
      a_vqs.ScaleSelf(m_nearHalfLengths[1]);
    }

  private:
    Vector4<Real>       m_basis[3];           //[f, l, u]
    Vector4<Real>       m_origin;
    Real                m_near;
    Real                m_far;
    Real                m_nearHalfLengths[2]; //[vertical, horizontal]
  };


}

#endif