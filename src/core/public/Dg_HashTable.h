#ifndef DGHashTable
#define DGHashTable

#include <cstdlib>
#include <cstring>

#include "DgErrorHandler.h"

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

  template
  <
      typename K
    , typename T
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
    //! Iterator for the HashMap.
    //!
    //! @author Frank B. Hart
    //! @date 21/05/2016
    class const_iterator
    {
      friend class iterator;

      const_iterator(Node * a_pNode)
        : m_pNode(a_pNode)
      {}

    public:

      const_iterator()
        : m_pNode(nullptr)
      {}

      ~const_iterator() {}

      //! Copy constructor.
      const_iterator(const_iterator const & it)
        : m_pNode(it.m_pNode)
      {}

      //! Assignment.
      const_iterator& operator= (const_iterator const & it)
      {
        m_pNode = it.m_pNode;
        return *this;
      }

      //! Comparison.
      bool operator==(const_iterator const & it) const
      {
        return m_pNode == it.m_pNode;
      }

      //! Comparison.
      bool operator!=(const_iterator const & it) const
      {
        return m_pNode != it.m_pNode;
      }

      //! Post increment
      //! Iterating past the end of the pool will crash.
      //! Iterating should not be past the 'end' iterator
      const_iterator operator++(int)
      {
        iterator result(*this);
        ++(*this);
        return result;
      }

      //! Pre increment
      //! Iterating past the end of the pool will crash.
      //! Iterating should not be past the 'end' iterator
      const_iterator & operator++()
      {
        m_pNode = m_pNode->pNext;
        return *this;
      }

      //! Post increment
      //! Iterating past the end of the pool will crash.
      //! Iterating should not be past the 'end' iterator
      const_iterator operator--(int)
      {
        iterator result(*this);
        --(*this);
        return result;
      }

      //! Pre increment
      //! Iterating past the end of the pool will crash.
      //! Iterating should not be past the 'end' iterator
      const_iterator & operator--()
      {
        m_pNode = m_pNode->pPrev;
        return *this;

      }
      //! Conversion
      T const * operator->() const { return &m_pNode->data; }

      //! Conversion
      T const & operator*() const { return m_pNode->data; }

    private:

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
      friend class iterator;

      iterator(Node * a_pThis)
        : m_pNode(a_pThis)
      {}

    public:

      iterator()
        : m_pNode(nullptr)
      {}

      ~iterator() {}

      //! Copy constructor.
      iterator(iterator const & it)
        : m_pNode(it.m_pNode)
      {}

      //! Assignment.
      iterator& operator= (iterator const & it)
      {
        m_pNode = it.m_pNode;
        return *this;
      }

      //! Comparison.
      bool operator==(iterator const & it) const
      {
        return m_pNode == it.m_pNode;
      }

      //! Comparison.
      bool operator!=(iterator const & it) const
      {
        return m_pNode != it.m_pNode;
      }

      //! Post increment
      //! Iterating past the end of the pool will crash.
      //! Iterating should not be past the 'end' iterator
      iterator operator++(int)
      {
        iterator result(*this);
        ++(*this);
        return result;
      }

      //! Pre increment
      //! Iterating past the end of the pool will crash.
      //! Iterating should not be past the 'end' iterator
      iterator & operator++()
      {
        m_pNode = m_pNode->pNext;
        return *this;
      }

      //! Post increment
      //! Iterating past the end of the pool will crash.
      //! Iterating should not be past the 'end' iterator
      iterator operator--(int)
      {
        iterator result(*this);
        --(*this);
        return result;
      }

      //! Pre increment
      //! Iterating past the end of the pool will crash.
      //! Iterating should not be past the 'end' iterator
      iterator & operator--()
      {
        m_pNode = m_pNode->pPrev;
        return *this;

      }

      //! Conversion
      operator const_iterator() const
      {
        return const_iterator(m_pNode);
      }

      //! Conversion
      T * operator->() { return &m_pNode->data; }

      //! Conversion
      T & operator*() { return m_pNode->data; }

    private:

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
      Wipe();
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
        Wipe();

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
      Node * pNode(nullptr);
      if (!GetItem(key, pNode))
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

    T * at(K a_key) const
    {
      size_t ind = m_fHasher(a_key);
      ind %= bucket_count();

      Node * pItem = m_pBuckets[ind];
      while (pItem)
      {
        if (pItem->key == a_key)  return &pItem->data;
        if (!pItem->pNext)        return nullptr;
        pItem = pItem->pNext;
      }
      return nullptr;
    }

    //! Will overwrite if exists
    T * insert(K key, T const & val)
    {
      Node * pNode(nullptr);
      bool existed = GetItem(key, pNode);
      if (!POD)
      {
        if (existed)
        {
          pNode->data.~T();
        }
        new (&pNode->data) T();
      }
      else
      {
        memcpy(&pNode->data, &val, sizeof(T));
      }
      return &pNode->data;
    }

    T * insert_no_overwrite(K key, T const & val)
    {
      Node * pNode(nullptr);
      if (!GetItem(key, pNode))
      {
        if (!POD)
        {
          new (&pNode->data) T();
        }
        else
        {
          memcpy(&pNode->data, &val, sizeof(T));
        }
      }
      return &pNode->data;
    }

    void clear()
    {
      
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

    void rehash(size_t a_bucketCount)
    {
      size_t newBucketCountIndex = GetBucketCountIndex(a_bucketCount);
      size_t newBucketCount = impl::validBucketCounts[newBucketCountIndex];
      Node ** newBuckets = static_cast<Node**>(malloc(newBucketCount * sizeof(Node**)));
      for (size_t bucket = 0; bucket < bucket_count(); ++bucket)
      {
        Node * in = m_pBuckets[bucket];
        while (in)
        {
          //Get a new node
          Node * newNode = GetNewNode();
          newNode->key = in->key;
          newNode->pData = in->pData;

          //Add new node in the new bucket list
          size_t ind = fHasher(in->key) % newBucketCount;
          Node ** out = &newBuckets[ind];
          while (*out) out = &out->pNext;
          *out = newNode;

          //Save the location of the next node to work with
          Node * next = in->pNext;

          //Remove from old bucket list
          in->pNext = m_pNextFree;
          m_pNextFree = in;

          //Update iterator nodes
          in = next;
        }
      }

      //Assign new bucket list and bucket count
      delete[] m_pBuckets;
      m_pBuckets = newBuckets;
      m_bucketCountIndex
    }

    void reserve(size_t)
    {

    }

    iterator erase(iterator const &)
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
      do
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
      } while (pItem->pNext)
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
      size_t oldPoolSize = m_poolSize;
      m_poolSize *= 2;
      DG_ASSERT(m_pNodes = static_cast<Node*>(realloc(m_pNodes, m_poolSize * sizeof(Node))));

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


    //Will create new item if does not exist
    //! @return true if item already existed
    bool GetItem(K a_key, Node *& a_out)
    {
      size_t ind = m_fHasher(a_key);
      ind %= bucket_count();

      //This will be the first item in this bucket.
      a_out = m_pBuckets[ind].pBegin;
      if (a_out == nullptr)
      {
        m_pBuckets[ind].pBegin = GetNewNode(a_key);
        a_out = m_pBuckets[ind].pBegin;
        return false;
      }

      //There are other items as well as this in the bucket
      while (true)
      {
        if (a_out->key == a_key)
        {
          return true;
        }
        if (!pNode->pNext) break;
        a_out = a_out>pNext;
      }

      //Item does not exist
      float newLoadFactor = static_cast<float>(m_nItems + 1) / m_bucketCountIndex;
      if (newLoadFactor > m_maxLoadFactor)
      {
        if (m_bucketCountIndex != (ARRAY_SIZE(impl::validBucketCounts) - 1))
        {
          rehash(impl::validBucketCounts[m_bucketCountIndex + 1]);
        }
      }

      a_out = GetNewNode(a_out, a_key);
      return false;
    }

    void PostMove(HashTable & a_other)
    {
      a_other.m_pNodes = nullptr;
      a_other.m_poolSize = 0;
      a_other.m_pNextFree = nullptr;
      a_other.m_pBuckets = nullptr;
      a_other.m_bucketCountIndex = s_invalidBucketIndex;
      a_other.m_nItems = 0;
    }

    //! @param[in] a_bucketCount Number of buckets you want
    size_t GetBucketCountIndex(size_t a_bucketCount)
    {
      size_t result = ARRAY_SIZE(impl::validBucketCounts) - 1;
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

    void init(size_t a_nBuckets)
    {
      //Update values
      m_bucketCountIndex = GetBucketCountIndex(a_nBuckets);
      m_nItems = 0;
      m_poolSize = s_minPoolSize;

      InitArrays();
    }

    //! Prep method for moving or deleting. Clears all memory, sets pointers to null.
    void Wipe()
    {
      //Delete current items
      if (!POD)
      {
        iterator it = begin();
        for (it; it != end(); ++it)
        {
          *it.data.~T();
        }
      }

      free(m_pData); m_pData = nullptr;
      free(m_pNodes); m_pNodes = nullptr;
      free(m_pBuckets); m_pBuckets = nullptr;
      m_pNextFree = nullptr;

      m_poolSize = 0;
      m_bucketCountIndex = s_invalidBucketIndex;
      m_nItems = 0;
    }

    void InitArrays()
    {
      //Update bucket pool
      DG_ASSERT(m_pNodes = static_cast<Node*>(malloc(m_poolSize * sizeof(Node))));
      for (size_t i = 0; i < m_poolSize - 1; ++i)
      {
        m_pNodes[i].pNext = &m_pNodes[i + 1];
      }
      m_pNodes[m_poolSize - 1].pNext = nullptr;

      //Create a new hash table
      DG_ASSERT(m_pBuckets = static_cast<Bucket*>(calloc(bucket_count() * sizeof(Bucket))));
      
      //Flag the next free bucket
      m_pNextFree = &m_pNodes[0];
    }

    Node * GetNewNode(Key a_key)
    {
      if (m_pNextFree->pNext == nullptr)
      {
        ExtendPool();
      }

      Node * pResult = m_pNextFree;
      m_pNextFree = m_pNextFree->pNext;
      pResult->pNext = nullptr;
      pResult.key = a_key;
      return pResult;
    }


    Node * GetNewNode(Node * pNode, Key a_key)
    {
      if (m_pNextFree->pNext == nullptr)
      {
        ExtendPool();
      }

      Node * pResult = m_pNextFree;
      m_pNextFree = m_pNextFree->pNext;
      pNode->pNext = pResult;
      pResult->pNext = nullptr;
      pResult.key = a_key;
      return pResult;
    }

    void init(HashTable const & a_other)
    {
      //Cleanup
      Wipe();

      //Assign values
      m_maxLoadFactor = a_other.m_maxLoadFactor;
      m_fHasher = a_other.m_fHasher;
      m_poolSize = a_other.m_poolSize;
      m_bucketCountIndex = a_other.m_bucketCountIndex;
      m_nItems = a_other.m_nItems;

      InitArrays();

      //Copy in from other
      for (size_t bi = 0; bi < a_other.bucket_count(); ++bi)
      {
        Node const * that_node(a_other.m_pBuckets[bi].pBegin);
        if (that_node)
        {
          m_pBuckets[bi].pBegin = GetNewNode(that_node.key);
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

            that_node = that_node.pNext;

            if (that_node)
            {
              this_node = GetNewNode(this_node, that_node.key);
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
