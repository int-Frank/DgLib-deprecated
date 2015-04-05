//================================================================================
// @ VQS.h
// 
// Class: VQS
//
// Vector, Quaternion, Scalar. This is an alternate to matrices to represent
// transformations in 3D space. In this class we have a translation component
// (Vector), orientation (Quaternion) and scaling component (Scalar). The main
// advantage over a 4x4 matrix is using a quaternion for orientation, arguably
// a more complete and robust method for orientation representation. 
//
// Some disatvantages include: we are restricted to the types of transformations
// we can do. We cannot shear or reflect for example. Also We must use uniform
// scaling.
//
// -------------------------------------------------------------------------------
//
// Author: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef VQS_H
#define VQS_H

#include "Vector4.h"
#include "Quaternion.h"
#include "dgmath.h"

namespace Dg
{
  template<typename Real>
  class VQS < Real > ;

  template<typename Real>
  VQS<Real> Inverse(const VQS<Real>&);

  //--------------------------------------------------------------------------------
  //	@	VQS
  //--------------------------------------------------------------------------------
  template<typename Real>
  class VQS
  {
  public:
    //Constructor/Destructor
    VQS() : m_v(static_cast<Real>(0.0), 
                static_cast<Real>(0.0), 
                static_cast<Real>(0.0),
                static_cast<Real>(0.0)), 
            m_q(static_cast<Real>(1.0), 
                static_cast<Real>(0.0), 
                static_cast<Real>(0.0), 
                static_cast<Real>(0.0)), 
            m_s(static_cast<Real>(1.0)) {}
    VQS(const Vector4<Real>& a_v, const Quaternion<Real>& a_q, Real a_s) :
      m_v(a_v), m_q(a_q), m_s(a_s) {}
    ~VQS() {}

    //Copy operations
    VQS(const VQS& a_other) : m_v(a_other.m_v), m_q(a_other.m_q), m_s(a_other.m_s) {}
    VQS& operator=(const VQS&);

    //Set data
    void Set(const Vector4<Real>&, const Quaternion<Real>&, Real);
    void Set(const Vector4<Real>& a_origin,
             const Vector4<Real>& a_target,
             const Vector4<Real>& a_up,
             Real a_scale = static_cast<Real>(1.0));
    void SetV(const Vector4<Real>&);
    void SetQ(const Quaternion<Real>&);
    void SetS(Real);

    //Ensure valid VQS.
    void MakeValid();

    //Update data
    void UpdateV(const Vector4<Real>&);
    void UpdateQ(const Quaternion<Real>&);
    void UpdateS(Real);

    //Concatenation
    VQS operator* (const VQS<Real>&) const;
    VQS& operator*= (const VQS<Real>&);

    //Operators
    Vector4 operator*(Vector4<Real>) const;

    //Transformations
    Vector4<Real> Translate(Vector4<Real>) const;
    Vector4<Real> Rotate(Vector4<Real>) const;
    Vector4<Real> Scale(Vector4<Real>) const;

    void TranslateSelf(Vector4<Real>&) const;
    void RotateSelf(Vector4<Real>&) const;
    void ScaleSelf(Vector4<Real>&) const;

    //Inverse 
    const VQS& Inverse();

    template<typename T>
    friend VQS<T> Inverse(const VQS<T>&);

    //Returns
    void Get(Vector4<Real>& a_v, Quaternion<Real>& a_q, Real& a_s) const;
    const Vector4<Real>& V()	  const	{ return m_v; }
    const Quaternion<Real>& Q() const	{ return m_q; }
    Real S()	                  const	{ return m_s; }

  private:
    //Data members
    Vector4<Real>		  m_v;		//translation
    Quaternion<Real>	m_q;		//rotation
    Real		          m_s;		//scale
  };

  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::Set()
  //--------------------------------------------------------------------------------
  //		Set translation vector
  //--------------------------------------------------------------------------------
  template<typename Real>
  void VQS<Real>::SetV(const Vector4<Real>& a_v)
  {
    m_v = a_v;

  }	//End: VQS<Real>::Set()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::SetQ()
  //--------------------------------------------------------------------------------
  //		Set quaternion
  //--------------------------------------------------------------------------------
  template<typename Real>
  void VQS<Real>::SetQ(const Quaternion<Real>& a_q)
  {
    m_q = a_q;
    m_q.MakeValid();

  }	//End: VQS<Real>::Set()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::SetS()
  //--------------------------------------------------------------------------------
  //		Set scale
  //--------------------------------------------------------------------------------
  template<typename Real>
  void VQS<Real>::SetS(Real a_s)
  {
    m_s = a_s;

  }	//End: VQS<Real>::Set()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::UpdateV()
  //--------------------------------------------------------------------------------
  //		Update translation vector
  //--------------------------------------------------------------------------------
  template<typename Real>
  void VQS<Real>::UpdateV(const Vector4<Real>& a_v)
  {
    m_v += a_v;

  }	//End: VQS<Real>::UpdateV()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::UpdateQ()
  //--------------------------------------------------------------------------------
  //		Update quaternion
  //--------------------------------------------------------------------------------
  template<typename Real>
  void VQS<Real>::UpdateQ(const Quaternion<Real>& a_q)
  {
    m_q *= a_q;
    m_q.MakeValid();

  }	//End: VQS<Real>::UpdateQ()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::UpdateS()
  //--------------------------------------------------------------------------------
  //		Update scale
  //--------------------------------------------------------------------------------
  template<typename Real>
  void VQS<Real>::UpdateS(Real a_s)
  {
    m_s *= a_s;

  }	//End: VQS<Real>::UpdateS()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::operator=()
  //--------------------------------------------------------------------------------
  //		Assignment
  //--------------------------------------------------------------------------------
  template<typename Real>
  VQS<Real>& VQS<Real>::operator=(const VQS<Real>& a_other)
  {
    m_v = a_other.m_v;
    m_q = a_other.m_q;
    m_s = a_other.m_s;

    return *this;

  }	//End: VQS<Real>::operator=();





  //--------------------------------------------------------------------------------
  //	@	Read_LookAt()
  //--------------------------------------------------------------------------------
  //		Read vqs from an origin, a point to look at and an up vector
  //--------------------------------------------------------------------------------
  template<typename Real>
  void Set(const Vector4<Real>& a_origin,
           const Vector4<Real>& a_target,
           const Vector4<Real>& a_up,
           Real a_scale)
  {
    m_q.Set(a_origin, a_target, a_up);

    m_v = a_origin;
    m_v.w() = static_cast<Real>(0.0);

    m_s = a_scale;

  }	//End: Read_LookAt()


  //--------------------------------------------------------------------------------
  //	@	operator>>()
  //--------------------------------------------------------------------------------
  //		Read from XML node
  //--------------------------------------------------------------------------------
  pugi::xml_node& operator>>(pugi::xml_node& node, VQS& dest)
  {
    //iterate through all nodes
    for (pugi::xml_node_iterator it = node.begin(); it != node.end(); ++it)
    {
      //Get the name of the node
      std::string tag = it->name();

      if (tag == "vqs")
      {
        Read_VQS(*it, dest);
      }
      else if (tag == "lookat")
      {
        Read_LookAt(*it, dest);
      }
    }

    return node;

  }	//End: operator>>()


  //--------------------------------------------------------------------------------
  //	@	operator<<()
  //--------------------------------------------------------------------------------
  //		Read from input
  //--------------------------------------------------------------------------------
  DgWriter& operator<<(DgWriter& out, const VQS& source)
  {
    out << source.m_v << '\n' << source.m_q << '\n' << source.m_s;

    return out;
  }	//End: operator>>()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::operator*()
  //--------------------------------------------------------------------------------
  //		Transform a 3D Point
  //--------------------------------------------------------------------------------
  template<typename Real>
  Point4 VQS<Real>::operator*(Point4 p) const
  {
    //Scale
    p *= m_s;

    //Rotate;
    m_q.RotateSelf(p);

    //Translate
    p += m_v;

    return p;

  }	//End: VQS<Real>::operator()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::Clean
  //--------------------------------------------------------------------------------
  //		Ensure valid VQS, especially m_q
  //--------------------------------------------------------------------------------
  template<typename Real>
  void VQS<Real>::MakeValid()
  {
    //No need to clean vector

    //Clean quaternion
    m_q.MakeValid();

    //Clean scale;
    if (m_s < EPSILON)
      m_s = static_cast<Real>(0.0);

  }	//End: VQS<Real>::Clean()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::operator*()
  //--------------------------------------------------------------------------------
  //		Transform a 3D Vector
  //--------------------------------------------------------------------------------
  template<typename Real>
  Vector4 VQS<Real>::operator*(Vector4 p) const
  {
    //Scale
    p *= m_s;

    //Rotate;
    m_q.RotateSelf(p);

    //Translate: DO NOT TRANSLATE VECTORS
    //p += m_v;

    return p;

  }	//End: VQS<Real>::operator*()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::Translate()
  //--------------------------------------------------------------------------------
  //		Translate a 3D Point
  //--------------------------------------------------------------------------------
  template<typename Real>
  Point4 VQS<Real>::Translate(Point4 p) const
  {
    //Translate
    p += m_v;

    return p;

  }	//End: VQS<Real>::Translate()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::Rotate()
  //--------------------------------------------------------------------------------
  //		Rotate a 3D Point
  //--------------------------------------------------------------------------------
  template<typename Real>
  Point4 VQS<Real>::Rotate(Point4 p) const
  {
    //Rotate;
    m_q.RotateSelf(p);

    return p;

  }	//End: VQS<Real>::Rotate()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::Scale()
  //--------------------------------------------------------------------------------
  //		Scale coordinates of a 3D Point
  //--------------------------------------------------------------------------------
  template<typename Real>
  Point4 VQS<Real>::Scale(Point4 p) const
  {
    //Scale
    p *= m_s;

    return p;

  }	//End: VQS<Real>::Scale()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::Translate()
  //--------------------------------------------------------------------------------
  //		Translate a 3D Vector
  //--------------------------------------------------------------------------------
  template<typename Real>
  Vector4 VQS<Real>::Translate(Vector4 p) const
  {
    //Translate
    p += m_v;

    return p;

  }	//End: VQS<Real>::Translate()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::Rotate()
  //--------------------------------------------------------------------------------
  //		Rotate a 3D Vector
  //--------------------------------------------------------------------------------
  template<typename Real>
  Vector4 VQS<Real>::Rotate(Vector4 p) const
  {
    //Rotate;
    m_q.RotateSelf(p);

    return p;

  }	//End: VQS<Real>::Rotate()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::Scale()
  //--------------------------------------------------------------------------------
  //		Scale coordinates of a 3D Vector
  //--------------------------------------------------------------------------------
  template<typename Real>
  Vector4 VQS<Real>::Scale(Vector4 p) const
  {
    //Scale
    p *= m_s;

    return p;

  }	//End: VQS<Real>::Scale()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::TranslateSelf()
  //--------------------------------------------------------------------------------
  //		Translate a HPoint, store in self
  //--------------------------------------------------------------------------------
  template<typename Real>
  void VQS<Real>::TranslateSelf(HPoint& p) const
  {
    //Translate
    p.x += m_v.x;
    p.y += m_v.y;
    p.z += m_v.z;

  }	//End: VQS<Real>::TranslateSelf()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::RotateSelf()
  //--------------------------------------------------------------------------------
  //		Rotate a Point4, store in self
  //--------------------------------------------------------------------------------
  template<typename Real>
  void VQS<Real>::RotateSelf(Point4& p) const
  {
    //Rotate;
    m_q.RotateSelf(p);

  }	//End: VQS<Real>::RotateSelf()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::RotateSelf()
  //--------------------------------------------------------------------------------
  //		Rotate a Vector4, store in self
  //--------------------------------------------------------------------------------
  template<typename Real>
  void VQS<Real>::RotateSelf(Vector4& m_v) const
  {
    //Rotate;
    m_q.RotateSelf(m_v);

  }	//End: VQS<Real>::RotateSelf()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::ScaleSelf()
  //--------------------------------------------------------------------------------
  //		Scale coordinates of a HPoint (first 3 elements), store in self
  //--------------------------------------------------------------------------------
  template<typename Real>
  void VQS<Real>::ScaleSelf(HPoint& p) const
  {
    //Scale
    p.x *= m_s;
    p.y *= m_s;
    p.z *= m_s;

  }	//End: VQS<Real>::ScaleSelf()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::Set()
  //--------------------------------------------------------------------------------
  //		Set all data
  //--------------------------------------------------------------------------------
  template<typename Real>
  void VQS<Real>::Set(const Vector4& _v, const Quaternion& _q, Real _s)
  {
    m_v = _v;
    m_q = _q;

    //Ensure m_q is valid
    m_q.MakeValid();

    m_s = _s;

  }	//End: VQS<Real>::Set()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::Set()
  //--------------------------------------------------------------------------------
  //		Get all data
  //--------------------------------------------------------------------------------
  template<typename Real>
  void VQS<Real>::Get(const Vector4& a_v, const Quaternion& a_q, Real a_s)
  {
    a_v = m_v;
    a_q = m_q;
    a_s = m_s;

  }	//End: VQS<Real>::Set()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::operator*()
  //--------------------------------------------------------------------------------
  //		Concatenation, return VQS
  //--------------------------------------------------------------------------------
  template<typename Real>
  VQS VQS<Real>::operator*(const VQS& rhs) const
  {
    VQS result;

    //Combine translation vectors
    result.m_v = m_q.Rotate(rhs.m_v)*m_s + m_v;

    //Combine quaternions
    result.m_q = m_q * rhs.m_q;

    //Combine scales
    result.m_s = m_s * rhs.m_s;

    //Return result
    return result;

  }	//End: VQS<Real>::operator*()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::Operator*=()
  //--------------------------------------------------------------------------------
  //		Concatenation, store in self
  //--------------------------------------------------------------------------------
  template<typename Real>
  VQS& VQS<Real>::operator*=(const VQS& rhs)
  {
    //Combine translation vectors
    m_v = m_q.Rotate(rhs.m_v)*m_s + m_v;

    //Combine quaternions
    m_q = m_q * rhs.m_q;

    //Combine scales
    m_s = m_s * rhs.m_s;

    return *this;

  }	//End: VQS<Real>::operator*=()


  //--------------------------------------------------------------------------------
  //	@	VQS<Real>::Inverse()
  //--------------------------------------------------------------------------------
  //		Sets self to inverse
  //--------------------------------------------------------------------------------
  template<typename Real>
  const VQS& VQS<Real>::Inverse()
  {
    //The method to find a VQS inverse
    //[1/m_s*(m_q-1*(-m_v)*m_q), m_q-1, 1/m_s]

    //Inverse scale
    m_s = static_cast<Real>(1.0) / m_s;

    //Inverse quaternion
    m_q.Inverse();

    //Inverse vector
    m_v = m_q.Rotate(-m_v) * m_s;

    return *this;

  }	//End: Inverse()


  //--------------------------------------------------------------------------------
  //	@	Inverse()
  //--------------------------------------------------------------------------------
  //		Returns inverse VQS
  //--------------------------------------------------------------------------------
  template<typename Real>
  VQS Inverse(const VQS& other)
  {
    VQS temp;

    //Inverse scale
    temp.m_s = static_cast<Real>(1.0) / other.m_s;

    //Inverse quaternion
    temp.m_q = Inverse(other.m_q);

    //Inverse vector
    temp.m_v = temp.m_q.Rotate(-other.m_v) * temp.m_s;

    return temp;

  }	//End: Inverse()
}



#endif