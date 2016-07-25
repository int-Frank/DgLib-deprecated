//! @file DgObjectWrapper.h
//!
//! @author: Frank B. Hart
//! @date 29/06/2016
//!
//! Class declaration: ObjectWrapper

#ifndef DGOBJECTWRAPPER
#define DGOBJECTWRAPPER

#include "DgObject.h"

namespace Dg
{
  //! @ingroup DgUtility_types
  //!
  //! @class ObjectWrapper
  //!
  //! Wraps base pointers for automatic deep copying and deletion.
  //! Type T should be type Object<T>. If not, the type should
  //! contain the method Clone().
  //!
  //! @author Frank Hart
  //! @date 29/06/2016
  template<class T>
  class ObjectWrapper
  {
  public:

    //! Build internal resource from input.
    //!
    //! @pre input type must contain the Clone() method.
    //! @post Pointer to new resource stored.
    ObjectWrapper(T const &);

    //! Build a new object on the heap from a pointer.
    //!
    //! @param[in] t Pointer to the object to clone.
    //! @param[in] dealloc Flag to determine if the input resource 't' should be freed.
    //!
    //! @pre 't' must be a valid pointer to an object contained the Clone() method.
    //! @post Pointer to new resource stored.
    ObjectWrapper(T* t, bool dealloc = true);

    //! Copy constructor
    ObjectWrapper(const ObjectWrapper& other);

    //! Assignment
    ObjectWrapper& ObjectWrapper::operator=(const ObjectWrapper& other);

    ~ObjectWrapper();

    //! Get pointer to resource.
    //! @return Pointer to the internal resource.
    operator T*() { return m_ptr; }

    //! Get const pointer to resource.
    //! @return Pointer to the internal resource.
    operator T const *() const { return m_ptr; }

    //! Get pointer to resource.
    //! @return Pointer to the internal resource.
    T* operator->() { return m_ptr; }

    //! Get const pointer to resource.
    //! @return Pointer to the internal resource.
    T const * operator->() const { return m_ptr; }


  private:
    //Data members
    T* m_ptr;

  };


  //--------------------------------------------------------------------------------
  //	@	ObjectWrapper<T>::ObjectWrapper()
  //--------------------------------------------------------------------------------
  template<class T>
  ObjectWrapper<T>::ObjectWrapper(const T& t)
  {
    m_ptr = t.Clone();

  }	//End: ObjectWrapper::ObjectWrapper()


  //--------------------------------------------------------------------------------
  //	@	ObjectWrapper<T>::ObjectWrapper()
  //--------------------------------------------------------------------------------
  template<class T>
  ObjectWrapper<T>::ObjectWrapper(T* t, bool dealloc) : m_ptr(nullptr)
  {
    if (t != nullptr)
    {
      m_ptr = t->Clone();

      if (dealloc)
        delete t;
    }
  }	//End: ObjectWrapper:ObjectWrapper()


  //--------------------------------------------------------------------------------
  //	@	ObjectWrapper<T>::~ObjectWrapper()
  //--------------------------------------------------------------------------------
  template<class T>
  ObjectWrapper<T>::~ObjectWrapper()
  {
    delete m_ptr;

  }	//End: ObjectWrapper::~ObjectWrapper()


  //--------------------------------------------------------------------------------
  //	@	ObjectWrapper<T>::ObjectWrapper()
  //--------------------------------------------------------------------------------
  template<class T>
  ObjectWrapper<T>::ObjectWrapper(const ObjectWrapper& other): m_ptr(nullptr)
  {
    if (other.m_ptr != nullptr)
    {
      m_ptr = other.m_ptr->Clone();
    }

  }	//End: ObjectWrapper::ObjectWrapper()


  //--------------------------------------------------------------------------------
  //	@	ObjectWrapper<T>::operator=()
  //--------------------------------------------------------------------------------
  template<class T>
  ObjectWrapper<T>& ObjectWrapper<T>::operator=(const ObjectWrapper& other)
  {
    if (this == &other)
    {
      return *this;
    }

    delete m_ptr;
    m_ptr = nullptr;

    if (other.m_ptr != nullptr)
    {
      m_ptr = (T*)other.m_ptr->Clone();
    }

    return *this;

  }	//End: ObjectWrapper::operator=()
}

#endif