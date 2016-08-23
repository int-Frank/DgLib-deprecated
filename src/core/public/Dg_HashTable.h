//! @file Dg_HashTable.h
//!
//! @author Frank Hart
//! @date 22/08/2016
//!
//! Class declaration: HashTable

#ifndef DGHashTable
#define DGHashTable

#define DGHASHTABLE_ENABLE_OUTPUT

#include <cstdlib>
#include <cstring>
#ifdef DGHASHTABLE_ENABLE_OUTPUT
#include <iostream>
#endif
#include "DgErrorHandler.h"

#define ARRAY_SIZE(a) sizeof(a) / sizeof(*a)

namespace Dg
{
  template<typename K>
  using fHasher = size_t(*)(K);

  //! @ingroup DgContainers_functions
  //!
  //! The default hasher simply returns the key cast to a size_t. 
  //! In a the table, the result will be modded against the (prime)
  //! number of buckets. 
  template<typename K>
  size_t DefaultHasher(K key)
  {
    return static_cast<size_t>(key);
  }

  namespace impl
  {
    //! An array listing possible values for the number of buckets.
    //! The ith value is the highest prime number closest to but below <2^(i/2 + 4)>
    static size_t const validBucketCounts[] = 
    {
      0x3, 0x5, 0x7, 0xb, 0xd,
      0x13, 0x1f, 0x2b, 0x3d, 0x59, 0x7f, 0xb5, 0xfb,
      0x167, 0x1fd, 0x2cf, 0x3fd, 0x5a7, 0x7f7, 0xb47, 0xffd,
      0x169f, 0x1fff, 0x2d3b, 0x3ffd, 0x5a7f, 0x7fed, 0xb501, 0xfff1,
      0x16a09, 0x1ffff, 0x2d413, 0x3fffb, 0x5a823, 0x7ffff, 0xb5037, 0xffffd,
      0x16a09b, 0x1ffff7, 0x2d413b, 0x3ffffd, 0x5a8279, 0x7ffff1, 0xb504ef, 0xfffffd,
      0x16a09e1, 0x1ffffd9, 0x2d413b7, 0x3fffffb, 0x5a82789, 0x7ffffd9, 0xb504f1b, 0xfffffc7,
      0x16a09e57, 0x1ffffffd, 0x2d413cbf, 0x3fffffdd, 0x5a827975, 0x7fffffff, 0xb504f32d, 0xfffffffb,
      0x16a09e655, 0x1fffffff7, 0x2d413ccab, 0x3ffffffd7, 0x5a827995d, 0x7ffffffe1, 0xb504f3321, 0xffffffffb,
      0x16a09e665b, 0x1fffffffe7, 0x2d413cccd1, 0x3fffffffd3, 0x5a827999f9, 0x7ffffffff9, 0xb504f333ef, 0xffffffffa9,
      0x16a09e667d7, 0x1ffffffffeb, 0x2d413cccfe1, 0x3fffffffff5, 0x5a827999fb3, 0x7ffffffffc7, 0xb504f333f99, 0xfffffffffef,
      0x16a09e667f05, 0x1fffffffffc9, 0x2d413cccfe73, 0x3fffffffffeb, 0x5a827999fce9, 0x7fffffffff8d, 0xb504f333f9dd, 0xffffffffffc5,
      0x16a09e667f3b3, 0x1ffffffffffaf, 0x2d413cccfe763, 0x3ffffffffffe5, 0x5a827999fcef9, 0x7ffffffffff7f, 0xb504f333f9d9b,
      0x10000000000015, 0x16a09e667f3bc9, 0x20000000000039, 0x2d413cccfe77f1, 0x3fffffffffffdf, 0x5a827999fcefa9, 0x80000000000099, 0xb504f333f9dfe3,
      0x100000000000213, 0x16a09e667f3bfdf, 0x200000000000429, 0x2d413cccfe77fd1, 0x4000000000008f1, 0x5a827999fcefff3, 0x8000000000011f9, 0xb504f333f9dfff7,
      0x10000000000024f1, 0x16a09e667f3c00ef, 0x2000000000004be1, 0x2d413cccfe7803f7, 0x40000000000097f3, 0x5a827999fcf00bff, 0x80000000000137db, 0xb504f333f9e01ffb, 0x7fffffffffffffe7
    };
  }

  //! @ingroup DgContainers
  //! 
  //! Much of this documentation is taken directly from 
  //! http://www.cplusplus.com/reference/unordered_map/unordered_map/
  //! as these containers share a lot of the same functions.
  //!
  //! HashTables are associative containers that store elements formed by 
  //! the combination of a key value and a mapped value, and which allows for 
  //! fast retrieval of individual elements based on their keys.
  //! 
  //! In a HashTable, the key value is generally used to uniquely identify 
  //! the element, while the mapped value is an object with the content 
  //! associated to this key. Types of key and mapped value may differ.
  //! 
  //! Internally, the elements in the HashTable are not sorted in any 
  //! particular order with respect to either their key or mapped values, 
  //! but organized into buckets depending on their hash values to allow for 
  //! fast access to individual elements directly by their key 
  //! values (with a constant average time complexity on average).
  //!
  //! HashTable containers are faster than map containers to access individual 
  //! elements by their key, although they are generally less efficient for 
  //! range iteration through a subset of their elements.
  //!
  //! HashTables implement the direct access operator (operator[]) which 
  //! allows for direct access of the mapped value using its key value as argument.
  //!
  //! @author Frank Hart
  //! @date 22/08/2016
  template
  <
    //! Key type
      typename K
    //! Data type
    , typename T
    //! Plain Old Data. If not POD, constructors and destructors will be called.
    , bool POD = true
  >
  class HashTable
  {
    struct Node
    {
      K       key;
      T       data;
      Node *  pNext;
    };

    struct Bucket
    {
      Node * pBegin;
    };

    static float  const   s_defaultLF;
    static float  const   s_minSetLF;
    static size_t const   s_minPoolSize;
    static int const      s_invalidBucketIndex;

  public:

    //! @class const_iterator
    //!
    //! Constant iterator for the HashTable. Iterating past the last element in the HashTable
    //! or before the first element will result in returning the HashTable::cend() iterator.
    //! Incrementing past the HashTable::cend() iterator will do nothing. Decrementing the 
    //! HashTable::cend() will give the last element in the list.
    //!
    //! @author Frank Hart
    //! @date 22/08/2016
    class const_iterator
    {
      friend class HashTable;

      //! Private constructor.
      const_iterator(Node * a_pNode
        , size_t a_bucketIndex
        , size_t a_bucketCount
        , Bucket * a_pBuckets)
        : m_pNode(a_pNode)
        , m_bucketIndex(a_bucketIndex)
        , m_bucketCount(a_bucketCount)
        , m_pBuckets(a_pBuckets)
      {}

    public:

      //! Default constructor.
      const_iterator()
        : m_pNode(nullptr)
        , m_bucketIndex(0)
        , m_bucketCount(0)
        , m_pBuckets(nullptr)
      {}

      ~const_iterator() {}

      //! Copy constructor.
      const_iterator(const_iterator const & it)
        : m_pNode(it.m_pNode)
        , m_bucketIndex(it.m_bucketIndex)
        , m_bucketCount(it.m_bucketCount)
        , m_pBuckets(it.m_pBuckets)
      {}

      //! Assignment.
      const_iterator& operator= (const_iterator const & it)
      {
        m_pNode = it.m_pNode;
        m_bucketIndex = it.m_bucketIndex;
        m_bucketCount = it.m_bucketCount;
        m_pBuckets = it.m_pBuckets;
        return *this;
      }

      //! Comparison.
      bool operator==(const_iterator const & it) const
      {
        return m_pNode == it.m_pNode
          && m_pBuckets == it.m_pBuckets;
      }

      //! Comparison.
      bool operator!=(const_iterator const & it) const
      {
        return m_pNode != it.m_pNode
          || m_pBuckets != it.m_pBuckets;
      }

      //! Post increment. Iterating past the last element in the HashTable will result in 
      //! returning the HashTable::cend() iterator. Incrementing past the HashTable::cend() 
      //! iterator will do nothing. 
      const_iterator operator++(int)
      {
        const_iterator result(*this);
        ++(*this);
        return result;
      }

      //! Pre increment. Iterating past the last element in the HashTable will result in 
      //! returning the HashTable::cend() iterator. Incrementing past the HashTable::cend() 
      //! iterator will do nothing. 
      const_iterator & operator++()
      {
        if (!m_pNode)
        {
          return *this;
        }

        if (m_pNode->pNext)
        {
          m_pNode = m_pNode->pNext;
          return *this;
        }

        m_bucketIndex++;
        for (m_bucketIndex; m_bucketIndex < m_bucketCount; m_bucketIndex++)
        {
          if (m_pBuckets[m_bucketIndex].pBegin)
          {
            m_pNode = m_pBuckets[m_bucketIndex].pBegin;
            return *this;
          }
        }

        //End
        m_bucketIndex = m_bucketCount - 1;
        m_pNode = nullptr;
        return *this;
      }

      //! Post decrement. Decrementing the HashTable::cbegin() iterator will result in returning the 
      //! HashTable::cend() iterator.
      const_iterator operator--(int)
      {
        const_iterator result(*this);
        --(*this);
        return result;
      }

      //! Pre decrement. Decrementing the HashTable::cbegin() iterator will result in returning the 
      //! HashTable::cend() iterator.
      const_iterator & operator--()
      {
        Node * curNode = m_pBuckets[m_bucketIndex].pBegin;
        while (curNode)
        {
          if (curNode->pNext == m_pNode)
          {
            m_pNode = curNode;
            return *this;
          }
          curNode = curNode->pNext;
        }

        for (m_bucketIndex; m_bucketIndex-- > 0;)
        {
          Node * curNode = m_pBuckets[m_bucketIndex].pBegin;
          while (curNode)
          {
            if (!curNode->pNext)
            {
              m_pNode = curNode;
              return *this;
            }
            curNode = curNode->pNext;
          }
        }

        //End
        m_pNode = nullptr;
        m_bucketIndex = m_bucketCount - 1;
        return *this;
      }

      //! Conversion
      T const * operator->() const { return &m_pNode->data; }

      //! Conversion
      T const & operator*() const { return m_pNode->data; }

    private:

      /// The 'end' const_iterator is denoted by:
      ///   m_pBuckets  = a valid pointer
      ///   m_pNode     = nullptr

      size_t          m_bucketIndex;
      size_t          m_bucketCount;
      Bucket *        m_pBuckets;
      Node const *    m_pNode;
    };


    //! @class iterator
    //!
    //! Iterator for the HashTable. Iterating past the last element in the HashTable
    //! or before the first element will result in returning the HashTable::end() iterator.
    //! Incrementing past the HashTable::end() iterator will do nothing. Decrementing the 
    //! HashTable::end() will give the last element in the list.
    //!
    //! @author Frank Hart
    //! @date 22/08/2016
    class iterator
    {
      friend class HashTable;

      //! Private constructor.
      iterator(Node * a_pNode
        , size_t a_bucketIndex
        , size_t a_bucketCount
        , Bucket * a_pBuckets)
        : m_pNode(a_pNode)
        , m_bucketIndex(a_bucketIndex)
        , m_bucketCount(a_bucketCount)
        , m_pBuckets(a_pBuckets)
      {}

    public:

      //! Default constructor.
      iterator()
        : m_pNode(nullptr)
        , m_bucketIndex(0)
        , m_bucketCount(0)
        , m_pBuckets(nullptr)
      {}

      ~iterator() {}

      //! Copy constructor.
      iterator(iterator const & it)
        : m_pNode(it.m_pNode)
        , m_bucketIndex(it.m_bucketIndex)
        , m_bucketCount(it.m_bucketCount)
        , m_pBuckets(it.m_pBuckets)
      {}

      //! Assignment.
      iterator& operator= (iterator const & it)
      {
        m_pNode = it.m_pNode;
        m_bucketIndex = it.m_bucketIndex;
        m_bucketCount = it.m_bucketCount;
        m_pBuckets = it.m_pBuckets;
        return *this;
      }

      //! Comparison.
      bool operator==(iterator const & it) const
      {
        return m_pNode == it.m_pNode
          && m_pBuckets == it.m_pBuckets;
      }

      //! Comparison.
      bool operator!=(iterator const & it) const
      {
        return m_pNode != it.m_pNode
          || m_pBuckets != it.m_pBuckets;
      }

      //! Post increment. Iterating past the last element in the HashTable will result in 
      //! returning the HashTable::end() iterator. Incrementing past the HashTable::end() 
      //! iterator will do nothing. 
      iterator operator++(int)
      {
        iterator result(*this);
        ++(*this);
        return result;
      }

      //! Pre increment. Iterating past the last element in the HashTable will result in 
      //! returning the HashTable::end() iterator. Incrementing past the HashTable::end() 
      //! iterator will do nothing. 
      iterator & operator++()
      {
        if (!m_pNode)
        {
          return *this;
        }

        if (m_pNode->pNext)
        {
          m_pNode = m_pNode->pNext;
          return *this;
        }

        m_bucketIndex++;
        for (m_bucketIndex; m_bucketIndex < m_bucketCount; m_bucketIndex++)
        {
          if (m_pBuckets[m_bucketIndex].pBegin)
          {
            m_pNode = m_pBuckets[m_bucketIndex].pBegin;
            return *this;
          }
        }

        //End
        m_bucketIndex = m_bucketCount - 1;
        m_pNode = nullptr;
        return *this;
      }

      //! Post decrement. Decrementing the HashTable::begin() iterator will result in returning the 
      //! HashTable::end() iterator.
      iterator operator--(int)
      {
        iterator result(*this);
        --(*this);
        return result;
      }

      //! Pre decrement. Decrementing the HashTable::begin() iterator will result in returning the 
      //! HashTable::end() iterator.
      iterator & operator--()
      {
        Node * curNode = m_pBuckets[m_bucketIndex].pBegin;
        while (curNode)
        {
          if (curNode->pNext == m_pNode)
          {
            m_pNode = curNode;
            return *this;
          }
          curNode = curNode->pNext;
        }

        for (m_bucketIndex; m_bucketIndex-- > 0;)
        {
          Node * curNode = m_pBuckets[m_bucketIndex].pBegin;
          while (curNode)
          {
            if (!curNode->pNext)
            {
              m_pNode = curNode;
              return *this;
            }
            curNode = curNode->pNext;
          }
        }

        //End
        m_pNode = nullptr;
        m_bucketIndex = m_bucketCount - 1;
        return *this;
      }

      //! Conversion to const_iterator.
      operator const_iterator() const
      {
      return const_iterator(m_pNode
                          , m_bucketIndex
                          , m_bucketCount
                          , m_pBuckets);
      } 

      //! Conversion
      T* operator->() const { return %m_pNode->data; }

      //! Conversion
      T& operator*() const { return m_pNode->data ; }

    private:

      /// The 'end' iterator is denoted by:
      ///   m_pBuckets  = a valid pointer
      ///   m_pNode     = nullptr

      size_t          m_bucketIndex;
      size_t          m_bucketCount;
      Bucket *        m_pBuckets;
      Node *          m_pNode;
    };


  public:

    //! Default constructor.
    HashTable()
      : m_pNodes(nullptr)
      , m_pBuckets(nullptr)
      , m_pNextFree(nullptr)
      , m_poolSize(0)
      , m_bucketCountIndex(s_invalidBucketIndex)
      , m_nItems(0)
      , m_fHasher(DefaultHasher)
      , m_maxLoadFactor(s_defaultLF)
    {
      init(impl::validBucketCounts[0]);
    }

    //! Constructor.
    //!
    //! @param[in] a_nBuckets Request the buckets to initialise the table with.
    //!                       The actual number will be from impl::validBucketCounts,
    //!                       and will be the value closest to but not below a_nBuckets.
    //! @param[in] a_hf       A custom hash function.
    HashTable(size_t a_nBuckets, fHasher<K> * a_hf = DefaultHasher)
      : m_pNodes(nullptr)
      , m_pBuckets(nullptr)
      , m_pNextFree(nullptr)
      , m_poolSize(0)
      , m_bucketCountIndex(s_invalidBucketIndex)
      , m_nItems(0)
      , m_fHasher(a_hf)
      , m_maxLoadFactor(s_defaultLF)
    {
      init(a_nBuckets);
    }

    //! Destructor.
    ~HashTable()
    {
      Wipe();
    }

    //! Copy constructor.
    HashTable(HashTable const & a_other)
      : m_pNodes(nullptr)
      , m_pBuckets(nullptr)
      , m_pNextFree(nullptr)
      , m_poolSize(0)
      , m_bucketCountIndex(s_invalidBucketIndex)
      , m_nItems(0)
      , m_fHasher(DefaultHasher)
      , m_maxLoadFactor(s_defaultLF)
    {
      init(a_other);
    }

    //! Assignment.
    HashTable & operator=(HashTable const & a_other)
    {
      if (this != &a_other)
      {
        Wipe();
        init(a_other);
      }
      return *this;
    }

    //! Copy move operator.
    HashTable(HashTable && a_other)
      : m_pNodes(a_other.m_pNodes)
      , m_pBuckets(a_other.m_pBuckets)
      , m_pNextFree(a_other.m_pNextFree)
      , m_poolSize(a_other.m_poolSize)
      , m_bucketCountIndex(a_other.m_bucketCountIndex)
      , m_nItems(a_other.m_nItems)
      , m_fHasher(a_other.m_fHasher)
      , m_maxLoadFactor(a_other.m_maxLoadFactor)
    {
      PostMove(a_other);
    }

    //! Assignment move operator.
    HashTable & operator=(HashTable && a_other)
    {
      if (this != &a_other)
      {
        Wipe();

        m_pNodes = a_other.m_pNodes;
        m_pBuckets = a_other.m_pBuckets;
        m_pNextFree = a_other.m_pNextFree;
        m_poolSize = a_other.m_poolSize;
        m_bucketCountIndex = a_other.m_bucketCountIndex;
        m_nItems = a_other.m_nItems;
        m_fHasher = a_other.m_fHasher;
        m_maxLoadFactor = a_other.m_maxLoadFactor;

        PostMove(a_other);
      }
      return *this;
    }

    //! Accessor. Returns element mapped to the key. Will insert element
    //! if the key is not present in the HashTable.
    T & operator[](K key)
    {
      bool existed(false);
      Node * pNode(GetItem(key, existed));
      if (!existed)
      {
        if (POD)
        {
          memset(&pNode->data, 0, sizeof(T));
        }
        else
        {
          new (&pNode->data) T();
        }
      }
      return pNode->data;
    }

    //! Get iterator to the first element of the HashTable.
    //! Returns HashTable::end() if the HashTable is empty.
    iterator begin()
    {
      for (size_t i = 0; i < bucket_count(); ++i)
      {
        if (m_pBuckets[i].pBegin)
        {
          return iterator(m_pBuckets[i].pBegin, i, bucket_count(), m_pBuckets);
        }
      }
      return end();
    }

    //! Get const_iterator to the first element of the HashTable.
    //! Returns HashTable::cend() if the HashTable is empty.
    const_iterator cbegin() const
    {
      for (size_t i = 0; i < bucket_count(); ++i)
      {
        if (m_pBuckets[i].pBegin)
        {
          return const_iterator(m_pBuckets[i].pBegin, i, bucket_count(), m_pBuckets);
        }
      }
      return cend();
    }

    //! Get iterator to one past the last element in the HashTable.
    iterator end()
    {
      return iterator(nullptr, bucket_count() - 1, bucket_count(), m_pBuckets);
    }

    //! Get const_iterator to one past the last element in the HashTable.
    const_iterator cend() const
    {
      return const_iterator(nullptr, bucket_count() - 1, bucket_count(), m_pBuckets);
    }

    //! Returns a pointer to the element in the map which matches the key.
    //! Returns a nullptr if there is no match.
    T * at(K a_key)
    {
      size_t ind = m_fHasher(a_key);
      ind %= bucket_count();

      Node * pItem = m_pBuckets[ind].pBegin;
      while (pItem)
      {
        if (pItem->key == a_key)  return &pItem->data;
        if (!pItem->pNext)        return nullptr;
        pItem = pItem->pNext;
      }
      return nullptr;
    }

    //! Insert a mapped value into the HashTable. This function will overwrite 
    //! any existing element mapped to this key.
    //!
    //! @return Pointer to the newly inserted element.
    T * insert(K key, T const & val)
    {
      bool existed(false);
      Node * pNode(GetItem(key, existed));
      if (!POD)
      {
        if (existed)
        {
          pNode->data.~T();
        }
        new (&pNode->data) T(val);
      }
      else
      {
        memcpy(&pNode->data, &val, sizeof(T));
      }
      return &pNode->data;
    }

    //! Insert a mapped value into the HashTable. This function will NOT overwrite 
    //! any existing element mapped to this key.
    //!
    //! @return Pointer to eith  the newly inserted element, or existing.
    T * insert_no_overwrite(K key, T const & val)
    {
      bool existed(false);
      Node * pNode(GetItem(key, existed));
      if (!existed)
      {
        if (!POD)
        {
          new (&pNode->data) T(val);
        }
        else
        {
          memcpy(&pNode->data, &val, sizeof(T));
        }
      }
      return &pNode->data;
    }

    //! Clear all elements. For non-POD HashTables, this function will call
    //! destructors of all current element items in the HashTable. For POD types,
    //! essentially, the item counts are set to 0.
    void clear()
    {
      if (!POD)
      {
        iterator it = begin();
        for (it; it != end(); ++it)
        {
          *it.~T();
        }
      }
      for (size_t i = 0; i < m_poolSize - 1; ++i)
      {
        m_pNodes[i].pNext = &m_pNodes[i + 1];
      }
      m_pNodes[m_poolSize - 1].pNext - nullptr;
      memset(m_pBuckets, 0, bucket_count() * sizeof(Bucket));
      m_nItems = 0;
    }

    //! Is the HashTable empty?
    bool empty() const
    {
      return m_nItems == 0;
    }

    //! Returns the current load factor in the unordered_map container.
    //!
    //! The load factor is the ratio between the number of elements in the 
    //! container(its size) and the number of buckets(bucket_count) :
    //!
    //!     load_factor = size / bucket_count
    //!
    //! The load factor influences the probability of collision in the HashTable
    //! (i.e., the probability of two elements being located in the same bucket).
    //!
    //! The container automatically increases the number of buckets to keep the 
    //! load factor below a specific threshold(its max_load_factor), causing a 
    //! rehash each time an expansion is needed.
    //!
    //! To retrieve or change this threshold, use member function max_load_factor.
    float load_factor()
    {
      return static_cast<float>(m_nItems) / m_bucketCountIndex;
    }

    //! Returns the current maximum load factor for the HashTable container.
    //!
    //! The load factor is the ratio between the number of elements in the 
    //! container(its size) and the number of buckets(bucket_count) :
    //!
    //!     load_factor = size / bucket_count
    //!
    //! The load factor influences the probability of collision in the HashTable
    //! (i.e., the probability of two elements being located in the same bucket).
    //!
    //! The container automatically increases the number of buckets to keep the 
    //! load factor below a specific threshold(its max_load_factor), causing a 
    //! rehash each time an expansion is needed.
    float max_load_factor()
    {
      return m_maxLoadFactor;
    }

    //! Set the maximum load factor for the HashTable container. There is a minimum 
    //! load factor: HashTable::s_minSetLF
    //!
    //! The load factor is the ratio between the number of elements in the 
    //! container(its size) and the number of buckets(bucket_count) :
    //!
    //!     load_factor = size / bucket_count
    //!
    //! The load factor influences the probability of collision in the HashTable
    //! (i.e., the probability of two elements being located in the same bucket).
    //!
    //! The container automatically increases the number of buckets to keep the 
    //! load factor below a specific threshold(its max_load_factor), causing a 
    //! rehash each time an expansion is needed.
    void max_load_factor(float a_val)
    {
      float prevLF = m_maxLoadFactor;
      m_maxLoadFactor = (a_val > s_minSetLF) ? a_val : s_minSetLF;
      if (prevLF < m_maxLoadFactor)
      {
        __rehash();
      }
    }

    //! Returns the maximum number of buckets that the unordered_map container can have.
    //!
    //! This is the maximum potential number of buckets the container can have due to 
    //! system constraints or limitations on its library implementation.
    size_t max_bucket_count() const
    {
      impl::validBucketCounts[ARRAY_SIZE(impl::validBucketCounts) - 1];
    }

    //! Returns the maximum number of elements that the unordered_map container can have.
    //!
    //! This is the maximum potential number of elements the container can have due to 
    //! system constraints or limitations on its library implementation.
    size_t max_size() const
    {
      return 0xFFFFFFFFFFFFFFFFLL / sizeof(T);
    }

    //! Sets the number of buckets in the container to a_bucketCount or more.
    //!
    //! If a_bucketCount is greater than the current number of buckets in the 
    //! container(bucket_count), a rehash is forced. The new bucket count can 
    //! either be equal or greater than a_bucketCount.
    //!
    //! If a_bucketCount is lower than the current number of buckets in the 
    //! container(bucket_count), the function may have no effect on the bucket 
    //! count and may not force a rehash.
    //!
    //! A rehash is the reconstruction of the hash table : All the elements in 
    //! the container are rearranged according to their hash value into the new 
    //! set of buckets.This may alter the order of iteration of elements within 
    //! the container.
    //!
    //! Rehashes are automatically performed by the container whenever its load 
    //! factor is going to surpass its max_load_factor in an operation.
    void rehash(size_t a_bucketCount)
    {
      float newLF = static_cast<float>(m_nItems) / static_cast<float>(a_bucketCount);
      if (newLF < m_maxLoadFactor)
      {
        __rehash();
      }
    }

    //! Request a capacity change.
    //! Sets the number of buckets in the container(bucket_count) to the most 
    //! appropriate to contain at least a_num elements. The memory block reserved
    //! for elements will also be extended to contain a_num elements.
    //!
    //! If a_num is greater than the current bucket_count multiplied by the max_load_factor, 
    //! the container's bucket_count is increased and a rehash is forced.
    //!
    //! If n is lower than that, the function may have no effect.
    void reserve(size_t a_num)
    {
      size_t bucketCount = static_cast<size_t>(static_cast<float>(a_num) / m_maxLoadFactor);
      if (bucketCount > bucket_count())
      {
        __rehash(bucketCount);
      }
      if (a_num > m_poolSize)
      {
        size_t exp = 1;
        while ((m_poolSize * (1 << exp)) < a_num)
        {
          exp++;
        }
        ExtendPool(exp);
      }
    }

    //! Removes from the HashTable container a single element.
    //!
    //! This effectively reduces the container size by 1.
    //!
    //! If not a POD, the element's destructor is called.
    iterator erase(iterator const & a_it)
    {
    }

    //! Removes from the HashTable container a single element.
    //!
    //! This effectively reduces the container size by 1.
    //!
    //! If not a POD, the element's destructor is called.
    void erase(K a_key)
    {
      size_t ind(m_fHasher(a_key));
      ind %= bucket_count();

      Node * pItem(m_pBuckets[ind].pBegin);
      Node * pPrev(nullptr);
      while (pItem)
      {
        if (pItem->key == a_key)
        {
          if (pPrev)
          {
            pPrev->pNext = pItem->pNext;
          }
          else
          {
            m_pBuckets[ind].pBegin = nullptr;
          }
          pItem->pNext = m_pNextFree;
          m_pNextFree = pItem;
          m_nItems--;
          break;
        }
        if (!pItem->pNext) break;
        pPrev = pItem;
        pItem = pItem->pNext;
      }
    }

    //! Returns the number of elements in the HashTable.
    size_t size() const 
    {
      return m_nItems;
    }

    //! Returns the number of buckets in the HashTable.
    size_t bucket_count() const
    {
      if (m_bucketCountIndex < 0)
      {
        return 0;
      }
      return impl::validBucketCounts[m_bucketCountIndex];
    }

    //! For debugging. The HashTable operates over a fixed memory block.
    //! The memory is essentially a linked list, with elements drawn from the
    //! list when items are inserted into the HashTable. This function returns the
    //! total number of used and unused elements in the linked list compared to the 
    //! size of the memory block. 
    size_t PoolSlotsWasted()
    {
      size_t total(0);

      for (size_t i = 0; i < bucket_count(); ++i)
      {
        Node * n = m_pBuckets[i].pBegin;
        while (n)
        {
          total++;
          n = n->pNext;
        }
      }

      Node * n = m_pNextFree;
      while (n)
      {
        total++;
        n = n->pNext;
      }

      return m_poolSize - total;
    }

#ifdef DGHASHTABLE_ENABLE_OUTPUT
    //! Output the HashTable for debugging.
    friend std::ostream & operator<<(std::ostream & os
                                   , HashTable<K, T, POD> const & ht) 
    {
      for (size_t i = 0; i < ht.bucket_count(); ++i)
      {
        os << i << ": ";
        Node * n = ht.m_pBuckets[i].pBegin;
        while (n)
        {
          os << n->key;
          if (n->pNext)
          {
            os << ", ";
          }
          n = n->pNext;
        }
        os << '\n';
      }
      return os;
    }
#endif

  private:

    //! Force a rehash.
    void __rehash(size_t a_bucketCount)
    {
      int newBucketCountIndex = GetBucketCountIndex(a_bucketCount);
      size_t newBucketCount = impl::validBucketCounts[newBucketCountIndex];
      Bucket * newBuckets = static_cast<Bucket*>(calloc(newBucketCount, sizeof(Bucket*)));
      DG_ASSERT((newBuckets == nullptr) ? 0 : 1);
      for (size_t bucket = 0; bucket < bucket_count(); ++bucket)
      {
        Node * in = m_pBuckets[bucket].pBegin;
        while (in)
        {
          Node * nextNode = in->pNext;

          //Add new node in the new bucket list
          size_t ind = m_fHasher(in->key) % newBucketCount;
          Node * prev = newBuckets[ind].pBegin;
          if (!prev)
          {
            newBuckets[ind].pBegin = in;
          }
          else
          {
            while (prev->pNext) prev = prev->pNext;
            prev->pNext = in;
          }
          in->pNext = nullptr;

          //Update iterator nodes
          in = nextNode;
        }
      }

      //Assign new bucket list and bucket count
      free(m_pBuckets);
      m_pBuckets = newBuckets;
      m_bucketCountIndex = newBucketCountIndex;
    }

    //! Extend the memory block reserved for elements (key, value).
    //! The order of nodes in the memory block wil NOT be changed.
    //! All indices to Nodes will remain valid.
    //!
    //! @param[in] a_exp The memory block will be increased 2^a_exp
    void ExtendPool(size_t a_exp = 1)
    {
      Node * pOldNodes = m_pNodes;
      size_t oldPoolSize = m_poolSize;
      m_poolSize *= (size_t(1) << a_exp);
      m_pNodes = static_cast<Node*>(realloc(m_pNodes, m_poolSize * sizeof(Node)));
      DG_ASSERT((m_pNodes == nullptr) ? 0 : 1);
      //Reset node pointers
      if (m_pNodes != pOldNodes)
      {
        for (size_t i = 0; i < bucket_count(); ++i)
        {
          if (m_pBuckets[i].pBegin)
          {
            m_pBuckets[i].pBegin = &m_pNodes[m_pBuckets[i].pBegin - pOldNodes];
            Node * n(m_pBuckets[i].pBegin);
            while (n->pNext)
            {
              n->pNext = &m_pNodes[n->pNext - pOldNodes];
              n = n->pNext;
            }
          }
        }

        //Reset pointer to next free
        m_pNextFree = &m_pNodes[m_pNextFree - pOldNodes];

        //Build list of free nodes
        Node * cur = m_pNextFree;
        while (cur->pNext)
        {
          cur->pNext = &m_pNodes[cur->pNext - pOldNodes];
          cur = cur->pNext;
        }
      }

      //Get the tail of the free elements
      Node * freeTail = m_pNextFree;
      while (freeTail->pNext)
      {
        freeTail = freeTail->pNext;
      }

      for (size_t i = oldPoolSize; i < m_poolSize; ++i)
      {
        freeTail->pNext = &m_pNodes[i];
        freeTail = freeTail->pNext;
      }
      m_pNodes[m_poolSize - 1].pNext = nullptr;
    }

    //! Returns true if had to rehash.
    bool RehashIfOver(size_t a_nItems)
    {
      float newLoadFactor = static_cast<float>(a_nItems) / bucket_count();
      if (newLoadFactor > m_maxLoadFactor)
      {
        if (m_bucketCountIndex != (ARRAY_SIZE(impl::validBucketCounts) - 1))
        {
          __rehash(impl::validBucketCounts[m_bucketCountIndex + 1]);
          return true;
        }
      }
      return false;
    }

    // Get the Node associated with the key, or create a new one if it
    //! is not in the map.
    Node * GetItem(K a_key, bool & a_existed)
    {
      size_t ind = m_fHasher(a_key);
      ind %= bucket_count();

      //This will be the first item in this bucket.
      Node * pResult = m_pBuckets[ind].pBegin;
      if (pResult == nullptr)
      {
        //If we rehash, we need to start this function again.
        if (RehashIfOver(m_nItems + 1))
        {
          return GetItem(a_key, a_existed);
        }
        m_pBuckets[ind].pBegin = GetNewNode(a_key);
        pResult = m_pBuckets[ind].pBegin;
        a_existed = false;
        return pResult;
      }

      //There are other items as well as this in the bucket
      while (true)
      {
        if (pResult->key == a_key)
        {
          a_existed = true;
          return pResult;
        }
        if (!pResult->pNext) break;
        pResult = pResult->pNext;
      }

      //Item does not exist
      //If we rehash, we need to start this function again.
      if (RehashIfOver(m_nItems + 1))
      {
        return GetItem(a_key, a_existed);
      }
      pResult = GetNewNode(pResult, a_key);
      a_existed = false;
      return pResult;
    }

    // Zero all members we have moved from.
    void PostMove(HashTable & a_other)
    {
      a_other.m_pNodes = nullptr;
      a_other.m_poolSize = 0;
      a_other.m_pNextFree = nullptr;
      a_other.m_pBuckets = nullptr;
      a_other.m_bucketCountIndex = s_invalidBucketIndex;
      a_other.m_nItems = 0;
    }

    //! Returns the index to the first valid bucket count index greater than 
    //! a_bucketCount. Valid bucket counts are taken from impl::validBucketCounts.
    int GetBucketCountIndex(size_t a_bucketCount)
    {
      int result = ARRAY_SIZE(impl::validBucketCounts) - 1;
      for (int i = 0; i < ARRAY_SIZE(impl::validBucketCounts); ++i)
      {
        if (a_bucketCount <= impl::validBucketCounts[i])
        {
          result = i;
          break;
        }
      }
      return result;
    }

    // Init the HashTabe.
    void init(size_t a_nBuckets)
    {
      //Update values
      m_bucketCountIndex = GetBucketCountIndex(a_nBuckets);
      m_nItems = 0;
      m_poolSize = s_minPoolSize;

      InitArrays();
    }

    //! Prep method for moving, assigning or deleting. Clears all memory, sets pointers to null,
    //! Calls destructors if not POD.
    void Wipe()
    {
      //Delete current items
      if (!POD)
      {
        iterator it = begin();
        for (it; it != end(); ++it)
        {
          (*it).~T();
        }
      }

      free(m_pNodes); m_pNodes = nullptr;
      free(m_pBuckets); m_pBuckets = nullptr;
      m_pNextFree = nullptr;

      m_poolSize = 0;
      m_bucketCountIndex = s_invalidBucketIndex;
      m_nItems = 0;
    }

    // Initialise all arrays based on current member values.
    void InitArrays()
    {
      //Update bucket pool
      if (m_poolSize > 0)
      {
        m_pNodes = static_cast<Node*>(malloc(m_poolSize * sizeof(Node)));
        DG_ASSERT((m_pNodes == nullptr) ? 0 : 1);
        for (size_t i = 0; i < m_poolSize - 1; ++i)
        {
          m_pNodes[i].pNext = &m_pNodes[i + 1];
        }
        m_pNodes[m_poolSize - 1].pNext = nullptr;
      }
      else
      {
        m_pNodes = nullptr;
      }

      //Create a new hash table
      if (bucket_count() > 0)
      {
        m_pBuckets = static_cast<Bucket*>(calloc(bucket_count(), sizeof(Bucket)));
        DG_ASSERT((m_pBuckets == nullptr) ? 0 : 1);
        //Flag the next free bucket
        m_pNextFree = &m_pNodes[0];
      }
      else
      {
        m_pBuckets = nullptr;
        m_pNextFree = nullptr;
      }
    }

    // Get a new node form the list of unused elements.
    Node * GetNewNode(K a_key)
    {
      if (m_pNextFree->pNext == nullptr)
      {
        ExtendPool();
      }

      Node * pResult = m_pNextFree;
      m_pNextFree = m_pNextFree->pNext;
      pResult->pNext = nullptr;
      pResult->key = a_key;
      m_nItems++;
      return pResult;
    }

    // Get a new node form the list of unused elements. Places it as the 
    // next node to pNode.
    Node * GetNewNode(Node * pNode, K a_key)
    {
      if (m_pNextFree->pNext == nullptr)
      {
        //The pNode may become invalid once we extend, but it's
        //index will not.
        size_t nodeInd = pNode - m_pNodes;
        ExtendPool();
        pNode = &m_pNodes[nodeInd];
      }

      Node * pResult = m_pNextFree;
      m_pNextFree = m_pNextFree->pNext;
      pNode->pNext = pResult;
      pResult->pNext = nullptr;
      pResult->key = a_key;
      m_nItems++;
      return pResult;
    }

    // Initialise a zeroed HashTable from other.
    void init(HashTable const & a_other)
    {
      //Assign values
      m_maxLoadFactor = a_other.m_maxLoadFactor;
      m_fHasher = a_other.m_fHasher;
      m_poolSize = a_other.m_poolSize;
      m_bucketCountIndex = a_other.m_bucketCountIndex;

      InitArrays();

      if (!m_pBuckets || !m_pNodes)
      {
        return;
      }

      //Copy in from other
      for (size_t bi = 0; bi < a_other.bucket_count(); ++bi)
      {
        Node const * that_node(a_other.m_pBuckets[bi].pBegin);
        if (that_node)
        {
          m_pBuckets[bi].pBegin = GetNewNode(that_node->key);
          Node * this_node(m_pBuckets[bi].pBegin);
          do
          {
            if (POD)
            {
              memcpy(&this_node->data, &that_node->data, sizeof(T));
            }
            else
            {
              new(&this_node->data) T(that_node->data);
            }

            that_node = that_node->pNext;

            if (that_node)
            {
              this_node = GetNewNode(this_node, that_node->key);
            }
            else
            {
              break;
            }

          } while (true);
        }
      }
    }

  private:

    float       m_maxLoadFactor;
    fHasher<K>  m_fHasher;

    Node *      m_pNodes;
    Node *      m_pNextFree;
    size_t      m_poolSize;

    Bucket *    m_pBuckets;
    int         m_bucketCountIndex; //Index to impl::validBucketCounts
    size_t      m_nItems;           //Number of curent elements
  };

  template<typename K, typename T, bool POD = true>
  float  const HashTable<K, T, POD>::s_defaultLF = 1.0f;

  template<typename K, typename T, bool POD = true>
  float  const HashTable<K, T, POD>::s_minSetLF = 0.5f;

  template<typename K, typename T, bool POD = true>
  size_t const HashTable<K, T, POD>::s_minPoolSize = 1;

  template<typename K, typename T, bool POD = true>
  int const HashTable<K, T, POD>::s_invalidBucketIndex = -1;

}

#endif