#ifndef DGFPCQUERY_H
#define DGFPCQUERY_H

#include "DgVector_generic.h"

namespace Dg
{
  namespace impl
  {
    //Find Point of Collision
    template <typename Real,
              int R,
              typename Type0,
              typename Type1>
    class FPCQuery
    {
    public:

      struct Result
      {

      };

      //! Perform the query
      Result operator()(Type0 const & a_primitive0, 
                        Vector_generic<Real, R> const & a_traj0,
                        Type0 const & a_primitive1,
                        Vector_generic<Real, R> const & a_traj1);
    };
  }
}

#endif