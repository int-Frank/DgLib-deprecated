//! @file Dg_map_pod.h
//!
//! @author: Frank B. Hart
//! @date 2/5/2015
//!
//! Class declaration: map_pod

#ifndef DG_MAP_POD_H
#define DG_MAP_POD_H

#include <cstdlib>
#include <cstring>

#define DG_CONTAINER_DEFAULT_SIZE 1024

namespace Dg
{
  //! @ingroup DgContainers
  //!
  //! @class map_pod
  //!
  //! Ordered mapped list. 
  //!
  //! Assumed types are POD, so no construction / assignment operators called
  //!
  //! @author Frank B. Hart
  //! @date 2/5/2015
  template<typename K, typename T>
  class map_pod
  {
  public:

    //! Constructor 
    //! If the constructor fails to allocate the map_pod, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
    map_pod();

    //! Initialize map_pod with to a capacity. 
    //! If the constructor fails to allocate the map_pod, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
    map_pod(int);
    ~map_pod();

    //! Copy constructor.
    map_pod(map_pod const &);

    //! Assigns new contents to the container, replacing its current content.
    map_pod& operator= (map_pod const &);

    //! Returns a reference to the \a i<SUP>th</SUP> element in the map_pod. 
    //! This function does not perform a range check.
    //!
    //! @return Reference to the item
    //!
    //! @param[in] i Index of the item
    T& operator[](int i)	{ return m_data[i]; }

    //! Returns a const reference to the \a i<SUP>th</SUP> element in the map_pod. 
    //! This function does not perform a range check.
    //!
    //! @return const reference to the item
    //!
    //! @param[in] i Index of the item
    T const & operator[](int i) const { return m_data[i]; }

    //! Return number of elements in the map_pod.
    int size() const	{ return m_currentSize; }

    //! Returns whether the map_pod is empty.
    bool empty() const	{ return m_currentSize == 0; }

    //! Returns number of elements the map_pod can hold before resizing.
    int max_size() const	{ return m_arraySize; }

    //! Returns the key of the ith element in the map_pod.
    //! This function does not perform a range check.
    //!
    //! @return Key
    //!
    //! @param[in] i Index of item to query
    K const & query_key(int i)	const { return m_keys[i]; }

    //! Searches the map_pod for an element with a key equivalent to \a k.
    //! @return True if the element was found with \a index being set to the 
    //!         index of the element inthe map. False if not found with \a index
    //!         set to one lower to where \a k would be.
    //! @param[in] k The key to search for.
    //! @param[out] index The resulting index is stored here.
    //! @param[in] lower Set a low bound to the search sublist.
    bool find(K k, int& index, int lower = 0) const;			 //Use binary search

    //! Searches the map_pod for an element with a key equivalent to \a k.
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
    bool insert(K k, T const & t);

    //! Set element with key \a k, with value \a t.
    //! @return True if key found.
    //!
    //! @param[in] k Key
    //! @param[in] t item
    bool set(K k, T t);

    //! Removes the item in the map_pod with key \a k.
    void erase(K k);

    //! Removes the item in the map_pod at position \a i.
    void erase_at_position(int);

    //! Clear all items from the map_pod, retains allocated memory.
    void clear();

    //! Resize the map_pod. The content of the map_pod is preserved up 
    //! to the lesser of the new and old sizes.
    //! If the function fails to allocate memory, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
    void resize(int);

    //! Clears the map_pod, reallocates memory to the map_pod.
    void reset();

  private:

    //! Doubles the memory allocated to the map_pod. Retains all data.
    //! If the function fails to allocate memory, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
    void extend();

    void init(map_pod const &);

  private:
    //Data members
    K*  m_keys;
    T*  m_data;

    int m_arraySize;
    int m_currentSize;
  };


  //--------------------------------------------------------------------------------
  //	@	map_pod<K,T>::map_pod()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  map_pod<K, T>::map_pod()
    : m_data(nullptr)
    , m_keys(nullptr)
    , m_arraySize(0)
    , m_currentSize(0)
  {
    resize(DG_CONTAINER_DEFAULT_SIZE);

  }	//End: map_pod::map_pod()


  //--------------------------------------------------------------------------------
  //	@	map_pod<K,T>::map_pod()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  map_pod<K, T>::map_pod(int a_size)
    : m_data(nullptr)
    , m_keys(nullptr)
    , m_arraySize(0)
    , m_currentSize(0)
  {
    T * tempData = static_cast<T*>(malloc(sizeof(T) * a_size));

    if (tempData == nullptr)
    {
      //TODO
    }

    K * tempKeys = static_cast<K*>(malloc(sizeof(K) * a_size));

    if (tempKeys == nullptr)
    {
      //TODO
    }

    m_data = tempData;
    m_keys = tempKeys;
    m_arraySize = a_size;
    m_currentSize = 0;

  }	//End: map_pod::map_pod()


  //--------------------------------------------------------------------------------
  //	@	map_pod<K,T>::~map_pod()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  map_pod<K, T>::~map_pod()
  {
    free(m_data);
    free(m_keys);

  }	//End: map_pod::~map_pod()


  //--------------------------------------------------------------------------------
  //	@	map_pod<K,T>::init()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  void map_pod<K, T>::init(map_pod const & a_other)
  {
    resize(a_other.m_arraySize);

    memcpy(m_data, a_other.m_data, a_other.m_currentSize * sizeof(T));
    memcpy(m_keys, a_other.m_keys, a_other.m_currentSize * sizeof(K));

    m_currentSize = a_other.m_currentSize;

  }	//End: map_pod::init()


  //--------------------------------------------------------------------------------
  //	@	map_pod<K,T>::map_pod()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  map_pod<K, T>::map_pod(map_pod const & a_other)
    : m_data(nullptr)
    , m_keys(nullptr)
    , m_arraySize(0)
    , m_currentSize(0)
  {
    init(a_other);

  }	//End: map_pod::map_pod()


  //--------------------------------------------------------------------------------
  //	@	map_pod<K,T>::operator=()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  map_pod<K, T>& map_pod<K, T>::operator=(map_pod const & a_other)
  {
    if (this == &a_other)
      return *this;

    init(a_other);

    return *this;

  }	//End: map_pod::operator=()


  //--------------------------------------------------------------------------------
  //	@	map_pod<K,T>::resize()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  void map_pod<K, T>::resize(int a_newSize)
  {
    T * tempData = static_cast<T*>(realloc(m_data, sizeof(T) * a_newSize));

    if (tempData == nullptr)
    {
      //TODO
    }

    K * tempKeys = static_cast<K*>(realloc(m_keys, sizeof(K) * a_newSize));

    if (tempKeys == nullptr)
    {
      //TODO
    }

    m_data = tempData;
    m_keys = tempKeys;
    m_arraySize = a_newSize;

    if (a_newSize < m_currentSize)
    {
      m_currentSize = a_newSize;
    }

  }	//End: map_pod::resize()


  //--------------------------------------------------------------------------------
  //	@	map_pod<K,T>::find()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  bool map_pod<K, T>::find(K a_key, int& a_index, int a_lower) const
  {
    return find(a_key, a_index, a_lower, (m_currentSize - 1));

  }	//End: map_pod::find()


  //--------------------------------------------------------------------------------
  //	@	map_pod<K,T>::find()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  bool map_pod<K, T>::find(K a_key, int& a_index, int a_lower, int a_upper) const
  {
    while (a_lower <= a_upper)
    {
      // calculate the midpoint for roughly equal partition
      a_index = ((a_upper + a_lower) >> 1);

      // determine which subarray to search
      if (m_keys[a_index] < a_key)
        // change min index to search upper subarray
        a_lower = a_index + 1;
      else if (m_keys[a_index] > a_key)
        // change max index to search lower subarray
        a_upper = a_index - 1;
      else
        // key found at index index
        return true;
    }

    //Set index closest (but lower) to key
    a_index = a_lower - 1;
    return false;

  }	//End: map_pod::find()


  //--------------------------------------------------------------------------------
  //	@	map_pod<K,T>::extend()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  void map_pod<K, T>::extend()
  {
    //Calculate new size
    int newSize = (m_arraySize << 1);

    //overflow, map_pod full
    if (newSize <= m_arraySize)
    {
      //TODO
    }

    resize(newSize);

  }	//End: map_pod::extend()


  //--------------------------------------------------------------------------------
  //	@	map_pod<K,T>::insert()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  bool map_pod<K, T>::insert(K a_key, T const & a_item)
  {
    //Find the index to insert to
    int index;
    if (find(a_key, index))
      return false;	//element already exists

    //Range check
    if (m_currentSize == m_arraySize)
      extend();

    //shift all RHS objects to the right by one.
    memmove(&m_data[index + 2], &m_data[index + 1], (m_currentSize - index - 1) * sizeof(T));
    memmove(&m_keys[index + 2], &m_keys[index + 1], (m_currentSize - index - 1) * sizeof(K));

    index++;

    memcpy(&m_keys[index], &a_key, sizeof(K));
    memcpy(&m_data[index], &a_item, sizeof(T));

    m_currentSize++;

    return true;

  }	//End: map_pod::insert()


  //--------------------------------------------------------------------------------
  //	@	map_pod<K,T>::erase()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  void map_pod<K, T>::erase(K a_key)
  {
    //Find the index
    int index;
    if (!find(a_key, index))
    {
      return;
    }

    memmove(&m_data[index], &m_data[index + 1], (m_currentSize - index - 1) * sizeof(T));
    memmove(&m_keys[index], &m_keys[index + 1], (m_currentSize - index - 1) * sizeof(K));

    m_currentSize--;

  }	//End: map_pod::erase()


  //--------------------------------------------------------------------------------
  //	@	map_pod<K,T>::erase_at_position()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  void map_pod<K, T>::erase_at_position(int a_i)
  {
    if (a_i > 0 && a_i < m_currentSize)
    {

      memmove(&m_keys[a_i], &m_keys[a_i + 1], (m_currentSize - a_i - 1) * sizeof(K));
      memmove(&m_data[a_i], &m_data[a_i + 1], (m_currentSize - a_i - 1) * sizeof(T));

      m_currentSize--;
    }

  }	//End: map_pod::erase_at_position()


  //--------------------------------------------------------------------------------
  //	@	map_pod<K,T>::set()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  bool map_pod<K, T>::set(K a_key, T a_item)
  {
    //Find the index to insert to
    int index;
    if (!find(a_key, index))
    {
      return false;	//element does not exist
    }
        
    memcpy(&m_data[index], &a_item, sizeof(T));

    return true;

  }	//End: map_pod::set()


  //--------------------------------------------------------------------------------
  //	@	Dgmap_p<K,T>::reset()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  void map_pod<K, T>::reset()
  {
    clear();
    resize(DG_CONTAINER_DEFAULT_SIZE);

  }	//End: map_pod::reset()


  //--------------------------------------------------------------------------------
  //	@	Dgmap_p<K,T>::clear()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  void map_pod<K, T>::clear()
  {
    m_currentSize = 0;

  }	//End: map_pod::clear()
};

#endif