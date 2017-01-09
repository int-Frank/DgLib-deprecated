//! @file DgR2Matrix.h
//!
//! @author: James M. Van Verth, Lars M. Bishop, Frank B. Hart
//! @date 4/10/2015
//!
//! Class declaration: Matrix

#ifndef DGR2Matrix33_H
#define DGR2Matrix33_H

#include "DgMath.h"
#include "DgMatrix.h"
#include "DgR2Vector.h"

//--------------------------------------------------------------------------------
//	@	Matrix
//--------------------------------------------------------------------------------
namespace Dg
{
  namespace R2
  {
    template<typename Real> class Matrix;

    //! Set self to matrix inverse, assuming a standard affine matrix (bottom row is 0 0 0 1).
    template<typename Real>
    Matrix<Real> AffineInverse(Matrix<Real> const &);

    //! @ingroup DgMath_types
    //!
    //! @class Matrix
    //!
    //! @brief 4 by 4 matrix calss.
    //!
    //! Originally retrieved From: Essential Mathematics for Games and Interactive Applications SE
    //! On Date: 2013
    //!
    //!     Memory layout: { x.x, x.y, 0, y.x, y.y 0, p.x, p.y, 1 }
    //!     Row major: [x.x x.y 0]
    //!                [y.x y.y 0]
    //!                [p.x p.y 1]
    //!
    //! Matrix concatenation uses left-to-right convention. For example the follow lines are equivalent
    //!
    //!     m_final = m_0 * m_1 * m_2 ... * m_n;
    //!     m_final = ( ... ((m_0 * m_1) * m_2) ... * m_n);
    //!
    //! @author James M. Van Verth, Lars M. Bishop, modified by Frank B. Hart
    //! @date 4/10/2015
    template<typename Real>
    class Matrix : public Dg::Matrix < 3, 3, Real >
    {
    public:
      //! Default constructor, initialized to identity matrix.
      Matrix() { Identity(); }
      ~Matrix() {}

      //! Copy Constructor
      Matrix(Dg::Matrix < 3, 3, Real > const & a_other) : Dg::Matrix<3, 3, Real>(a_other) {}

      //! Assignment
      Matrix& operator=(Dg::Matrix < 3, 3, Real > const &);

      //! Set matrix by rows
      void SetRows(Dg::Matrix<1, 3, Real> const & row0, Dg::Matrix<1, 3, Real> const & row1,
        Dg::Matrix<1, 3, Real> const & row2);

      //! Get rows of the matrix
      void GetRows(Dg::Matrix<1, 3, Real>& row0, Dg::Matrix<1, 3, Real>& row1,
        Dg::Matrix<1, 3, Real>& row2) const;

      //! Set matrix by columns
      void SetColumns(Dg::Matrix<3, 1, Real> const & col0, Dg::Matrix<3, 1, Real> const & col1,
        Dg::Matrix<3, 1, Real> const & col2);

      //! Get matrix columns
      void GetColumns(Dg::Matrix<3, 1, Real>& col0, Dg::Matrix<3, 1, Real>& col1,
        Dg::Matrix<3, 1, Real>& col2) const;

      //! Set self to matrix inverse, assuming a standard affine matrix (bottom row is 0 0 0 1).
      Matrix& AffineInverse();

      //! Set self to matrix inverse, assuming a standard affine matrix (bottom row is 0 0 0 1).
      friend Matrix Dg::R2::AffineInverse(Matrix<Real> const &);

      //! Set as translation matrix based on vector
      Matrix& Translation(Dg::Matrix<1, 3, Real> const &);

      //! Sets the matrix to a rotation matrix.
      Matrix& Rotation(Real);

      //! Set as scaling matrix based on vector.
      Matrix& Scaling(Dg::Matrix<1, 3, Real> const &);

      //! Uniform scaling.
      Matrix& Scaling(Real);
    };

    //--------------------------------------------------------------------------------
    //	@	Matrix::operator=()
    //--------------------------------------------------------------------------------
    template<typename Real>
    Matrix<Real>& Matrix<Real>::operator=(Dg::Matrix < 3, 3, Real > const & a_other)
    {
      Dg::Matrix<3, 3, Real>::operator=(a_other);
      return *this;
    }

    //--------------------------------------------------------------------------------
    //	@	Matrix::SetRows()
    //--------------------------------------------------------------------------------
    template<typename Real>
    void Matrix<Real>::SetRows(Dg::Matrix<1, 3, Real> const & a_row0, Dg::Matrix<1, 3, Real> const & a_row1,
      Dg::Matrix<1, 3, Real> const & a_row2)
    {
      SetRow(0, a_row0);
      SetRow(1, a_row1);
      SetRow(2, a_row2);

    }	//End: Matrix::SetRows()


      //--------------------------------------------------------------------------------
      //	@	Matrix::GetRows()
      //--------------------------------------------------------------------------------
    template<typename Real>
    void Matrix<Real>::GetRows(Dg::Matrix<1, 3, Real>& a_row0, Dg::Matrix<1, 3, Real>& a_row1,
      Dg::Matrix<1, 3, Real>& a_row2) const
    {
      GetRow(0, a_row0);
      GetRow(1, a_row1);
      GetRow(2, a_row2);
    }	//End: Matrix::GetRows()


      //--------------------------------------------------------------------------------
      //	@	Matrix::SetColumns()
      //--------------------------------------------------------------------------------
    template<typename Real>
    void Matrix<Real>::SetColumns(Dg::Matrix<3, 1, Real> const & a_col0, Dg::Matrix<3, 1, Real> const & a_col1,
      Dg::Matrix<3, 1, Real> const & a_col2)
    {
      SetColumn(0, a_col0);
      SetColumn(1, a_col1);
      SetColumn(2, a_col2);
    }	//End: Matrix::SetColumns()


      //--------------------------------------------------------------------------------
      //	@	Matrix::GetColumns()
      //--------------------------------------------------------------------------------
    template<typename Real>
    void Matrix<Real>::GetColumns(Dg::Matrix<3, 1, Real>& a_col0, Dg::Matrix<3, 1, Real>& a_col1,
      Dg::Matrix<3, 1, Real>& a_col2) const
    {
      GetColumn(0, a_col0);
      GetColumn(1, a_col1);
      GetColumn(2, a_col2);
    }	//End: Matrix::GetColumns()


      //--------------------------------------------------------------------------------
      //	@	Matrix::AffineInverse()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Matrix<Real>& Matrix<Real>::AffineInverse()
    {
      *this = Dg::AffineInverse(*this);

      return *this;

    }	//End: Matrix::AffineInverse


      //--------------------------------------------------------------------------------
      //	@	AffineInverse()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Matrix<Real> AffineInverse(Matrix<Real> const & a_mat)
    {
      Matrix<Real> result;

      //compute upper left 3x3 matrix determinant
      Real cofactor0 = a_mat.m_V[4] * a_mat.m_V[8] - a_mat.m_V[7] * a_mat.m_V[5];
      Real cofactor1 = a_mat.m_V[5] * a_mat.m_V[6] - a_mat.m_V[3] * a_mat.m_V[8];
      Real cofactor2 = a_mat.m_V[3] * a_mat.m_V[7] - a_mat.m_V[6] * a_mat.m_V[4];
      Real det = a_mat.m_V[0] * cofactor0 + a_mat.m_V[1] * cofactor1 + a_mat.m_V[2] * cofactor2;
      if (Dg::IsZero(det))
      {
        return result;
      }

      // create adjunct matrix and multiply by 1/det to get upper 3x3
      Real invDet = static_cast<Real>(1.0) / det;
      result.m_V[0] = invDet*cofactor0;
      result.m_V[3] = invDet*cofactor1;
      result.m_V[6] = invDet*cofactor2;

      result.m_V[1] = invDet*(a_mat.m_V[2] * a_mat.m_V[7] - a_mat.m_V[1] * a_mat.m_V[8]);
      result.m_V[4] = invDet*(a_mat.m_V[0] * a_mat.m_V[8] - a_mat.m_V[2] * a_mat.m_V[6]);
      result.m_V[7] = invDet*(a_mat.m_V[1] * a_mat.m_V[6] - a_mat.m_V[0] * a_mat.m_V[7]);

      result.m_V[2] = invDet*(a_mat.m_V[1] * a_mat.m_V[5] - a_mat.m_V[2] * a_mat.m_V[4]);
      result.m_V[5] = invDet*(a_mat.m_V[2] * a_mat.m_V[3] - a_mat.m_V[0] * a_mat.m_V[5]);
      result.m_V[8] = invDet*(a_mat.m_V[0] * a_mat.m_V[4] - a_mat.m_V[1] * a_mat.m_V[3]);

      return result;

    }	//End: ::AffineInverse()


      //-------------------------------------------------------------------------------
      //	@	Matrix::Translation()
      //-------------------------------------------------------------------------------
    template<typename Real>
    Matrix<Real>& Matrix<Real>::Translation(Dg::Matrix<1, 3, Real> const & a_xlate)
    {
      m_V[0] = static_cast<Real>(1.0);
      m_V[1] = static_cast<Real>(0.0);
      m_V[2] = static_cast<Real>(0.0);

      m_V[3] = static_cast<Real>(0.0);
      m_V[4] = static_cast<Real>(1.0);
      m_V[5] = static_cast<Real>(0.0);

      m_V[6] = a_xlate[0];
      m_V[7] = a_xlate[1];
      m_V[8] = static_cast<Real>(1.0);

      return *this;

    }   // End: Matrix::Translation()


        //-------------------------------------------------------------------------------
        //	@	Matrix::Scaling()
        //-------------------------------------------------------------------------------
    template<typename Real>
    Matrix<Real>& Matrix<Real>::Scaling(Dg::Matrix<1, 3, Real> const & a_scaleFactors)
    {
      m_V[0] = a_scaleFactors.m_V[0];
      m_V[1] = static_cast<Real>(0.0);
      m_V[2] = static_cast<Real>(0.0);

      m_V[3] = static_cast<Real>(0.0);
      m_V[4] = a_scaleFactors.m_V[1];
      m_V[5] = static_cast<Real>(0.0);

      m_V[6] = static_cast<Real>(0.0);
      m_V[7] = static_cast<Real>(0.0);
      m_V[8] = static_cast<Real>(1.0);

      return *this;

    }   // End: Matrix::Scaling()


        //-------------------------------------------------------------------------------
        //	@	Matrix::Scaling()
        //-------------------------------------------------------------------------------
    template<typename Real>
    Matrix<Real>& Matrix<Real>::Scaling(Real a_val)
    {
      m_V[0] = a_val;
      m_V[1] = static_cast<Real>(0.0);
      m_V[2] = static_cast<Real>(0.0);

      m_V[3] = static_cast<Real>(0.0);
      m_V[4] = a_val;
      m_V[5] = static_cast<Real>(0.0);

      m_V[6] = static_cast<Real>(0.0);
      m_V[7] = static_cast<Real>(0.0);
      m_V[8] = static_cast<Real>(1.0);

      return *this;

    }   // End: Matrix::Scaling()


        //-------------------------------------------------------------------------------
        //	@	Matrix::Rotation()
        //-------------------------------------------------------------------------------
    template<typename Real>
    Matrix<Real>& Matrix<Real>::Rotation(Real a_angle)
    {
      Real sintheta = Real(sin(a_angle));
      Real costheta = Real(cos(a_angle));

      m_V[0] = costheta;
      m_V[1] = sintheta;
      m_V[2] = static_cast<Real>(0.0);

      m_V[3] = -sintheta;
      m_V[4] = costheta;
      m_V[5] = static_cast<Real>(0.0);

      m_V[6] = static_cast<Real>(0.0);
      m_V[7] = static_cast<Real>(0.0);
      m_V[8] = static_cast<Real>(1.0);

      return *this;

    }   // End: Matrix::Rotation()
  }
}
#endif