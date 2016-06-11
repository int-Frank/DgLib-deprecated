//! @file DgDCPQuery.h
//!
//! @author: Adapted from http://www.geometrictools.com
//! @date 29/05/2016
//!
//! Class declaration: DCPQuery

#ifndef DGDCPQUERY_H
#define DGDCPQUERY_H

namespace Dg
{
  //! @ingroup DgMath_geoQueries
  //!
  //! Closest-point and distance queries.
  template <typename Real, typename Type0, typename Type1>
  class DCPQuery
  {
  public:

    //! A DCPQuery-base class B must define a B::Result struct with member
    //! 'Real distance'.  A DCPQuery-derived class D must also derive a
    //! D::Result from B:Result but may have no members.  The idea is to
    //! allow Result to store closest-point information in addition to the
    //! distance.  The operator() is non-const to allow DCPQuery to store
    //! and modify private state that supports the query.
    struct Result
    {

    };

    //! Perform the query
    Result operator()(Type0 const & a_primitive0, Type1 const & a_primitive1);
  };

}

#endif