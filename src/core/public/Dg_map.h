//! @file Dg_map.h
//!
//! @author: Frank B. Hart
//! @date 2/5/2015
//!
//! Class declaration: map

#ifndef DG_MAP_H
#define DG_MAP_H

#include <cstring>
#include <new>
#include <type_traits>

#include "impl/DgContainerBase.h"
#include "DgErrorHandler.h"

namespace Dg
{
  //! @ingroup DgContainers
  //!
  //! @class map
  //!
  //! Ordered mapped list. 
  //!
  //! @author Frank B. Hart
  //! @date 2/5/2015
  template<typename K, typename T>
  class map : public ContainerBase
  {
  public:

    //! Constructor 
    //! If the constructor fails to allocate the map, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
    map();

    //! Initialize map with to a capacity. 
    //! If the constructor fails to allocate the map, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
    //!
    //! @param[in] size Requested size
    map(int size);
    ~map();

    //! Copy constructor.
    map(map const &);

    //! Assigns new contents to the container, replacing its current content.
    map& operator= (map const &);

    //! Move constructor.
    map(map &&);

    //! Move assignment
    map& operator= (map &&);

    //! Returns a reference to the \a i<SUP>th</SUP> element in the map. 
    //! This function does not perform a range check.
    //!
    //! @return Reference to the item
    //!
    //! @param[in] i Index of the item
    T& operator[](int i)	{ return m_pData[i]; }

    //! Returns a const reference to the \a i<SUP>th</SUP> element in the map. 
    //! This function does not perform a range check.
    //!
    //! @return const reference to the item
    //!
    //! @param[in] i Index of the item
    T const & operator[](int i) const { return m_pData[i]; }

    //! Return number of elements in the map.
    int size() const	{ return m_nItems; }

    //! Returns whether the map is empty.
    bool empty() const	{ return m_nItems == 0; }

    //! Returns the key of the ith element in the map.
    //! This function does not perform a range check.
    //!
    //! @return Key
    //!
    //! @param[in] i Index of item to query
    K const & query_key(int i)	const { return m_pKeys[i]; }

    //! Searches the map for an element with a key equivalent to \a k.
    //! @return True if the element was found with \a index being set to the 
    //!         index of the element inthe map. False if not found with \a index
    //!         set to one lower to where \a k would be.
    //! @param[in] k The key to search for.
    //! @param[out] index The resulting index is stored here.
    //! @param[in] lower Set a low bound to the search sublist.
    bool find(K k, int& index, int lower = 0) const;			 //Use binary search

    //! Searches the map for an element with a key equivalent to \a k.
    //! @return True if the element was found with \a index being set to the 
    //!         index of the element inthe map. False if not found with \a index
    //!         set to one lower to where \a k would be.
    //! @param[in] k The key to search for.
    //! @param[out] index The resulting index is stored here.
    //! @param[in] lower Set a low bound to the search sublist.
    //! @param[in] upper Set an upper bound to the search sublist.
    bool find(K k, int& index, int lower, int upper) const;	//Use binary search

    //! Extends the container by inserting new elements, effectively increasing 
    //! the container size by the number of elements inserted.
    //! If the function fails to allocate memory, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
    //!
    //! @return False if key already exists in the map.
    //!
    //! @param[in] k The key associated with the item
    //! @param[in] t The item to insert
    bool insert(K k, T t);

    //! Set element with key \a k, with value \a t.
    //! @return True if key found.
    //!
    //! @param[in] k Key
    //! @param[in] t item
    bool set(K k, T t);

    //! Removes the item in the map with key \a k.
    void erase(K k);

    //! Clear all items from the map, retains allocated memory.
    void clear();

    //! Resize the map. The content of the map is preserved up 
    //! to the lesser of the new and old sizes.
    //! If the function fails to allocate memory, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
    void resize(int);

    //! Clears the map, reallocates memory to the map.
    void reset();

  private:

    //! Doubles the memory allocated to the map. Retains all data.
    //! If the function fails to allocate memory, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
    void extend();

    void init(map const &);

  private:
    //Data members
    K*  m_pKeys;
    T*  m_pData;

    int m_nItems;
  };


  //--------------------------------------------------------------------------------
  //	@	map<K,T>::map()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  map<K, T>::map()
    : ContainerBase()
    , m_pData(nullptr)
    , m_pKeys(nullptr)
    , m_nItems(0)
  {
    resize(static_cast<int>(pool_size()));

  }	//End: map::map()


  //--------------------------------------------------------------------------------
  //	@	map<K,T>::map()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  map<K, T>::map(int a_size)
    : ContainerBase(a_size)
    , m_pData(nullptr)
    , m_pKeys(nullptr)
    , m_nItems(0)
  {
    m_pData = static_cast<T*>(malloc(sizeof(T) * pool_size()));
    DG_ASSERT(m_pData != nullptr);

    m_pKeys = static_cast<K*>(malloc(sizeof(K) * pool_size()));
    DG_ASSERT(m_pKeys != nullptr);

  }	//End: map::map()


  //--------------------------------------------------------------------------------
  //	@	map<K,T>::~map()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  map<K, T>::~map()
  {
    clear();

    free(m_pData);
    free(m_pKeys);

  }	//End: map::~map()


  //--------------------------------------------------------------------------------
  //	@	map<K,T>::init()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  void map<K, T>::init(map const & a_other)
  {
    resize(static_cast<int>(a_other.pool_size()));
    m_nItems = a_other.m_nItems;

    if (std::is_trivially_copy_constructible<K>::value)
    {
      memcpy(m_pKeys, a_other.m_pKeys, m_nItems * sizeof(K));
    }
    else
    {
      for (int i = 0; i < a_other.m_nItems; ++i)
      {
        new (&m_pKeys[i]) K(a_other.m_pKeys[i]);
      }
    }

    if (std::is_trivially_copy_constructible<T>::value)
    {
      memcpy(m_pData, a_other.m_pData, m_nItems * sizeof(T));
    }
    else
    {
      for (int i = 0; i < a_other.m_nItems; ++i)
      {
        new (&m_pData[i]) T(a_other.m_pData[i]);
      }
    }
  }	//End: map::init()


  //--------------------------------------------------------------------------------
  //	@	map<K,T>::map()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  map<K, T>::map(map const & a_other)
    : ContainerBase(a_other)
    , m_pData(nullptr)
    , m_pKeys(nullptr)
    , m_nItems(0)
  {
    init(a_other);

  }	//End: map::map()


  //--------------------------------------------------------------------------------
  //	@	map<K,T>::operator=()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  map<K, T>& map<K, T>::operator=(map const & a_other)
  {
    if (this == &a_other)
      return *this;

    clear();
    ContainerBase::operator=(a_other);
    init(a_other);

    return *this;

  }	//End: map::operator=()


  //--------------------------------------------------------------------------------
  //	@	map<K,T>::resize()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  void map<K, T>::resize(int a_newSize)
  {
    int newSize = static_cast<int>(pool_size(a_newSize));

    if (newSize < m_nItems)
    {
      if (!std::is_trivially_destructible<K>::value)
      {
        for (int i = newSize; i < m_nItems; ++i)
        {
          m_pKeys[i].~K();
        }
      }
      if (!std::is_trivially_destructible<T>::value)
      {
        for (int i = newSize; i < m_nItems; ++i)
        {
          m_pData[i].~T();
        }
      }
      m_nItems = newSize;
    }

    m_pData = static_cast<T*>(realloc(m_pData, sizeof(T) * newSize));
    DG_ASSERT(m_pData != nullptr);

    m_pKeys = static_cast<K*>(realloc(m_pKeys, sizeof(K) * newSize));
    DG_ASSERT(m_pKeys != nullptr);
  }	//End: map::resize()


  //--------------------------------------------------------------------------------
  //	@	map<K,T>::find()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  bool map<K, T>::find(K a_key, int& a_index, int a_lower) const
  {
    return find(a_key, a_index, a_lower, (m_nItems - 1));

  }	//End: map::find()


  //--------------------------------------------------------------------------------
  //	@	map<K,T>::find()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  bool map<K, T>::find(K a_key, int& a_index, int a_lower, int a_upper) const
  {
    while (a_lower <= a_upper)
    {
      // calculate the midpoint for roughly equal partition
      a_index = ((a_upper + a_lower) >> 1);

      // determine which subarray to search
      if (m_pKeys[a_index] < a_key)
        // change min index to search upper subarray
        a_lower = a_index + 1;
      else if (m_pKeys[a_index] > a_key)
        // change max index to search lower subarray
        a_upper = a_index - 1;
      else
        // key found at index index
        return true;
    }

    //Set index closest (but lower) to key
    a_index = a_lower - 1;
    return false;

  }	//End: map::find()


  //--------------------------------------------------------------------------------
  //	@	map<K,T>::extend()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  void map<K, T>::extend()
  {
    //Calculate new size
    int oldPoolSize = static_cast<int>(pool_size());

    //overflow, map full
    DG_ASSERT(oldPoolSize < set_next_pool_size());

    resize(static_cast<int>(pool_size()));

  }	//End: map::extend()


  //--------------------------------------------------------------------------------
  //	@	map<K,T>::insert()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  bool map<K, T>::insert(K a_key, T a_item)
  {
    //Find the index to insert to
    int index;
    if (find(a_key, index))
      return false;	//element already exists

    //Range check
    if (m_nItems == pool_size())
      extend();

    //shift all RHS objects to the right by one.
    memmove(&m_pData[index + 2], &m_pData[index + 1], (m_nItems - index - 1) * sizeof(T));
    memmove(&m_pKeys[index + 2], &m_pKeys[index + 1], (m_nItems - index - 1) * sizeof(K));

    index++;

    //Construct new element.
    if (std::is_trivially_copy_constructible<K>::value)
    {
      memcpy(&m_pKeys[index], &a_key, sizeof(K));
    }
    else
    {
      new (&m_pKeys[index]) K(a_key);
    }

    if (std::is_trivially_copy_constructible<T>::value)
    {
      memcpy(&m_pData[index], &a_item, sizeof(T));
    }
    else
    {
      new (&m_pData[index]) T(a_item);
    }

    m_nItems++;

    return true;

  }	//End: map::insert()


  //--------------------------------------------------------------------------------
  //	@	map<K,T>::erase()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  void map<K, T>::erase(K a_key)
  {
    //Find the index
    int index;
    if (!find(a_key, index))
    {
      return;
    }

    if (!std::is_trivially_destructible<K>::value)
    {
      m_pKeys[index].~K();
    }

    if (!std::is_trivially_destructible<T>::value)
    {
      m_pData[index].~T();
    }

    memmove(&m_pData[index], &m_pData[index + 1], (m_nItems - index - 1) * sizeof(T));
    memmove(&m_pKeys[index], &m_pKeys[index + 1], (m_nItems - index - 1) * sizeof(K));

    m_nItems--;

  }	//End: map::erase()


  //--------------------------------------------------------------------------------
  //	@	map<K,T>::set()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  bool map<K, T>::set(K a_key, T a_item)
  {
    //Find the index to insert to
    int index;
    if (!find(a_key, index))
    {
      return false;	//element does not exist
    }

    m_pData[index] = a_item;

    return true;

  }	//End: map::set()


  //--------------------------------------------------------------------------------
  //	@	Dgmap_p<K,T>::reset()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  void map<K, T>::reset()
  {
    clear();
    resize(DG_CONTAINER_DEFAULT_SIZE);

  }	//End: map::reset()


  //--------------------------------------------------------------------------------
  //	@	Dgmap_p<K,T>::clear()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  void map<K, T>::clear()
  {
    for (int i = 0; i < m_nItems; ++i)
    {
      if (!std::is_trivially_destructible<K>::value) m_pKeys[i].~K();
      if (!std::is_trivially_destructible<T>::value) m_pData[i].~T();
    }

    m_nItems = 0;

  }	//End: map::clear()
};

#endif