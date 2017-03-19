//! @file Dg_vector.h
//!
//! @author Frank Hart
//! @date 22/07/2016
//!
//! Class header: vector<>

#ifndef DG_VECTOR_P_H
#define DG_VECTOR_P_H

#include <cstdlib>
#include <cstring>
#include <new>
#include <type_traits>

#include "impl/DgContainerBase.h"
#include "DgErrorHandler.h"

namespace Dg
{
  //! @ingroup DgContainers
  //!
  //! @class vector
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
  class vector : public ContainerBase
  {
  public:
    //Constructor / destructor
    vector();

    //! Construct with a set size
    vector(size_t);
    ~vector();

    //! Copy constructor
    vector(vector const &);

    //! Assignment
    vector& operator= (vector const &);

    //! Move constructor
    vector(vector &&);

    //! Move assignment
    vector& operator= (vector &&);

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

    void init(vector const &);

  private:
    //Data members
    T* m_pData;
    size_t m_nItems;
  };


  //--------------------------------------------------------------------------------
  //	@	vector<T>::vector()
  //--------------------------------------------------------------------------------
  template<class T>
  vector<T>::vector() 
    : ContainerBase()
    , m_pData(nullptr)
    , m_nItems(0)
  {
    m_pData = static_cast<T*>(malloc(pool_size() * sizeof(T)));
    DG_ASSERT(m_pData != nullptr);

  }	//End: vector::vector()


  //--------------------------------------------------------------------------------
  //	@	vector<T>::vector()
  //--------------------------------------------------------------------------------
  template<class T>
  vector<T>::vector(size_t a_size)
    : ContainerBase(a_size)
    , m_pData(nullptr)
    , m_nItems(0)
  {
    //Initialise pointers
    m_pData = static_cast<T*>(malloc(pool_size() * sizeof(T)));
    DG_ASSERT(m_pData != nullptr);

  }	//End: vector::vector()


  //--------------------------------------------------------------------------------
  //	@	vector<T>::~vector()
  //--------------------------------------------------------------------------------
  template<class T>
  vector<T>::~vector()
  {
    if (!std::is_trivially_destructible<T>::value)
    {
      for (size_t i = 0; i < m_nItems; i++)
      {
        m_pData[i].~T();
      }
    }

    free(m_pData);

  }	//End: vector::~vector()


  //--------------------------------------------------------------------------------
  //	@	vector<T>::init()
  //--------------------------------------------------------------------------------
  template<class T>
  void vector<T>::init(vector const & a_other)
  {
    pool_size(a_other.pool_size());
    m_nItems = a_other.m_nItems;
    m_pData = static_cast<T*>(realloc(m_pData, pool_size() * sizeof(T)));
    DG_ASSERT(m_pData != nullptr);

    if (!std::is_trivially_copy_constructible<T>::value)
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

  }	//End: vector<T>::init()


  //--------------------------------------------------------------------------------
  //	@	vector<T>::vector()
  //--------------------------------------------------------------------------------
  template<class T>
  vector<T>::vector(vector<T> const & other)
    : ContainerBase(other)
    , m_pData(nullptr)
  {
    init(other);

  }	//End: vector::vector()


  //--------------------------------------------------------------------------------
  //	@	vector<T>::vector()
  //--------------------------------------------------------------------------------
  template<class T>
  vector<T>::vector(vector<T> && other) 
    : ContainerBase(std::move(other))
    , m_pData(other.m_pData)
  {
    other.m_pData = nullptr;
    other.m_nItems = 0;
  }	//End: vector::vector()


  //--------------------------------------------------------------------------------
  //	@	vector<T>::vector()
  //--------------------------------------------------------------------------------
  template<class T>
  vector<T> & vector<T>::operator=(vector<T> && other)
  {
    if (this != &other)
    {
      //Assign to this
      m_nItems = other.m_nItems;
      m_pData = other.m_pData;
      pool_size(other.pool_size());

      //Clear other
      other.m_pData = nullptr;
      other.m_nItems = 0;
    }
    return *this;
  }	//End: vector::vector()


  //--------------------------------------------------------------------------------
  //	@	vector<T>::operator=()
  //--------------------------------------------------------------------------------
  template<class T>
  vector<T>& vector<T>::operator=(vector const & other)
  {
    if (this == &other)
      return *this;

    clear();
    ContainerBase::operator=(other);
    init(other);

    return *this;
  }	//End: vector::operator=()


  //--------------------------------------------------------------------------------
  //	@	vector<T>::push_back()
  //--------------------------------------------------------------------------------
  template<class T>
  void vector<T>::push_back(T const & a_item)
  {
    //Range check
    if (m_nItems == pool_size())
    {
      extend();
    }

    if (std::is_trivially_copy_constructible<T>::value)
    {
      memcpy(&m_pData[m_nItems], &a_item, sizeof(T));
    }
    else
    {
      new(&m_pData[m_nItems]) T(a_item);
    }

    m_nItems++;

  }	//End: vector<T>::push_back()


  //--------------------------------------------------------------------------------
  //	@	vector<T>::pop_back
  //--------------------------------------------------------------------------------
  template<class T>
  void vector<T>::pop_back()
  {
    if (!std::is_trivially_destructible<T>::value)
    {
      m_pData[pool_size() - 1].~T();
    }
    --m_nItems;

  }	//End: vector<T>::pop_back()


  //--------------------------------------------------------------------------------
  //	@	vector<T>::clear()
  //--------------------------------------------------------------------------------
  template<class T>
  void vector<T>::clear()
  {
    if (!std::is_trivially_destructible<T>::value)
    {
      for (size_t i = 0; i < pool_size(); i++)
      {
        m_pData[i].~T();
      }
    }
    m_nItems = 0;

  }	//End: vector::clear()


  //--------------------------------------------------------------------------------
  //	@	vector<T>::resize()
  //--------------------------------------------------------------------------------
  template<class T>
  void vector<T>::resize(size_t a_size)
  {
    pool_size(a_size);

    if (pool_size() < m_nItems)
    {
      if (!std::is_trivially_destructible<T>::value)
      {
        for (size_t i = pool_size(); i < m_nItems; i++)
        {
          m_pData[i].~T();
        }
      }
      m_nItems = pool_size();
    }

    m_pData = static_cast<T*>(realloc(m_pData, a_size * sizeof(T)));
    DG_ASSERT(m_pData != nullptr);
  }	//End: vector::resize()


  //--------------------------------------------------------------------------------
  //	@	vector<T>::erase_swap()
  //--------------------------------------------------------------------------------
  template<class T>
  void vector<T>::erase_swap(size_t a_ind)
  {
    if (!std::is_trivially_destructible<T>::value)
    {
      m_pData[a_ind].~T();
    }
    memmove(&m_pData[ind], &m_pData[m_nItems - 1], sizeof(T));
    --m_nItems;
  }	//End: vector::erase_swap()


  //--------------------------------------------------------------------------------
  //	@	vector<T>::extend()
  //--------------------------------------------------------------------------------
  template<class T>
  void vector<T>::extend()
  {
    resize(set_next_pool_size());

  }	//End: vector::extend()



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
  T* find(vector<T>& container, T const & val)
  {
    for (size_t i = 0; i < container.size(); i++)
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
  void fill(vector<T>& container, T const & val)
  {
    for (size_t i = 0; i < container.size(); i++)
    {
      container[i] = val;
    }

  }	//End: fill()

};
#endif