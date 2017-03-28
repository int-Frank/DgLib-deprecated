//! @file Dg_vector.h
//!
//! @author Frank Hart
//! @date 22/07/2016
//!
//! Class header: DynamicArray<>

#ifndef DGDYNAMICARRAY_H
#define DGDYNAMICARRAY_H

#include <cstdlib>
#include <cstring>
#include <new>
#include <type_traits>

#include "impl/DgContainerBase.h"
#include "DgMath.h"
#include "DgErrorHandler.h"

namespace Dg
{
  //! @ingroup DgContainers
  //!
  //! @class DynamicArray
  //!
  //! @brief Contiguous array, similar to std::vector.
  //!
  //! http://www.cplusplus.com/reference/DynamicArray/DynamicArray/
  //!
  //! Similar to std::vector. Constructors/destructors are not called, so use for pod types only.
  //!
  //!
  //! @author Frank Hart
  //! @date 7/01/2014
  template<class T>
  class DynamicArray : public ContainerBase
  {
  public:
    //Constructor / destructor
    DynamicArray();

    //! Construct with a set size
    DynamicArray(size_t);
    ~DynamicArray();

    //! Copy constructor
    DynamicArray(DynamicArray const &);

    //! Assignment
    DynamicArray& operator= (DynamicArray const &);

    //! Move constructor
    DynamicArray(DynamicArray &&);

    //! Move assignment
    DynamicArray& operator= (DynamicArray &&);

    //! Access element
    T & operator[](size_t i)				{ return m_pData[i]; }

    //! Accessor, no range check.
    T const & operator[](size_t i) const	{ return m_pData[i]; }

    //! Get last element
    //! Calling this function on an empty container causes undefined behavior.
    //!
    //! @return Reference to item in the DynamicArray
    T& back() { return m_pData[m_nItems - 1]; }

    //! Get last element
    //! Calling this function on an empty container causes undefined behavior.
    //!
    //! @return const reference to item in the DynamicArray
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
    //! Calling this on an empty DynamicArray will no doubt cause a crash.
    void erase_swap(size_t);

  private:
    //! Exteneds the total size of the array (current + reserve) by a factor of 2
    void extend();

    void init(DynamicArray const &);

  private:
    //Data members
    T* m_pData;
    size_t m_nItems;
  };


  //--------------------------------------------------------------------------------
  //	@	DynamicArray<T>::DynamicArray()
  //--------------------------------------------------------------------------------
  template<class T>
  DynamicArray<T>::DynamicArray() 
    : ContainerBase()
    , m_pData(nullptr)
    , m_nItems(0)
  {
    m_pData = static_cast<T*>(malloc(pool_size() * sizeof(T)));
    DG_ASSERT(m_pData != nullptr);

  }	//End: DynamicArray::DynamicArray()


  //--------------------------------------------------------------------------------
  //	@	DynamicArray<T>::DynamicArray()
  //--------------------------------------------------------------------------------
  template<class T>
  DynamicArray<T>::DynamicArray(size_t a_size)
    : ContainerBase(a_size)
    , m_pData(nullptr)
    , m_nItems(0)
  {
    //Initialise pointers
    m_pData = static_cast<T*>(malloc(pool_size() * sizeof(T)));
    DG_ASSERT(m_pData != nullptr);

  }	//End: DynamicArray::DynamicArray()


  //--------------------------------------------------------------------------------
  //	@	DynamicArray<T>::~DynamicArray()
  //--------------------------------------------------------------------------------
  template<class T>
  DynamicArray<T>::~DynamicArray()
  {
    if (!std::is_trivially_destructible<T>::value)
    {
      for (size_t i = 0; i < m_nItems; i++)
      {
        m_pData[i].~T();
      }
    }

    free(m_pData);

  }	//End: DynamicArray::~DynamicArray()


  //--------------------------------------------------------------------------------
  //	@	DynamicArray<T>::init()
  //--------------------------------------------------------------------------------
  template<class T>
  void DynamicArray<T>::init(DynamicArray const & a_other)
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

  }	//End: DynamicArray<T>::init()


  //--------------------------------------------------------------------------------
  //	@	DynamicArray<T>::DynamicArray()
  //--------------------------------------------------------------------------------
  template<class T>
  DynamicArray<T>::DynamicArray(DynamicArray<T> const & other)
    : ContainerBase(other)
    , m_pData(nullptr)
  {
    init(other);

  }	//End: DynamicArray::DynamicArray()


  //--------------------------------------------------------------------------------
  //	@	DynamicArray<T>::DynamicArray()
  //--------------------------------------------------------------------------------
  template<class T>
  DynamicArray<T>::DynamicArray(DynamicArray<T> && other) 
    : ContainerBase(std::move(other))
    , m_pData(other.m_pData)
  {
    other.m_pData = nullptr;
    other.m_nItems = 0;
  }	//End: DynamicArray::DynamicArray()


  //--------------------------------------------------------------------------------
  //	@	DynamicArray<T>::DynamicArray()
  //--------------------------------------------------------------------------------
  template<class T>
  DynamicArray<T> & DynamicArray<T>::operator=(DynamicArray<T> && other)
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
  }	//End: DynamicArray::DynamicArray()


  //--------------------------------------------------------------------------------
  //	@	DynamicArray<T>::operator=()
  //--------------------------------------------------------------------------------
  template<class T>
  DynamicArray<T>& DynamicArray<T>::operator=(DynamicArray const & other)
  {
    if (this == &other)
      return *this;

    clear();
    ContainerBase::operator=(other);
    init(other);

    return *this;
  }	//End: DynamicArray::operator=()


  //--------------------------------------------------------------------------------
  //	@	DynamicArray<T>::push_back()
  //--------------------------------------------------------------------------------
  template<class T>
  void DynamicArray<T>::push_back(T const & a_item)
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

  }	//End: DynamicArray<T>::push_back()


  //--------------------------------------------------------------------------------
  //	@	DynamicArray<T>::pop_back
  //--------------------------------------------------------------------------------
  template<class T>
  void DynamicArray<T>::pop_back()
  {
    if (!std::is_trivially_destructible<T>::value)
    {
      m_pData[pool_size() - 1].~T();
    }
    --m_nItems;

  }	//End: DynamicArray<T>::pop_back()


  //--------------------------------------------------------------------------------
  //	@	DynamicArray<T>::clear()
  //--------------------------------------------------------------------------------
  template<class T>
  void DynamicArray<T>::clear()
  {
    if (!std::is_trivially_destructible<T>::value)
    {
      for (size_t i = 0; i < pool_size(); i++)
      {
        m_pData[i].~T();
      }
    }
    m_nItems = 0;

  }	//End: DynamicArray::clear()


  //--------------------------------------------------------------------------------
  //	@	DynamicArray<T>::resize()
  //--------------------------------------------------------------------------------
  template<class T>
  void DynamicArray<T>::resize(size_t a_size)
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

    m_pData = static_cast<T*>(realloc(m_pData, pool_size() * sizeof(T)));
    DG_ASSERT(m_pData != nullptr);
  }	//End: DynamicArray::resize()


  //--------------------------------------------------------------------------------
  //	@	DynamicArray<T>::erase_swap()
  //--------------------------------------------------------------------------------
  template<class T>
  void DynamicArray<T>::erase_swap(size_t a_ind)
  {
    if (!std::is_trivially_destructible<T>::value)
    {
      m_pData[a_ind].~T();
    }
    memmove(&m_pData[ind], &m_pData[m_nItems - 1], sizeof(T));
    --m_nItems;
  }	//End: DynamicArray::erase_swap()


  //--------------------------------------------------------------------------------
  //	@	DynamicArray<T>::extend()
  //--------------------------------------------------------------------------------
  template<class T>
  void DynamicArray<T>::extend()
  {
    set_next_pool_size();
    m_pData = static_cast<T*>(realloc(m_pData, pool_size() * sizeof(T)));
    DG_ASSERT(m_pData != nullptr);
  }	//End: DynamicArray::extend()



  //--------------------------------------------------------------------------------
  //		Helpful functions
  //--------------------------------------------------------------------------------


  //! @ingroup Containers_functions
  //!
  //! Find a value in the list, returns pointer to element.
  //!
  //! @return Pointer to object
  //!
  //! @param container DynamicArray to search
  //! @param val Item to search for
  template<class T>
  T* find(DynamicArray<T>& container, T const & val)
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
  //! Fills the DynamicArray with a value.
  //!
  //! @param container Target container
  //! @param val value to fill set each element
  template<class T>
  void fill(DynamicArray<T>& container, T const & val)
  {
    for (size_t i = 0; i < container.size(); i++)
    {
      container[i] = val;
    }

  }	//End: fill()

  template<>
  class DynamicArray<bool> : public ContainerBase
  {
    template<int T>
    struct Attr
    {
    };

    template<>
    struct Attr<1>
    {
      static int const shift = 3;
      static size_t const mask = 7;
      static int const nBits = CHAR_BIT;
    };

    template<>
    struct Attr<2>
    {
      static int const shift = 4;
      static size_t const mask = 15;
      static int const nBits = CHAR_BIT * 2;
    };

    template<>
    struct Attr<4>
    {
      static int const shift = 5;
      static size_t const mask = 31;
      static int const nBits = CHAR_BIT * 4;
    };

    template<>
    struct Attr<8>
    {
      static int const shift = 6;
      static size_t const mask = 63;
      static int const nBits = CHAR_BIT * 8;
    };

  public:

    class reference 
    {
      friend class DynamicArray<bool>;
      reference(size_t & a_rBucket, int a_bitIndex)
        : m_rBucket(a_rBucket)
        , m_bitIndex(a_bitIndex)
      {

      }

    public:

      ~reference() {}

      operator bool() const
      {
        return ((m_rBucket >> m_bitIndex) & size_t(1)) != 0;
      }

      reference & operator= (bool a_val)
      {
        size_t x = (a_val) ? 1 : 0;
        m_rBucket = m_rBucket & ~(size_t(1) << m_bitIndex) | (x << m_bitIndex);
        return *this;
      }

      reference & operator= (reference const & a_val)
      {
        size_t x = (a_val.m_rBucket >> a_val.m_bitIndex) & size_t(1);
        m_rBucket = m_rBucket & ~(size_t(1) << m_bitIndex) | (x << m_bitIndex);
        return *this;
      }

    private:

      size_t &  m_rBucket;
      int       m_bitIndex;
    };

  public:

    DynamicArray()
      : ContainerBase()
      , m_pBuckets(nullptr)
      , m_nItems(0)
    {
      m_pBuckets = static_cast<size_t*>(malloc(pool_size() * sizeof(size_t)));
      DG_ASSERT(m_pBuckets != nullptr);
    }

    //! Construct with a set size
    DynamicArray(size_t a_size)
      : ContainerBase(a_size)
      , m_pBuckets(nullptr)
      , m_nItems(0)
    {
      m_pBuckets = static_cast<size_t*>(malloc(pool_size() * sizeof(size_t)));
      DG_ASSERT(m_pBuckets != nullptr);
    }

    ~DynamicArray()
    {
      free(m_pBuckets);
    }

    //! Copy constructor
    DynamicArray(DynamicArray const & a_other)
      : ContainerBase(a_other)
      , m_pBuckets(nullptr)
    {
      init(a_other);
    }

    //! Assignment
    DynamicArray& operator= (DynamicArray const & a_other)
    {
      if (this == &a_other)
        return *this;

      clear();
      ContainerBase::operator=(a_other);
      init(a_other);

      return *this;
    }

    //! Move constructor
    DynamicArray(DynamicArray && a_other)
      : ContainerBase(std::move(a_other))
      , m_pBuckets(a_other.m_pBuckets)
    {
      a_other.m_pBuckets = nullptr;
      a_other.m_nItems = 0;
    }

    //! Move assignment
    DynamicArray& operator= (DynamicArray && a_other)
    {
      if (this != &a_other)
      {
        //Assign to this
        m_nItems = a_other.m_nItems;
        m_pBuckets = a_other.m_pBuckets;
        pool_size(a_other.pool_size());

        //Clear other
        a_other.m_pBuckets = nullptr;
        a_other.m_nItems = 0;
      }
      return *this;
    }

    //! Access element
    bool operator[](size_t i) const
    {
      size_t bucket(i >> Attr<sizeof(size_t)>::shift);
      size_t n = i & Attr<sizeof(size_t)>::mask;

      return ((m_pBuckets[bucket] >> n) & size_t(1)) != 0;
    }

    //! Access element
    reference operator[](size_t i)
    {
      size_t bucket(i >> Attr<sizeof(size_t)>::shift);
      size_t n = i & Attr<sizeof(size_t)>::mask;

      return reference(m_pBuckets[bucket], static_cast<int>(n));
    }

    void Set(size_t i, bool a_val)
    {
      size_t bucket(i >> Attr<sizeof(size_t)>::shift);
      size_t n = i & Attr<sizeof(size_t)>::mask;
      size_t x = (a_val) ? 1 : 0;

      m_pBuckets[bucket] = (m_pBuckets[bucket] & ~(size_t(1) << n)) | (x << n);
    }

    //! Get last element
    //! Calling this function on an empty container causes undefined behavior.
    //!
    //! @return Reference to item in the DynamicArray
    bool back() { return this->operator[](size() - 1); }

    //! Current size of the array
    size_t size()		const { return m_nItems; }

    //! Is the array empty
    bool empty()		const { return m_nItems == 0; }

    //! Add element to the back of the array.
    void push_back(bool a_val)
    {
      if (m_nItems == pool_size() * (Attr<sizeof(size_t)>::nBits))
      {
        extend();
      }

      Set(m_nItems, a_val);
      m_nItems++;
    }

    //! Remove element from the back of the array.
    void pop_back()
    {
      m_nItems--;
    }

    //! Current size is set to 0
    void clear()
    {
      m_nItems = 0;
    }

    //! Set the current size to 0 and the reserve to new_size
    void resize(size_t newSize)
    {
      newSize = newSize >> Attr<sizeof(size_t)>::shift;
      newSize = pool_size(newSize);
      m_pBuckets = static_cast<size_t*>(realloc(m_pBuckets, pool_size() * sizeof(size_t)));
      DG_ASSERT(m_pBuckets != nullptr);
    }

  private:

    //! Exteneds the total size of the array (current + reserve) by a factor of 2
    void extend()
    {
      set_next_pool_size();
      m_pBuckets = static_cast<size_t*>(realloc(m_pBuckets, pool_size() * sizeof(size_t)));
      DG_ASSERT(m_pBuckets != nullptr);
    }

    void init(DynamicArray const & a_other)
    {
      resize(a_other.pool_size() << Attr<sizeof(size_t)>::shift);
      size_t sze = ((a_other.m_nItems + 7) >> 3);
      memcpy(m_pBuckets, a_other.m_pBuckets, sze);
      m_nItems = a_other.m_nItems;
    }

  private:
    //Data members
    size_t *    m_pBuckets;
    size_t      m_nItems;
  };

};
#endif