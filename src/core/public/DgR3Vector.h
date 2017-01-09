//! @file DgR3Vector.h
//!
//! @author: Frank B. Hart
//! @date 4/10/2015
//!
//! Class declaration: Vector

#ifndef DGR3VECTOR4_H
#define DGR3VECTOR4_H

#include "DgMatrix.h"
#include "dgmath.h"

namespace Dg
{
  namespace R3
  {
    template<typename Real> class Vector;
    template<typename Real> class Matrix44;
    template<typename Real> class Plane;
    template<typename Real> class Quaternion;
    template<typename Real> class VQS;

    //! VQS transform
    template<typename Real>
    Vector<Real> operator*(Vector<Real> const &, VQS<Real> const &);

    //! Cross product
    template<typename Real>
    Vector<Real> Cross(Vector<Real> const &, Vector<Real> const &);

    //! Creates an orthogonal basis from two input vectors. 
    //!   - The vector 'A_in' is considered the principle axis.
    //!   - If A_in is a zero vector, A_out will be set to Vector<Real>::xAxis()
    //!   - If A_in and B_in are parallel, or B_in is a Zero vector, B_out will be chosen such that it is orthogonal to A_in
    //!   - Finally, C_out is created from A_out X B_out.
    template<typename Real>
    void GetBasis(Vector<Real> const & A_in,
      Vector<Real> const & B_in,
      Vector<Real>& A_out,
      Vector<Real>& B_out,
      Vector<Real>& C_out);

    //! Returns a perpendicular vector.
    template<typename Real>
    Vector<Real> Perpendicular(Vector<Real> const & axis);

    //! Squared distance between two points.
    template<typename Real>
    Real SquaredDistance(Vector<Real> const &, Vector<Real> const &);

    //! Distance between two points.
    template<typename Real>
    Real Distance(Vector<Real> const &, Vector<Real> const &);

    //! @ingroup DgMath_types
    //!
    //! @class Vector
    //!
    //! @brief Three dimensional homogeneous vector class [x, y, z, w].
    //!
    //! @author Frank Hart
    //! @date 4/10/2015
    template<typename Real>
    class Vector : public Matrix<1, 4, Real>
    {
      friend class Plane<Real>;
      friend class Matrix44<Real>;
      friend class Quaternion<Real>;
      friend class VQS<Real>;

    public:

      //! Default constructor. Members not initialized.
      Vector() {}

      //! Construct vector from coefficients
      Vector(Real x, Real y, Real z, Real w);
      ~Vector() {}

      //! Copy constructor
      Vector(Matrix<1, 4, Real> const & a_other) : Matrix<1, 4, Real>(a_other) {}

      //! Assignment
      Vector& operator=(Matrix<1, 4, Real> const &);

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
      friend Real Dg::R3::SquaredDistance(Vector<Real> const &, Vector<Real> const &);

      //! Distance between two points.
      friend Real Dg::R3::Distance(Vector<Real> const &, Vector<Real> const &);

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
      static Vector Origin();

      //! v = [0, 0, 0, 0]
      static Vector ZeroVector();

      //! v = [1, 0, 0, 0]
      static Vector xAxis();

      //! v = [0, 1, 0, 0]
      static Vector yAxis();

      //! v = [0, 0, 1, 0]
      static Vector zAxis();

      //! v = [1, 1, 1, 1]
      static Vector Ones();
    };


    //-------------------------------------------------------------------------------
    //	@	Vector::Origin()
    //-------------------------------------------------------------------------------
    template<typename Real>
    Vector<Real> Vector<Real>::Origin()
    {
      return Vector(static_cast<Real>(0.0),
        static_cast<Real>(0.0),
        static_cast<Real>(0.0),
        static_cast<Real>(1.0));
    }   // End:  Vector::Origin()


        //-------------------------------------------------------------------------------
        //	@	Vector::ZeroVector()
        //-------------------------------------------------------------------------------
    template<typename Real>
    Vector<Real> Vector<Real>::ZeroVector()
    {
      return Vector(static_cast<Real>(0.0),
        static_cast<Real>(0.0),
        static_cast<Real>(0.0),
        static_cast<Real>(0.0));
    }   // End:  Vector::ZeroVector()


        //-------------------------------------------------------------------------------
        //	@	Vector::xAxis()
        //-------------------------------------------------------------------------------
    template<typename Real>
    Vector<Real> Vector<Real>::xAxis()
    {
      return Vector(static_cast<Real>(1.0),
        static_cast<Real>(0.0),
        static_cast<Real>(0.0),
        static_cast<Real>(0.0));
    }   // End:  Vector::xAxis()


        //-------------------------------------------------------------------------------
        //	@	Vector::yAxis()
        //-------------------------------------------------------------------------------
    template<typename Real>
    Vector<Real> Vector<Real>::yAxis()
    {
      return Vector(static_cast<Real>(0.0),
        static_cast<Real>(1.0),
        static_cast<Real>(0.0),
        static_cast<Real>(0.0));
    }   // End:  Vector::yAxis()


        //-------------------------------------------------------------------------------
        //	@	Vector::zAxis()
        //-------------------------------------------------------------------------------
    template<typename Real>
    Vector<Real> Vector<Real>::zAxis()
    {
      return Vector(static_cast<Real>(0.0),
        static_cast<Real>(0.0),
        static_cast<Real>(1.0),
        static_cast<Real>(0.0));
    }   // End:  Vector::zAxis()


        //-------------------------------------------------------------------------------
        //	@	Vector::Ones()
        //-------------------------------------------------------------------------------
    template<typename Real>
    Vector<Real> Vector<Real>::Ones()
    {
      return Vector(static_cast<Real>(1.0),
        static_cast<Real>(1.0),
        static_cast<Real>(1.0),
        static_cast<Real>(1.0));
    }   // End:  Vector::Ones()


        //-------------------------------------------------------------------------------
        //	@	Vector::operator=()
        //-------------------------------------------------------------------------------
    template<typename Real>
    Vector<Real>& Vector<Real>::operator=(Matrix<1, 4, Real> const & a_other)
    {
      Matrix<1, 4, Real>::operator=(a_other);
      return *this;
    }   // End:  Vector::operator=()


        //-------------------------------------------------------------------------------
        //	@	Vector::Vector()
        //-------------------------------------------------------------------------------
    template<typename Real>
    Vector<Real>::Vector(Real a_x, Real a_y, Real a_z, Real a_w)
    {
      m_V[0] = a_x;
      m_V[1] = a_y;
      m_V[2] = a_z;
      m_V[3] = a_w;
    }   // End:  Vector::Vector()

        //-------------------------------------------------------------------------------
        //	@	Vector::Set()
        //-------------------------------------------------------------------------------
    template<typename Real>
    void Vector<Real>::Set(Real a_x, Real a_y, Real a_z, Real a_w)
    {
      m_V[0] = a_x;
      m_V[1] = a_y;
      m_V[2] = a_z;
      m_V[3] = a_w;

    }   // End: Vector::Set()

        //-------------------------------------------------------------------------------
        //	@	Vector::Length()
        //-------------------------------------------------------------------------------
    template<typename Real>
    Real Vector<Real>::Length() const
    {
      return sqrt(m_V[0] * m_V[0] +
        m_V[1] * m_V[1] +
        m_V[2] * m_V[2] +
        m_V[3] * m_V[3]);

    }   // End:  Vector::Length()


        //-------------------------------------------------------------------------------
        //	@	Vector::LengthSquared()
        //-------------------------------------------------------------------------------
    template<typename Real>
    Real Vector<Real>::LengthSquared() const
    {
      return (m_V[0] * m_V[0] +
        m_V[1] * m_V[1] +
        m_V[2] * m_V[2] +
        m_V[3] * m_V[3]);

    }   // End:  Vector::LengthSquared()


        //-------------------------------------------------------------------------------
        //	@	SquaredDistance()
        //-------------------------------------------------------------------------------
    template<typename Real>
    Real SquaredDistance(Vector<Real> const & a_p0,
      Vector<Real> const & a_p1)
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
    Real Distance(Vector<Real> const & a_p0,
      Vector<Real> const & a_p1)
    {
      Real a = a_p0.m_V[0] - a_p1.m_V[0];
      Real b = a_p0.m_V[1] - a_p1.m_V[1];
      Real c = a_p0.m_V[2] - a_p1.m_V[2];

      return sqrt(a*a + b*b + c*c);

    } // End: Distance()


      //-------------------------------------------------------------------------------
      //	@	Vector::IsUnit()
      //-------------------------------------------------------------------------------
    template<typename Real>
    bool Vector<Real>::IsUnit() const
    {
      return Dg::IsZero(static_cast<Real>(1.0) - LengthSquared());

    }   // End:  Vector::IsUnit()


        //-------------------------------------------------------------------------------
        //	@	Vector::Normalize()
        //-------------------------------------------------------------------------------
    template<typename Real>
    void Vector<Real>::Normalize()
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

    }   // End:  Vector::Normalize()


        //-------------------------------------------------------------------------------
        //	@	Cross()
        //--------------------------------------------------------------------------------
    template<typename Real>
    Vector<Real> Cross(Vector<Real> const & v1, Vector<Real> const & v2)
    {
      Vector<Real> result;
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
    void GetBasis(Vector<Real> const & a_A_in,
      Vector<Real> const & a_B_in,
      Vector<Real>& a_A_out,
      Vector<Real>& a_B_out,
      Vector<Real>& a_C_out)
    {
      if (a_A_in.IsZero())
      {
        a_A_out = Vector<Real>::xAxis();
      }
      else
      {
        a_A_out = a_A_in;
        a_A_out.Normalize();
      }

      a_C_out = Cross(a_A_out, a_B_in);

      //Test to see if A_in and B_in are parallel
      if (IsZero(a_C_out.LengthSquared()))
      {
        a_B_out = Perpendicular(a_A_out);
        a_C_out = Cross(a_A_out, a_B_out);
      }
      else
      {
        a_C_out.Normalize();
        a_B_out = Cross(a_C_out, a_A_out);
      }
    } //End: GetBasis()


      //--------------------------------------------------------------------------------
      //	@	Perpendicular()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Vector<Real> Perpendicular(Vector<Real> const & a_vector)
    {
      Vector<Real> result;

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

      result.Normalize();
      return result;
    }	//End: Perpendicular()
  }
}


#endif