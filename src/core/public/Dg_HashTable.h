#ifndef DGHashTable
#define DGHashTable

#include <cstdlib>
#include <cstring>

#define ARRAY_SIZE(a) sizeof(a) / sizeof(*a)

namespace Dg
{
  template<typename K>
  using fHasher = size_t(*)(K);

  template<typename K>
  size_t DefaultHasher(K key)
  {
    return static_cast<size_t>(key) * 14695981039346656037;
  }

  namespace impl
  {
    static size_t const validBucketCounts[65] = 
    {
      0x1, 0x2, 0x3, 0x7, 0xd, 
      0x1f, 0x3d, 0x7f, 0xfb, 
      0x1fd, 0x3fd, 0x7f7, 0xffd, 
      0x1fff, 0x3ffd, 0x7fed, 0xfff1, 
      0x1ffff, 0x3fffb, 0x7ffff, 0xffffd, 
      0x1ffff7, 0x3ffffd, 0x7ffff1, 0xfffffd, 
      0x1ffffd9, 0x3fffffb, 0x7ffffd9, 0xfffffc7, 
      0x1ffffffd, 0x3fffffdd, 0x7fffffff, 0xfffffffb, 
      0x1fffffff7, 0x3ffffffd7, 0x7ffffffe1, 0xffffffffb, 
      0x1fffffffe7, 0x3fffffffd3, 0x7ffffffff9, 0xffffffffa9, 
      0x1ffffffffeb, 0x3fffffffff5, 0x7ffffffffc7, 0xfffffffffef, 
      0x1fffffffffc9, 0x3fffffffffeb, 0x7fffffffff8d, 0xffffffffffc5, 
      0x1ffffffffffaf, 0x3ffffffffffe5, 0x7ffffffffff7f, 0xfffffffffffd1, 
      0x1fffffffffff91, 0x3fffffffffffdf, 0x7fffffffffffc9, 0xfffffffffffffb, 
      0x1fffffffffffff3, 0x3ffffffffffffe5, 0x7ffffffffffffc9, 0xfffffffffffffa3, 
      0x1fffffffffffffff, 0x3fffffffffffffc7, 0x7fffffffffffffe7, 0xffffffffffffffc5
    };
  }

  template
  <
      typename K
    , typename T
    , bool POD = true
  >
  class HashTable
  {
    class Node
    {
    public:

      Node()
        : pNext(nullptr)
        , pPrev(nullptr)
        , pData(nullptr)
      {}

      ~Node() {}

      Node(Node const & a_other)
        : pNext(a_other.pNext)
        , pPrev(a_other.pPrev)
        , pData(a_other.pData)
        , key(a_other.key)
      {}

      Node & operator=(Node const & a_other)
      {
        pNext = a_other.pNext;
        pPrev = a_other.pPrev;
        pData = a_other.pData;
        key = a_other.key;
        return *this;
      }

      K       key;
      T *     pData;
      Node *  pNext;
    };

    static float  const   s_defaultLF;
    static float  const   s_minSetLF;
    static size_t const   s_minPoolSize;
    static int const      s_invalidBucketIndex;

  public:


    //! @class const_iterator
    //!
    //! Iterator for the HashMap.
    //!
    //! @author Frank B. Hart
    //! @date 21/05/2016
    class const_iterator
    {
      friend class HashTable;
      friend class iterator;

      const_iterator(Node * a_pNode
      , size_t a_bucketIndex
      , size_t a_bucketCount
      , Node ** a_pBuckets)
        : m_pNode(a_pNode)
        , m_bucketIndex(a_bucketIndex)
        , m_bucketCount(a_bucketCount)
        , m_pBuckets(a_pBuckets)
      {}

    public:

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

      //! Post increment
      const_iterator operator++(int)
      {
        iterator result(*this);
        ++(*this);
        return result;
      }

      //! Pre increment
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

        for (m_bucketIndex; m_bucketIndex++ < m_bucketCount;)
        {
          if (m_pBuckets[m_bucketIndex])
          {
            m_pNode = m_pBuckets[m_bucketIndex];
            return *this;
          }
        }

        //End
        m_pNode = nullptr;
        return *this;
      }

      //! Post decrement
      const_iterator operator--(int)
      {
        iterator result(*this);
        --(*this);
        return result;
      }

      //! Pre decrement
      const_iterator & operator--()
      {
        if (!m_pNode)
        {
          return *this;
        }

        for (m_bucketIndex; m_bucketIndex-- > 0;)
        {
          Node const * curNode = m_pBuckets[m_bucketIndex];
          while (curNode)
          {
            if (curNode->pNext == m_pNode)
            {
              m_pNode = curNode;
              return *this;
            }
            curNode = curNode->pNext;
          }
        }

        //End
        m_pNode = nullptr;
        m_bucketIndex = m_bucketCount + 1;
        return *this;
      }

      //! Conversion
      T const * operator->() const { return m_pNode->pData; }

      //! Conversion
      T const & operator*() const { return *(m_pNode->pData); }

    private:

      /// The 'end' iterator is denoted by:
      ///   m_pBuckets  = a valid pointer
      ///   m_pNode     = nullptr

      size_t                m_bucketIndex;
      size_t                m_bucketCount;
      Node const * const *  m_pBuckets;
      Node const *          m_pNode;
    };


    //! @class iterator
    //!
    //! Iterator for the HashMap.
    //!
    //! @author Frank B. Hart
    //! @date 21/05/2016
    class iterator
    {
      friend class HashTable;

      iterator(Node * a_pNode
        , size_t a_bucketIndex
        , size_t a_bucketCount
        , Node ** a_pBuckets)
        : m_pNode(a_pNode)
        , m_bucketIndex(a_bucketIndex)
        , m_bucketCount(a_bucketCount)
        , m_pBuckets(a_pBuckets)
      {}

    public:

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

      //! Post increment
      iterator operator++(int)
      {
        iterator result(*this);	
        ++(*this);
        return result;
      }

      //! Pre increment
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

        for (m_bucketIndex; m_bucketIndex++ < m_bucketCount ;)
        {
          if (m_pBuckets[m_bucketIndex])
          {
            m_pNode = m_pBuckets[m_bucketIndex];
            return *this;
          }
        }
        
        //End
        m_pNode = nullptr;
        return *this;
      }

      //! Post decrement
      iterator operator--(int)
      {
        iterator result(*this);
        --(*this);
        return result;
      }

      //! Pre decrement
      iterator & operator--()
      {
        if (!m_pNode)
        {
          return *this;
        }

        for (m_bucketIndex; m_bucketIndex-- > 0 ;)
        {
          Node * curNode = m_pBuckets[m_bucketIndex];
          while (curNode)
          {
            if (curNode->pNext == m_pNode)
            {
              m_pNode = curNode;
              return *this;
            }
            curNode = curNode->pNext;
          }
        }

        //End
        m_pNode = nullptr;
        m_bucketIndex = m_bucketCount + 1;
        return *this;
      }

      //! Conversion
      /*operator const_iterator() const 
      { 
        return const_iterator(m_pNode
                            , m_bucketIndex
                            , m_bucketCount
                            , m_pBuckets); 
      }*/

      //! Conversion
      T* operator->() const { return m_pNode->pData; }

      //! Conversion
      T& operator*() const { return *(m_pNode->pData); }

    private:

      /// The 'end' iterator is denoted by:
      ///   m_pBuckets  = a valid pointer
      ///   m_pNode     = nullptr

      size_t          m_bucketIndex;
      size_t          m_bucketCount;
      Node * const *  m_pBuckets;
      Node *          m_pNode;
    };

  public:

    HashTable()
      : m_pData(nullptr)
      , m_pNodes(nullptr)
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

    explicit HashTable(size_t a_nBuckets)
      : m_pData(nullptr)
      , m_pNodes(nullptr)
      , m_pBuckets(nullptr)
      , m_pNextFree(nullptr)
      , m_poolSize(0)
      , m_bucketCountIndex(s_invalidBucketIndex)
      , m_nItems(0)
      , m_fHasher(DefaultHasher)
      , m_maxLoadFactor(s_defaultLF)
    {
      init(a_nBuckets);
    }

    //! The constructor will take ownership of the hasher function
    HashTable(size_t a_nBuckets, fHasher<K> * a_hf)
      : m_pData(nullptr)
      , m_pNodes(nullptr)
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

    ~HashTable()
    {
      Zero();
    }

    HashTable(HashTable const & a_other)
    {
      init(a_other);
    }

    HashTable & operator=(HashTable const & a_other)
    {
      if (this != &a_other)
      {
        init(a_other);
      }
      return *this;
    }

    HashTable(HashTable && a_other)
      : m_pData(a_other.m_pData)
      , m_pNodes(a_other.m_pNodes)
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

    HashTable & operator=(HashTable && a_other)
    {
      if (this != &a_other)
      {
        Zero();

        m_pData = a_other.m_pData;
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

    //! Returns element mapped to key. Will insert element
    //! if does not exist.
    T & operator[](K key)
    {
      return m_pData[GetItemIndex(key)];
    }

    //! Returns element mapped to key. Will insert element
    //! if does not exist.
    T const & operator[](K key) const
    {
      return m_pData[GetItemIndex(key)];
    }

    iterator begin()
    {
      for (size_t i = 0; i < bucket_count(); ++i)
      {
        if (m_pBuckets[i])
        {
          return iterator(m_pBuckets[i], i, bucket_count(), m_pBuckets);
        }
      }
      return end();
    }


    const_iterator cbegin() const
    {
      for (size_t i = 0; i < bucket_count(); ++i)
      {
        if (m_pBuckets[i])
        {
          return const_iterator(m_pBuckets[i], i, bucket_count(), m_pBuckets);
        }
      }
      return cend();
    }

    iterator end()
    {
      return iterator(nullptr, bucket_count() + 1, bucket_count(), m_pBuckets);
    }

    const_iterator cend() const
    {
      return const_iterator(nullptr, bucket_count() + 1, bucket_count(), m_pBuckets);
    }

    bool at(K a_key) const
    {
      size_t ind = m_fHasher(a_key);

      if (m_pBuckets[ind] == nullptr)
      {
        return false;
      }

      Node * pItem = m_pBuckets[ind];
      while (true)
      {
        if (pItem->key == a_key)  return true;
        if (!pItem->pNext)        return false;
        pItem = pItem->pNext;
      }
    }

    //! Will overwrite if exists
    void insert(K key, T const & val)
    {
      m_pData[GetItemIndex(key)] = val;
    }

    void clear()
    {
      if (!POD)
      {

      }
    }

    bool empty() const
    {
      return m_nItems == 0;
    }

    float load_factor()
    {
      return static_cast<float>(m_nItems) / m_bucketCountIndex;
    }

    float max_load_factor()
    {
      return m_maxLoadFactor;
    }

    void max_load_factor(float a_val)
    {
      float prevLF = m_maxLoadFactor;
      m_maxLoadFactor = (a_val > s_minSetLF) ? a_val : s_minSetLF;
      if (prevLF < m_maxLoadFactor)
      {
        rehash();
      }
    }

    size_t max_bucket_count() const
    {
      impl::validBucketCounts[ARRAY_SIZE(impl::validBucketCounts) - 1];
    }

    size_t max_size() const
    {
      return 0x8000000000000000;
    }

    void rehash(size_t)
    {

    }

    void reserve(size_t)
    {

    }

    void erase(K a_key)
    {
      size_t ind = Hash(a_key);

      if (m_pBuckets[ind] == nullptr)
      {
        return;
      }

      Node * pItem = m_pBuckets[ind];
      Node * pPrev(nullptr);
      while (true)
      {
        if (pItem->key == a_key)
        {
          if (pPrev)
          {
            pPrev->pNext = pItem->pNext;
          }
          pItem->pNext = m_pNextFree;
          m_pNextFree = pItem;
          break;
        }
        if (!pItem->pNext) break;
        pPrev = pItem;
        pItem = pItem->pNext;
      }
    }

    size_t size() const 
    {
      return m_nItems;
    }

    size_t bucket_count() const
    {
      return impl::validBucketCounts[m_bucketCountIndex];
    }

  private:

    void ExtendPool()
    {
      Node * pOldNodes = m_pNodes;
      T * pOldData = m_pData;
      size_t oldPoolSize = m_poolSize;
      m_poolSize *= 2;
      m_pData = static_cast<T*>(realloc(m_pData, m_poolSize * sizeof(T)));
      m_pNodes = static_cast<Node*>(realloc(m_pNodes, m_poolSize * sizeof(Node)));

      //Reset node pointers
      if (m_pNodes != pOldNodes)
      {
        for (size_t i = 0; i < bucket_count(); ++i)
        {
          if (m_pBuckets[i])
          {
            m_pBuckets[i] = &m_pNodes[m_pBuckets[i] - pOldNodes];
            Node * n(m_pBuckets[i]);
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

      freeTail->pNext = &m_pNodes[oldPoolSize];
      for (size_t i = 0; i < m_poolSize - 1; ++i)
      {
        m_pNodes[i].pNext = &m_pNodes[i + 1];
      }
      m_pNodes[m_poolSize - 1].pNext = nullptr;

      //Reset pointers to data
      if (m_pNodes != pOldNodes || m_pData != pOldData)
      {
        iterator it = begin();
        for (it; it != end(); ++it)
        {
          it->~T();
        }
      }
    }

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
      ++m_nItems;
      return pResult;
    }

    //Will create new item if does not exist
    size_t GetItemIndex(K a_key)
    {
      size_t ind = m_fHasher(a_key);
      ind %= bucket_count();

      if (m_pBuckets[ind] == nullptr)
      {
        m_pBuckets[ind] = GetNewNode(a_key);
        return (m_pBuckets[ind] - m_pNodes) / sizeof(Node*);
      }

      Node * item = m_pBuckets[ind];
      while (true)
      {
        if (pItem->key == a_key)
        {
          return item - m_pNodes;
        }
        if (!pItem->pNext) break;
        pItem = pItem->pNext;
      }

      return (GetNewNode(a_key) - m_pNodes) / sizeof(Node*);
    }

    void PostMove(HashTable & a_other)
    {
      a_other.m_pData = nullptr;
      a_other.m_pNodes = nullptr;
      a_other.m_poolSize = 0;
      a_other.m_pNextFree = nullptr;
      a_other.m_pBuckets = nullptr;
      a_other.m_bucketCountIndex = 0;
      a_other.m_nItems = 0;
    }

    void init(size_t a_nBuckets)
    {
      //Update values
      m_bucketCountIndex = ARRAY_SIZE(impl::validBucketCounts) - 1;
      for (int i = 0; i < ARRAY_SIZE(impl::validBucketCounts); ++i)
      {
        if (a_nBuckets <= impl::validBucketCounts[i])
        {
          m_bucketCountIndex = i;
          break;
        }
      }
      m_nItems = 0;
      m_poolSize = s_minPoolSize;

      InitArrays();
    }

    //! Prep method for moving or deleting. Clears all memory, sets pointers to null.
    void Zero()
    {
      //Delete current items
      clear();

      delete[] m_pData; m_pData = nullptr;
      delete[] m_pNodes; m_pNodes = nullptr;
      m_pNextFree = nullptr;
      delete[] m_pBuckets; m_pBuckets = nullptr;

      m_poolSize = 0;
      m_bucketCountIndex = s_invalidBucketIndex;
      m_nItems = 0;
    }

    void InitArrays()
    {
      //Resize datablocks
      m_pData = static_cast<T*>(malloc(m_poolSize * sizeof(T)));

      //Update bucket pool
      m_pNodes = static_cast<Node*>(malloc(m_poolSize * sizeof(Node)));
      for (size_t i = 0; i < m_poolSize - 1; ++i)
      {
        m_pNodes[i].pNext = &m_pNodes[i + 1];
      }
      m_pNodes[m_poolSize - 1].pNext = nullptr;

      //Create a new hash table
      m_pBuckets = static_cast<Node**>(malloc(bucket_count() * sizeof(Node*)));
      for (int i = 0; i < bucket_count(); ++i)
      {
        m_pBuckets[i] = nullptr;
      }

      //Flag the next free bucket
      m_pNextFree = &m_pNodes[0];
    }


    void init(HashTable const & a_other)
    {
      //Cleanup
      if (!POD)
      {
        iterator it = begin();
        for (it; it != end(); ++it)
        {
          it->~T();
        }
      }

      //Assign values
      m_nItems = a_other.m_nItems;
      m_bucketCountIndex = a_other.m_bucketCountIndex;
      m_poolSize = a_other.m_poolSize;
      m_maxLoadFactor = a_other.m_maxLoadFactor;

      InitArrays();

      //Copy in from other
      size_t currentAvailInd(0);
      for (size_t i = 0; i < a_other.bucket_count(); ++i)
      {
        Node * n(a_other.m_pBuckets[i]);
        if (n)
        {
          do
          {
            if (!POD)
            {
              new(&m_pData[currentAvailInd]) T(a_other.m_pData[n - a_other.m_pNodes]);
            }
            else
            {
              memcpy(&m_pData[currentAvailInd], &a_other.m_pData[n - a_other.m_pNodes], sizeof(T));
            }
            if (!n->pNext)
            {
              m_pNodes[currentAvailInd].pNext = nullptr;
            }
            currentAvailInd++;
          } while (n->pNext);
        }
      }

      //Flag the next free bucket
      m_pNextFree = &m_pNodes[currentAvailInd];
    }

  private:

    float       m_maxLoadFactor;
    fHasher<K>  m_fHasher;

    T  *        m_pData;
    Node *      m_pNodes;
    size_t      m_poolSize;

    Node *      m_pNextFree;
    Node **     m_pBuckets;
    int         m_bucketCountIndex; //The bucket count is obtained from bucket_count()
    size_t      m_nItems;   //Number of curent elements
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
