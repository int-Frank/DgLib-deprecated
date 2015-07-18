//! @file Matrix.h
//!
//! @author: Frank B. Hart
//! @date 4/10/2015
//!
//! Class declaration: Matrix

#ifndef DGMATRIX_H
#define DGMATRIX_H

#include "dgmath.h"

typedef size_t sizeType;

namespace Dg
{
  template<sizeType M, sizeType N, typename Real> class Matrix;

  template<sizeType M, sizeType N, typename Real>
  Matrix<N, M, Real> Transpose(Matrix<M, N, Real> const &);

  template<sizeType M, sizeType N, typename Real>
  Real Dot(Matrix<M, N, Real> const &, Matrix<M, N, Real> const &);

  template<sizeType M, sizeType N, typename Real>
  Matrix<M, N, Real> operator*(Real, Matrix<M, N, Real> const &);

  //! @ingroup Math_classes
  //!
  //! @class Matrix
  //!
  //! @brief Generic M * N matrix class.
  //!
  //! @author Frank B. Hart
  //! @date 4/10/2015
  template<sizeType M, sizeType N, typename Real>
  class Matrix
  {
    static_assert(M > 0 && N > 0, "Matrix cannot have a zero dimension.");

    template<sizeType _M, sizeType _N, typename T> friend class Matrix;

  public:
    //! Default constructor, initialized to identity matrix.
    Matrix();
    ~Matrix() {}

    Matrix(Matrix const &);
    Matrix& operator=(Matrix const &);

    //! Accessor i: row, j:column.
    Real& operator()(sizeType m, sizeType n);

    //! Accessor i: row, j:column.
    Real operator()(sizeType m, sizeType n) const;

    //! Accessor element by index.
    Real& operator[](sizeType i) { return m_V[i]; }
    Real operator[](sizeType i) const { return m_V[i]; }

    bool operator== (Matrix const &) const;
    bool operator!= (Matrix const &) const;

    //! Checks if all elements are below the tolerance.
    bool IsZero() const;

    //! Set all elements to zero.
    void Zero();

    //! Checks if matrix is equal to the identity, within some tolerance.
    bool IsIdentity() const;

    //! Directly set the elements.
    void Set(Real const [M * N]);

    void SetRow(sizeType m, Matrix<1, N, Real> const &);
    void GetRow(sizeType m, Matrix<1, N, Real>&) const;
    void SetColumn(sizeType n, Matrix<M, 1, Real> const &);
    void GetColumn(sizeType n, Matrix<M, 1, Real>&) const;

    //! Sets near-zero elements to 0.
    void Clean();

    //! Set to identity matrix. For aquare martices only.
    void Identity();

    //! Compute the transpose matrix.
    template<sizeType _M, sizeType _N, typename _Real>
    friend Matrix<_N, _M, _Real> Transpose(Matrix<_M, _N, _Real> const &);

    //! Set matrix to its transpose. For square matrices only.
    Matrix& Transpose();

    Matrix operator+ (Matrix const &) const;
    Matrix& operator+= (Matrix const &);
    Matrix operator- (Matrix const &) const;
    Matrix& operator-= (Matrix const &);

    //! Negate matrix.
    Matrix operator-() const;

    Matrix& operator*= (Matrix const &);
    Real Dot(Matrix const &, Matrix const &);

    template<sizeType _M>
    Matrix<M, _M, Real> operator* (Matrix<N, _M, Real> const &) const;

    Matrix& operator*= (Real);
    Matrix operator* (Real) const;
    Matrix& operator/= (Real);
    Matrix operator/ (Real) const;

    template<sizeType _M, sizeType _N, typename _Real>
    friend Matrix<_M, _N, _Real> operator* (_Real, Matrix<_M, _N, _Real> const &);

  protected:

    //Data Members
    Real m_V[M * N];

  };

  //--------------------------------------------------------------------------------
  //	@	Matrix::Matrix()
  //--------------------------------------------------------------------------------
  template<sizeType M, sizeType N, typename Real>
  Matrix<M, N, Real>::Matrix()
  {
  }	//End: Matrix::Matrix()


  //--------------------------------------------------------------------------------
  //	@	Matrix::operator()
  //--------------------------------------------------------------------------------
  template<sizeType M, sizeType N, typename Real>
  Real& Matrix<M, N, Real>::operator()(sizeType a_m, sizeType a_n)
  {
    return m_V[a_m * N + a_n];

  }	//End: Matrix::operator()


  //--------------------------------------------------------------------------------
  //	@	Matrix::operator()
  //--------------------------------------------------------------------------------
  template<sizeType M, sizeType N, typename Real>
  Real Matrix<M, N, Real>::operator()(sizeType a_m, sizeType a_n) const
  {
    return m_V[a_m * N + a_n];

  }	//End: Matrix::operator()


  //--------------------------------------------------------------------------------
  //	@	Matrix::operator=()
  //--------------------------------------------------------------------------------
  template<sizeType M, sizeType N, typename Real>
  Matrix<M, N, Real>& Matrix<M, N, Real>::operator= (Matrix<M, N, Real>const & a_other)
  {
    for (size_t i = 0; i < M * N; ++i)
    {
      m_V[i] = a_other.m_V[i];
    }

    return *this;

  }	//End: Matrix::operator=()


  //--------------------------------------------------------------------------------
  //	@	Matrix::Matrix()
  //--------------------------------------------------------------------------------
  template<sizeType M, sizeType N, typename Real>
  Matrix<M, N, Real>::Matrix(Matrix<M, N, Real> const & a_other)
  {
    for (size_t i = 0; i < M * N; ++i)
    {
      m_V[i] = a_other.m_V[i];
    }

  }	//End: Matrix::Matrix()


  //--------------------------------------------------------------------------------
  //	@	Matrix::operator==()
  //--------------------------------------------------------------------------------
  template<sizeType M, sizeType N, typename Real>
  bool Matrix<M, N, Real>::operator== (Matrix<M, N, Real> const & a_other) const
  {
    for (sizeType i = 0; i < M * N; i++)
    {
      if (!Dg::AreEqual(m_V[i], a_other.m_V[i]))
        return false;
    }

    return true;

  }	//End: Matrix::operator==()


  //--------------------------------------------------------------------------------
  //	@	Matrix::operator!=()
  //--------------------------------------------------------------------------------
  template<sizeType M, sizeType N, typename Real>
  bool Matrix<M, N, Real>::operator!= (Matrix<M, N, Real> const & a_other) const
  {
    for (sizeType i = 0; i < M * N; i++)
    {
      if (!Dg::AreEqual(m_V[i], a_other.m_V[i]))
        return true;
    }

    return false;

  }	//End: Matrix::operator!=()


  //--------------------------------------------------------------------------------
  //	@	Matrix44Dg::IsZero()
  //--------------------------------------------------------------------------------
  template<sizeType M, sizeType N, typename Real>
  void Matrix<M, N, Real>::Zero()
  {
    for (sizeType i = 0; i < M * N; i++)
    {
      m_V[i] = static_cast<Real>(0.0);
    }

  }	//End: Matrix::Zero()


  //--------------------------------------------------------------------------------
  //	@	Matrix44Dg::IsZero()
  //--------------------------------------------------------------------------------
  template<sizeType M, sizeType N, typename Real>
  bool Matrix<M, N, Real>::IsZero() const
  {
    for (sizeType i = 0; i < M * N; i++)
    {
      if (!Dg::IsZero(m_V[i]))
        return false;
    }

    return true;

  }	//End: Matrix::IsZero()


  //--------------------------------------------------------------------------------
  //	@	Matrix::IsIdentity()
  //--------------------------------------------------------------------------------
  template<sizeType M, sizeType N, typename Real>
  bool Matrix<M, N, Real>::IsIdentity() const
  {
    static_assert(M == N, "Only square matrices can be identity");

    for (sizeType m = 0; m < M - 1; ++m)
    {
      for (sizeType n = m + 1; n < N; ++n)
      {
        if (!Dg::IsZero(m_V[m * N + n]) || !Dg::IsZero(m_V[n * M + m]))
        {
          return false;
        }
      }
    }

    for (sizeType i = 0; i < M * N; i += (N + 1))
    {
      if (!Dg::IsZero(static_cast<Real>(1.0) - m_V[i]))
      {
        return false;
      }
    }

    return true;

  }	//End: Matrix::IsIdentity()


  //--------------------------------------------------------------------------------
  //	@	Matrix::Set()
  //--------------------------------------------------------------------------------
  template<sizeType M, sizeType N, typename Real>
  void Matrix<M, N, Real>::Set(Real const a_data[M * N])
  {
    for (size_t i = 0; i < M * N; ++i)
    {
      m_V[i] = a_data[i];
    }
  }// End: Matrix<M, N, Real>::Set()


  //--------------------------------------------------------------------------------
  //	@	Matrix::SetRow()
  //--------------------------------------------------------------------------------
  template<sizeType M, sizeType N, typename Real>
  void Matrix<M, N, Real>::SetRow(sizeType a_m, Matrix<1, N, Real> const & a_row)
  {
    a_m *= N;

    for (sizeType i = 0; i < N; ++i)
    {
      m_V[a_m +i] = a_row.m_V[i];
    }

  }	//End: Matrix::SetRow()


  //--------------------------------------------------------------------------------
  //	@	Matrix::GetRow()
  //--------------------------------------------------------------------------------
  template<sizeType M, sizeType N, typename Real>
  void Matrix<M, N, Real>::GetRow(sizeType a_m, Matrix<1, N, Real>& a_out) const
  {
    a_m *= N;

    for (sizeType i = 0; i < N; ++i)
    {
      a_out.m_V[i] = m_V[a_m + i];
    }

  }	//End: Matrix::GetRow()


  //--------------------------------------------------------------------------------
  //	@	Matrix::SetColumn()
  //--------------------------------------------------------------------------------
  template<sizeType M, sizeType N, typename Real>
  void Matrix<M, N, Real>::SetColumn(sizeType a_n, Matrix<M, 1, Real> const & a_col)
  {
    for (sizeType i = 0; i < M; ++i)
    {
      m_V[i * N + a_n] = a_col.m_V[i];
    }

  }	//End: Matrix::SetColumn()


  //--------------------------------------------------------------------------------
  //	@	Matrix::GetColumn()
  //--------------------------------------------------------------------------------
  template<sizeType M, sizeType N, typename Real>
  void Matrix<M, N, Real>::GetColumn(sizeType a_n, Matrix<M, 1, Real>& a_out) const
  {
    for (sizeType i = 0; i < M; ++i)
    {
      a_out.m_V[i] = m_V[i * N + a_n];
    }

  }	//End: Matrix::GetColumn()


  //--------------------------------------------------------------------------------
  //	@	Matrix::Clean()
  //--------------------------------------------------------------------------------
  template<sizeType M, sizeType N, typename Real>
  void Matrix<M, N, Real>::Clean()
  {
    for (sizeType i = 0; i < M * N; i++)
    {
      if (Dg::IsZero(m_V[i]))
        m_V[i] = static_cast<Real>(0.0);
    }

  }	//End: Matrix::Clean()


  //--------------------------------------------------------------------------------
  //	@	Matrix::Indentity()
  //--------------------------------------------------------------------------------
  template<sizeType M, sizeType N, typename Real>
  void Matrix<M, N, Real>::Identity()
  {
    static_assert(M == N, "Only square matrices can be identity");

    for (sizeType m = 0; m < M - 1; ++m)
    {
      for (sizeType n = m + 1; n < N; ++n)
      {
        m_V[m * N + n] = m_V[n * M + m] = static_cast<Real>(0.0);
      }
    }

    for (sizeType i = 0; i < M * N; i += (N + 1))
    {
      m_V[i] = static_cast<Real>(1.0);
    }

  }	//End: Matrix::Identity()


  //--------------------------------------------------------------------------------
  //	@	Transpose()
  //-------------------------------------------------------------------------------
  template<sizeType M, sizeType N, typename Real>
  Matrix<N, M, Real> Transpose(Matrix<M, N, Real> const & a_mat)
  {
    Matrix<N, M, Real> result;

    for (sizeType m = 0; m < M; ++m)
    {
      for (sizeType n = 0; n < N; ++n)
      {
        result.m_V[n * M + m] = a_mat.m_V[m * N + n];
      }
    }

    return result;

  }   // End: Transpose()


  //--------------------------------------------------------------------------------
  //	@	Transpose()
  //-------------------------------------------------------------------------------
  template<sizeType M, sizeType N, typename Real>
  Matrix<M, N, Real>& Matrix<M, N, Real>::Transpose()
  {
    static_assert(M == N, "Can only transpose a square matrix");

    for (sizeType m = 0; m < M - 1; ++m)
    {
      for (sizeType n = m + 1; n < N; ++n)
      {
        Real temp;
        temp = m_V[m * N + n];
        m_V[m * N + n] = m_V[n * N + m];
        m_V[n * N + m] = temp;
      }
    }

    return *this;

  }   // End: Transpose()


  //-------------------------------------------------------------------------------
  //	@	Matrix::operator+()
  //-------------------------------------------------------------------------------
  template<sizeType M, sizeType N, typename Real>
  Matrix<M, N, Real> Matrix<M, N, Real>::operator+(Matrix<M, N, Real> const & a_other) const
  {
    Matrix<M, N, Real> result;

    for (sizeType i = 0; i < M * N; ++i)
    {
      result.m_V[i] = m_V[i] + a_other.m_V[i];
    }

    return result;

  }   // End: Matrix::operator+()


  //-------------------------------------------------------------------------------
  //	@	Matrix::operator+=()
  //-------------------------------------------------------------------------------
  template<sizeType M, sizeType N, typename Real>
  Matrix<M, N, Real>& Matrix<M, N, Real>::operator+=(Matrix<M, N, Real> const & a_other)
  {
    for (sizeType i = 0; i < M * N; ++i)
    {
      m_V[i] += a_other.m_V[i];
    }

    return *this;

  }   // End: Matrix::operator+=()


  //-------------------------------------------------------------------------------
  //	@	Matrix::operator-()
  //-------------------------------------------------------------------------------
  template<sizeType M, sizeType N, typename Real>
  Matrix<M, N, Real> Matrix<M, N, Real>::operator-(Matrix<M, N, Real> const & other) const
  {
    Matrix<M, N, Real> result;

    for (sizeType i = 0; i < M * N; ++i)
    {
      result.m_V[i] = m_V[i] - other.m_V[i];
    }

    return result;

  }   // End: Matrix::operator-()


  //-------------------------------------------------------------------------------
  //	@	Matrix::operator-=()
  //-------------------------------------------------------------------------------
  template<sizeType M, sizeType N, typename Real>
  Matrix<M, N, Real>& Matrix<M, N, Real>::operator-=(Matrix<M, N, Real> const & a_other)
  {
    for (sizeType i = 0; i < M * N; ++i)
    {
      m_V[i] -= a_other.m_V[i];
    }

    return *this;

  }   // End: Matrix::operator-=()


  //-------------------------------------------------------------------------------
  //	@	Matrix::operator-()
  //-------------------------------------------------------------------------------
  template<sizeType M, sizeType N, typename Real>
  Matrix<M, N, Real> Matrix<M, N, Real>::operator-() const
  {
    Matrix<M, N, Real> result;

    for (sizeType i = 0; i < M * N; ++i)
    {
      result.m_V[i] = -m_V[i];
    }

    return result;

  }    // End: Matrix::operator-()


  //-------------------------------------------------------------------------------
  //	@	Matrix::operator*()
  //-------------------------------------------------------------------------------
  template<sizeType M, sizeType N, typename Real>
  template<sizeType _M>
  Matrix<M, _M, Real> Matrix<M, N, Real>::operator*(Matrix<N, _M, Real> const & a_other) const
  {
    Matrix<M, _M, Real> result;
    result.Zero();

    for (sizeType m = 0; m < M; ++m)
    {
      for (sizeType _m = 0; _m < _M; ++_m)
      {
        for (sizeType n = 0; n < N; ++n)
        {
          result.m_V[m * _M + _m] += (m_V[m * N + n] * a_other.m_V[n * _M + _m]);
        }
      }
    }

    return result;

  }   // End: Matrix::operator*()

  //-------------------------------------------------------------------------------
  //	@	Matrix::operator*()
  //-------------------------------------------------------------------------------
  template<sizeType M, sizeType N, typename Real>
  Matrix<M, N, Real>& Matrix<M, N, Real>::operator*=(Matrix<M, N, Real> const & a_other)
  {
    static_assert(M == N, "Can only assign to self if a square matrix");

    Matrix<M, N, Real> result;
    result.Zero();

    for (sizeType m = 0; m < M; ++m)
    {
      for (sizeType n = 0; n < N; ++n)
      {
        for (sizeType i = 0; i < N; ++i)
        {
          result.m_V[m * N + n] += (m_V[m * N + i] * a_other.m_V[i * N + n]);
        }
      }
    }

    Set(result.m_V);

    return *this;

  }   // End: Matrix::operator*()


  //-------------------------------------------------------------------------------
  //	@	Matrix::Dot()
  //-------------------------------------------------------------------------------
  template<sizeType M, sizeType N, typename Real>
  Real Dot(Matrix<M, N, Real> const & a_m0,
           Matrix<M, N, Real> const & a_m1)
  {
    Real result = static_cast<Real>(0.0);
    for (sizeType i = 0; i < N * M; ++i)
    {
      result += a_m0.m_V[i] * a_m1.m_V[i];
    }

    return result;

  }   // End: Matrix::operator*()


  //-------------------------------------------------------------------------------
  //	@	Matrix::operator*=()
  //-------------------------------------------------------------------------------
  template<sizeType M, sizeType N, typename Real>
  Matrix<M, N, Real>& Matrix<M, N, Real>::operator*=(Real a_scalar)
  {
    for (sizeType i = 0; i < M * N; ++i)
    {
      m_V[i] *= a_scalar;
    }

    return *this;

  }  // End: Matrix::operator*=()


  //-------------------------------------------------------------------------------
  //	@	Matrix::operator*()
  //-------------------------------------------------------------------------------
  template<sizeType M, sizeType N, typename Real>
  Matrix<M, N, Real> operator*(Real a_scalar, Matrix<M, N, Real> const & matrix)
  {
    Matrix<M, N, Real> result;

    for (sizeType i = 0; i < M * N; ++i)
    {
      result.m_V[i] = matrix.m_V[i] * a_scalar;
    }

    return result;

  }  // End: operator*()


  //-------------------------------------------------------------------------------
  //	@	Matrix::operator*()
  //-------------------------------------------------------------------------------
  template<sizeType M, sizeType N, typename Real>
  Matrix<M, N, Real> Matrix<M, N, Real>::operator*(Real a_scalar) const
  {
    Matrix<M, N, Real> result;

    for (sizeType i = 0; i < M * N; ++i)
    {
      result.m_V[i] = m_V[i] * a_scalar;
    }

    return result;

  }  // End: Matrix::operator*()


  //-------------------------------------------------------------------------------
  //	@	Matrix::operator/()
  //-------------------------------------------------------------------------------
  template<sizeType M, sizeType N, typename Real>
  Matrix<M, N, Real> Matrix<M, N, Real>::operator/(Real a_scalar) const
  {
    Matrix<M, N, Real> result;

    for (sizeType i = 0; i < M * N; ++i)
    {
      result.m_V[i] = m_V[i] / a_scalar;
    }

    return result;

  }  // End: Matrix::operator/()


  //-------------------------------------------------------------------------------
  //	@	Matrix::operator/=()
  //-------------------------------------------------------------------------------
  template<sizeType M, sizeType N, typename Real>
  Matrix<M, N, Real>& Matrix<M, N, Real>::operator/=(Real a_scalar)
  {
    for (sizeType i = 0; i < M * N; ++i)
    {
      m_V[i] /= a_scalar;
    }

    return *this;

  }  // End: Matrix::operator/=()

}


#endif