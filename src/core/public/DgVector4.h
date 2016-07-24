//! @file DgVector4.h
//!
//! @author: Frank B. Hart
//! @date 4/10/2015
//!
//! Class declaration: Vector4

#ifndef DGVECTOR4_H
#define DGVECTOR4_H

#include "DgRNG.h"
#include "DgMatrix.h"
#include "dgmath.h"

namespace Dg
{
  template<typename Real> class Vector4;
  template<typename Real> class Matrix44;
  template<typename Real> class Plane;
  template<typename Real> class Quaternion;
  template<typename Real> class VQS;

  //! VQS transform
  template<typename Real>
  Vector4<Real> operator*(Vector4<Real> const &, VQS<Real> const &);

  //! Cross product
  template<typename Real>
  Vector4<Real> Cross(Vector4<Real> const &, Vector4<Real> const &);

  //! Returns a random unit vector.
  template<typename Real>
  Vector4<Real> GetRandomVector();

  //! Creates an orthogonal basis from two input vectors
  template<typename Real>
  void GetBasis(Vector4<Real> const & a0,
                Vector4<Real> const & a1,
                Vector4<Real>& x0,
                Vector4<Real>& x1,
                Vector4<Real>& x2);

  //! Returns a perpendicular vector.
  template<typename Real>
  Vector4<Real> Perpendicular(Vector4<Real> const & axis);

  //! Returns a random orthonormal vector to an axis.
  //! @pre Input must be a unit vector.
  //!
  //! @return Vector
  //!
  //! @param axis Resulting vector will be orthormal to this
  template<typename Real>
  Vector4<Real> GetRandomOrthonormalVector(Vector4<Real> const & axis);

  //! Returns a random vector at an angle to an axis.
  //! @pre Input axis must be a unit vector.
  //!
  //! @return Vector
  //!
  //! @param axis Axis
  //! @param angle Angle
  template<typename Real>
  Vector4<Real> GetRandomVector(Vector4<Real> const & axis, Real angle);

  //! Squared distance between two points.
  template<typename Real>
  Real SquaredDistance(Vector4<Real> const &, Vector4<Real> const &);

  //! Distance between two points.
  template<typename Real>
  Real Distance(Vector4<Real> const &, Vector4<Real> const &);

  //! @ingroup DgMath_types
  //!
  //! @class Vector4
  //!
  //! @brief Three dimensional homogeneous vector class [x, y, z, w].
  //!
  //! @author Frank Hart
  //! @date 4/10/2015
  template<typename Real>
  class Vector4 : public Matrix<1, 4, Real>
  {
    friend class Plane<Real>;
    friend class Matrix44<Real>;
    friend class Quaternion<Real>;
    friend class VQS<Real>;

  public:

    //! Default constructor. Members not initialized.
    Vector4() {}

    //! Construct vector from coefficients
    Vector4(Real x, Real y, Real z, Real w);
    ~Vector4() {}

    //! Copy constructor
    Vector4(Matrix<1, 4, Real> const & a_other) : Matrix<1, 4, Real>(a_other) {}

    //! Assignment
    Vector4& operator=(Matrix<1, 4, Real> const &);

    //! Determines if the vector is the unit vector within some tolerance.
    bool IsUnit() const;

    //! Set elements
    void Set(Real x, Real y, Real z, Real w);

    //! Make unit vector
    void Normalize();

    //! Vector length
    Real Length() const;

    //! Squared length
    Real LengthSquared() const;

    //! Squared distance between two points.
    template<typename Real>
    friend Real SquaredDistance(Vector4<Real> const &, Vector4<Real> const &);

    //! Distance between two points.
    template<typename Real>
    friend Real Distance(Vector4<Real> const &, Vector4<Real> const &);

    //! Access element-x by value
    Real x() const { return m_V[0]; }

    //! Access element-y by value
    Real y() const { return m_V[1]; }

    //! Access element-z by value
    Real z() const { return m_V[2]; }

    //! Access element-w by value
    Real w() const { return m_V[3]; }

    //! Access element-x by reference
    Real & x() { return m_V[0]; }

    //! Access element-y by reference
    Real & y() { return m_V[1]; }

    //! Access element-z by reference
    Real & z() { return m_V[2]; }

    //! Access element-w by reference
    Real & w() { return m_V[3]; }

  public:

    //! v = [0, 0, 0, 1]
    static Vector4 Origin();

    //! v = [0, 0, 0, 0]
    static Vector4 ZeroVector();

    //! v = [1, 0, 0, 0]
    static Vector4 xAxis();

    //! v = [0, 1, 0, 0]
    static Vector4 yAxis();

    //! v = [0, 0, 1, 0]
    static Vector4 zAxis();

    //! v = [1, 1, 1, 1]
    static Vector4 Ones();
  };


  //-------------------------------------------------------------------------------
  //	@	Vector4::Origin()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> Vector4<Real>::Origin()
  {
    return Vector4(static_cast<Real>(0.0),
      static_cast<Real>(0.0),
      static_cast<Real>(0.0),
      static_cast<Real>(1.0));
  }   // End:  Vector4::Origin()


  //-------------------------------------------------------------------------------
  //	@	Vector4::ZeroVector()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> Vector4<Real>::ZeroVector()
  {
    return Vector4(static_cast<Real>(0.0),
                   static_cast<Real>(0.0), 
                   static_cast<Real>(0.0), 
                   static_cast<Real>(0.0));
  }   // End:  Vector4::ZeroVector()


  //-------------------------------------------------------------------------------
  //	@	Vector4::xAxis()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> Vector4<Real>::xAxis()
  {
    return Vector4(static_cast<Real>(1.0),
                   static_cast<Real>(0.0),
                   static_cast<Real>(0.0),
                   static_cast<Real>(0.0));
  }   // End:  Vector4::xAxis()


  //-------------------------------------------------------------------------------
  //	@	Vector4::yAxis()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> Vector4<Real>::yAxis()
  {
    return Vector4(static_cast<Real>(0.0),
                   static_cast<Real>(1.0),
                   static_cast<Real>(0.0),
                   static_cast<Real>(0.0));
  }   // End:  Vector4::yAxis()


  //-------------------------------------------------------------------------------
  //	@	Vector4::zAxis()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> Vector4<Real>::zAxis()
  {
    return Vector4(static_cast<Real>(0.0),
                   static_cast<Real>(0.0),
                   static_cast<Real>(1.0),
                   static_cast<Real>(0.0));
  }   // End:  Vector4::zAxis()


  //-------------------------------------------------------------------------------
  //	@	Vector4::Ones()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> Vector4<Real>::Ones()
  {
    return Vector4(static_cast<Real>(1.0),
                   static_cast<Real>(1.0),
                   static_cast<Real>(1.0),
                   static_cast<Real>(1.0));
  }   // End:  Vector4::Ones()


  //-------------------------------------------------------------------------------
  //	@	Vector4::operator=()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real>& Vector4<Real>::operator=(Matrix<1, 4, Real> const & a_other)
  {
    Matrix<1, 4, Real>::operator=(a_other);
    return *this;
  }   // End:  Vector4::operator=()


  //-------------------------------------------------------------------------------
  //	@	Vector4::Vector4()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real>::Vector4(Real a_x, Real a_y, Real a_z, Real a_w)
  {
    m_V[0] = a_x;
    m_V[1] = a_y;
    m_V[2] = a_z;
    m_V[3] = a_w;
  }   // End:  Vector4::Vector4()

  //-------------------------------------------------------------------------------
  //	@	Vector4::Set()
  //-------------------------------------------------------------------------------
  template<typename Real>
  void Vector4<Real>::Set(Real a_x, Real a_y, Real a_z, Real a_w)
  {
    m_V[0] = a_x;
    m_V[1] = a_y;
    m_V[2] = a_z;
    m_V[3] = a_w;

  }   // End: Vector4::Set()

  //-------------------------------------------------------------------------------
  //	@	Vector4::Length()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Real Vector4<Real>::Length() const
  {
    return sqrt(m_V[0] * m_V[0] + 
                m_V[1] * m_V[1] + 
                m_V[2] * m_V[2] + 
                m_V[3] * m_V[3]);

  }   // End:  Vector4::Length()


  //-------------------------------------------------------------------------------
  //	@	Vector4::LengthSquared()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Real Vector4<Real>::LengthSquared() const
  {
    return (m_V[0] * m_V[0] + 
            m_V[1] * m_V[1] + 
            m_V[2] * m_V[2] + 
            m_V[3] * m_V[3]);

  }   // End:  Vector4::LengthSquared()


  //-------------------------------------------------------------------------------
  //	@	SquaredDistance()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Real SquaredDistance(Vector4<Real> const & a_p0,
                       Vector4<Real> const & a_p1)
  {
    Real a = a_p0.m_V[0] - a_p1.m_V[0];
    Real b = a_p0.m_V[1] - a_p1.m_V[1];
    Real c = a_p0.m_V[2] - a_p1.m_V[2];

    return a*a + b*b + c*c;

  } // End: SquaredDistance()


  //-------------------------------------------------------------------------------
  //	@	Distance()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Real Distance(Vector4<Real> const & a_p0,
                Vector4<Real> const & a_p1)
  {
    Real a = a_p0.m_V[0] - a_p1.m_V[0];
    Real b = a_p0.m_V[1] - a_p1.m_V[1];
    Real c = a_p0.m_V[2] - a_p1.m_V[2];

    return sqrt(a*a + b*b + c*c);

  } // End: Distance()


  //-------------------------------------------------------------------------------
  //	@	Vector4::IsUnit()
  //-------------------------------------------------------------------------------
  template<typename Real>
  bool Vector4<Real>::IsUnit() const
  {
    return Dg::IsZero(static_cast<Real>(1.0) - LengthSquared());

  }   // End:  Vector4::IsUnit()


  //-------------------------------------------------------------------------------
  //	@	Vector4::Normalize()
  //-------------------------------------------------------------------------------
  template<typename Real>
  void Vector4<Real>::Normalize()
  {
    Real lengthsq = LengthSquared();

    if (Dg::IsZero(lengthsq))
    {
      m_V[0] = static_cast<Real>(1.0);
      m_V[1] = static_cast<Real>(0.0);
      m_V[2] = static_cast<Real>(0.0);
      m_V[3] = static_cast<Real>(0.0);
    }
    else
    {
      Real factor = static_cast<Real>(1.0) / sqrt(lengthsq);
      m_V[0] *= factor;
      m_V[1] *= factor;
      m_V[2] *= factor;
      m_V[3] *= factor;
    }

  }   // End:  Vector4::Normalize()


  //-------------------------------------------------------------------------------
  //	@	Cross()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> Cross(Vector4<Real> const & v1, Vector4<Real> const & v2)
  {
    Vector4<Real> result;
    result[0] = v1[1] * v2[2] - v1[2] * v2[1];
    result[1] = v1[2] * v2[0] - v1[0] * v2[2];
    result[2] = v1[0] * v2[1] - v1[1] * v2[0];
    result[3] = static_cast<Real>(0.0);

    return result;

  }	//End: Cross()


  //--------------------------------------------------------------------------------
  //	@	GetBasis()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void GetBasis(Vector4<Real> const & a_a0,
                Vector4<Real> const & a_a1,
                Vector4<Real>& a_x0,
                Vector4<Real>& a_x1,
                Vector4<Real>& a_x2)
  {
    bool is_a1_zero = a_a1.IsZero();

    //Check for zero vectors, handle separately
    if (a_a0.IsZero())
    {
      //Both x0, x1 are zero vectors
      if (is_a1_zero)
      {
        a_x0[0] = a_x1[1] = a_x2[2] = static_cast<Real>(1.0);
        a_x0[1] = a_x0[2] = a_x0[3] = static_cast<Real>(0.0);
        a_x1[0] = a_x1[2] = a_x1[3] = static_cast<Real>(0.0);
        a_x2[0] = a_x2[1] = a_x2[3] = static_cast<Real>(0.0);

        return;
      }
      //x0 only is zero vector
      else
      {
        //Build the basis off a_a1
        a_x0 = a_a1;
        a_x0.Normalize();

        //Set x1
        a_x1 = Perpendicular(a_x0);

        //Find perpendicular vector to x0, x1.
        a_x2 = Cross(a_x0, a_x1);

        return;
      }
    }
    //x1 only is zero vector
    else if (is_a1_zero)
    {
      //Build the basis off a_a0
      a_x0 = a_a0;
      a_x0.Normalize();

      //Set x1
      a_x1 = Perpendicular(a_x0);

      //Find perpendicular vector to x0, x1.
      a_x2 = Cross(a_x0, a_x1);

      return;
    }

    //Assign x0
    a_x0 = a_a0;
    a_x0.Normalize();

    //Calculate x2
    a_x2 = Cross(a_x0, a_a1);

    //Test to see if a_a0 and a_a1 are parallel
    if (IsZero(a_x2.LengthSquared()))
    {
      //Find a perpendicular vector
      a_x1 = Perpendicular(a_x0);

      //Calculate x2
      a_x2 = Cross(a_x0, a_x1);
    }
    else
    {
      a_x2.Normalize();
      a_x1 = Cross(a_x2, a_x0);
    }
  } //End: GetBasis()


  //--------------------------------------------------------------------------------
  //	@	Perpendicular()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> Perpendicular(Vector4<Real> const & a_vector)
  {
    Vector4<Real> result;

    if (!Dg::IsZero(a_vector[0]) || !Dg::IsZero(a_vector[1]))
    {
      result[0] = -a_vector[1];
      result[1] = a_vector[0];
      result[2] = static_cast<Real>(0.0);
      result[3] = static_cast<Real>(0.0);
    }
    else
    {
      result[0] = -a_vector[2];
      result[1] = static_cast<Real>(0.0);
      result[2] = a_vector[0];
      result[3] = static_cast<Real>(0.0);
    }

    return result;
  }	//End: Perpendicular()


  //-------------------------------------------------------------------------------
  //		@	GetRandomVector()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> GetRandomVector()
  {
    RNG generator;

    Real theta = generator.GetUniform<Real>(static_cast<Real>(0.0), static_cast<Real>(2.0) * static_cast<Real>(Dg::PI_d));
    Real rho = generator.GetUniform<Real>(static_cast<Real>(-1.0), static_cast<Real>(1.0));

    Real val = sqrt(static_cast<Real>(1.0) - rho * rho);

    Real x = val * cos(theta);
    Real y = val * sin(theta);
    Real z = rho;

    return Vector4<Real>({ x, y, z, static_cast<Real>(0.0) });
  }	//End: GetRandomVector()


  //-------------------------------------------------------------------------------
  //		@ GetRandomOrthonormalVector()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> GetRandomOrthonormalVector(Vector4<Real> const & a_axis)
  {
    RNG gen;
    Vector4<Real> perp = Perpendicular(a_axis);
    Vector4<Real> crs = Cross(a_axis, perp);
    Real phi = gen.GetUniform(static_cast<Real>(0.0)
                            , static_cast<Real>(Dg::PI_d * 2.0));

    return (cos(phi) * perp + sin(phi) * crs);
  }	//End: GetRandomOrthonormalVector()


  //-------------------------------------------------------------------------------
  //		@ GetRandomVector()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> GetRandomVector(Vector4<Real> const & a_axis, Real theta)
  { 
    RNG generator;

    Dg::WrapNumber(static_cast<Real>(0.0)
                 , static_cast<Real>(Dg::PI_d)
                 , theta);

    Real bound = cos(static_cast<Real>(Dg::PI_d) - theta);
    Real x = generator.GetUniform(static_cast<Real>(-1.0), bound);
    Real phi = static_cast<Real>(Dg::PI_d) - acos(x);
    return (cos(phi) * a_axis + sin(phi) * GetRandomOrthonormalVector(a_axis));
  }	//End: GetRandomVector()

}


#endif