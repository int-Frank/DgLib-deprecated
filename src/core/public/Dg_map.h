//! @file Dg_map.h
//!
//! @author: Frank B. Hart
//! @date 2/5/2015
//!
//! Class declaration: map

#ifndef DG_MAP_H
#define DG_MAP_H

#include <stdexcept>

#define DG_CONTAINER_DEFAULT_SIZE 1024

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
  class map
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

    //! Returns a reference to the \a i<SUP>th</SUP> element in the map. 
    //! This function does not perform a range check.
    //!
    //! @return Reference to the item
    //!
    //! @param[in] i Index of the item
    T& operator[](int i)	{ return m_data[i]; }

    //! Returns a const reference to the \a i<SUP>th</SUP> element in the map. 
    //! This function does not perform a range check.
    //!
    //! @return const reference to the item
    //!
    //! @param[in] i Index of the item
    T const & operator[](int i) const { return m_data[i]; }

    //! Return number of elements in the map.
    int size() const	{ return m_currentSize; }

    //! Returns whether the map is empty.
    bool empty() const	{ return m_currentSize == 0; }

    //! Returns number of elements the map can hold before resizing.
    int max_size() const	{ return m_arraySize; }

    //! Returns the key of the ith element in the map.
    //! This function does not perform a range check.
    //!
    //! @return Key
    //!
    //! @param[in] i Index of item to query
    K const & query_key(int i)	const { return m_keys[i]; }

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
    K*  m_keys;
    T*  m_data;

    int m_arraySize;
    int m_currentSize;
  };


  //--------------------------------------------------------------------------------
  //	@	map<K,T>::map()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  map<K, T>::map()
    : m_data(nullptr)
    , m_keys(nullptr)
    , m_arraySize(0)
    , m_currentSize(0)
  {
    resize(DG_CONTAINER_DEFAULT_SIZE);

  }	//End: map::map()


  //--------------------------------------------------------------------------------
  //	@	map<K,T>::map()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  map<K, T>::map(int a_size)
    : m_data(nullptr)
    , m_keys(nullptr)
    , m_arraySize(0)
    , m_currentSize(0)
  {
    T * tempData = static_cast<T*>(malloc(sizeof(T) * a_size));

    if (tempData == nullptr)
    {
      //throw std::bad_alloc();
    }

    K * tempKeys = static_cast<K*>(malloc(sizeof(K) * a_size));

    if (tempKeys == nullptr)
    {
      //throw std::bad_alloc();
    }

    m_data = tempData;
    m_keys = tempKeys;
    m_arraySize = a_size;
    m_currentSize = 0;

  }	//End: map::map()


  //--------------------------------------------------------------------------------
  //	@	map<K,T>::~map()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  map<K, T>::~map()
  {
    for (int i = 0; i < m_currentSize; ++i)
    {
      m_keys[i].~K();
      m_data[i].~T();
    }

    free(m_data);
    free(m_keys);

  }	//End: map::~map()


  //--------------------------------------------------------------------------------
  //	@	map<K,T>::init()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  void map<K, T>::init(map const & a_other)
  {
    clear();
    resize(a_other.m_arraySize);

    for (int i = 0; i < a_other.m_currentSize; ++i)
    {
      new (&m_keys[i]) K(a_other.m_keys[i]);
      new (&m_data[i]) T(a_other.m_data[i]);
    }

    m_currentSize = a_other.m_currentSize;

  }	//End: map::init()


  //--------------------------------------------------------------------------------
  //	@	map<K,T>::map()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  map<K, T>::map(map const & a_other)
    : m_data(nullptr)
    , m_keys(nullptr)
    , m_arraySize(0)
    , m_currentSize(0)
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

    init(a_other);

    return *this;

  }	//End: map::operator=()


  //--------------------------------------------------------------------------------
  //	@	map<K,T>::resize()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  void map<K, T>::resize(int a_newSize)
  {
    T * tempData = static_cast<T*>(realloc(m_data, sizeof(T) * a_newSize));

    if (tempData == nullptr)
    {
      //throw std::bad_alloc();
    }

    K * tempKeys = static_cast<K*>(realloc(m_keys, sizeof(K) * a_newSize));

    if (tempKeys == nullptr)
    {
      //throw std::bad_alloc();
    }

    m_data = tempData;
    m_keys = tempKeys;
    m_arraySize = a_newSize;

    if (a_newSize < m_currentSize)
    {
      m_currentSize = a_newSize;
    }

  }	//End: map::resize()


  //--------------------------------------------------------------------------------
  //	@	map<K,T>::find()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  bool map<K, T>::find(K a_key, int& a_index, int a_lower) const
  {
    return find(a_key, a_index, a_lower, (m_currentSize - 1));

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

  }	//End: map::find()


  //--------------------------------------------------------------------------------
  //	@	map<K,T>::extend()
  //--------------------------------------------------------------------------------
  template<typename K, typename T>
  void map<K, T>::extend()
  {
    //Calculate new size
    int newSize = (m_arraySize << 1);

    //overflow, map full
    if (newSize <= m_arraySize)
    {
      //throw std::overflow_error("m_arraySize");
    }

    resize(newSize);

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
    if (m_currentSize == m_arraySize)
      extend();

    //shift all RHS objects to the right by one.
    memmove(&m_data[index + 2], &m_data[index + 1], (m_currentSize - index - 1) * sizeof(T));
    memmove(&m_keys[index + 2], &m_keys[index + 1], (m_currentSize - index - 1) * sizeof(K));

    index++;

    //Construct new element.
    new (&m_keys[index]) K(a_key);
    new (&m_data[index]) T(a_item);

    m_currentSize++;

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

    //Destroy element
    m_keys[index].~K();
    m_data[index].~T();

    memmove(&m_data[index], &m_data[index + 1], (m_currentSize - index - 1) * sizeof(T));
    memmove(&m_keys[index], &m_keys[index + 1], (m_currentSize - index - 1) * sizeof(K));

    m_currentSize--;

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

    m_data[index] = a_item;

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
    for (int i = 0; i < m_currentSize; ++i)
    {
      m_keys[i].~K();
      m_data[i].~T();
    }

    m_currentSize = 0;

  }	//End: map::clear()
};

#endif