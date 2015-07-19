//================================================================================
// @ Sphere.h
// 
// Class: Sphere
//
// A circle can be defined by the set of all points p at distance <= r away from 
// a central point c. It is implemented simply by defining a point and m_radius.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef SPHERE_H
#define SPHERE_H

#include "Vector4.h"

namespace Dg
{
  template<typename Real> class Vector4;
  template<typename Real> class Quaternion;
  template<typename Real> class VQS;

  //--------------------------------------------------------------------------------
  //	@	Sphere
  //--------------------------------------------------------------------------------
  template<typename Real>
  class Sphere
  {
  public:
    //Constructor/Destructor
    Sphere() : m_center(Vector4<Real>::Origin())), m_radius(static_cast<Real>(1.0)) {}
    Sphere(const Vector4<Real>&, float);
    ~Sphere() {}

    //Copy operations
    Sphere(const Sphere& s) :
      m_center(s.m_center), m_radius(s.m_radius) {}
    Sphere& operator= (const Sphere&);

    //Comparison 
    bool operator== (const Sphere&) const;
    bool operator!= (const Sphere&) const;

    //Input/output
    friend DgReader& operator>> (DgReader& in, Sphere& c);
    friend DgWriter& operator<< (DgWriter& out, const Sphere& c);

    //Set functions
    inline void SetCenter(const Vector4<Real>&);
    void Translate(const Vector4&);
    inline void SetRadius(float val);
    inline void Set(const Vector4<Real>&, float val);
    void Set(const OBB&);

    //Return functions
    const Vector4<Real>& Center()	const { return m_center; }
    float Radius()			const { return m_radius; }
    float SqRadius()		const { return m_radius*m_radius; }

    inline void Clean();

    bool IsInside(const Vector4<Real>& p) const;

    Vector4<Real> ClosestPoint(const Vector4<Real>&) const;

    //Transformations do nothing by default
    void Transform(const VQS&);
    void TransformQuick(const VQS&);

  private:

    //Data members
    Vector4<Real> m_center;
    Real          m_radius;

  };


  //--------------------------------------------------------------------------------
  //		Inlines
  //--------------------------------------------------------------------------------


  //--------------------------------------------------------------------------------
  //	@	Sphere::SetCenter()
  //--------------------------------------------------------------------------------
  //		Set m_center
  //--------------------------------------------------------------------------------
  inline void Sphere::SetCenter(const Vector4<Real>& p)
  {
    m_center = p;

  }	//End: Sphere::SetCenter()


  //--------------------------------------------------------------------------------
  //	@	Sphere::SetRadius()
  //--------------------------------------------------------------------------------
  //		Set m_radius
  //--------------------------------------------------------------------------------
  inline void Sphere::SetRadius(float val)
  {
    assert(val >= 0.0f);
    m_radius = val;

  }	//End: Sphere::SetRadius()


  //--------------------------------------------------------------------------------
  //	@	Sphere::Set()
  //--------------------------------------------------------------------------------
  //		Set all data
  //--------------------------------------------------------------------------------
  inline void Sphere::Set(const Vector4<Real>& p, float val)
  {
    m_center = p;
    SetRadius(val);

  }	//End: Sphere::Set()


  //--------------------------------------------------------------------------------
  //	@	Sphere::Clean()
  //--------------------------------------------------------------------------------
  //		Set near-zero values to zero
  //--------------------------------------------------------------------------------
  inline void Sphere::Clean()
  {
    m_center.Clean();
    if (m_radius < 0.0f)
      m_radius = 0.0f;

  }	//End: Sphere::Clean()


  //--------------------------------------------------------------------------------
  //		Function Declarations
  //--------------------------------------------------------------------------------

  //--------------------------------------------------------------------------------
  //	@	TestSphereSphere()
  //--------------------------------------------------------------------------------
  /*		Summary: Tests if two spheres are intersecting
  --------------------------------------
  Post:
  Case - Intersection: returns 1, sqDist set.
  Case - No Intersection: returns 0, sqDist set.
  --------------------------------------
  Param<s1>:	input Sphere
  Param<s2>:	input Sphere
  Param<sqDist>: Distance squared between the sphere centers
  */
  //--------------------------------------------------------------------------------
  uint8 TestSphereSphere(const Sphere&, const Sphere&, float& sqDist);


  //--------------------------------------------------------------------------------
  //	@	TestOBBSphere()
  //--------------------------------------------------------------------------------
  /*		Summary: Tests if a Sphere and OBB are colliding
  --------------------------------------
  Post:
  Case - Intersection: returns 1, sqDist set.
  Case - No Intersection: returns 0, sqDist set.
  --------------------------------------
  Param<s>:	input Sphere
  Param<b>:	input OBB
  Param<sqDist>: Distance squared between teh sphere and OBB
  */
  //--------------------------------------------------------------------------------
  uint8 TestOBBSphere(const OBB& b, const Sphere& s, float& sqDist);


  //--------------------------------------------------------------------------------
  //	@	TestSpherePlane()
  //--------------------------------------------------------------------------------
  /*		Summary: Tests if a Sphere intersects a plane
  --------------------------------------
  Post:
  Case - No Intersection, sphere on +ve side: returns 0, d set.
  Case - No Intersection, sphere on -ve side: returns 1, d set.
  Case - Intersection: returns 2, d set.
  --------------------------------------
  Param<s>:	input Sphere
  Param<p>:	input plane
  Param<d>:	Distance between the sphere and the plane
  */
  //--------------------------------------------------------------------------------
  uint8 TestPlaneSphere(const Sphere& s, const Plane4& p, float& d);
}




#endif