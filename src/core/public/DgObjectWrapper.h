#ifndef DGOBJECTWRAPPER
#define DGOBJECTWRAPPER

#include "DgObject.h"

namespace Dg
{
  /*!
  * @ingroup utility_container
  *
  * @class ObjectWrapper
  *
  * @brief Wraps base pointers for automatic deep copying and deletion.
  *
  * Type T should be type BaseClass<T>. If not, the type should
  * contain the method clone().
  *
  * @author Frank Hart
  * @date 2/01/2014
  */
  template<class T>
  class ObjectWrapper
  {
  public:
    /*!
    * @brief Build internal resource from input.
    *
    * @pre input type must contain the clone() method.
    * @post Pointer to new resource stored.
    */
    ObjectWrapper(const T&);

    /*!
    * @brief Build a new object on the heap from a pointer.
    *
    * @param[in] t Pointer to the object to clone.
    * @param[in] dealloc Flag to determine if the input resource 't' should be freed.
    *
    * @pre 't' must be a valid pointer to an object contained the clone() method.
    * @post Pointer to new resource stored.
    */
    ObjectWrapper(T* t, bool dealloc = true);

    /*!
    * @brief Internal pointer set to NULL
    */
    ObjectWrapper() : ptr(NULL) {}


    ObjectWrapper(const ObjectWrapper& other);
    ObjectWrapper& ObjectWrapper::operator=(const ObjectWrapper& other);


    /*!
    * @brief Internal resource cleared
    */
    ~ObjectWrapper();

    /*!
    * @brief Clears current resource and build a new from input.
    */
    void Set(const T&);

    /*!
    * @brief Clears current resource and build a new from input.
    *
    * @param[in] t Pointer to the object to clone.
    * @param[in] dealloc Flag to determine if the input resource 't' should be cleared.
    *
    * @pre 't' must be a valid pointer to an object containing the clone() method.
    * @post New derived class created, reference stored.
    */
    void Set(const T* t, bool dealloc = true);

    /*!
    * @brief Deletes the object
    */
    void Clear() { delete ptr; ptr = NULL; }

    /*!
    * @brief Get pointer to resource.
    * @return Pointer to the internal resource.
    */
    operator T*() { return ptr; }

    /*!
    * @brief Get pointer to resource.
    * @return Pointer to the internal resource.
    */
    operator const T*() const { return ptr; }

    /*!
    * @brief Get pointer to resource.
    * @return Pointer to the internal resource.
    */
    T* operator->() { return ptr; }


  private:
    //Data members
    T* ptr;

  };


  //--------------------------------------------------------------------------------
  //	@	ObjectWrapper<T>::ObjectWrapper()
  //--------------------------------------------------------------------------------
  //		Constructor
  //--------------------------------------------------------------------------------
  template<class T>
  ObjectWrapper<T>::ObjectWrapper(const T& t)
  {
    ptr = t.Clone();

  }	//End: ObjectWrapper::ObjectWrapper()


    //--------------------------------------------------------------------------------
    //	@	ObjectWrapper<T>::ObjectWrapper()
    //--------------------------------------------------------------------------------
    //		Constructor
    //--------------------------------------------------------------------------------
  template<class T>
  ObjectWrapper<T>::ObjectWrapper(T* t, bool dealloc)
  {
    ptr = t->Clone();

    if (dealloc)
      delete t;

  }	//End: ObjectWrapper:ObjectWrapper()


    //--------------------------------------------------------------------------------
    //	@	ObjectWrapper<T>::~ObjectWrapper()
    //--------------------------------------------------------------------------------
    //		Destructor
    //--------------------------------------------------------------------------------
  template<class T>
  ObjectWrapper<T>::~ObjectWrapper()
  {
    delete ptr;

  }	//End: ObjectWrapper::~ObjectWrapper()


    //--------------------------------------------------------------------------------
    //	@	ObjectWrapper<T>::ObjectWrapper()
    //--------------------------------------------------------------------------------
    //		Copy constructor
    //--------------------------------------------------------------------------------
  template<class T>
  ObjectWrapper<T>::ObjectWrapper(const ObjectWrapper& other)
  {
    ptr = other.ptr->Clone();

  }	//End: ObjectWrapper::ObjectWrapper()


    //--------------------------------------------------------------------------------
    //	@	ObjectWrapper<T>::operator=()
    //--------------------------------------------------------------------------------
    //		Assignment
    //--------------------------------------------------------------------------------
  template<class T>
  ObjectWrapper<T>& ObjectWrapper<T>::operator=(const ObjectWrapper& other)
  {
    delete ptr;

    if (other.ptr != NULL)
    {
      T* temp_ptr(other.ptr->Clone());
      ptr = temp_ptr;
    }

    return *this;

  }	//End: ObjectWrapper::operator=()


    //--------------------------------------------------------------------------------
    //	@	ObjectWrapper<T>::Set()
    //--------------------------------------------------------------------------------
    //		Set data from other
    //--------------------------------------------------------------------------------
  template<class T>
  void ObjectWrapper<T>::Set(const T& t)
  {
    //Free memory
    delete ptr;

    //Assign new object
    ptr = t.Clone();

  }	//End: ObjectWrapper:Set()


    //--------------------------------------------------------------------------------
    //	@	ObjectWrapper<T>::Set()
    //--------------------------------------------------------------------------------
    //		Set from other. Frees memory of original pointer if dealloc is true.
    //--------------------------------------------------------------------------------
  template<class T>
  void ObjectWrapper<T>::Set(const T* t, bool dealloc)
  {
    //Free memory
    delete ptr;

    //Assign new object
    ptr = t->Clone();

    if (dealloc)
      delete t;

  } //End: ObjectWrapper:Set()
}

#endif