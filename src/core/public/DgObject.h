//! @file DgObject.h
//!
//! @author: Frank B. Hart
//! @date 29/06/2016
//!
//! Class declaration: Object

#ifndef DGOBJECT_H
#define DGOBJECT_H


namespace Dg
{
  //! @ingroup DgUtility_types
  //!
  //! @class Object
  //!
  //! A base class containing a few useful methods.
  //!
  //! @author Frank Hart
  //! @date 29/06/2016
  template<typename T>
  class Object
  {
  public:

    Object() {}
    virtual ~Object() {}

    //! Creates a deep copy of a derived object.
    //!
    //! @return Pointer to the newly created object.
    virtual T* Clone() const = 0;
  };
}

#endif