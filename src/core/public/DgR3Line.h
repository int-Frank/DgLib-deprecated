//! @file DgR3Line.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: Line

#ifndef DGR3LINE_H
#define DGR3LINE_H

#include "impl/DgLine_generic.h"
#include "DgR3Matrix.h"
#include "DgR3VQS.h"

namespace Dg
{
  namespace R3
  {
    template<typename Real>
    using Line = Dg::impl::Line_generic<Real, 3>;


    //! Transform Line with Matrix
    template<typename Real, int R>
    Line<Real> Transform(Line<Real> const & a_line,
                         Matrix<Real> const & a_mat)
    {
      return Line<Real>(a_line.Origin() * a_mat, 
                        a_line.Direction() * a_mat);
    }	//End: Transform()


    //! Transform line with VQS
    template<typename Real>
    Line<Real> Transform(Line<Real> const & a_line,
                         VQS<Real> const & a_vqs)
    {
      return Line<Real>(a_vqs.TransformPoint(a_line.Origin()),
                        a_vqs.Rotate(a_line.Direction()));
    }	//End: Line_generic::GetTransformed()
  }
}
#endif