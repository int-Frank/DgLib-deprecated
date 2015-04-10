//! @file Quaternion.h
//!
//! @author: James M. Van Verth, Lars M. Bishop, Frank B. Hart
//! @date 4/10/2015
//!
//! Class declaration: Quaternion

//================================================================================
// @ Quaternion.h
// 
// Class: Quaternion
//
// Quaternion class with methods tailored for rotating points and vectors. The
// quaternion 4-tuple can theoretically take on any values, however for 
// orientation representation in 3D space, it is required the quaternion is 
// normalised. It is up to the user to ensure the 
// quaternion is normalised if setting each element itself, for eg
//
//		Quaternion q(1.0, 2.3, -2.9, 4.6);	//Not a valid rotational quaternion
//		q.Normalize();						//Valid rotational quaternion
//
// Quaternions constructed or set by any other means will be valid for eg
//
//		//Valid quaternion set
//		void Set( Real zRotation, Real yRotation, Real xRotation ); 
//
// -------------------------------------------------------------------------------
//
// Original Authors: James M. Van Verth, Lars M. Bishop
// Retrieved From: Essential Mathematics for Games and Interactive Applications SE
// On Date: 2013
//
// Modified by: Frank Hart
// Date last modified: 2013
//
//================================================================================

#ifndef QUATERNION_H
#define QUATERNION_H

#include "dgmath.h"
#include "Vector4.h"

namespace Dg
{
  template<typename Real> class Vector4;
  template<typename Real> class Quaternion;

  template<typename Real>
  Quaternion<Real> operator*(Real, const Quaternion<Real>&);

  template<typename Real>
  Real Dot(const Quaternion<Real>&, const Quaternion<Real>&);

  template<typename Real>
  Quaternion<Real> Conjugate(const Quaternion<Real>&);

  template<typename Real>
  Quaternion<Real> Inverse(const Quaternion<Real>&);

  template<typename Real>
  void Lerp(Quaternion<Real>& a_result, 
            const Quaternion<Real>& a_start, 
            const Quaternion<Real>& a_end, 
            Real a_t);

  template<typename Real>
  void Slerp(Quaternion<Real>& a_result, 
            const Quaternion<Real>& a_start, 
            const Quaternion<Real>& a_end, 
            Real a_t);
  
  template<typename Real>
  void ApproxSlerp(Quaternion<Real>& a_result, 
                   const Quaternion<Real>& a_start, 
                   const Quaternion<Real>& a_end, 
                   Real a_t);

  template<typename Real>
  Quaternion<Real> Conjugate(const Quaternion<Real>&);

  //! @ingroup Math_classes
  //!
  //! @class Quaternion
  //!
  //! @brief Three dimensional homogeneous vector class [x, y, z, w].
  //!
  //! Quaternion class with methods tailored for rotating points and vectors. The
  //! quaternion 4-tuple can theoretically take on any values, however for 
  //! orientation representation in 3D space, it is required the quaternion is 
  //! normalised. It is up to the user to ensure the 
  //! quaternion is normalised if setting each element itself, for eg
  //!
  //!	    Quaternion q(1.0, 2.3, -2.9, 4.6);  //Not a valid rotational quaternion
  //!	    q.Normalize();                      //Valid rotational quaternion
  //!
  //! Quaternions constructed or set by any other means will be valid for eg
  //!
  //!	    void Set( Real zRotation, Real yRotation, Real xRotation );
  //!
  //! @author James M. Van Verth, Lars M. Bishop, modified by Frank B. Hart
  //! @date 4/10/2015
  template<typename Real>
  class Quaternion
  {
    friend class Matrix44<Real>;
    //friend class VQS<Real>;
  public:
    
    //! Default constructed to the identity quaternion (w(1), x(0), y(0), z(0)).
    Quaternion() : m_w(static_cast<Real>(1.0)), m_x(static_cast<Real>(0.0)), m_y(static_cast<Real>(0.0)), m_z(static_cast<Real>(0.0)) {}
    Quaternion(Real a_w, Real a_x, Real a_y, Real a_z) :
      m_w(a_w), m_x(a_x), m_y(a_y), m_z(a_z) {}
    ~Quaternion() {}

    //! Construct quaternion based on axis-angle.
    Quaternion(const Vector4<Real>& axis, Real angle);

    //! Construct quaternion based on start and end vectors.
    Quaternion(const Vector4<Real>& from, const Vector4<Real>& to);

    //! Construct quaternion from vector elements.
    explicit Quaternion(const Vector4<Real>&);

    Quaternion(const Quaternion&);
    Quaternion& operator=(const Quaternion&);

    //! Accessor member by index
    Real& operator[](unsigned int a_i)         { return (&m_w)[a_i]; }

    //! Accessor member by index
    Real operator[](unsigned int a_i) const    { return (&m_w)[a_i]; }

    //! Length.
    Real Magnitude() const;

    //! Squared length.
    Real Norm() const;

    // comparison
    bool operator==(const Quaternion&) const;
    bool operator!=(const Quaternion&) const;

    //! Determines if the quaternion is the zero quaternion within some tolerance.
    bool IsZero() const;

    //! Determines if the quaternion is the unit quaternion within some tolerance.
    bool IsUnit() const;

    //! Determines if the quaternion is the identity quaternion within some tolerance.
    bool IsIdentity() const;

    //! Set quaternion elements.
    void Set(Real a_w, Real a_x, Real a_y, Real a_z);

    //! Set quaternion based on fixed angles.
    void Set(Real a_zRotation, Real a_yRotation, Real a_xRotation);

    //! Set based on an origin, a target to orientate to, and a final up vector.
    //void Set(const Vector4& a_origin, const Vector4& a_target, const Vector4& a_up);

    //! Set quaternion based on axis-angle.
    void Set(const Vector4<Real>& axis, Real angle);

    //! Set quaternion based on start and end vectors.
    void Set(const Vector4<Real>& from, const Vector4<Real>& to);

    //! Ensures quaternion is a valid rotational quaternion.
    void MakeValid();

    //! Get axis-angle based on quaternion.
    void GetAxisAngle(Vector4<Real>& axis, Real& angle);

    //! Sets near-zero elements to 0.
    void Clean();

    //! Sets all elements to 0.
    inline void Zero();

    //! Sets to unit quaternion.
    void Normalize();		

    //! Sets to identity quaternion.
    void Identity();		

    //! Compute complex conjugate.
    template<typename T>
    friend Quaternion<T> Conjugate(const Quaternion<T>&);

    //! Set self to complex conjugate.
    const Quaternion& Conjugate();

    //! Invert quaternion.
    template<typename T>
    friend Quaternion<T> Inverse(const Quaternion<T>&);

    //! Set self to inverse.
    const Quaternion& Inverse();

    // operators

    // addition/subtraction
    Quaternion operator+(const Quaternion&) const;
    Quaternion& operator+=(const Quaternion&);
    Quaternion operator-(const Quaternion&) const;
    Quaternion& operator-=(const Quaternion&);

    Quaternion operator-() const;

    // scalar multiplication
    template<typename T>
    friend Quaternion<T> operator*(T scalar, const Quaternion<T>& vector);

    Quaternion&          operator*=(Real);

    //! Quaternion concatenation.
    Quaternion operator*(const Quaternion&) const;

    //! Quaternion concatenation.
    Quaternion& operator*=(const Quaternion&);

    template<typename T>
    friend T Dot(const Quaternion<T>&, const Quaternion<T>&);

    //! Vector rotation.
    //! @pre Quaternion is normalized.
    Vector4<Real> Rotate(const Vector4<Real>&) const;

    //! Vector rotation. 
    //! @pre Quaternion is normalized.
    void RotateSelf(Vector4<Real>&) const;

    //! Linearly interpolate two quaternions.
    //! This will always take the shorter path between them.
    template<typename T>
    friend void Lerp(Quaternion<T>& a_result, const Quaternion<T>& a_start, const Quaternion<T>& a_end, T a_t);
    
    //! Spherical linearly interpolate two quaternions.
    //! This will always take the shorter path between them.
    template<typename T>
    friend void Slerp(Quaternion<T>& a_result, const Quaternion<T>& a_start, const Quaternion<T>& a_end, T a_t);

    //! Approximate spherical linear interpolation of two quaternions.
    //! Based on "Hacking Quaternions", Jonathan Blow, Game Developer, March 2002.
    //! See Game Developer, February 2004 for an alternate method.
    template<typename T>
    friend void ApproxSlerp(Quaternion<T>& a_result, const Quaternion<T>& a_start, const Quaternion<T>& a_end, T a_t);

  private:
    // member variables
    Real m_w, m_x, m_y, m_z;

  };


  //-------------------------------------------------------------------------------
  //	@	Quaternion::Set()
  //-------------------------------------------------------------------------------
  template<typename Real>
  void Quaternion<Real>::Set(Real a_w, Real a_x, Real a_y, Real a_z)
  {
    m_w = a_w; 
    m_x = a_x; 
    m_y = a_y; 
    m_z = a_z;

  }   // End of Quaternion::Set()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::Zero()
  //-------------------------------------------------------------------------------
  template<typename Real>
  void Quaternion<Real>::Zero()
  {
    m_x = m_y = m_z = m_w = static_cast<Real>(0.0);
  }   // End of Quaternion::Zero()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::Identity()
  //-------------------------------------------------------------------------------
  template<typename Real>
  void Quaternion<Real>::Identity()
  {
    m_x = m_y = m_z = static_cast<Real>(0.0);
    m_w = static_cast<Real>(1.0);
  }   // End of Quaternion::Identity

  //-------------------------------------------------------------------------------
  //	@	Quaternion::Quaternion()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Quaternion<Real>::Quaternion(const Vector4<Real>& a_axis, Real a_angle)
  {
    Set(a_axis, a_angle);

  }   // End of Quaternion::Quaternion()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::Quaternion()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Quaternion<Real>::Quaternion(const Vector4<Real>& a_from, const Vector4<Real>& a_to)
  {
    Set(a_from, a_to);

  }   // End of Quaternion::Quaternion()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::Quaternion()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Quaternion<Real>::Quaternion(const Vector4<Real>& a_vector)
  {
    Set(static_cast<Real>(0.0), a_vector.m_x, a_vector.m_y, a_vector.m_z);

  }   // End of Quaternion::Quaternion()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::Quaternion()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Quaternion<Real>::Quaternion(const Quaternion<Real>& a_other) :
    m_w(a_other.m_w), m_x(a_other.m_x), m_y(a_other.m_y), m_z(a_other.m_z)
  {

  }   // End of Quaternion::Quaternion()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::operator=()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Quaternion<Real>& Quaternion<Real>::operator=(const Quaternion<Real>& a_other)
  {
    m_w = a_other.m_w;
    m_x = a_other.m_x;
    m_y = a_other.m_y;
    m_z = a_other.m_z;

    return *this;

  }   // End of Quaternion::operator=()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::Magnitude()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Real Quaternion<Real>::Magnitude() const
  {
    return sqrt(m_w*m_w + m_x*m_x + m_y*m_y + m_z*m_z);

  }   // End of Quaternion::Magnitude()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::Norm()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Real Quaternion<Real>::Norm() const
  {
    return (m_w*m_w + m_x*m_x + m_y*m_y + m_z*m_z);

  }   // End of Quaternion::Norm()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::operator==()
  //-------------------------------------------------------------------------------
  template<typename Real>
  bool Quaternion<Real>::operator==(const Quaternion<Real>& a_other) const
  {
    return (Dg::IsZero(a_other.m_w - m_w)
            && Dg::IsZero(a_other.m_x - m_x)
            && Dg::IsZero(a_other.m_y - m_y)
            && Dg::IsZero(a_other.m_z - m_z));

  }   // End of Quaternion::operator==()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::operator!=()
  //-------------------------------------------------------------------------------
  template<typename Real>
  bool Quaternion<Real>::operator!=(const Quaternion<Real>& a_other) const
  {
    return !(Dg::IsZero(a_other.m_w - m_w)
             && Dg::IsZero(a_other.m_x - m_x)
             && Dg::IsZero(a_other.m_y - m_y)
             && Dg::IsZero(a_other.m_z - m_z));
  }   // End of Quaternion::operator!=()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::IsZero()
  //-------------------------------------------------------------------------------
  template<typename Real>
  bool Quaternion<Real>::IsZero() const
  {
    return Dg::IsZero(m_w*m_w + m_x*m_x + m_y*m_y + m_z*m_z);

  }   // End of Quaternion::IsZero()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::IsUnit()
  //-------------------------------------------------------------------------------
  template<typename Real>
  bool Quaternion<Real>::IsUnit() const
  {
    return Dg::IsZero(static_cast<Real>(1.0) - m_w*m_w - m_x*m_x - m_y*m_y - m_z*m_z);

  }   // End of Quaternion::IsUnit()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::IsIdentity()
  //-------------------------------------------------------------------------------
  template<typename Real>
  bool Quaternion<Real>::IsIdentity() const
  {
    return (Dg::IsZero(static_cast<Real>(1.0) - m_w)
           && Dg::IsZero(m_x)
           && Dg::IsZero(m_y)
           && Dg::IsZero(m_z));

  }   // End of Quaternion::IsIdentity()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::Set()
  //-------------------------------------------------------------------------------
  template<typename Real>
  void Quaternion<Real>::Set(Real a_zRotation, Real a_yRotation, Real a_xRotation)
  {
    Real half = static_cast<Real>(0.5);
    a_zRotation *= half;
    a_yRotation *= half;
    a_xRotation *= half;

    // get sines and cosines of half angles
    Real Cx = Real(cos(a_xRotation));
    Real Sx = Real(sin(a_xRotation));

    Real Cy = Real(cos(a_yRotation));
    Real Sy = Real(sin(a_yRotation));

    Real Cz = Real(cos(a_zRotation));
    Real Sz = Real(sin(a_zRotation));

    // multiply it out
    m_w = Cx*Cy*Cz - Sx*Sy*Sz;
    m_x = Sx*Cy*Cz + Cx*Sy*Sz;
    m_y = Cx*Sy*Cz - Sx*Cy*Sz;
    m_z = Cx*Cy*Sz + Sx*Sy*Cx;

    //Ensure valid orientation quaternion
    MakeValid();

  }   // End of Quaternion::Set()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::Set()
  //-------------------------------------------------------------------------------
  template<typename Real>
  void Quaternion<Real>::Set(const Vector4<Real>& a_axis, Real a_angle)
  {
    // if axis of rotation is zero vector, just set to identity quat
    Real length = a_axis.LengthSquared();
    if (Dg::IsZero(length))
    {
      Identity();
      return;
    }

    // take half-angle
    a_angle *= static_cast<Real>(0.5);

    Real sintheta = Real(sin(a_angle));
    Real costheta = Real(cos(a_angle));

    Real scaleFactor = sintheta / Real(sqrt(length));

    m_w = costheta;
    m_x = scaleFactor * a_axis.m_x;
    m_y = scaleFactor * a_axis.m_y;
    m_z = scaleFactor * a_axis.m_z;

    //Ensure valid orientation quaternion
    MakeValid();

  }   // End of Quaternion::Set()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::()
  //-------------------------------------------------------------------------------
  //		Set based on an origin, a target to orientate to, and a final up vector.
  //-------------------------------------------------------------------------------
  //template<typename Real>
  //void Quaternion<Real>::Set(const Vector4<Real>& a_origin, 
  //                           const Vector4<Real>& a_target, 
  //                           const Vector4<Real>& a_up)
  //{
  //  //Find the direction to the target
  //  Vector4<Real> direction = a_target - a_origin;

  //  //Find the basis vectors of the orientation
  //  BasisR3 basis(direction, up);

  //  //Convert basis to a matrix
  //  Matrix44 rot(basis);

  //  //Set quaternion from the basis
  //  Set(rot);

  //}	//End: Quaternion::Set()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::()
  //-------------------------------------------------------------------------------
  template<typename Real>
  void Quaternion<Real>::Set(const Vector4<Real>& a_from, const Vector4<Real>& a_to)
  {
    // get axis of rotation
    Vector4<Real> axis = Cross<Real>(a_from, a_to);

    // get scaled cos of angle between vectors and set initial quaternion
    Set(Dot(a_from, a_to), axis.m_x, axis.m_y, axis.m_z);
    // quaternion at this point is ||from||*||a_to||*( cos(theta), r*sin(theta) )

    // normalize a_to remove ||from||*||a_to|| factor
    Normalize();
    // quaternion at this point is ( cos(theta), r*sin(theta) )
    // what we want is ( cos(theta/2), r*sin(theta/2) )

    // set up for half angle calculation
    m_w += static_cast<Real>(1.0f);

    // now when we normalize, we'll be dividing by sqrt(2*(1+cos(theta))), which is 
    // what we want for r*sin(theta) a_to give us r*sin(theta/2)  (see pages 487-488)
    // 
    // w will become 
    //                 1+cos(theta)
    //            ----------------------
    //            sqrt(2*(1+cos(theta)))        
    // which simplifies a_to
    //                cos(theta/2)

    // before we normalize, check if vectors are opposing
    if (m_w <= static_cast<Real>(EPSILON_d))
    {
      // rotate pi radians around orthogonal vector
      // take cross product with x axis
      if (a_from.m_z*a_from.m_z > a_from.m_x*a_from.m_x)
        Set(static_cast<Real>(0.0), static_cast<Real>(0.0), a_from.m_z, -a_from.m_y);
      // or take cross product with z axis
      else
        Set(static_cast<Real>(0.0), a_from.m_y, -a_from.m_x, static_cast<Real>(0.0));
    }

    // normalize again a_to get rotation quaternion
    Normalize();

  }   // End of Quaternion::Set()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::Set()
  //-------------------------------------------------------------------------------
  //		Set based on Rotation matrix
  //-------------------------------------------------------------------------------
  //template<typename Real>
  //void Quaternion<Real>::Set(const Matrix44& rotation)
  //{
  //  //Get trace
  //  Real trace = rotation.mV[0] + rotation.mV[5] + rotation.mV[10];
  //  if (trace > 0.0f)
  //  {
  //    Real s = ::DgSqrt(trace + 1.0f);
  //    w = s*0.5f;
  //    Real recip = 0.5f / s;
  //    x = (rotation(2, 1) - rotation(1, 2))*recip;
  //    y = (rotation(0, 2) - rotation(2, 0))*recip;
  //    z = (rotation(1, 0) - rotation(0, 1))*recip;
  //  }
  //  else
  //  {
  //    uint32 i = 0;
  //    if (rotation(1, 1) > rotation(0, 0))
  //      i = 1;
  //    if (rotation(2, 2) > rotation(i, i))
  //      i = 2;
  //    uint32 j = (i + 1) % 3;
  //    uint32 k = (j + 1) % 3;
  //    Real s = ::DgSqrt(rotation(i, i) - rotation(j, j) - rotation(k, k) + 1.0f);
  //    (*this)[i] = 0.5f*s;
  //    Real recip = 0.5f / s;
  //    w = (rotation(k, j) - rotation(j, k))*recip;
  //    (*this)[j] = (rotation(j, i) + rotation(i, j))*recip;
  //    (*this)[k] = (rotation(k, i) + rotation(i, k))*recip;
  //  }

  //}   // End of Quaternion::Quaternion()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::Set()
  //-------------------------------------------------------------------------------
  //		Set based on 3 basis vectors
  //-------------------------------------------------------------------------------
  //template<typename Real>
  //void Quaternion<Real>::Set(const BasisR3& basis)
  //{
  //  //Convert basis to a matrix
  //  Matrix44 rot(basis);

  //  //Set quaternion from the basis
  //  Set(rot);

  //}   // End of Quaternion::Quaternion()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::MakeValid()
  //-------------------------------------------------------------------------------
  template<typename Real>
  void Quaternion<Real>::MakeValid()
  {
    //Set near-zero elements to zero
    Clean();

    //Find Norm
    Real lengthsq = m_w*m_w + m_x*m_x + m_y*m_y + m_z*m_z;

    //Return if valid
    if (Dg::IsZero(static_cast<Real>(1.0) - lengthsq))
      return;

    //If zero quaternion
    if (Dg::IsZero(lengthsq))
    {
      m_w = static_cast<Real>(1.0);
      m_x = m_y = m_z = static_cast<Real>(0.0);
    }
    else
    {
      Real factor = static_cast<Real>(1.0) / Real(sqrt(lengthsq));
      m_w *= factor;
      m_x *= factor;
      m_y *= factor;
      m_z *= factor;
    }

  }   // End of Quaternion::Identity


  //-------------------------------------------------------------------------------
  //	@	Quaternion::GetAxisAngle()
  //-------------------------------------------------------------------------------
  template<typename Real>
  void Quaternion<Real>::GetAxisAngle(Vector4<Real>& a_axis, Real& a_angle)
  {
    a_angle = static_cast<Real>(2.0) * Real(acos(m_w));
    Real length = sqrt(static_cast<Real>(1.0) - m_w * m_w);
    if (Dg::IsZero(length))
    {
      a_axis.Zero();
    }
    else
    {
      length = static_cast<Real>(1.0) / length;
      a_axis.Set(m_x * length, m_y * length, m_z * length, static_cast<Real>(0.0));
    }

  }   // End of Quaternion::GetAxisAngle()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::Clean()
  //-------------------------------------------------------------------------------
  template<typename Real>
  void Quaternion<Real>::Clean()
  {
    if (Dg::IsZero(m_w))
      m_w = static_cast<Real>(0.0);
    if (Dg::IsZero(m_x))
      m_x = static_cast<Real>(0.0);
    if (Dg::IsZero(m_y))
      m_y = static_cast<Real>(0.0);
    if (Dg::IsZero(m_z))
      m_z = static_cast<Real>(0.0);

  }   // End of Quaternion::Clean()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::Normalize()
  //-------------------------------------------------------------------------------
  template<typename Real>
  void Quaternion<Real>::Normalize()
  {
    Real lengthsq = m_w * m_w + m_x * m_x + m_y * m_y + m_z * m_z;

    if (Dg::IsZero(lengthsq))
    {
      Zero();
    }
    else
    {
      Real factor = static_cast<Real>(1.0) / Real(sqrt(lengthsq));
      m_w *= factor;
      m_x *= factor;
      m_y *= factor;
      m_z *= factor;
    }

  }   // End of Quaternion::Normalize()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::Conjugate()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Quaternion<Real> Conjugate(const Quaternion<Real>& a_quat)
  {
    return Quaternion<Real>(a_quat.m_w, -a_quat.m_x, -a_quat.m_y, -a_quat.m_z);

  }   // End of Conjugate()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::Conjugate()
  //-------------------------------------------------------------------------------
  template<typename Real>
  const Quaternion<Real>& Quaternion<Real>::Conjugate()
  {
    m_x = -m_x;
    m_y = -m_y;
    m_z = -m_z;

    return *this;

  }   // End of Conjugate()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::Inverse()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Quaternion<Real> Inverse(const Quaternion<Real>& a_quat)
  {
    Real norm = a_quat.m_w * a_quat.m_w 
              + a_quat.m_x * a_quat.m_x
              + a_quat.m_y * a_quat.m_y
              + a_quat.m_z * a_quat.m_z;

    // if we're the zero quaternion, just return identity
    if (Dg::IsZero(norm))
    {
      return Quaternion<Real>();
    }

    Real normRecip = static_cast<Real>(1.0) / norm;
    return Quaternion<Real>(normRecip * a_quat.m_w,
                           -normRecip * a_quat.m_x, 
                           -normRecip * a_quat.m_y,
                           -normRecip * a_quat.m_z);

  }   // End of Inverse()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::Inverse()
  //-------------------------------------------------------------------------------
  template<typename Real>
  const Quaternion<Real>& Quaternion<Real>::Inverse()
  {
    Real norm = m_w * m_w + m_x * m_x + m_y * m_y + m_z * m_z;
    // if we're the zero quaternion, just return
    if (Dg::IsZero(norm))
      return *this;

    Real normRecip = static_cast<Real>(1.0) / norm;
    m_w = normRecip * m_w;
    m_x = -normRecip * m_x;
    m_y = -normRecip * m_y;
    m_z = -normRecip * m_z;

    return *this;

  }   // End of Inverse()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::operator+()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Quaternion<Real> Quaternion<Real>::operator+(const Quaternion<Real>& a_other) const
  {
    return Quaternion<Real>(m_w + a_other.m_w,
                            m_x + a_other.m_x, 
                            m_y + a_other.m_y, 
                            m_z + a_other.m_z);

  }   // End of Quaternion::operator+()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::operator+=()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Quaternion<Real>& Quaternion<Real>::operator+=(const Quaternion<Real>& a_other)
  {
    m_w += a_other.m_w;
    m_x += a_other.m_x;
    m_y += a_other.m_y;
    m_z += a_other.m_z;

    return *this;

  }   // End of Quaternion::operator+=()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::operator-()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Quaternion<Real> Quaternion<Real>::operator-(const Quaternion<Real>& a_other) const
  {
    return Quaternion<Real>(m_w - a_other.m_w,
                            m_x - a_other.m_x, 
                            m_y - a_other.m_y, 
                            m_z - a_other.m_z);

  }   // End of Quaternion::operator-()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::operator-=()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Quaternion<Real>& Quaternion<Real>::operator-=(const Quaternion<Real>& a_other)
  {
    m_w -= a_other.m_w;
    m_x -= a_other.m_x;
    m_y -= a_other.m_y;
    m_z -= a_other.m_z;

    return *this;

  }   // End of Quaternion::operator-=()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::operator-()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Quaternion<Real> Quaternion<Real>::operator-() const
  {
    return Quaternion<Real>(-m_w, -m_x, -m_y, -m_z);

  }    // End of Quaternion::operator-()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::operator*()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Quaternion<Real> operator*(Real a_scalar, const Quaternion<Real>& a_quat)
  {
    return Quaternion<Real>(a_scalar * a_quat.m_w, 
                            a_scalar * a_quat.m_x, 
                            a_scalar * a_quat.m_y, 
                            a_scalar * a_quat.m_z);

  }   // End of operator*()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::operator*=()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Quaternion<Real>& Quaternion<Real>::operator*=(Real a_scalar)
  {
    m_w *= a_scalar;
    m_x *= a_scalar;
    m_y *= a_scalar;
    m_z *= a_scalar;

    return *this;

  }   // End of Quaternion::operator*=()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::operator*()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Quaternion<Real> Quaternion<Real>::operator*(const Quaternion<Real>& a_other) const
  {
    return Quaternion<Real>(
      m_w*a_other.m_w - m_x*a_other.m_x - m_y*a_other.m_y - m_z*a_other.m_z,
      m_w*a_other.m_x + m_x*a_other.m_w + m_y*a_other.m_z - m_z*a_other.m_y,
      m_w*a_other.m_y + m_y*a_other.m_w + m_z*a_other.m_x - m_x*a_other.m_z,
      m_w*a_other.m_z + m_z*a_other.m_w + m_x*a_other.m_y - m_y*a_other.m_x);

  }   // End of Quaternion::operator*()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::operator*=()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Quaternion<Real>& Quaternion<Real>::operator*=(const Quaternion<Real>& a_other)
  {
    Set(m_w*a_other.m_w - m_x*a_other.m_x - m_y*a_other.m_y - m_z*a_other.m_z,
        m_w*a_other.m_x + m_x*a_other.m_w + m_y*a_other.m_z - m_z*a_other.m_y,
        m_w*a_other.m_y + m_y*a_other.m_w + m_z*a_other.m_x - m_x*a_other.m_z,
        m_w*a_other.m_z + m_z*a_other.m_w + m_x*a_other.m_y - m_y*a_other.m_x);

    return *this;

  }   // End of Quaternion::operator*=()


  //-------------------------------------------------------------------------------
  //	@	Dot()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Real Dot(const Quaternion<Real>& a_quat1, const Quaternion<Real>& a_quat2)
  {
    return (a_quat1.m_w * a_quat2.m_w
          + a_quat1.m_x * a_quat2.m_x 
          + a_quat1.m_y * a_quat2.m_y
          + a_quat1.m_z * a_quat2.m_z);

  }   // End of Dot()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::Rotate()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> Quaternion<Real>::Rotate(const Vector4<Real>& a_vector) const
  {
    Real vMult = static_cast<Real>(2.0) * (m_x*a_vector.m_x + m_y*a_vector.m_y + m_z*a_vector.m_z);
    Real crossMult = static_cast<Real>(2.0) * m_w;
    Real pMult = crossMult * m_w - static_cast<Real>(1.0);

    return Vector4<Real>(pMult*a_vector.m_x + vMult*m_x + crossMult*(m_y*a_vector.m_z - m_z*a_vector.m_y),
                         pMult*a_vector.m_y + vMult*m_y + crossMult*(m_z*a_vector.m_x - m_x*a_vector.m_z),
                         pMult*a_vector.m_z + vMult*m_z + crossMult*(m_x*a_vector.m_y - m_y*a_vector.m_x),
                         static_cast<Real>(0.0));

  }   // End of Quaternion::Rotate()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::RotateSelf()
  //-------------------------------------------------------------------------------
  template<typename Real>
  void Quaternion<Real>::RotateSelf(Vector4<Real>& a_vector) const
  {
    Real vMult = static_cast<Real>(2.0) * (m_x*a_vector.m_x + m_y*a_vector.m_y + m_z*a_vector.m_z);
    Real crossMult = static_cast<Real>(2.0) * m_w;
    Real pMult = crossMult * m_w - static_cast<Real>(1.0);

    a_vector.Set(pMult*a_vector.m_x + vMult*m_x + crossMult*(m_y*a_vector.m_z - m_z*a_vector.m_y),
                 pMult*a_vector.m_y + vMult*m_y + crossMult*(m_z*a_vector.m_x - m_x*a_vector.m_z),
                 pMult*a_vector.m_z + vMult*m_z + crossMult*(m_x*a_vector.m_y - m_y*a_vector.m_x),
                 static_cast<Real>(0.0));

  }   // End of Quaternion::RotateSelf()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::Lerp()
  //-------------------------------------------------------------------------------
  template<typename Real>
  void Lerp(Quaternion<Real>& a_result, const Quaternion<Real>& a_start, const Quaternion<Real>& a_end, Real a_t)
  {
    // get cos of "angle" between quaternions
    Real cosTheta = Dot<Real>(a_start, a_end);

    // initialize a_result
    a_result = a_t * a_end;

    // if "angle" between quaternions is less than 90 degrees
    if (cosTheta >= static_cast<Real>(EPSILON_d))
    {
      // use standard interpolation
      a_result += (static_cast<Real>(1.0) - a_t) * a_start;
    }
    else
    {
      // otherwise, take the shorter path
      a_result += (a_t - static_cast<Real>(1.0)) * a_start;
    }

  }   // End of Lerp()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::Slerp()
  //-------------------------------------------------------------------------------
  template<typename Real>
  void Slerp(Quaternion<Real>& a_result, const Quaternion<Real>& a_start, const Quaternion<Real>& a_end, Real a_t)
  {
    // get cosine of "angle" between quaternions
    Real cosTheta = Dot<Real>(a_start, a_end);
    Real startInterp, endInterp;

    // if "angle" between quaternions is less than 90 degrees
    if (cosTheta >= static_cast<Real>(EPSILON_d))
    {
      // if angle is greater than zero
      if ((static_cast<Real>(1.0) - cosTheta) > static_cast<Real>(EPSILON_d))
      {
        // use standard slerp
        Real theta = Real(acos(cosTheta));
        Real recipSinTheta = static_cast<Real>(1.0) / Real(sin(theta));

        startInterp = Real(sin((static_cast<Real>(1.0) - a_t) * theta)) * recipSinTheta;
        endInterp = Real(sin(a_t*theta)) * recipSinTheta;
      }
      // angle is close to zero
      else
      {
        // use linear interpolation
        startInterp = static_cast<Real>(1.0) - a_t;
        endInterp = a_t;
      }
    }
    // otherwise, take the shorter route
    else
    {
      // if angle is less than 180 degrees
      if ((static_cast<Real>(1.0) + cosTheta) > static_cast<Real>(EPSILON_d))
      {
        // use slerp w/negation of a_start quaternion
        Real theta = Real(acos(-cosTheta));
        Real recipSinTheta = static_cast<Real>(1.0) / Real(sin(theta));

        startInterp = Real(sin((a_t - static_cast<Real>(1.0)) * theta)) * recipSinTheta;
        endInterp = Real(sin(a_t * theta)) * recipSinTheta;
      }
      // angle is close to 180 degrees
      else
      {
        // use lerp w/negation of a_start quaternion
        startInterp = a_t - static_cast<Real>(1.0);
        endInterp = a_t;
      }
    }

    a_result = startInterp * a_start + endInterp * a_end;

  }   // End of Slerp()


  //-------------------------------------------------------------------------------
  //	@	Quaternion::ApproxSlerp()
  //-------------------------------------------------------------------------------
  template<typename Real>
  void ApproxSlerp(Quaternion<Real>& a_result, const Quaternion<Real>& a_start, const Quaternion<Real>& a_end, Real a_t)
  {
    Real cosTheta = Dot(a_start, a_end);

    // correct time by using cosine of angle between quaternions
    Real factor = static_cast<Real>(1.0) - static_cast<Real>(0.7878088) * cosTheta;
    Real k = static_cast<Real>(0.5069269);
    factor *= factor;
    k *= factor;

    Real b = static_cast<Real>(2.0) * k;
    Real c = static_cast<Real>(-3.0) * k;
    Real d = static_cast<Real>(1.0) + k;

    a_t = a_t * (b * a_t + c) + d;

    // initialize a_result
    a_result = a_t * a_end;

    // if "angle" between quaternions is less than 90 degrees
    if (cosTheta >= static_cast<Real>(EPSILON_d))
    {
      // use standard interpolation
      a_result += (static_cast<Real>(1.0) - a_t) * a_start;
    }
    else
    {
      // otherwise, take the shorter path
      a_result += (a_t - static_cast<Real>(1.0)) * a_start;
    }

  }   // a_end of ApproxSlerp()

}

#endif
