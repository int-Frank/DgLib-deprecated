//! @file DgR2Line.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: Line

#ifndef DGR2LINE_H
#define DGR2LINE_H

#include "impl/DgLine_generic.h"
#include "DgR2Matrix.h"

namespace Dg
{
  namespace R2
  {
    template<typename Real>
    using Line = Dg::impl::Line_generic<Real, 2>;


    //! Transform Line with Matrix
    template<typename Real, int R>
    Line<Real> Transform(Line<Real> const & a_line,
                         Matrix<Real> const & a_mat)
    {
      return Line<Real>(a_line.Origin() * a_mat, 
                        a_line.Direction() * a_mat);
    }	//End: Transform()
  }
}
#endif