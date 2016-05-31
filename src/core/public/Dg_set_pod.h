//! @file Dg_set_pod.h
//!
//! @author: Frank B. Hart
//! @date 23/05/2016
//!
//! Class declaration: set_pod

#ifndef DG_SET_H
#define DG_SET_H

#include <exception>
#include <assert.h>

#include "container_common.h"

namespace Dg
{
  //! @ingroup DgContainers
  //!
  //! @class set_pod
  //!
  //! An ordered array. Elements are inserted in order. Uses contiguous memory to 
  //! store m_data, therefore is best used for very small m_data types (ie literals) 
  //! and types with cheap assignment operators.
  //!
  //! Assumed types are POD, so no construction / assignment operators called
  //!
  //! @author Frank B. Hart
  //! @date 23/05/2016
  template<class T>
  class set_pod
  {
  public:
    //! Constructor 
    //! If the constructor fails to allocate the set_pod, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
    set_pod();

    //! Initialize set_pod with to a capacity. 
    //! If the constructor fails to allocate the set_pod, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
    //! 
    //! @param[in] size Requested size of set
    set_pod(size_t size);
    ~set_pod();

    //! Copy constructor.
    set_pod(set_pod const &);

    //! Assigns new contents to the container, replacing its current content.
    set_pod& operator= (set_pod const &);

    //! Returns a reference to the \a i<SUP>th</SUP> element in the set_pod. 
    //! This function does not perform a range check.
    //!
    //! @return Reference to the item
    //!
    //! @param[in] i Index of the item
    T& operator[](size_t i)	{ return m_data[i]; }

    //! Returns a const reference to the \a i<SUP>th</SUP> element in the set_pod. 
    //! This function does not perform a range check.
    //!
    //! @return const reference to the item
    //!
    //! @param[in] i Index of the item
    T const & operator[](size_t i) const { return m_data[i]; }

    //! Return number of elements in the set_pod.
    int size() const	{ return m_currentSize; }

    //! Returns whether the set_pod is empty.
    bool empty() const	{ return m_currentSize == 0; }

    //! Returns number of elements the set_pod can hold before resizing.
    int max_size() const	{ return m_arraySize; }

    //! Searches the set for an element equivalent to \a t.
    //! @return True if the element was found with \a index being set_pod to the 
    //!         index of the element in the set_pod. False if not found with \a index
    //!         set_pod to one lower to where \a t would be.
    //! @param[in] t The item to search for
    //! @param[out] index The resulting index is stored here.
    //! @param[in] lower Set a low bound to the search sublist.
    bool find(T const & t, int& index, int lower = 0) const;

    //! Searches the set_pod for an element with a key equivalent to \a k.
    //! @return True if the element was found with \a index being set_pod to the 
    //!         index of the element in the set_pod. False if not found with \a index
    //!         set_pod to one lower to where \a t would be.
    //! @param[in] t The item to search for
    //! @param[out] index The resulting index is stored here.
    //! @param[in] lower Set a low bound to the search sublist.
    //! @param[in] upper Set an upper bound to the search sublist.
    bool find(T const & t, int& index, int lower, int upper) const;

    //! Extends the container by inserting new elements, effectively increasing 
    //! the container size by the number of elements inserted.
    //! If the function fails to allocate memory, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
    //!
    //! @param[in] t Item to insert
    void insert(T const & t);

    //! Extends the container by inserting new elements, effectively increasing 
    //! the container size by the number of elements inserted.
    //! If the function fails to allocate memory, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
    //! 
    //! @return False if t already exists in the set.
    //!
    //! @param[in] t Item to insert
    bool insert_unique(T const & t);

    //! Removes a single element equal to t from the set.
    void erase(T const & t);

    //! Removes all elements equal to t from the set.
    void erase_all(T const & t);

    //! Clear all items from the set_pod, retains allocated memory.
    void clear();

    //! Resize the set_pod. The content of the set_pod is preserved up 
    //! to the lesser of the new and old sizes.
    //! If the function fails to allocate memory, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
    void resize(int);

    //! Clears the set_pod, reallocates memory to the set_pod.
    void reset();

  private:
    //! Doubles the memory allocated to the set_pod. Retains all data.
    //! If the function fails to allocate memory, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
    void extend();

    void init(set_pod const &);

  private:
    //m_data members
    T* m_data;

    int m_arraySize;
    int m_currentSize;

  };


  //--------------------------------------------------------------------------------
  //	@	set_pod<T>::set_pod()
  //--------------------------------------------------------------------------------
  template<class T>
  set_pod<T>::set_pod()
    : m_data(nullptr)
    , m_arraySize(0)
    , m_currentSize(0)
  {
    resize(DG_CONTAINER_DEFAULT_SIZE);
  }	//End: set_pod::set_pod()


  //--------------------------------------------------------------------------------
  //	@	set_pod<T>::set_pod()
  //--------------------------------------------------------------------------------
  template<class T>
  set_pod<T>::set_pod(size_t a_newSize)
    : m_data(nullptr)
    , m_arraySize(0)
    , m_currentSize(0)
  {
    assert(a_newSize > 0);

    resize(a_newSize);

  }	//End: set_pod::set_pod()


  //--------------------------------------------------------------------------------
  //	@	set_pod<T>::~set_pod()
  //--------------------------------------------------------------------------------
  template<class T>
  set_pod<T>::~set_pod()
  {
    //Free memory
    free( m_data );

  }	//End: set_pod::~set_pod()


  //--------------------------------------------------------------------------------
  //	@	set_pod<T>::set_pod()
  //--------------------------------------------------------------------------------
  template<class T>
  void set_pod<T>::init(set_pod const & other)
  {
    //Resize lists
    int sze = (other.m_arraySize>0) ? other.m_arraySize : 1;
    resize(sze);

    memcpy(m_data, other.m_data, other.m_currentSize * sizeof(T));

    m_currentSize = other.m_currentSize;

  }	//End: set_pod::init()


  //--------------------------------------------------------------------------------
  //	@	set_pod<T>::set_pod()
  //--------------------------------------------------------------------------------
  template<class T>
  set_pod<T>::set_pod(set_pod const & other) :
    m_data(nullptr), m_arraySize(0), m_currentSize(0)
  {
    init(other);

  }	//End: set_pod::set_pod()


  //--------------------------------------------------------------------------------
  //	@	set_pod<T>::operator=()
  //--------------------------------------------------------------------------------
  template<class T>
  set_pod<T>& set_pod<T>::operator=(set_pod const & other)
  {
    if (this == &other)
      return *this;

    init(other);

    return *this;

  }	//End: set_pod::operator=()


  //--------------------------------------------------------------------------------
  //	@	set_pod<T>::resize()
  //--------------------------------------------------------------------------------
  template<class T>
  void set_pod<T>::resize(int a_newSize)
  {
    assert(a_newSize > 0);

    //Delete old m_data
    T * tempPtr = static_cast<T *>(realloc(m_data, sizeof(T) * a_newSize));

    if (tempPtr == nullptr)
    {
      throw std::bad_alloc();
    }

    m_data = tempPtr;
    m_arraySize = a_newSize;
    if (a_newSize < m_currentSize)
    {
      m_currentSize = a_newSize;
    }

  }	//End: set_pod::resize()


  //--------------------------------------------------------------------------------
  //	@	set_pod<T>::find()
  //--------------------------------------------------------------------------------
  template<class T>
  bool set_pod<T>::find(T const & a_item, int& a_index, int a_lower) const
  {
    return find(a_item, a_index, a_lower, (m_currentSize - 1));

  }	//End: set_pod::find()


  //--------------------------------------------------------------------------------
  //	@	set_pod<T>::find()
  //--------------------------------------------------------------------------------
  template<class T>
  bool set_pod<T>::find(T const & a_item, int& a_index, int a_lower, int a_upper) const
  {
    while (a_lower <= a_upper)
    {
      // calculate the midpoint for roughly equal partition
      a_index = ((a_upper + a_lower) >> 1);

      // determine which subarray to search
      if (m_data[a_index] < a_item)
        // change min index to search upper subarray
        a_lower = a_index + 1;
      else if (m_data[a_index] > a_item)
        // change max index to search lower subarray
        a_upper = a_index - 1;
      else
        // key found at index index
        return true;
    }

    //Set index closest (but lower) to key
    a_index = a_lower - 1;
    return false;

  }	//End: set_pod::find()


  //--------------------------------------------------------------------------------
  //	@	set_pod<T>::extend()
  //--------------------------------------------------------------------------------
  template<class T>
  void set_pod<T>::extend()
  {
    //Calculate new size
    int new_size = (m_arraySize << 1);

    //overflow, map_p full
    if (new_size <= m_arraySize)
    {
      throw std::overflow_error("m_arraySize");
    }

    T * tempPtr = static_cast<T*>(realloc(m_data, sizeof(T) * new_size));
    if (tempPtr == nullptr)
    {
      throw std::bad_alloc();
    }

    m_data = tempPtr;
    m_arraySize = new_size;

  }	//End: set_pod::extend()


  //--------------------------------------------------------------------------------
  //	@	set_pod<T>::insert()
  //--------------------------------------------------------------------------------
  template<class T>
  void set_pod<T>::insert(T const & a_item)
  {
    //Find the index to insert to
    int index;
    find(a_item, index);

    //Range check
    if (m_currentSize == m_arraySize)
      extend();

    //shift all RHS objects to the right by one.
    memmove(&m_data[index + 2], &m_data[index + 1], (m_currentSize - index - 1) * sizeof(T));

    index++;

    memcpy(&m_data[index], &a_item, sizeof(T));

    m_currentSize++;

    return true;

  }	//End: set_pod::insert()


  //--------------------------------------------------------------------------------
  //	@	set_pod<T>::insert_unique()
  //--------------------------------------------------------------------------------
  template<class T>
  bool set_pod<T>::insert_unique(T const & item)
  {
    //Find the index to insert to
    int index;
    if (find(item, index))
      return false;

    //Range check
    if (m_currentSize == m_arraySize)
      extend();

    //shift all RHS objects to the right by one.
    memmove(&m_data[index + 2], &m_data[index + 1], (m_currentSize - index - 1) * sizeof(T));

    index++;

    memcpy(&m_data[index], &item, sizeof(T));

    m_currentSize++;

    return true;

  }	//End: set_pod::insert_unique()


  //--------------------------------------------------------------------------------
  //	@	set_pod<T>::erase()
  //--------------------------------------------------------------------------------
  template<class T>
  void set_pod<T>::erase(T const & a_item)
  {
    //Find the index
    int index;
    if (!find(a_item, index))
      return;	//element not found

    //Remove elements
    memmove(&m_data[index], &m_data[index + 1], (m_currentSize - index - 1) * sizeof(T));

    //Adjust m_currentSize
    m_currentSize--;

  }	//End: set_pod::erase()
  

  //--------------------------------------------------------------------------------
  //	@	set_pod<T>::erase_all()
  //--------------------------------------------------------------------------------
  template<class T>
  void set_pod<T>::erase_all(T const & a_item)
  {
    //Find the index
    int lower, upper;
    if (!find(a_item, lower))
      return false;	//element not found

    //Initial upper bounds
    upper = lower + 1;

    //Find lower bounds
    while (lower >= 0 && m_data[lower] == a_item)
      --lower;

    //Find upper bounds
    while (upper <= m_currentSize && m_data[upper] == a_item)
      ++upper;

    //Number of elements to remove
    lower++;
    int num = upper - lower;

    //Remove elements
    memmove(&m_data[lower], &m_data[lower + num], (m_currentSize - index - 1 - num) * sizeof(T));

    //Adjust m_currentSize
    m_currentSize -= num;

    return true;

  }	//End: set_pod::erase_all()


  //--------------------------------------------------------------------------------
  //	@	set_pod<T>::reset()
  //--------------------------------------------------------------------------------
  template<class T>
  void set_pod<T>::reset()
  {
    clear();
    resize(DG_CONTAINER_DEFAULT_SIZE);

  }	//End: set_pod::reset()


  //--------------------------------------------------------------------------------
  //	@	set_pod<T>::clear()
  //--------------------------------------------------------------------------------
  template<class T>
  void set_pod<T>::clear()
  {
    m_currentSize = 0;

  }	//End: set_pod::clear()
};

#endif