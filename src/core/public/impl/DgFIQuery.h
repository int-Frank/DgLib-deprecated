//! @file DgFIQuery.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016
//!
//! Class declaration: FIQuery

#ifndef DGFIQUERY_H
#define DGFIQUERY_H

namespace Dg
{
  namespace impl
  {
    //! @ingroup DgMath_geoQueries
    //!
    //! Closest-point queries.
    template <typename Real,
              int R,
              typename Type0,
              typename Type1>
    class FIQuery
    {
    public:

      //! A FIQuery-base class B must define a B::Result struct with member
      //! 'Real distance'.  A FIQuery-derived class D must also derive a
      //! D::Result from B:Result but may have no members.  The idea is to
      //! allow Result to store closest-point information in addition to the
      //! distance.  The operator() is non-const to allow CPQuery to store
      //! and modify private state that supports the query.
      struct Result
      {

      };

      //! Perform the query
      Result operator()(Type0 const & a_primitive0, Type1 const & a_primitive1);
    };
  }
}

#endif