//! @file Dg_set.h
//!
//! @author: Frank B. Hart
//! @date 23/05/2016
//!
//! Class declaration: set

#ifndef DG_SET_H
#define DG_SET_H

#include <cstdlib>
#include <cstring>
#include <new>
#include <type_traits>

#include "impl/DgContainerBase.h"
#include "DgErrorHandler.h"

//TODO Deprecatd
namespace Dg
{
  //! @ingroup DgContainers
  //!
  //! @class set
  //!
  //! An ordered array. Elements are inserted in order. Uses contiguous memory to 
  //! store m_pData, therefore is best used for very small m_pData types (ie literals) 
  //! and types with cheap assignment operators.
  //!
  //! @author Frank B. Hart
  //! @date 23/05/2016
  template<class T>
  class set : public ContainerBase
  {
  public:
    //! Constructor 
    //! If the constructor fails to allocate the set, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
    set();

    //! Initialize set with to a capacity. 
    //! If the constructor fails to allocate the set, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
    //! 
    //! @param[in] size Requested size of set
    set(size_t size);
    ~set();

    //! Copy constructor.
    set(set const &);

    //! Move constructor
    set(set &&);

    //! Move assignment
    set & operator=(set &&);

    //! Assigns new contents to the container, replacing its current content.
    set& operator= (set const &);

    //! Returns a reference to the \a i<SUP>th</SUP> element in the set. 
    //! This function does not perform a range check.
    //!
    //! @return Reference to the item
    //!
    //! @param[in] i Index of the item
    T& operator[](size_t i)	{ return m_pData[i]; }

    //! Returns a const reference to the \a i<SUP>th</SUP> element in the set. 
    //! This function does not perform a range check.
    //!
    //! @return const reference to the item
    //!
    //! @param[in] i Index of the item
    T const & operator[](size_t i) const { return m_pData[i]; }

    //! Return number of elements in the set.
    int size() const	{ return m_nItems; }

    //! Returns whether the set is empty.
    bool empty() const	{ return m_nItems == 0; }

    //! Searches the set for an element equivalent to \a t.
    //! @return True if the element was found with \a index being set to the 
    //!         index of the element in the set. False if not found with \a index
    //!         set to one lower to where \a t would be.
    //! @param[in] t The item to search for
    //! @param[out] index The resulting index is stored here.
    //! @param[in] lower Set a low bound to the search sublist.
    bool find(T const & t, int& index, int lower = 0) const;

    //! Searches the set for an element with a key equivalent to \a k.
    //! @return True if the element was found with \a index being set to the 
    //!         index of the element in the set. False if not found with \a index
    //!         set to one lower to where \a t would be.
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

    //! Clear all items from the set, retains allocated memory.
    void clear();

    //! Resize the set. The content of the set is preserved up 
    //! to the lesser of the new and old sizes.
    //! If the function fails to allocate memory, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
    void resize(int);

    //! Clears the set, reallocates memory to the set.
    void reset();

  private:
    //! Doubles the memory allocated to the set. Retains all data.
    //! If the function fails to allocate memory, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
    void extend();

    void init(set const &);

  private:
    //m_pData members
    T* m_pData;
    int m_nItems;
  };


  //--------------------------------------------------------------------------------
  //	@	set<T>::set()
  //--------------------------------------------------------------------------------
  template<class T>
  set<T>::set()
    : ContainerBase()
    , m_pData(nullptr)
    , m_nItems(0)
  {
    resize(static_cast<int>(pool_size()));
  }	//End: set::set()


  //--------------------------------------------------------------------------------
  //	@	set<T>::set()
  //--------------------------------------------------------------------------------
  template<class T>
  set<T>::set(size_t a_newSize)
    : ContainerBase(a_newSize)
    , m_pData(nullptr)
    , m_nItems(0)
  {
    resize(a_newSize);
  }	//End: set::set()


  //--------------------------------------------------------------------------------
  //	@	set<T>::~set()
  //--------------------------------------------------------------------------------
  template<class T>
  set<T>::~set()
  {
    free( m_pData );

  }	//End: set::~set()


  //--------------------------------------------------------------------------------
  //	@	set<T>::set()
  //--------------------------------------------------------------------------------
  template<class T>
  void set<T>::init(set const & a_other)
  {
    //Resize lists
    resize(static_cast<int>(a_other.pool_size()));

    if (std::is_trivially_copy_constructible<T>::value)
    {
      memcpy(m_pData, a_other.m_pData, a_other.m_nItems * sizeof(T));
    }
    else
    {
      for (int i = 0; i < a_other.m_nItems; i++)
      {
        new (&m_pData[i]) T(a_other.m_pData[i]);
      }
    }

    m_nItems = a_other.m_nItems;

  }	//End: set::init()


  //--------------------------------------------------------------------------------
  //	@	set<T>::set()
  //--------------------------------------------------------------------------------
  template<class T>
  set<T>::set(set const & other) 
    : ContainerBase(other)
    , m_pData(nullptr)
    , m_nItems(0)
  {
    init(other);

  }	//End: set::set()


  //--------------------------------------------------------------------------------
  //	@	set<T>::operator=()
  //--------------------------------------------------------------------------------
  template<class T>
  set<T>& set<T>::operator=(set const & other)
  {
    if (this == &other)
      return *this;

    clear();
    ContainerBase::operator=(a_other);
    init(other);

    return *this;

  }	//End: set::operator=()


  //--------------------------------------------------------------------------------
  //	@	set<T>::resize()
  //--------------------------------------------------------------------------------
  template<class T>
  void set<T>::resize(int a_newSize)
  {
    pool_size(a_newSize);
    if (pool_size() < m_nItems)
    {
      if (!std::is_trivially_destructible<T>::value)
      {
        for (int i = static_cast<int>(pool_size()); i < m_nItems; i++)
        {
          m_pData[i].~T();
        }
      }
      m_nItems = static_cast<int>(pool_size());
    }

    //Delete old m_pData
    m_pData = static_cast<T *>(realloc(m_pData, sizeof(T) * pool_size()));
    DG_ASSERT(m_pData != nullptr);

  }	//End: set::resize()


  //--------------------------------------------------------------------------------
  //	@	set<T>::find()
  //--------------------------------------------------------------------------------
  template<class T>
  bool set<T>::find(T const & a_item, int& a_index, int a_lower) const
  {
    return find(a_item, a_index, a_lower, (m_nItems - 1));

  }	//End: set::find()


  //--------------------------------------------------------------------------------
  //	@	set<T>::find()
  //--------------------------------------------------------------------------------
  template<class T>
  bool set<T>::find(T const & a_item, int& a_index, int a_lower, int a_upper) const
  {
    while (a_lower <= a_upper)
    {
      // calculate the midpoint for roughly equal partition
      a_index = ((a_upper + a_lower) >> 1);

      // determine which subarray to search
      if (m_pData[a_index] < a_item)
        // change min index to search upper subarray
        a_lower = a_index + 1;
      else if (m_pData[a_index] > a_item)
        // change max index to search lower subarray
        a_upper = a_index - 1;
      else
        // key found at index index
        return true;
    }

    //Set index closest (but lower) to key
    a_index = a_lower - 1;
    return false;

  }	//End: set::find()


  //--------------------------------------------------------------------------------
  //	@	set<T>::extend()
  //--------------------------------------------------------------------------------
  template<class T>
  void set<T>::extend()
  {
    int new_size = static_cast<int>(set_next_pool_size());
    DG_ASSERT(new_size > static_cast<int>(pool_size()));
    resize(new_size);

  }	//End: set::extend()


  //--------------------------------------------------------------------------------
  //	@	set<T>::insert()
  //--------------------------------------------------------------------------------
  template<class T>
  void set<T>::insert(T const & a_item)
  {
    //Find the index to insert to
    int index;
    find(a_item, index);

    //Range check
    if (m_nItems == pool_size())
      extend();

    //shift all RHS objects to the right by one.
    memmove(&m_pData[index + 2], &m_pData[index + 1], (m_nItems - index - 1) * sizeof(T));

    index++;

    if (std::is_trivially_copy_constructible<T>::value)
    {
      memcpy(&m_pData[index], &a_item, sizeof(T));
    }
    else
    {
      new (&m_pData[index]) T(a_item);
    }

    m_nItems++;
  }	//End: set::insert()


  //--------------------------------------------------------------------------------
  //	@	set<T>::insert_unique()
  //--------------------------------------------------------------------------------
  template<class T>
  bool set<T>::insert_unique(T const & a_item)
  {
    //Find the index to insert to
    int index;
    if (find(a_item, index))
      return false;

    //Range check
    if (m_nItems == pool_size())
      extend();

    //shift all RHS objects to the right by one.
    memmove(&m_pData[index + 2], &m_pData[index + 1], (m_nItems - index - 1) * sizeof(T));

    index++;

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

  }	//End: set::insert_unique()


  //--------------------------------------------------------------------------------
  //	@	set<T>::erase()
  //--------------------------------------------------------------------------------
  template<class T>
  void set<T>::erase(T const & a_item)
  {
    //Find the index
    int index;
    if (!find(a_item, index))
      return;	//element not found

    if (!std::is_trivially_destructible<T>::value)
    {
      m_pData[index].~T();
    }

    //Remove elements
    memmove(&m_pData[index], &m_pData[index + 1], (m_nItems - index - 1) * sizeof(T));

    //Adjust m_nItems
    m_nItems--;

  }	//End: set::erase()
  

  //--------------------------------------------------------------------------------
  //	@	set<T>::erase_all()
  //--------------------------------------------------------------------------------
  template<class T>
  void set<T>::erase_all(T const & a_item)
  {
    //Find the index
    int lowerInd, upperInd;
    if (!find(a_item, lowerInd))
      return;	//element not found

    //Initial upper bounds
    upperInd = lowerInd;

    //Find lower bounds
    while (lowerInd-- > 0 && m_pData[lowerInd] == a_item);
    lowerInd++;

    //Find upper bounds
    while (++upperInd < m_nItems && m_pData[upperInd] == a_item);
    upperInd--;

    //Number of elements to remove
    int num = upperInd - lowerInd + 1;

    if (!std::is_trivially_destructible<T>::value)
    {
      for (int i = lowerInd; i <= upperInd; i++)
      {
        m_pData[i].~T();
      }
    }

    //Remove elements
    memmove(&m_pData[lowerInd], &m_pData[upperInd + 1], (m_nItems - upperInd - 1) * sizeof(T));

    //Adjust m_nItems
    m_nItems -= num;
  }	//End: set::erase_all()


  //--------------------------------------------------------------------------------
  //	@	set<T>::reset()
  //--------------------------------------------------------------------------------
  template<class T>
  void set<T>::reset()
  {
    clear();
    resize(DG_CONTAINER_DEFAULT_SIZE);

  }	//End: set::reset()


  //--------------------------------------------------------------------------------
  //	@	set<T>::clear()
  //--------------------------------------------------------------------------------
  template<class T>
  void set<T>::clear()
  {
    if (!std::is_trivially_destructible<T>::value)
    {
      for (int i = 0; i < m_nItems; i++)
      {
        m_pData[i].~T();
      }
    }

    m_nItems = 0;

  }	//End: set::clear()
};

#endif