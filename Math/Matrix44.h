//================================================================================
// @ Matrix44.h
// 
// Class: Matrix44
//
// 4 by 4 matrix calss.
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
// Memory layout: { x.x x.y x.z 0 y.x y.y y.z 0 z.x z.y z.z 0 p.x p.y p.z 1 }
// Row major: [x.x x.y x.z 0]
//            [y.x y.y y.z 0]
//            [z.x z.y z.z 0]
//            [p.x p.y p.z 1]
//================================================================================

#ifndef MATRIX44_H
#define MATRIX44_H

#include "dgmath.h"
#include "Vector4.h"
#include "Quaternion.h"

//class BasisR3;

//--------------------------------------------------------------------------------
//	@	Matrix44
//--------------------------------------------------------------------------------
namespace Dg
{
  template<typename Real>
  class Matrix44;

  template<typename Real>
  Matrix44<Real> AffineInverse(const Matrix44<Real>&);

  template<typename Real>
  Matrix44<Real> Transpose(const Matrix44<Real>&);

  template<typename Real>
  Vector4<Real> operator*(const Vector4<Real>&, const Matrix44<Real>&);

  template<typename Real>
  Matrix44<Real> operator*(Real, const Matrix44<Real>&);

  template<typename Real>
  class Matrix44
  {
    friend class Quaternion<Real>;
  public:
    //constructor/destructor
    Matrix44() { Identity(); }
    ~Matrix44() {}

    //Copy operations
    Matrix44(const Matrix44&);
    Matrix44& operator=(const Matrix44&);

    //Accessors
    Real& operator()(unsigned int a_i, unsigned int a_j);
    Real operator()(unsigned int a_i, unsigned int a_j) const;

    //Comparison
    bool operator== (const Matrix44&) const;
    bool operator!= (const Matrix44&) const;
    bool IsZero() const;
    bool IsIdentity() const;

    void Set(const Real[16]);

    //Manipulators
    void SetRows(const Vector4<Real>& a_row1, const Vector4<Real>& a_row2,
                 const Vector4<Real>& a_row3, const Vector4<Real>& a_row4);
    void GetRows(Vector4<Real>& a_row1, Vector4<Real>& a_row2,
                 Vector4<Real>& a_row3, Vector4<Real>& a_row4);
    void SetColumns(const Vector4<Real>& a_col1, const Vector4<Real>& a_col2,
                    const Vector4<Real>& a_col3, const Vector4<Real>& a_col4);
    void GetColumns(Vector4<Real>& a_col1, Vector4<Real>& a_col2,
                    Vector4<Real>& a_col3, Vector4<Real>& a_col4);
    void SetRow(unsigned int a_i, const Vector4<Real>& a_row);
    void SetColumn(unsigned int a_i, const Vector4<Real>& a_col);

    //void Set(const BasisR3&);

    void Clean();
    void Identity();

    Matrix44& AffineInverse();

    template<typename T>
    friend Matrix44<T> AffineInverse(const Matrix44<T>&);

    Matrix44& Transpose();

    template<typename T>
    friend Matrix44<T> Transpose(const Matrix44<T>&);

    //transformations
    Matrix44& Translation(const Vector4<Real>&);
    Matrix44& Rotation(Real a_zRotation, Real a_yRotation, Real a_xRotation);
    Matrix44& Rotation(const Vector4<Real>& a_axis, Real a_angle);
    Matrix44& Rotation(const Quaternion<Real>&);

    Matrix44& Scaling(const Vector4<Real>&);
    Matrix44& Scaling(Real);

    Matrix44& RotationX(Real);
    Matrix44& RotationY(Real);
    Matrix44& RotationZ(Real);

    void GetFixedAngles(Real& a_zRotation, Real& a_yRotation, Real& a_xRotation) const;
    void GetAxisAngle(Vector4<Real>& a_axis, Real& a_angle) const;
    void GetQuaternion(Quaternion<Real>&) const;

    //operators

    Matrix44 operator+ (const Matrix44&) const;
    Matrix44& operator+= (const Matrix44&);
    Matrix44 operator- (const Matrix44&) const;
    Matrix44& operator-= (const Matrix44&);

    Matrix44 operator-() const;

    //Multiplication
    Matrix44& operator*= (const Matrix44&);
    Matrix44 operator* (const Matrix44&) const;

    //Column vector multiplier
    Vector4<Real> operator* (const Vector4<Real>&) const;

    //Row vector multiplier
    template<typename T>
    friend Vector4<T> operator* (const Vector4<T>&, const Matrix44<T>&);

    //Scalar multipication
    Matrix44& operator*= (Real);

    template<typename T>
    friend Matrix44<T> operator* (T, const Matrix44<T>&);

    Matrix44 operator* (Real) const;

    //Low-level data accessors - implementation-dependant
    operator Real*() { return m_V; }
    operator const Real*() const { return m_V; }

  private:
    //Data Members
    Real m_V[16];

  private:
  };

  //--------------------------------------------------------------------------------
  //	@	Matrix44::operator()
  //--------------------------------------------------------------------------------
  //		2D array accessor
  //--------------------------------------------------------------------------------
  template<typename Real>
  Real& Matrix44<Real>::operator()(unsigned int a_i, unsigned int a_j)
  {
    return m_V[a_i * 4 + a_j];

  }	//End: Matrix44::operator()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::operator()
  //--------------------------------------------------------------------------------
  //		2D array accessor
  //--------------------------------------------------------------------------------
  template<typename Real>
  Real Matrix44<Real>::operator()(unsigned int a_i, unsigned int a_j) const
  {
    return m_V[a_i * 4 + a_j];

  }	//End: Matrix44::operator()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::operator=()
  //--------------------------------------------------------------------------------
  //		Assignment operator
  //--------------------------------------------------------------------------------
  template<typename Real>
  Matrix44<Real>& Matrix44<Real>::operator= (const Matrix44<Real>& a_other)
  {
    m_V[0] = a_other.m_V[0];
    m_V[1] = a_other.m_V[1];
    m_V[2] = a_other.m_V[2];
    m_V[3] = a_other.m_V[3];
    m_V[4] = a_other.m_V[4];
    m_V[5] = a_other.m_V[5];
    m_V[6] = a_other.m_V[6];
    m_V[7] = a_other.m_V[7];
    m_V[8] = a_other.m_V[8];
    m_V[9] = a_other.m_V[9];
    m_V[10] = a_other.m_V[10];
    m_V[11] = a_other.m_V[11];
    m_V[12] = a_other.m_V[12];
    m_V[13] = a_other.m_V[13];
    m_V[14] = a_other.m_V[14];
    m_V[15] = a_other.m_V[15];

    return *this;

  }	//End: Matrix::operator=()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::Matrix44()
  //--------------------------------------------------------------------------------
  //		Construct from VQS
  //--------------------------------------------------------------------------------
  //template<typename Real>
  //Matrix44<Real>::Matrix44(const VQS<Real>& a_vqs)
  //{
  //  //Set rotational and scale part
  //  Real xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;

  //  xs = a_vqs.q.x + a_vqs.q.x;
  //  ys = a_vqs.q.y + a_vqs.q.y;
  //  zs = a_vqs.q.z + a_vqs.q.z;
  //  wx = a_vqs.q.w*xs;
  //  wy = a_vqs.q.w*ys;
  //  wz = a_vqs.q.w*zs;
  //  xx = a_vqs.q.x*xs;
  //  xy = a_vqs.q.x*ys;
  //  xz = a_vqs.q.x*zs;
  //  yy = a_vqs.q.y*ys;
  //  yz = a_vqs.q.y*zs;
  //  zz = a_vqs.q.z*zs;

  //  m_V[0] = (static_cast<Real>(1.0) - (yy + zz)) * a_vqs.s;
  //  m_V[4] = (xy - wz) * a_vqs.s;
  //  m_V[8] = (xz + wy) * a_vqs.s;

  //  m_V[1] = (xy + wz) * a_vqs.s;
  //  m_V[5] = (static_cast<Real>(1.0) - (xx + zz)) * a_vqs.s;
  //  m_V[9] = (yz - wx) * a_vqs.s;

  //  m_V[2] = (xz - wy) * a_vqs.s;
  //  m_V[6] = (yz + wx) * a_vqs.s;
  //  m_V[10] = (1.0f - (xx + yy)) * a_vqs.s;

  //  m_V[3] = static_cast<Real>(0.0);
  //  m_V[7] = static_cast<Real>(0.0);
  //  m_V[11] = static_cast<Real>(0.0);
  //  m_V[15] = static_cast<Real>(1.0);

  //  //set translation part
  //  m_V[12] = a_vqs.v.x;
  //  m_V[13] = a_vqs.v.y;
  //  m_V[14] = a_vqs.v.z;
  //  m_V[15] = static_cast<Real>(1.0);

  //}	//End: Matrix44::Matrix44()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::Matrix44()
  //--------------------------------------------------------------------------------
  //		Copy constructor
  //--------------------------------------------------------------------------------
  template<typename Real>
  Matrix44<Real>::Matrix44(const Matrix44<Real>& a_other)
  {
    m_V[0] = a_other.m_V[0];
    m_V[1] = a_other.m_V[1];
    m_V[2] = a_other.m_V[2];
    m_V[3] = a_other.m_V[3];
    m_V[4] = a_other.m_V[4];
    m_V[5] = a_other.m_V[5];
    m_V[6] = a_other.m_V[6];
    m_V[7] = a_other.m_V[7];
    m_V[8] = a_other.m_V[8];
    m_V[9] = a_other.m_V[9];
    m_V[10] = a_other.m_V[10];
    m_V[11] = a_other.m_V[11];
    m_V[12] = a_other.m_V[12];
    m_V[13] = a_other.m_V[13];
    m_V[14] = a_other.m_V[14];
    m_V[15] = a_other.m_V[15];

  }	//End: Matrix44::Matrix44()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::operator==()
  //--------------------------------------------------------------------------------
  //		Comparison operator
  //--------------------------------------------------------------------------------
  template<typename Real>
  bool Matrix44<Real>::operator== (const Matrix44<Real>& a_other) const
  {
    for (unsigned int i = 0; i < 16; i++)
    {
      if (!Dg::AreEqual(m_V[i], a_other.m_V[i]))
        return false;
    }

    return true;

  }	//End: Matrix44::operator==()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::operator!=()
  //--------------------------------------------------------------------------------
  //		Comparison operator
  //--------------------------------------------------------------------------------
  template<typename Real>
  bool Matrix44<Real>::operator!= (const Matrix44<Real>& a_other) const
  {
    for (unsigned int i = 0; i < 16; i++)
    {
      if (!Dg::AreEqual(m_V[i], a_other.m_V[i]))
        return true;
    }

    return false;

  }	//End: Matrix44::operator!=()


  //--------------------------------------------------------------------------------
  //	@	Matrix44Dg::IsZero()
  //--------------------------------------------------------------------------------
  //		Check for static_cast<Real>(0.0) matrix
  //--------------------------------------------------------------------------------
  template<typename Real>
  bool Matrix44<Real>::IsZero() const
  {
    for (unsigned int i = 0; i < 16; i++)
    {
      if (!Dg::IsZero(m_V[i]))
        return false;
    }

    return true;

  }	//End: Matrix44::IsZero()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::IsIdentity()
  //--------------------------------------------------------------------------------
  //		Check for identity
  //--------------------------------------------------------------------------------
  template<typename Real>
  bool Matrix44<Real>::IsIdentity() const
  {
    return Dg::AreEqual(static_cast<Real>(1.0), m_V[0])
      && Dg::AreEqual(static_cast<Real>(1.0), m_V[5])
      && Dg::AreEqual(static_cast<Real>(1.0), m_V[10])
      && Dg::AreEqual(static_cast<Real>(1.0), m_V[15])
      && Dg::IsZero(m_V[1])
      && Dg::IsZero(m_V[2])
      && Dg::IsZero(m_V[3])
      && Dg::IsZero(m_V[4])
      && Dg::IsZero(m_V[6])
      && Dg::IsZero(m_V[7])
      && Dg::IsZero(m_V[8])
      && Dg::IsZero(m_V[9])
      && Dg::IsZero(m_V[11])
      && Dg::IsZero(m_V[12])
      && Dg::IsZero(m_V[13])
      && Dg::IsZero(m_V[14]);

  }	//End: Matrix44::IsIdentity()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::Set()
  //--------------------------------------------------------------------------------
  //		Set Matrix elements
  //--------------------------------------------------------------------------------
  template<typename Real>
  void Matrix44<Real>::Set(const Real a_data[16])
  {
    for (size_t i = 0; i < 16; ++i)
    {
      m_V[i] = a_data[i];
    }
  }// End: Matrix44<Real>::Set()

  //--------------------------------------------------------------------------------
  //	@	Matrix44::SetRows()
  //--------------------------------------------------------------------------------
  //		Set Matrix, Row by row
  //--------------------------------------------------------------------------------
  template<typename Real>
  void Matrix44<Real>::SetRows(const Vector4<Real>& a_row1, const Vector4<Real>& a_row2,
                               const Vector4<Real>& a_row3, const Vector4<Real>& a_row4)
  {
    m_V[0] = a_row1.m_x;
    m_V[1] = a_row1.m_y;
    m_V[2] = a_row1.m_z;
    m_V[3] = a_row1.m_w;

    m_V[4] = a_row2.m_x;
    m_V[5] = a_row2.m_y;
    m_V[6] = a_row2.m_z;
    m_V[7] = a_row2.m_w;

    m_V[8] = a_row3.m_x;
    m_V[9] = a_row3.m_y;
    m_V[10] = a_row3.m_z;
    m_V[11] = a_row3.m_w;

    m_V[12] = a_row4.m_x;
    m_V[13] = a_row4.m_y;
    m_V[14] = a_row4.m_z;
    m_V[15] = a_row4.m_w;

  }	//End: Matrix44::SetRows()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::GetRows()
  //--------------------------------------------------------------------------------
  //		Get Matrix, row by row
  //--------------------------------------------------------------------------------
  template<typename Real>
  void Matrix44<Real>::GetRows(Vector4<Real>& a_row1, Vector4<Real>& a_row2,
                               Vector4<Real>& a_row3, Vector4<Real>& a_row4)
  {
    a_row1.m_x = m_V[0];
    a_row1.m_y = m_V[1];
    a_row1.m_z = m_V[2];
    a_row1.m_w = m_V[3];

    a_row2.m_x = m_V[4];
    a_row2.m_y = m_V[5];
    a_row2.m_z = m_V[6];
    a_row2.m_w = m_V[7];

    a_row3.m_x = m_V[8];
    a_row3.m_y = m_V[9];
    a_row3.m_z = m_V[10];
    a_row3.m_w = m_V[11];

    a_row4.m_x = m_V[12];
    a_row4.m_y = m_V[13];
    a_row4.m_z = m_V[14];
    a_row4.m_w = m_V[15];

  }	//End: Matrix44::GetRows()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::SetColumns()
  //--------------------------------------------------------------------------------
  //		Set Matrix, Column by column
  //--------------------------------------------------------------------------------
  template<typename Real>
  void Matrix44<Real>::SetColumns(const Vector4<Real>& a_col1, const Vector4<Real>& a_col2,
                                  const Vector4<Real>& a_col3, const Vector4<Real>& a_col4)
  {
    m_V[0] = a_col1.m_x;
    m_V[4] = a_col1.m_y;
    m_V[8] = a_col1.m_z;
    m_V[12] = a_col1.m_w;

    m_V[1] = a_col2.m_x;
    m_V[5] = a_col2.m_y;
    m_V[9] = a_col2.m_z;
    m_V[13] = a_col2.m_w;

    m_V[2] = a_col3.m_x;
    m_V[6] = a_col3.m_y;
    m_V[10] = a_col3.m_z;
    m_V[14] = a_col3.m_w;

    m_V[3] = a_col4.m_x;
    m_V[7] = a_col4.m_y;
    m_V[11] = a_col4.m_z;
    m_V[15] = a_col4.m_w;

  }	//End: Matrix44::SetColumns()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::GetColumns()
  //--------------------------------------------------------------------------------
  //		Get Matrix, Column by column
  //--------------------------------------------------------------------------------
  template<typename Real>
  void Matrix44<Real>::GetColumns(Vector4<Real>& a_col1, Vector4<Real>& a_col2,
                                  Vector4<Real>& a_col3, Vector4<Real>& a_col4)
  {
    a_col1.m_x = m_V[0];
    a_col1.m_y = m_V[4];
    a_col1.m_z = m_V[8];
    a_col1.m_w = m_V[12];

    a_col2.m_x = m_V[1];
    a_col2.m_y = m_V[5];
    a_col2.m_z = m_V[9];
    a_col2.m_w = m_V[13];

    a_col3.m_x = m_V[2];
    a_col3.m_y = m_V[6];
    a_col3.m_z = m_V[10];
    a_col3.m_w = m_V[14];

    a_col4.m_x = m_V[3];
    a_col4.m_y = m_V[7];
    a_col4.m_z = m_V[11];
    a_col4.m_w = m_V[15];

  }	//End: Matrix44::GetColumns()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::SetRow()
  //--------------------------------------------------------------------------------
  //		Set a single Row
  //--------------------------------------------------------------------------------
  template<typename Real>
  void Matrix44<Real>::SetRow(unsigned int a_i, const Vector4<Real>& a_row)
  {
    a_i *= 4;
    m_V[a_i] = a_row.m_x;
    m_V[a_i + 1] = a_row.m_y;
    m_V[a_i + 2] = a_row.m_z;
    m_V[a_i + 3] = a_row.m_w;

  }	//End: Matrix44::SetRow()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::SetColumn()
  //--------------------------------------------------------------------------------
  //		Set a single column
  //--------------------------------------------------------------------------------
  template<typename Real>
  void Matrix44<Real>::SetColumn(unsigned int a_i, const Vector4<Real>& a_col)
  {
    m_V[a_i] = a_col.m_x;
    m_V[a_i + 4] = a_col.m_y;
    m_V[a_i + 8] = a_col.m_z;
    m_V[a_i + 12] = a_col.m_w;

  }	//End: Matrix44::SetColumn()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::Clean()
  //--------------------------------------------------------------------------------
  //		Set elements close to static_cast<Real>(0.0) equal to static_cast<Real>(0.0)
  //--------------------------------------------------------------------------------
  template<typename Real>
  void Matrix44<Real>::Clean()
  {
    for (unsigned int i = 0; i < 16; i++)
    {
      if (Dg::IsZero(m_V[i]))
        m_V[i] = static_cast<Real>(0.0);
    }

  }	//End: Matrix44::Clean()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::Indentity()
  //--------------------------------------------------------------------------------
  //		Set to identity matrix
  //--------------------------------------------------------------------------------
  template<typename Real>
  void Matrix44<Real>::Identity()
  {
    m_V[0] = static_cast<Real>(1.0);
    m_V[1] = static_cast<Real>(0.0);
    m_V[2] = static_cast<Real>(0.0);
    m_V[3] = static_cast<Real>(0.0);
    m_V[4] = static_cast<Real>(0.0);
    m_V[5] = static_cast<Real>(1.0);
    m_V[6] = static_cast<Real>(0.0);
    m_V[7] = static_cast<Real>(0.0);
    m_V[8] = static_cast<Real>(0.0);
    m_V[9] = static_cast<Real>(0.0);
    m_V[10] = static_cast<Real>(1.0);
    m_V[11] = static_cast<Real>(0.0);
    m_V[12] = static_cast<Real>(0.0);
    m_V[13] = static_cast<Real>(0.0);
    m_V[14] = static_cast<Real>(0.0);
    m_V[15] = static_cast<Real>(1.0);

  }	//End: Matrix44::Identity()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::AffineInverse()
  //--------------------------------------------------------------------------------
  //		Set self to matrix inverse, assuming a standard affine matrix
  //		(bottom row is 0 0 0 1)
  //--------------------------------------------------------------------------------
  template<typename Real>
  Matrix44<Real>& Matrix44<Real>::AffineInverse()
  {
    *this = Dg::AffineInverse(*this);

    return *this;

  }	//End: Matrix44::AffineInverse


  //--------------------------------------------------------------------------------
  //	@	AffineInverse()
  //--------------------------------------------------------------------------------
  //		Compute matrix inverse, assuming a standard affine matrix
  //		(bottom row is 0 0 0 1)
  //--------------------------------------------------------------------------------
  template<typename Real>
  Matrix44<Real> AffineInverse(const Matrix44<Real>& a_mat)
  {
    Matrix44<Real> result;

    //compute upper left 3x3 matrix determinant
    Real cofactor0 = a_mat.m_V[5] * a_mat.m_V[10] - a_mat.m_V[6] * a_mat.m_V[9];
    Real cofactor4 = a_mat.m_V[2] * a_mat.m_V[9] - a_mat.m_V[1] * a_mat.m_V[10];
    Real cofactor8 = a_mat.m_V[1] * a_mat.m_V[6] - a_mat.m_V[2] * a_mat.m_V[5];
    Real det = a_mat.m_V[0] * cofactor0 + a_mat.m_V[4] * cofactor4 + a_mat.m_V[8] * cofactor8;
    if (Dg::IsZero(det))
    {
      //std::cerr << "@Matrix44::AffintInverse() -> No Inverse exists." << std::endl;
      return result;
    }

    // create adjunct matrix and multiply by 1/det to get upper 3x3
    Real invDet = static_cast<Real>(1.0) / det;
    result.m_V[0] = invDet*cofactor0;
    result.m_V[1] = invDet*cofactor4;
    result.m_V[2] = invDet*cofactor8;

    result.m_V[4] = invDet*(a_mat.m_V[6] * a_mat.m_V[8] - a_mat.m_V[4] * a_mat.m_V[10]);
    result.m_V[5] = invDet*(a_mat.m_V[0] * a_mat.m_V[10] - a_mat.m_V[2] * a_mat.m_V[8]);
    result.m_V[6] = invDet*(a_mat.m_V[2] * a_mat.m_V[4] - a_mat.m_V[0] * a_mat.m_V[6]);

    result.m_V[8] = invDet*(a_mat.m_V[4] * a_mat.m_V[9] - a_mat.m_V[5] * a_mat.m_V[8]);
    result.m_V[9] = invDet*(a_mat.m_V[1] * a_mat.m_V[8] - a_mat.m_V[0] * a_mat.m_V[9]);
    result.m_V[10] = invDet*(a_mat.m_V[0] * a_mat.m_V[5] - a_mat.m_V[1] * a_mat.m_V[4]);

    // multiply -translation by inverted 3x3 to get its inverse

    result.m_V[12] = -result.m_V[0] * a_mat.m_V[12] - result.m_V[4] * a_mat.m_V[13] - result.m_V[8] * a_mat.m_V[14];
    result.m_V[13] = -result.m_V[1] * a_mat.m_V[12] - result.m_V[5] * a_mat.m_V[13] - result.m_V[9] * a_mat.m_V[14];
    result.m_V[14] = -result.m_V[2] * a_mat.m_V[12] - result.m_V[6] * a_mat.m_V[13] - result.m_V[10] * a_mat.m_V[14];

    return result;

  }	//End: ::AffineInverse()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::Transpose()
  //-------------------------------------------------------------------------------
  //		Set self to transpose
  //-------------------------------------------------------------------------------
  template<typename Real>
  Matrix44<Real>& Matrix44<Real>::Transpose()
  {
    Real temp = m_V[1];
    m_V[1] = m_V[4];
    m_V[4] = temp;

    temp = m_V[2];
    m_V[2] = m_V[8];
    m_V[8] = temp;

    temp = m_V[3];
    m_V[2] = m_V[12];
    m_V[12] = temp;

    temp = m_V[6];
    m_V[6] = m_V[9];
    m_V[9] = temp;

    temp = m_V[7];
    m_V[7] = m_V[13];
    m_V[13] = temp;

    temp = m_V[11];
    m_V[11] = m_V[14];
    m_V[14] = temp;

    return *this;

  }   // End: Matrix44::Transpose()


  //--------------------------------------------------------------------------------
  //	@	Transpose()
  //-------------------------------------------------------------------------------
  //		Compute matrix transpose
  //-------------------------------------------------------------------------------
  template<typename Real>
  Matrix44<Real> Transpose(const Matrix44<Real>& a_mat)
  {
    Matrix44<Real> result;

    result.m_V[0] = a_mat.m_V[0];
    result.m_V[1] = a_mat.m_V[4];
    result.m_V[2] = a_mat.m_V[8];
    result.m_V[3] = a_mat.m_V[12];
    result.m_V[4] = a_mat.m_V[1];
    result.m_V[5] = a_mat.m_V[5];
    result.m_V[6] = a_mat.m_V[9];
    result.m_V[7] = a_mat.m_V[13];
    result.m_V[8] = a_mat.m_V[2];
    result.m_V[9] = a_mat.m_V[6];
    result.m_V[10] = a_mat.m_V[10];
    result.m_V[11] = a_mat.m_V[14];
    result.m_V[12] = a_mat.m_V[3];
    result.m_V[13] = a_mat.m_V[7];
    result.m_V[14] = a_mat.m_V[11];
    result.m_V[15] = a_mat.m_V[15];

    return result;

  }   // End: Transpose()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::Translation()
  //-------------------------------------------------------------------------------
  //		Set as translation matrix based on vector
  //-------------------------------------------------------------------------------
  template<typename Real>
  Matrix44<Real>& Matrix44<Real>::Translation(const Vector4<Real>& a_xlate)
  {
    m_V[0] = static_cast<Real>(1.0);
    m_V[1] = static_cast<Real>(0.0);
    m_V[2] = static_cast<Real>(0.0);
    m_V[3] = static_cast<Real>(0.0);
    m_V[4] = static_cast<Real>(0.0);
    m_V[5] = static_cast<Real>(1.0);
    m_V[6] = static_cast<Real>(0.0);
    m_V[7] = static_cast<Real>(0.0);
    m_V[8] = static_cast<Real>(0.0);
    m_V[9] = static_cast<Real>(0.0);
    m_V[10] = static_cast<Real>(1.0);
    m_V[11] = static_cast<Real>(0.0);
    m_V[12] = a_xlate.m_x;
    m_V[13] = a_xlate.m_y;
    m_V[14] = a_xlate.m_z;
    m_V[15] = static_cast<Real>(1.0);

    return *this;

  }   // End: Matrix44::Translation()


  //----------------------------------------------------------------------------
  //	@	Matrix44::Rotation()
  //----------------------------------------------------------------------------
  //		Sets the matrix to a rotation matrix (by Euler angles).
  //----------------------------------------------------------------------------
  template<typename Real>
  Matrix44<Real>& Matrix44<Real>::Rotation(Real a_zRotation, Real a_yRotation, Real a_xRotation)
  {
    // This is an "unrolled" contatenation of rotation matrices X Y & Z
    Real Cx = static_cast<Real>(cos(a_xRotation));
    Real Sx = static_cast<Real>(sin(a_xRotation));

    Real Cy = static_cast<Real>(cos(a_yRotation));
    Real Sy = static_cast<Real>(sin(a_yRotation));

    Real Cz = static_cast<Real>(cos(a_zRotation));
    Real Sz = static_cast<Real>(sin(a_zRotation));

    m_V[0] = (Cy * Cz);
    m_V[4] = -(Cy * Sz);
    m_V[8] = Sy;
    m_V[12] = static_cast<Real>(0.0);

    m_V[1] = (Sx * Sy * Cz) + (Cx * Sz);
    m_V[5] = -(Sx * Sy * Sz) + (Cx * Cz);
    m_V[9] = -(Sx * Cy);
    m_V[13] = static_cast<Real>(0.0);

    m_V[2] = -(Cx * Sy * Cz) + (Sx * Sz);
    m_V[6] = (Cx * Sy * Sz) + (Sx * Cz);
    m_V[10] = (Cx * Cy);
    m_V[14] = static_cast<Real>(0.0);

    m_V[3] = static_cast<Real>(0.0);
    m_V[7] = static_cast<Real>(0.0);
    m_V[11] = static_cast<Real>(0.0);
    m_V[15] = static_cast<Real>(1.0);

    return *this;

  }  // End: Matrix44::Rotation()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::Rotation()
  //----------------------------------------------------------------------------
  //		Sets the matrix to a rotation matrix (by axis and angle).
  //----------------------------------------------------------------------------
  template<typename Real>
  Matrix44<Real>& Matrix44<Real>::Rotation(const Vector4<Real>& a_axis, Real a_angle)
  {
    Real c = Real(cos(a_angle));
    Real s = Real(sin(a_angle));
    Real t = static_cast<Real>(1.0) - c;

    Vector4<Real> nAxis = a_axis;
    nAxis.m_w = static_cast<Real>(0.0);
    nAxis.Normalize();

    // intermediate values
    Real tx = t*nAxis.m_x;  Real ty = t*nAxis.m_y;  Real tz = t*nAxis.m_z;
    Real sx = s*nAxis.m_x;  Real sy = s*nAxis.m_y;  Real sz = s*nAxis.m_z;
    Real txy = tx*nAxis.m_y; Real tyz = tx*nAxis.m_z; Real txz = tx*nAxis.m_z;

    // set matrix
    m_V[0] = tx*nAxis.m_x + c;
    m_V[4] = txy - sz;
    m_V[8] = txz + sy;
    m_V[12] = static_cast<Real>(0.0);

    m_V[1] = txy + sz;
    m_V[5] = ty*nAxis.m_y + c;
    m_V[9] = tyz - sx;
    m_V[13] = static_cast<Real>(0.0);

    m_V[2] = txz - sy;
    m_V[6] = tyz + sx;
    m_V[10] = tz*nAxis.m_z + c;
    m_V[14] = static_cast<Real>(0.0);

    m_V[3] = static_cast<Real>(0.0);
    m_V[7] = static_cast<Real>(0.0);
    m_V[11] = static_cast<Real>(0.0);
    m_V[15] = static_cast<Real>(1.0);

    return *this;

  }  // End: Matrix44::Rotation()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::Rotation()
  //-------------------------------------------------------------------------------
  //		Set as rotation matrix based on quaternion
  //-------------------------------------------------------------------------------
  template<typename Real>
  Matrix44<Real>& Matrix44<Real>::Rotation(const Quaternion<Real>& a_rotate)
  {
    //assert(a_rotate.IsUnit());

    Real xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;

    xs = a_rotate.m_x + a_rotate.m_x;
    ys = a_rotate.m_y + a_rotate.m_y;
    zs = a_rotate.m_z + a_rotate.m_z;
    wx = a_rotate.m_w * xs;
    wy = a_rotate.m_w * ys;
    wz = a_rotate.m_w * zs;
    xx = a_rotate.m_x * xs;
    xy = a_rotate.m_x * ys;
    xz = a_rotate.m_x * zs;
    yy = a_rotate.m_y * ys;
    yz = a_rotate.m_y * zs;
    zz = a_rotate.m_z * zs;

    m_V[0] = static_cast<Real>(1.0) - (yy + zz);
    m_V[1] = xy - wz;
    m_V[2] = xz + wy;
    m_V[3] = static_cast<Real>(0.0);

    m_V[4] = xy + wz;
    m_V[5] = static_cast<Real>(1.0) - (xx + zz);
    m_V[6] = yz - wx;
    m_V[7] = static_cast<Real>(0.0);

    m_V[8] = xz - wy;
    m_V[9] = yz + wx;
    m_V[10] = static_cast<Real>(1.0) - (xx + yy);
    m_V[11] = static_cast<Real>(0.0);

    m_V[12] = static_cast<Real>(0.0);
    m_V[13] = static_cast<Real>(0.0);
    m_V[14] = static_cast<Real>(0.0);
    m_V[15] = static_cast<Real>(1.0);

    return *this;

  }   // End of Matrix44::Rotation()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::Scaling()
  //-------------------------------------------------------------------------------
  //		Set as scaling matrix based on vector
  //-------------------------------------------------------------------------------
  template<typename Real>
  Matrix44<Real>& Matrix44<Real>::Scaling(const Vector4<Real>& a_scaleFactors)
  {
    m_V[0] = a_scaleFactors.m_x;
    m_V[1] = static_cast<Real>(0.0);
    m_V[2] = static_cast<Real>(0.0);
    m_V[3] = static_cast<Real>(0.0);
    m_V[4] = static_cast<Real>(0.0);
    m_V[5] = a_scaleFactors.m_y;
    m_V[6] = static_cast<Real>(0.0);
    m_V[7] = static_cast<Real>(0.0);
    m_V[8] = static_cast<Real>(0.0);
    m_V[9] = static_cast<Real>(0.0);
    m_V[10] = a_scaleFactors.m_z;
    m_V[11] = static_cast<Real>(0.0);
    m_V[12] = static_cast<Real>(0.0);
    m_V[13] = static_cast<Real>(0.0);
    m_V[14] = static_cast<Real>(0.0);
    m_V[15] = static_cast<Real>(1.0);

    return *this;

  }   // End: Matrix44::Scaling()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::Scaling()
  //-------------------------------------------------------------------------------
  //		Uniform scaling
  //-------------------------------------------------------------------------------
  template<typename Real>
  Matrix44<Real>& Matrix44<Real>::Scaling(Real a_val)
  {
    m_V[0] = a_val;
    m_V[1] = static_cast<Real>(0.0);
    m_V[2] = static_cast<Real>(0.0);
    m_V[3] = static_cast<Real>(0.0);
    m_V[4] = static_cast<Real>(0.0);
    m_V[5] = a_val;
    m_V[6] = static_cast<Real>(0.0);
    m_V[7] = static_cast<Real>(0.0);
    m_V[8] = static_cast<Real>(0.0);
    m_V[9] = static_cast<Real>(0.0);
    m_V[10] = a_val;
    m_V[11] = static_cast<Real>(0.0);
    m_V[12] = static_cast<Real>(0.0);
    m_V[13] = static_cast<Real>(0.0);
    m_V[14] = static_cast<Real>(0.0);
    m_V[15] = static_cast<Real>(1.0);

    return *this;

  }   // End: Matrix44::Scaling()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::RotationX()
  //-------------------------------------------------------------------------------
  //		Set as rotation matrix, rotating by 'angle' radians around x-axis
  //-------------------------------------------------------------------------------
  template<typename Real>
  Matrix44<Real>& Matrix44<Real>::RotationX(Real a_angle)
  {
    Real sintheta = Real(sin(a_angle));
    Real costheta = Real(cos(a_angle));

    m_V[0] = static_cast<Real>(1.0);
    m_V[1] = static_cast<Real>(0.0);
    m_V[2] = static_cast<Real>(0.0);
    m_V[3] = static_cast<Real>(0.0);
    m_V[4] = static_cast<Real>(0.0);
    m_V[5] = costheta;
    m_V[6] = sintheta;
    m_V[7] = static_cast<Real>(0.0);
    m_V[8] = static_cast<Real>(0.0);
    m_V[9] = -sintheta;
    m_V[10] = costheta;
    m_V[11] = static_cast<Real>(0.0);
    m_V[12] = static_cast<Real>(0.0);
    m_V[13] = static_cast<Real>(0.0);
    m_V[14] = static_cast<Real>(0.0);
    m_V[15] = static_cast<Real>(1.0);

    return *this;

  }   // End: Matrix44::RotationX()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::RotationY()
  //-------------------------------------------------------------------------------
  //		Set as rotation matrix, rotating by 'angle' radians around y-axis
  //-------------------------------------------------------------------------------
  template<typename Real>
  Matrix44<Real>& Matrix44<Real>::RotationY(Real a_angle)
  {
    Real sintheta = Real(sin(a_angle));
    Real costheta = Real(cos(a_angle));

    m_V[0] = costheta;
    m_V[1] = static_cast<Real>(0.0);
    m_V[2] = -sintheta;
    m_V[3] = static_cast<Real>(0.0);
    m_V[4] = static_cast<Real>(0.0);
    m_V[5] = static_cast<Real>(1.0);
    m_V[6] = static_cast<Real>(0.0);
    m_V[7] = static_cast<Real>(0.0);
    m_V[8] = sintheta;
    m_V[9] = static_cast<Real>(0.0);
    m_V[10] = costheta;
    m_V[11] = static_cast<Real>(0.0);
    m_V[12] = static_cast<Real>(0.0);
    m_V[13] = static_cast<Real>(0.0);
    m_V[14] = static_cast<Real>(0.0);
    m_V[15] = static_cast<Real>(1.0);

    return *this;

  }   // End: Matrix44::RotationY()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::RotationZ()
  //-------------------------------------------------------------------------------
  //		Set as rotation matrix, rotating by 'angle' radians around z-axis
  //-------------------------------------------------------------------------------
  template<typename Real>
  Matrix44<Real>& Matrix44<Real>::RotationZ(Real a_angle)
  {
    Real sintheta = Real(sin(a_angle));
    Real costheta = Real(cos(a_angle));

    m_V[0] = costheta;
    m_V[1] = sintheta;
    m_V[2] = static_cast<Real>(0.0);
    m_V[3] = static_cast<Real>(0.0);
    m_V[4] = -sintheta;
    m_V[5] = costheta;
    m_V[6] = static_cast<Real>(0.0);
    m_V[7] = static_cast<Real>(0.0);
    m_V[8] = static_cast<Real>(0.0);
    m_V[9] = static_cast<Real>(0.0);
    m_V[10] = static_cast<Real>(1.0);
    m_V[11] = static_cast<Real>(0.0);
    m_V[12] = static_cast<Real>(0.0);
    m_V[13] = static_cast<Real>(0.0);
    m_V[14] = static_cast<Real>(0.0);
    m_V[15] = static_cast<Real>(1.0);

    return *this;

  }   // End: Matrix44::RotationZ()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::GetFixedAngles()
  // ---------------------------------------------------------------------------
  //		Gets static_cast<Real>(1.0) set of possible z-y-x fixed angles that will generate this 
  //		matrix. Assumes that upper 3x3 is a rotation matrix
  //----------------------------------------------------------------------------
  template<typename Real>
  void Matrix44<Real>::GetFixedAngles(Real& a_zRotation, Real& a_yRotation, Real& a_xRotation) const
  {
    Real Cx, Sx;
    Real Cy, Sy;
    Real Cz, Sz;

    Sy = m_V[8];
    Cy = Real(sqrt(static_cast<Real>(1.0) - Sy*Sy));
    // normal case
    if (!Dg::IsZero(Cy))
    {
      Real factor = static_cast<Real>(1.0) / Cy;
      Sx = -m_V[9] * factor;
      Cx = m_V[10] * factor;
      Sz = -m_V[4] * factor;
      Cz = m_V[0] * factor;
    }
    // x and z axes aligned
    else
    {
      Sz = static_cast<Real>(0.0);
      Cz = static_cast<Real>(1.0);
      Sx = m_V[6];
      Cx = m_V[5];
    }

    a_zRotation = Real(atan2(Sz, Cz));
    a_yRotation = Real(atan2(Sy, Cy));
    a_xRotation = Real(atan2(Sx, Cx));

  }  // End: Matrix44::GetFixedAngles()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::GetAxisAngle()
  // ---------------------------------------------------------------------------
  //		Gets static_cast<Real>(1.0) possible axis-angle pair that will generate this matrix
  //		Assumes that upper 3x3 is a rotation matrix
  //----------------------------------------------------------------------------
  template<typename Real>
  void Matrix44<Real>::GetAxisAngle(Vector4<Real>& a_axis, Real& a_angle) const
  {
    Real trace = m_V[0] + m_V[5] + m_V[10];
    Real cosTheta = static_cast<Real>(0.5)*(trace - static_cast<Real>(1.0));
    a_angle = Real(acos(cosTheta));

    // a_angle is static_cast<Real>(0.0), a_axis can be anything
    if (Dg::IsZero(a_angle))
    {
      a_axis.Set(static_cast<Real>(1.0), static_cast<Real>(0.0), static_cast<Real>(0.0), static_cast<Real>(0.0));
    }
    // standard case
    else if (a_angle < static_cast<Real>(PI_d - EPSILON_d))
    {
      a_axis.Set(m_V[6] - m_V[9], m_V[8] - m_V[2], m_V[1] - m_V[4], static_cast<Real>(0.0));
      a_axis.Normalize();
    }
    // a_angle is 180 degrees
    else
    {
      unsigned int i = 0;
      if (m_V[5] > m_V[0])
        i = 1;
      if (m_V[10] > m_V[i + 4 * i])
        i = 2;
      unsigned int j = (i + 1) % 3;
      unsigned int k = (j + 1) % 3;
      Real s = sqrt(m_V[i + 4 * i] - m_V[j + 4 * j] - m_V[k + 4 * k] + static_cast<Real>(1.0));
      a_axis[i] = static_cast<Real>(0.5) * s;
      Real recip = static_cast<Real>(1.0) / s;
      a_axis[j] = (m_V[i + 4 * j])*recip;
      a_axis[k] = (m_V[k + 4 * i])*recip;
      a_axis.w() = static_cast<Real>(0.0);
    }

  }  // End: Matrix44::GetAxisAngle()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::GetQuaternion()
  // ---------------------------------------------------------------------------
  //		Assumes rotation matrix
  //----------------------------------------------------------------------------
  template<typename Real>
  void Matrix44<Real>::GetQuaternion(Quaternion<Real>& a_out) const
  {
    //Get trace
    Real trace = m_V[0] + m_V[5] + m_V[10];
    Real half = static_cast<Real>(0.5);
    if (trace > static_cast<Real>(0.0))
    {
      Real s = Real(sqrt(trace + static_cast<Real>(1.0)));
      a_out.m_w = s * half;
      Real recip = half / s;
      a_out.m_x = (m_V[6] - m_V[9]) * recip;
      a_out.m_y = (m_V[2] - m_V[2]) * recip;
      a_out.m_z = (m_V[1] - m_V[1]) * recip;
    }
    else
    {
      unsigned int i = 0;
      if (m_V[5] > m_V[0])
      {
        i = 1;
      }        
      if (m_V[6] > m_V[i + i * 4])
      {
        i = 2;
      }
      
      unsigned int j = (i + 1) % 3;
      unsigned int k = (j + 1) % 3;
      Real s = Real(sqrt(m_V[i + i * 4] - m_V[j + j * 4] - m_V[k + k * 4] + static_cast<Real>(1.0)));
      a_out[i] = half * s;
      Real recip = half / s;
      a_out.m_w = (m_V[k + l * 4] - m_V[j + k * 4]) * recip;
      a_out[j] = (m_V[j + i * 4] + m_V[i + j * 4])*recip;
      a_out[k] = (m_V[k + i * 4] + m_V[i + k * 4])*recip;
    }
  } // End: Matrix44::GetQuaternion()

  //--------------------------------------------------------------------------------
  //	@	Matrix44::operator+()
  //-------------------------------------------------------------------------------
  //		Matrix addition 
  //-------------------------------------------------------------------------------
  template<typename Real>
  Matrix44<Real> Matrix44<Real>::operator+(const Matrix44<Real>& a_other) const
  {
    Matrix44<Real> result;

    for (unsigned int i = 0; i < 16; ++i)
    {
      result.m_V[i] = m_V[i] + a_other.m_V[i];
    }

    return result;

  }   // End: Matrix44::operator+()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::operator+=()
  //-------------------------------------------------------------------------------
  //		Matrix addition by self
  //-------------------------------------------------------------------------------
  template<typename Real>
  Matrix44<Real>& Matrix44<Real>::operator+=(const Matrix44<Real>& a_other)
  {
    for (unsigned int i = 0; i < 16; ++i)
    {
      m_V[i] += a_other.m_V[i];
    }

    return *this;

  }   // End: Matrix44::operator+=()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::operator-()
  //-------------------------------------------------------------------------------
  //		Matrix subtraction 
  //-------------------------------------------------------------------------------
  template<typename Real>
  Matrix44<Real> Matrix44<Real>::operator-(const Matrix44<Real>& other) const
  {
    Matrix44<Real> result;

    for (unsigned int i = 0; i < 16; ++i)
    {
      result.m_V[i] = m_V[i] - other.m_V[i];
    }

    return result;

  }   // End: Matrix44::operator-()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::operator-=()
  //-------------------------------------------------------------------------------
  //		Matrix subtraction by self
  //-------------------------------------------------------------------------------
  template<typename Real>
  Matrix44<Real>& Matrix44<Real>::operator-=(const Matrix44<Real>& a_other)
  {
    for (unsigned int i = 0; i < 16; ++i)
    {
      m_V[i] -= a_other.m_V[i];
    }

    return *this;

  }   // End: Matrix44::operator-=()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::operator-()
  //-------------------------------------------------------------------------------
  //		Negate self and return
  //-------------------------------------------------------------------------------
  template<typename Real>
  Matrix44<Real> Matrix44<Real>::operator-() const
  {
    Matrix44<Real> result;

    for (unsigned int i = 0; i < 16; ++i)
    {
      result.m_V[i] = -m_V[i];
    }

    return result;

  }    // End: Matrix44::operator-()

  
  //--------------------------------------------------------------------------------
  //	@	Matrix44::operator*()
  //-------------------------------------------------------------------------------
  //		Matrix multiplication
  //-------------------------------------------------------------------------------
  template<typename Real>
  Matrix44<Real> Matrix44<Real>::operator*(const Matrix44<Real>& a_other) const
  {
    Matrix44<Real> result;

    result.m_V[0] = m_V[0] * a_other.m_V[0] + m_V[1] * a_other.m_V[4] + m_V[2] * a_other.m_V[8]
      + m_V[3] * a_other.m_V[12];
    result.m_V[1] = m_V[0] * a_other.m_V[1] + m_V[1] * a_other.m_V[5] + m_V[2] * a_other.m_V[9]
      + m_V[3] * a_other.m_V[13];
    result.m_V[2] = m_V[0] * a_other.m_V[2] + m_V[1] * a_other.m_V[6] + m_V[2] * a_other.m_V[10]
      + m_V[3] * a_other.m_V[14];
    result.m_V[3] = m_V[0] * a_other.m_V[3] + m_V[1] * a_other.m_V[7] + m_V[2] * a_other.m_V[11]
      + m_V[3] * a_other.m_V[15];

    result.m_V[4] = m_V[4] * a_other.m_V[0] + m_V[5] * a_other.m_V[4] + m_V[6] * a_other.m_V[8]
      + m_V[7] * a_other.m_V[12];
    result.m_V[5] = m_V[4] * a_other.m_V[1] + m_V[5] * a_other.m_V[5] + m_V[6] * a_other.m_V[9]
      + m_V[7] * a_other.m_V[13];
    result.m_V[6] = m_V[4] * a_other.m_V[2] + m_V[5] * a_other.m_V[6] + m_V[6] * a_other.m_V[10]
      + m_V[7] * a_other.m_V[14];
    result.m_V[7] = m_V[4] * a_other.m_V[3] + m_V[5] * a_other.m_V[7] + m_V[6] * a_other.m_V[11]
      + m_V[7] * a_other.m_V[15];

    result.m_V[8] = m_V[8] * a_other.m_V[0] + m_V[9] * a_other.m_V[4] + m_V[10] * a_other.m_V[8]
      + m_V[11] * a_other.m_V[12];
    result.m_V[9] = m_V[8] * a_other.m_V[1] + m_V[9] * a_other.m_V[5] + m_V[10] * a_other.m_V[9]
      + m_V[11] * a_other.m_V[13];
    result.m_V[10] = m_V[8] * a_other.m_V[2] + m_V[9] * a_other.m_V[6] + m_V[10] * a_other.m_V[10]
      + m_V[11] * a_other.m_V[14];
    result.m_V[11] = m_V[8] * a_other.m_V[3] + m_V[9] * a_other.m_V[7] + m_V[10] * a_other.m_V[11]
      + m_V[11] * a_other.m_V[15];

    result.m_V[12] = m_V[12] * a_other.m_V[0] + m_V[13] * a_other.m_V[4] + m_V[14] * a_other.m_V[8]
      + m_V[15] * a_other.m_V[12];
    result.m_V[13] = m_V[12] * a_other.m_V[1] + m_V[13] * a_other.m_V[5] + m_V[14] * a_other.m_V[9]
      + m_V[15] * a_other.m_V[13];
    result.m_V[14] = m_V[12] * a_other.m_V[2] + m_V[13] * a_other.m_V[6] + m_V[14] * a_other.m_V[10]
      + m_V[15] * a_other.m_V[14];
    result.m_V[15] = m_V[12] * a_other.m_V[3] + m_V[13] * a_other.m_V[7] + m_V[14] * a_other.m_V[11]
      + m_V[15] * a_other.m_V[15];

    return result;

  }   // End: Matrix44::operator*()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::operator*=()
  //-------------------------------------------------------------------------------
  //		Matrix multiplication by self
  //-------------------------------------------------------------------------------
  template<typename Real>
  Matrix44<Real>& Matrix44<Real>::operator*=(const Matrix44<Real>& a_other)
  {
    Matrix44<Real> result;

    result.m_V[0] = m_V[0] * a_other.m_V[0] + m_V[1] * a_other.m_V[4] + m_V[2] * a_other.m_V[8]
      + m_V[3] * a_other.m_V[12];
    result.m_V[1] = m_V[0] * a_other.m_V[1] + m_V[1] * a_other.m_V[5] + m_V[2] * a_other.m_V[9]
      + m_V[3] * a_other.m_V[13];
    result.m_V[2] = m_V[0] * a_other.m_V[2] + m_V[1] * a_other.m_V[6] + m_V[2] * a_other.m_V[10]
      + m_V[3] * a_other.m_V[14];
    result.m_V[3] = m_V[0] * a_other.m_V[3] + m_V[1] * a_other.m_V[7] + m_V[2] * a_other.m_V[11]
      + m_V[3] * a_other.m_V[15];

    result.m_V[4] = m_V[4] * a_other.m_V[0] + m_V[5] * a_other.m_V[4] + m_V[6] * a_other.m_V[8]
      + m_V[7] * a_other.m_V[12];
    result.m_V[5] = m_V[4] * a_other.m_V[1] + m_V[5] * a_other.m_V[5] + m_V[6] * a_other.m_V[9]
      + m_V[7] * a_other.m_V[13];
    result.m_V[6] = m_V[4] * a_other.m_V[2] + m_V[5] * a_other.m_V[6] + m_V[6] * a_other.m_V[10]
      + m_V[7] * a_other.m_V[14];
    result.m_V[7] = m_V[4] * a_other.m_V[3] + m_V[5] * a_other.m_V[7] + m_V[6] * a_other.m_V[11]
      + m_V[7] * a_other.m_V[15];

    result.m_V[8] = m_V[8] * a_other.m_V[0] + m_V[9] * a_other.m_V[4] + m_V[10] * a_other.m_V[8]
      + m_V[11] * a_other.m_V[12];
    result.m_V[9] = m_V[8] * a_other.m_V[1] + m_V[9] * a_other.m_V[5] + m_V[10] * a_other.m_V[9]
      + m_V[11] * a_other.m_V[13];
    result.m_V[10] = m_V[8] * a_other.m_V[2] + m_V[9] * a_other.m_V[6] + m_V[10] * a_other.m_V[10]
      + m_V[11] * a_other.m_V[14];
    result.m_V[11] = m_V[8] * a_other.m_V[3] + m_V[9] * a_other.m_V[7] + m_V[10] * a_other.m_V[11]
      + m_V[11] * a_other.m_V[15];

    result.m_V[12] = m_V[12] * a_other.m_V[0] + m_V[13] * a_other.m_V[4] + m_V[14] * a_other.m_V[8]
      + m_V[15] * a_other.m_V[12];
    result.m_V[13] = m_V[12] * a_other.m_V[1] + m_V[13] * a_other.m_V[5] + m_V[14] * a_other.m_V[9]
      + m_V[15] * a_other.m_V[13];
    result.m_V[14] = m_V[12] * a_other.m_V[2] + m_V[13] * a_other.m_V[6] + m_V[14] * a_other.m_V[10]
      + m_V[15] * a_other.m_V[14];
    result.m_V[15] = m_V[12] * a_other.m_V[3] + m_V[13] * a_other.m_V[7] + m_V[14] * a_other.m_V[11]
      + m_V[15] * a_other.m_V[15];

    for (unsigned int i = 0; i < 16; ++i)
    {
      m_V[i] = result.m_V[i];
    }

    return *this;

  }   // End: Matrix44::operator*=()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::operator*()
  //-------------------------------------------------------------------------------
  //		Matrix-row vector multiplication
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> Matrix44<Real>::operator*(const Vector4<Real>& a_vector) const
  {
    Vector4<Real> result;

    result.m_x = m_V[0] * a_vector.m_x + m_V[1] * a_vector.m_y + m_V[2] * a_vector.m_z + m_V[3] * a_vector.m_w;
    result.m_y = m_V[4] * a_vector.m_x + m_V[5] * a_vector.m_y + m_V[6] * a_vector.m_z + m_V[7] * a_vector.m_w;
    result.m_z = m_V[8] * a_vector.m_x + m_V[9] * a_vector.m_y + m_V[10] * a_vector.m_z + m_V[11] * a_vector.m_w;
    result.m_w = m_V[12] * a_vector.m_x + m_V[13] * a_vector.m_y + m_V[14] * a_vector.m_z + m_V[15] * a_vector.m_w;

    return result;

  }   // End: Matrix44::operator*()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::operator*()
  //-------------------------------------------------------------------------------
  //		Matrix-column vector multiplication
  //-------------------------------------------------------------------------------
  template<typename Real>
  Vector4<Real> operator*(const Vector4<Real>& a_vector, const Matrix44<Real>& a_matrix)
  {
    Vector4<Real> result;

    result.m_x = a_matrix.m_V[0] * a_vector.m_x + a_matrix.m_V[4] * a_vector.m_y
             + a_matrix.m_V[8] * a_vector.m_z + a_matrix.m_V[12] * a_vector.m_w;
    result.m_y = a_matrix.m_V[1] * a_vector.m_x + a_matrix.m_V[5] * a_vector.m_y
             + a_matrix.m_V[9] * a_vector.m_z + a_matrix.m_V[13] * a_vector.m_w;
    result.m_z = a_matrix.m_V[2] * a_vector.m_x + a_matrix.m_V[6] * a_vector.m_y
             + a_matrix.m_V[10] * a_vector.m_z + a_matrix.m_V[14] * a_vector.m_w;
    result.m_w = a_matrix.m_V[3] * a_vector.m_x + a_matrix.m_V[7] * a_vector.m_y
             + a_matrix.m_V[11] * a_vector.m_z + a_matrix.m_V[15] * a_vector.m_w;

    return result;

  }   // End: a_matrix44::operator*()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::operator*=()
  //-------------------------------------------------------------------------------
  //		Matrix-scalar multiplication
  //-------------------------------------------------------------------------------
  template<typename Real>
  Matrix44<Real>& Matrix44<Real>::operator*=(Real a_scalar)
  {
    m_V[0] *= a_scalar;
    m_V[1] *= a_scalar;
    m_V[2] *= a_scalar;
    m_V[3] *= a_scalar;
    m_V[4] *= a_scalar;
    m_V[5] *= a_scalar;
    m_V[6] *= a_scalar;
    m_V[7] *= a_scalar;
    m_V[8] *= a_scalar;
    m_V[9] *= a_scalar;
    m_V[10] *= a_scalar;
    m_V[11] *= a_scalar;
    m_V[12] *= a_scalar;
    m_V[13] *= a_scalar;
    m_V[14] *= a_scalar;
    m_V[15] *= a_scalar;

    return *this;

  }  // End: Matrix44::operator*=()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::operator*()
  //-------------------------------------------------------------------------------
  //		Matrix-scalar multiplication
  //-------------------------------------------------------------------------------
  template<typename Real>
  Matrix44<Real> operator*(Real a_scalar, const Matrix44<Real>& matrix)
  {
    Matrix44<Real> result;
    result.m_V[0] = matrix.m_V[0] * a_scalar;
    result.m_V[1] = matrix.m_V[1] * a_scalar;
    result.m_V[2] = matrix.m_V[2] * a_scalar;
    result.m_V[3] = matrix.m_V[3] * a_scalar;
    result.m_V[4] = matrix.m_V[4] * a_scalar;
    result.m_V[5] = matrix.m_V[5] * a_scalar;
    result.m_V[6] = matrix.m_V[6] * a_scalar;
    result.m_V[7] = matrix.m_V[7] * a_scalar;
    result.m_V[8] = matrix.m_V[8] * a_scalar;
    result.m_V[9] = matrix.m_V[9] * a_scalar;
    result.m_V[10] = matrix.m_V[10] * a_scalar;
    result.m_V[11] = matrix.m_V[11] * a_scalar;
    result.m_V[12] = matrix.m_V[12] * a_scalar;
    result.m_V[13] = matrix.m_V[13] * a_scalar;
    result.m_V[14] = matrix.m_V[14] * a_scalar;
    result.m_V[15] = matrix.m_V[15] * a_scalar;

    return result;

  }  // End: operator*()


  //--------------------------------------------------------------------------------
  //	@	Matrix44::operator*()
  //-------------------------------------------------------------------------------
  //		Matrix-scalar multiplication
  //-------------------------------------------------------------------------------
  template<typename Real>
  Matrix44<Real> Matrix44<Real>::operator*(Real a_scalar) const
  {
    Matrix44<Real> result;
    result.m_V[0] = m_V[0] * a_scalar;
    result.m_V[1] = m_V[1] * a_scalar;
    result.m_V[2] = m_V[2] * a_scalar;
    result.m_V[3] = m_V[3] * a_scalar;
    result.m_V[4] = m_V[4] * a_scalar;
    result.m_V[5] = m_V[5] * a_scalar;
    result.m_V[6] = m_V[6] * a_scalar;
    result.m_V[7] = m_V[7] * a_scalar;
    result.m_V[8] = m_V[8] * a_scalar;
    result.m_V[9] = m_V[9] * a_scalar;
    result.m_V[10] = m_V[10] * a_scalar;
    result.m_V[11] = m_V[11] * a_scalar;
    result.m_V[12] = m_V[12] * a_scalar;
    result.m_V[13] = m_V[13] * a_scalar;
    result.m_V[14] = m_V[14] * a_scalar;
    result.m_V[15] = m_V[15] * a_scalar;

    return result;

  }  // End: Matrix44::operator*()

}


#endif