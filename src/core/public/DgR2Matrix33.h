//! @file DgR2Matrix33.h
//!
//! @author: James M. Van Verth, Lars M. Bishop, Frank B. Hart
//! @date 4/10/2015
//!
//! Class declaration: Matrix33

#ifndef DGR2Matrix33_H
#define DGR2Matrix33_H

#include "DgMath.h"
#include "DgMatrix.h"
#include "DgR2Vector3.h"

//--------------------------------------------------------------------------------
//	@	Matrix33
//--------------------------------------------------------------------------------
namespace Dg
{
  namespace R2
  {
    template<typename Real> class Matrix33;

    //! Set self to matrix inverse, assuming a standard affine matrix (bottom row is 0 0 0 1).
    template<typename Real>
    Matrix33<Real> AffineInverse(Matrix33<Real> const &);

    //! @ingroup DgMath_types
    //!
    //! @class Matrix33
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
    class Matrix33 : public Matrix < 3, 3, Real >
    {
    public:
      //! Default constructor, initialized to identity matrix.
      Matrix33() { Identity(); }
      ~Matrix33() {}

      //! Copy Constructor
      Matrix33(Matrix < 3, 3, Real > const & a_other) : Matrix<3, 3, Real>(a_other) {}

      //! Assignment
      Matrix33& operator=(Matrix < 3, 3, Real > const &);

      //! Set matrix by rows
      void SetRows(Matrix<1, 3, Real> const & row0, Matrix<1, 3, Real> const & row1,
        Matrix<1, 3, Real> const & row2);

      //! Get rows of the matrix
      void GetRows(Matrix<1, 3, Real>& row0, Matrix<1, 3, Real>& row1,
        Matrix<1, 3, Real>& row2) const;

      //! Set matrix by columns
      void SetColumns(Matrix<3, 1, Real> const & col0, Matrix<3, 1, Real> const & col1,
        Matrix<3, 1, Real> const & col2);

      //! Get matrix columns
      void GetColumns(Matrix<3, 1, Real>& col0, Matrix<3, 1, Real>& col1,
        Matrix<3, 1, Real>& col2) const;

      //! Set self to matrix inverse, assuming a standard affine matrix (bottom row is 0 0 0 1).
      Matrix33& AffineInverse();

      //! Set self to matrix inverse, assuming a standard affine matrix (bottom row is 0 0 0 1).
      friend Matrix33 Dg::R2::AffineInverse(Matrix33<Real> const &);

      //! Set as translation matrix based on vector
      Matrix33& Translation(Matrix<1, 3, Real> const &);

      //! Sets the matrix to a rotation matrix.
      Matrix33& Rotation(Real);

      //! Set as scaling matrix based on vector.
      Matrix33& Scaling(Matrix<1, 3, Real> const &);

      //! Uniform scaling.
      Matrix33& Scaling(Real);
    };

    //--------------------------------------------------------------------------------
    //	@	Matrix33::operator=()
    //--------------------------------------------------------------------------------
    template<typename Real>
    Matrix33<Real>& Matrix33<Real>::operator=(Matrix < 3, 3, Real > const & a_other)
    {
      Matrix<3, 3, Real>::operator=(a_other);
      return *this;
    }

    //--------------------------------------------------------------------------------
    //	@	Matrix33::SetRows()
    //--------------------------------------------------------------------------------
    template<typename Real>
    void Matrix33<Real>::SetRows(Matrix<1, 3, Real> const & a_row0, Matrix<1, 3, Real> const & a_row1,
      Matrix<1, 3, Real> const & a_row2)
    {
      SetRow(0, a_row0);
      SetRow(1, a_row1);
      SetRow(2, a_row2);

    }	//End: Matrix33::SetRows()


      //--------------------------------------------------------------------------------
      //	@	Matrix33::GetRows()
      //--------------------------------------------------------------------------------
    template<typename Real>
    void Matrix33<Real>::GetRows(Matrix<1, 3, Real>& a_row0, Matrix<1, 3, Real>& a_row1,
      Matrix<1, 3, Real>& a_row2) const
    {
      GetRow(0, a_row0);
      GetRow(1, a_row1);
      GetRow(2, a_row2);
    }	//End: Matrix33::GetRows()


      //--------------------------------------------------------------------------------
      //	@	Matrix33::SetColumns()
      //--------------------------------------------------------------------------------
    template<typename Real>
    void Matrix33<Real>::SetColumns(Matrix<3, 1, Real> const & a_col0, Matrix<3, 1, Real> const & a_col1,
      Matrix<3, 1, Real> const & a_col2)
    {
      SetColumn(0, a_col0);
      SetColumn(1, a_col1);
      SetColumn(2, a_col2);
    }	//End: Matrix33::SetColumns()


      //--------------------------------------------------------------------------------
      //	@	Matrix33::GetColumns()
      //--------------------------------------------------------------------------------
    template<typename Real>
    void Matrix33<Real>::GetColumns(Matrix<3, 1, Real>& a_col0, Matrix<3, 1, Real>& a_col1,
      Matrix<3, 1, Real>& a_col2) const
    {
      GetColumn(0, a_col0);
      GetColumn(1, a_col1);
      GetColumn(2, a_col2);
    }	//End: Matrix33::GetColumns()


      //--------------------------------------------------------------------------------
      //	@	Matrix33::AffineInverse()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Matrix33<Real>& Matrix33<Real>::AffineInverse()
    {
      *this = Dg::AffineInverse(*this);

      return *this;

    }	//End: Matrix33::AffineInverse


      //--------------------------------------------------------------------------------
      //	@	AffineInverse()
      //--------------------------------------------------------------------------------
    template<typename Real>
    Matrix33<Real> AffineInverse(Matrix33<Real> const & a_mat)
    {
      Matrix33<Real> result;

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
      //	@	Matrix33::Translation()
      //-------------------------------------------------------------------------------
    template<typename Real>
    Matrix33<Real>& Matrix33<Real>::Translation(Matrix<1, 3, Real> const & a_xlate)
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

    }   // End: Matrix33::Translation()


        //-------------------------------------------------------------------------------
        //	@	Matrix33::Scaling()
        //-------------------------------------------------------------------------------
    template<typename Real>
    Matrix33<Real>& Matrix33<Real>::Scaling(Matrix<1, 3, Real> const & a_scaleFactors)
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

    }   // End: Matrix33::Scaling()


        //-------------------------------------------------------------------------------
        //	@	Matrix33::Scaling()
        //-------------------------------------------------------------------------------
    template<typename Real>
    Matrix33<Real>& Matrix33<Real>::Scaling(Real a_val)
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

    }   // End: Matrix33::Scaling()


        //-------------------------------------------------------------------------------
        //	@	Matrix33::Rotation()
        //-------------------------------------------------------------------------------
    template<typename Real>
    Matrix33<Real>& Matrix33<Real>::Rotation(Real a_angle)
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

    }   // End: Matrix33::Rotation()
  }
}
#endif