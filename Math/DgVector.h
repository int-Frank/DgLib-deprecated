//! @file Vector.h
//!
//! @author: Frank B. Hart
//! @date 4/10/2015
//!
//! Class declaration: Vector

#ifndef Vector_H
#define Vector_H
 
#include "SimpleRNG.h"
#include "DgMatrix.h"
#include "dgmath.h"

namespace Dg
{
  template<sizeType N, typename Real> class Vector;
  template<typename Real> class Matrix44;
  template<typename Real> class Plane4;
  template<typename Real> class Quaternion;
  template<typename Real> class VQS;

  //! VQS transform
  template<sizeType N, typename Real>
  Vector<N, Real> operator*(Vector<N, Real> const &, VQS<Real> const &);

  template<sizeType N, typename Real>
  Real Dot(Vector<N, Real> const &, Vector<N, Real> const &);

  template<typename Real>
  Vector<3, Real> Cross(Vector<3, Real> const &, Vector<3, Real> const &);

  template<typename Real>
  Vector<4, Real> Cross(Vector<4, Real> const &, Vector<4, Real> const &);

  template<typename Real>
  Vector<3, Real> PerpDot(Vector<2, Real> const &, Vector<2, Real> const &);

  template<typename Real>
  Vector<3, Real> PerpDot(Vector<3, Real> const &, Vector<3, Real> const &);

  //! Returns a random unit vector.
  template<typename Real>
  Vector<4, Real> GetRandomVector();

  //! Creates an orthogonal basis from two input vectors
  template<typename Real>
  void GetBasis(Vector<4, Real> const & a0,
                Vector<4, Real> const & a1,
                Vector<4, Real>& x0,
                Vector<4, Real>& x1,
                Vector<4, Real>& x2);

  //! Returns a perpendicular vector.
  template<sizeType N, typename Real>
  Vector<N, Real> Perpendicular(Vector<N, Real> const & axis, sizeType a_limit = N);

  //! Returns a random orthonormal vector to an axis.
  //! @pre Input must be a unit vector.
  template<typename Real>
  Vector<4, Real> GetRandomOrthonormalVector(Vector<4, Real> const & axis);

  //! Returns a random vector at an angle to an axis.
  //! @pre Input axis must be a unit vector.
  template<typename Real>
  Vector<4, Real> GetRandomVector(Vector<4, Real> const & axis, Real angle);

  //! @ingroup Math_classes
  //!
  //! @class Vector
  //!
  //! @brief Three dimensional homogeneous vector class [x, y, z, w].
  //!
  //! @author Frank Hart
  //! @date 4/10/2015
  template<sizeType N, typename Real>
  class Vector : public Matrix<1, N, Real>
  {
    template<sizeType _N, typename T> friend class Vector;
    friend class Plane4<Real>;
    friend class Matrix44<Real>;
    friend class Quaternion<Real>;
    friend class VQS<Real>;

  public:

    //! Default constructor. Members not initialized.
    Vector() {}

    Vector(std::initializer_list<Real>);
    ~Vector() {}

    // copy operations
    Vector(Matrix<1, N, Real> const & a_other) : Matrix<1, N, Real>(a_other) {}
    Vector& operator=(Matrix<1, 4, Real> const &);

    //! Determines if the vector is the unit vector within some tolerance.
    bool IsUnit() const;

    //! Set elements
    void Set(std::initializer_list<Real>);

    //! Make unit vector
    void Normalize();  

    //! Dot product.
    template<sizeType _N, typename T>
    friend T Dot(Vector<_N, T> const &, Vector<_N, T> const &);

    template<typename _Real>
    friend Vector<3, _Real> Cross(Vector<3, _Real> const &, Vector<3, _Real> const &);

    template<typename _Real>
    friend Vector<4, _Real> Cross(Vector<4, _Real> const &, Vector<4, _Real> const &);

    template<typename _Real>
    friend Real PerpDot(Vector<2, _Real> const &, Vector<2, _Real> const &);

    template<typename _Real>
    friend Real PerpDot(Vector<3, _Real> const &, Vector<3, _Real> const &);

    Real Length() const;
    Real LengthSquared() const;

  };

  //-------------------------------------------------------------------------------
  //	@	Vector::operator=()
  //-------------------------------------------------------------------------------
  template<sizeType N, typename Real>
  Vector<N, Real>& Vector<N, Real>::operator=(Matrix<1, 4, Real> const & a_other)
  {
    Matrix<1, 4, Real>::operator=(a_other);
    return *this;
  }   // End:  Vector::operator=()


  //-------------------------------------------------------------------------------
  //	@	Vector::Vector()
  //-------------------------------------------------------------------------------
  template<sizeType N, typename Real>
  Vector<N, Real>::Vector(std::initializer_list<Real> a_list)
  {
    Set(a_list);
  }   // End:  Vector::Vector()

  //-------------------------------------------------------------------------------
  //	@	Vector::Set()
  //-------------------------------------------------------------------------------
  template<sizeType N, typename Real>
  void Vector<N, Real>::Set(std::initializer_list<Real> a_list)
  {
    auto it = a_list.begin();
    int i = 0;
    for (auto item : a_list)
    {
      if (i < N)
      {
        m_V[i] = item;
      }
      else
      {
        break;
      }
      i++;
    }

    for (; i < N; ++i)
    {
      m_V[i] = static_cast<Real>(0.0);
    }

  }   // End: Vector::Set()

  //-------------------------------------------------------------------------------
  //	@	Vector::Length()
  //-------------------------------------------------------------------------------
  template<sizeType N, typename Real>
  Real Vector<N, Real>::Length() const
  {
    Real temp = static_cast<Real>(0.0);
    for (sizeType i = 0; i < N; ++i)
    {
      temp += (m_V[i] * m_V[i]);
    }

    return sqrt(temp);

  }   // End:  Vector::Length()


  //-------------------------------------------------------------------------------
  //	@	Vector::LengthSquared()
  //-------------------------------------------------------------------------------
  template<sizeType N, typename Real>
  Real Vector<N, Real>::LengthSquared() const
  {
    Real temp = static_cast<Real>(0.0);
    for (sizeType i = 0; i < N; ++i)
    {
      temp += (m_V[i] * m_V[i]);
    }

    return temp;

  }   // End:  Vector::LengthSquared()


  //-------------------------------------------------------------------------------
  //	@	Vector::IsUnit()
  //-------------------------------------------------------------------------------
  template<sizeType N, typename Real>
  bool Vector<N, Real>::IsUnit() const
  {
    return Dg::IsZero(static_cast<Real>(1.0) - LengthSquared());

  }   // End:  Vector::IsUnit()


  //-------------------------------------------------------------------------------
  //	@	Vector::Normalize()
  //-------------------------------------------------------------------------------
  template<sizeType N, typename Real>
  void Vector<N, Real>::Normalize()
  {
    Real lengthsq = LengthSquared();

    if (Dg::IsZero(lengthsq))
    {
      Zero();
      m_V[0] = static_cast<Real>(1.0);
    }
    else
    {
      Real factor = static_cast<Real>(1.0) / sqrt(lengthsq);
      for (sizeType i = 0; i < N; ++i)
      {
        m_V[i] *= factor;
      }
    }

  }   // End:  Vector::Normalize()


  //-------------------------------------------------------------------------------
  //	@	Dot()
  //-------------------------------------------------------------------------------
  template<sizeType N, typename Real>
  Real Dot(Vector<N, Real> const & a_v0, Vector<N, Real> const & a_v1)
  {
    Real temp = static_cast<Real>(0.0);
    for (sizeType i = 0; i < N; ++i)
    {
      temp += (a_v0.m_V[i] * a_v1.m_V[i]);
    }

    return temp;

  }   // End:  Dot()


  //-------------------------------------------------------------------------------
  //	@	Cross()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Vector<4, Real> Cross(Vector<4, Real> const & v1, Vector<4, Real> const & v2)
  {
    Vector<4, Real> result;
    result.m_V[0] = v1.m_V[1] * v2.m_V[2] - v1.m_V[2] * v2.m_V[1];
    result.m_V[1] = v1.m_V[2] * v2.m_V[0] - v1.m_V[0] * v2.m_V[2];
    result.m_V[2] = v1.m_V[0] * v2.m_V[1] - v1.m_V[1] * v2.m_V[0];
    result.m_V[3] = static_cast<Real>(0.0);

    return result;

  }	//End: Cross()


  //-------------------------------------------------------------------------------
  //	@	Cross()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Vector<3, Real> Cross(Vector<3, Real> const & v1, Vector<3, Real> const & v2)
  {
    Vector<3, Real> result;
    result.m_V[0] = v1.m_V[1] * v2.m_V[2] - v1.m_V[2] * v2.m_V[1];
    result.m_V[1] = v1.m_V[2] * v2.m_V[0] - v1.m_V[0] * v2.m_V[2];
    result.m_V[2] = v1.m_V[0] * v2.m_V[1] - v1.m_V[1] * v2.m_V[0];

    return result;

  }	//End: Cross()


  //-------------------------------------------------------------------------------
  //	@	PerpDot()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Real PerpDot(Vector<3, Real> const & v1, Vector<3, Real> const & v2)
  {
    return = v1.m_V[0] * v2.m_V[1] - v1.m_V[1] * v2.m_V[0];

  }	//End: PerpDot()


  //-------------------------------------------------------------------------------
  //	@	PerpDot()
  //--------------------------------------------------------------------------------
  template<typename Real>
  Real PerpDot(Vector<2, Real> const & v1, Vector<2, Real> const & v2)
  {
    return = v1.m_V[0] * v2.m_V[1] - v1.m_V[1] * v2.m_V[0];

  }	//End: PerpDot()


  //--------------------------------------------------------------------------------
  //	@	GetBasis()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void GetBasis(Vector<4, Real> const & a_a0,
                Vector<4, Real> const & a_a1,
                Vector<4, Real>& a_x0,
                Vector<4, Real>& a_x1,
                Vector<4, Real>& a_x2)
  {
    bool is_a1_zero = a_a1.IsZero();

    //Check for zero vectors, handle separately
    if (a_a0.IsZero())
    {
      //Both x0, x1 are zero vectors
      if (is_a1_zero)
      {
        a_x0[0] = a_x1[1] = a_x2[2] = static_cast<Real>(1.0);
        a_x0[1] = a_x0[2] = a_x0[3] =
          a_x1[0] = a_x1[2] = a_x1[3] =
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

      return;
    }

    //Normalize x2
    a_x2.Normalize();

    //Calculate x1
    a_x1 = Cross(a_x2, a_x0);

  } //End: GetBasis()


  //--------------------------------------------------------------------------------
  //	@	Perpendicular()
  //--------------------------------------------------------------------------------
  template<sizeType N, typename Real>
  Vector<N, Real> Perpendicular(Vector<N, Real> const & a_vector, sizeType a_limit)
  {
    static_assert(N > 1, "Cannot find a perpendicular vector of a 1-vector");

    Vector<N, Real> result;
    result.Zero();

    if (!Dg::IsZero(a_vector[0]))
    {
      result[0] = -a_vector[1];
      result[1] = a_vector[0];
      return result;
    }

    for (sizeType i = 1; i < ((a_limit > N) ? N : a_limit); ++i)
    {
      if (!Dg::IsZero(a_vector[i]))
      {
        result[i - 1] = -a_vector[i];
        result[i] = a_vector[i - 1];
      }
    }
    return result;
  }	//End: Perpendicular()


  //-------------------------------------------------------------------------------
  //		@	GetRandomVector()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector<4, Real> GetRandomVector()
  {
    SimpleRNG generator;

    Real theta = generator.GetUniform<Real>(static_cast<Real>(0.0), static_cast<Real>(2.0) * static_cast<Real>(Dg::PI_d));
    Real phi = generator.GetUniform<Real>(static_cast<Real>(0.0), static_cast<Real>(Dg::PI_d));

    Real sinTheta = sin(theta);

    Real x = sinTheta * cos(phi);
    Real y = sinTheta * sin(phi);
    Real z = cos(theta);

    return Vector<4, Real>({ x, y, z, static_cast<Real>(0.0) });
  }	//End: GetRandomVector()


  //-------------------------------------------------------------------------------
  //		@ GetRandomOrthonormalVector()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector<4, Real> GetRandomOrthonormalVector(Vector<4, Real> const & a_axis)
  {
    SimpleRNG generator;

    //Get a random unit vector
    Vector<4, Real> v(GetRandomVector<Real>());

    //Find the cross product, to find random orthogonal vector to the axis
    Vector<4, Real> o(Cross(a_axis, v));

    if (o.IsZero())
      o = Perpendicular(a_axis);

    o.Normalize();

    return o;
  }	//End: GetRandomOrthonormalVector()


  //-------------------------------------------------------------------------------
  //		@ GetRandomVector()
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector<4, Real> GetRandomVector(Vector<4, Real> const & a_axis, Real theta)
  {
    SimpleRNG generator;

    //Find random angle [0, theta]
    Real phi = generator.GetUniform<Real>(static_cast<Real>(0.0), theta);

    return (cos(phi) * a_axis + sin(phi) * GetRandomOrthonormalVector(a_axis));
  }	//End: GetRandomVector()

}


#endif