//! @file Dg_vector_pod.h
//!
//! @author Frank Hart
//! @date 22/07/2016
//!
//! Class header: vector_pod<>

#ifndef DG_VECTOR_P_H
#define DG_VECTOR_P_H

#include <cstdlib>
#include <cstring>
#include <new>
#include <type_traits>

#include "DgErrorHandler.h"

#define DG_CONTAINER_DEFAULT_SIZE 1024

namespace Dg
{
  //! @ingroup DgContainers
  //!
  //! @class vector_pod
  //!
  //! @brief Contiguous array, similar to std::vector.
  //!
  //! http://www.cplusplus.com/reference/vector/vector/
  //!
  //! Similar to std::vector. Constructors/destructors are not called, so use for pod types only.
  //!
  //!
  //! @author Frank Hart
  //! @date 7/01/2014
  template<class T>
  class vector_pod
  {
  public:
    //Constructor / destructor
    vector_pod();

    //! Construct with a set size
    vector_pod(size_t);
    ~vector_pod();

    //! Copy constructor
    vector_pod(vector_pod const &);

    //! Assignment
    vector_pod& operator= (vector_pod const &);

    //! Access element
    T & operator[](size_t i)				{ return m_pData[i]; }

    //! Accessor, no range check.
    T const & operator[](size_t i) const	{ return m_pData[i]; }

    //! Get last element
    //! Calling this function on an empty container causes undefined behavior.
    //!
    //! @return Reference to item in the vector
    T& back() { return m_pData[m_nItems - 1]; }

    //! Get last element
    //! Calling this function on an empty container causes undefined behavior.
    //!
    //! @return const reference to item in the vector
    T const & back() const { return m_pData[m_nItems - 1]; }

    //! Current size of the array
    size_t size()		const			{ return m_nItems; }

    //! Is the array empty
    bool empty()		const			{ return m_nItems == 0; }

    //! Size of the reserved memory.
    size_t max_size()	const			{ return m_poolSize; }

    //! Get pointer to first element.
    T* data()							{ return m_pData; }

    //! Get pointer to first element.
    const T* Data()		const			{ return m_pData; }

    //! Add element to the back of the array.
    void push_back(T const &);

    //! Remove element from the back of the array.
    void pop_back();

    //! Current size is flagged as 0. Elements are NOT destroyed.
    void clear();

    //! Set the current size to 0 and the reserve to new_size
    void resize(size_t new_size);

    //! Erase the element at index by swapping in the last element.
    //! Calling this on an empty vector will no doubt cause a crash.
    void erase_swap(size_t);

  private:
    //! Exteneds the total size of the array (current + reserve) by a factor of 2
    void extend();

    void init(vector_pod const &);

  private:
    //Data members
    T* m_pData;
    size_t m_poolSize;
    size_t m_nItems;
  };


  //--------------------------------------------------------------------------------
  //	@	vector_pod<T>::vector_pod()
  //--------------------------------------------------------------------------------
  template<class T>
  vector_pod<T>::vector_pod() 
    : m_pData(nullptr)
    , m_poolSize(DG_CONTAINER_DEFAULT_SIZE)
    , m_nItems(0)
  {
    m_pData = static_cast<T*>(malloc(m_poolSize * sizeof(T)));
    DG_ASSERT(m_pData != nullptr);

  }	//End: vector_pod::vector_pod()


  //--------------------------------------------------------------------------------
  //	@	vector_pod<T>::vector_pod()
  //--------------------------------------------------------------------------------
  template<class T>
  vector_pod<T>::vector_pod(size_t a_size)
    : m_pData(nullptr)
    , m_nItems(0)
    , m_poolSize(a_size)
  {
    //Initialise pointers
    m_pData = static_cast<T*>(malloc(m_poolSize * sizeof(T)));
    DG_ASSERT(m_pData != nullptr);

  }	//End: vector_pod::vector_pod()


  //--------------------------------------------------------------------------------
  //	@	vector_pod<T>::~vector_pod()
  //--------------------------------------------------------------------------------
  template<class T>
  vector_pod<T>::~vector_pod()
  {
    if (!std::is_pod<T>::value)
    {
      for (size_t i = 0; i < m_nItems; i++)
      {
        m_pData[i].~T();
      }
    }

    free(m_pData);

  }	//End: vector_pod::~vector_pod()


  //--------------------------------------------------------------------------------
  //	@	vector_pod<T>::init()
  //--------------------------------------------------------------------------------
  template<class T>
  void vector_pod<T>::init(vector_pod const & a_other)
  {
    m_pData = static_cast<T*>(realloc(m_pData, a_other.m_poolSize * sizeof(T)));
    DG_ASSERT(m_pData != nullptr);

    //Set sizes
    m_poolSize = a_other.m_poolSize;
    m_nItems = a_other.m_nItems;

    if (!std::is_pod<T>::value)
    {
      for (size_t i = 0; i < m_nItems; i++)
      {
        new (&m_pData[i]) T(a_other.m_pData[i]);
      }
    }
    else
    {
      memcpy(m_pData, a_other.m_pData, a_other.m_nItems * sizeof(T));
    }

  }	//End: vector_pod<T>::init()


  //--------------------------------------------------------------------------------
  //	@	vector_pod<T>::vector_pod()
  //--------------------------------------------------------------------------------
  template<class T>
  vector_pod<T>::vector_pod(vector_pod const & other) : m_pData(nullptr)
  {
    init(other);

  }	//End: vector_pod::vector_pod()


  //--------------------------------------------------------------------------------
  //	@	vector_pod<T>::operator=()
  //--------------------------------------------------------------------------------
  template<class T>
  vector_pod<T>& vector_pod<T>::operator=(vector_pod const & other)
  {
    if (this == &other)
      return *this;

    clear();
    init(other);

    return *this;
  }	//End: vector_pod::operator=()


  //--------------------------------------------------------------------------------
  //	@	vector_pod<T>::push_back()
  //--------------------------------------------------------------------------------
  template<class T>
  void vector_pod<T>::push_back(T const & a_item)
  {
    //Range check
    if (m_nItems == m_poolSize)
    {
      extend();
    }

    //Set element
    memcpy(&m_pData[m_nItems], &a_item, sizeof(T));

    //increment current size
    ++m_nItems;

  }	//End: vector_pod<T>::push_back()


  //--------------------------------------------------------------------------------
  //	@	vector_pod<T>::pop_back
  //--------------------------------------------------------------------------------
  template<class T>
  void vector_pod<T>::pop_back()
  {
    //Range check
    if (m_nItems == 0)
      return;

    //Deincrement current size
    --m_nItems;

  }	//End: vector_pod<T>::pop_back()


  //--------------------------------------------------------------------------------
  //	@	vector_pod<T>::clear()
  //--------------------------------------------------------------------------------
  template<class T>
  void vector_pod<T>::clear()
  {
    //Set current size to 0
    m_nItems = 0;

  }	//End: vector_pod::clear()


  //--------------------------------------------------------------------------------
  //	@	vector_pod<T>::resize()
  //--------------------------------------------------------------------------------
  template<class T>
  void vector_pod<T>::resize(size_t a_size)
  {
    T * tempPtr = static_cast<T*>(realloc(m_pData, a_size * sizeof(T)));

    if (tempPtr == nullptr)
    {
      //TODO
    }

    m_pData = tempPtr;
    m_poolSize = a_size;
    if (a_size < m_nItems)
    {
      m_nItems = a_size;
    }

  }	//End: vector_pod::resize()


  //--------------------------------------------------------------------------------
  //	@	vector_pod<T>::erase_swap()
  //--------------------------------------------------------------------------------
  template<class T>
  void vector_pod<T>::erase_swap(size_t a_ind)
  {
    if (ind < m_nItems - 1)
    {
      memcpy(&m_pData[ind], &m_pData[m_nItems - 1], sizeof(T));
    }
    --m_nItems;
  }	//End: vector_pod::erase_swap()


  //--------------------------------------------------------------------------------
  //	@	vector_pod<T>::extend()
  //--------------------------------------------------------------------------------
  template<class T>
  void vector_pod<T>::extend()
  {
    //Calculate new size 
    size_t newSize = m_poolSize << 1;

    if (newSize < m_poolSize)
    {
      //TODO
    }

    resize(newSize);

  }	//End: vector_pod::extend()



  //--------------------------------------------------------------------------------
  //		Helpful functions
  //--------------------------------------------------------------------------------


  //! @ingroup Containers_functions
  //!
  //! Find a value in the list, returns pointer to element.
  //!
  //! @return Pointer to object
  //!
  //! @param container vector to search
  //! @param val Item to search for
  template<class T>
  T* find(vector_pod<T>& container, T const & val)
  {
    for (size_t i = 0; i < container.size(); ++i)
    {
      if (container[i] == val)
        return &container[i];
    }

    return nullptr;

  }	//End find()


  //! @ingroup Containers_functions
  //!
  //! Fills the vector with a value.
  //!
  //! @param container Target container
  //! @param val value to fill set each element
  template<class T>
  void fill(vector_pod<T>& container, T const & val)
  {
    for (size_t i = 0; i < container.size(); ++i)
    {
      container[i] = val;
    }

  }	//End: fill()

};
#endif