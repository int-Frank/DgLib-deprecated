#ifndef DGHashTable
#define DGHashTable

#include <cstdlib>

namespace Dg
{
  //! Other hashers must derive from this
  template<typename K>
  struct Hasher
  {
    //! The default is no has is applied, although the key is always modded
    //! against the (prime) number of buckets in the HashTable
    virtual size_t operator()(K key)
    {
      return key;
    }
  };

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
      Node *  pNext;
    };

    static size_t const   s_minBuckets  = 13;
    static float  const   s_defaultLF   = 1.0f;
    static float  const   s_minSetLF    = 0.5f;

  public:

    HashTable()
      : m_dataPool(nullptr)
      , m_nodePool(nullptr)
      , m_pBuckets(nullptr)
      , m_nextFree(nullptr)
      , m_poolSize(0)
      , m_nBuckets(0)
      , m_nItems(0)
      , m_pHasher(new Hasher<K>())
      , m_maxLoadFactor(s_defaultLF)
    {
      init(DG_DEFAULT_MIN_N_BUCKETS);
    }

    explicit HashTable(size_t a_nBuckets)
      : m_dataPool(nullptr)
      , m_nodePool(nullptr)
      , m_pBuckets(nullptr)
      , m_nextFree(nullptr)
      , m_poolSize(0)
      , m_nBuckets(0)
      , m_nItems(0)
      , m_pHasher(new Hasher<K>())
      , m_maxLoadFactor(s_defaultLF)
    {
      init(a_nBuckets);
    }

    //! The constructor will take ownership of the hasher function
    HashTable(size_t a_nBuckets, Hasher<K> * a_hf)
      : m_dataPool(nullptr)
      , m_nodePool(nullptr)
      , m_pBuckets(nullptr)
      , m_nextFree(nullptr)
      , m_poolSize(0)
      , m_nBuckets(0)
      , m_nItems(0)
      , m_pHasher(a_hf)
      , m_maxLoadFactor(s_defaultLF)
    {
      if (!a_hf)
      {
        m_pHasher = new Hasher<K>();
      }
      init(a_nBuckets);
    }

    HashTable(HashTable const & a_other)
    {

    }

    HashTable & operator=(HashTable const & a_other)
    {

    }

    HashTable(HashTable && a_other)
    {

    }

    HashTable & operator=(HashTable && a_other)
    {

    }

    //! Returns element mapped to key. Will insert element
    //! if does not exist.
    T & operator[](K key)
    {
      return m_dataPool[GetItemIndex(key)];
    }

    //! Returns element mapped to key. Will insert element
    //! if does not exist.
    T const & operator[](K key) const
    {
      return m_dataPool[GetItemIndex(key)];
    }

    bool at(K a_key) const
    {
      size_t ind = Hash(a_key);

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
      m_dataPool[GetItemIndex(key)] = val;
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
      return static_cast<float>(m_nItems) / m_nBuckets;
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

    }

    size_t max_size() const
    {

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
          pItem->pNext = m_nextFree;
          m_nextFree = pItem;
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
      return m_nBuckets;
    }

  private:

    //Be sure to re-hash!!
    void IncreaseNumBuckets()
    {

    }

    //Container must be full.
    void ExtendPool()
    {
      if (m_nextFree->pNext != nullptr)
      {
        return;
      }

      Node * pTemp = m_nodePool;
      m_poolSize *= 2;
      m_dataPool = static_cast<T*>(realloc(m_dataPool, m_poolSize * sizeof(T)));
      m_nodePool = static_cast<Node*>(realloc(m_nodePool, m_poolSize * sizeof(Node)));

      if (m_nodePool != pTemp)
      {

      }
      else
      {
        Node * pNode = m_nextFree;
        for (size_t i = m_nItems)
      }
    }

    Node * GetNewNode(K a_key)
    {
      if (m_nextFree->pNext == nullptr)
      {
        ExtendPool();
      }
      Node * ptemp = m_nextFree;
      m_nextFree = m_nextFree->pNext;
      ptemp->pNext = nullptr;
      ptemp->key = a_key;
      ++m_nItems;
      return ptemp;
    }

    //Will create new item if does not exist
    size_t GetItemIndex(K a_key)
    {
      size_t ind = Hash(a_key);

      if (m_pBuckets[ind] == nullptr)
      {
        m_pBuckets[ind] = GetNewNode(a_key);
        return (m_pBuckets[ind] - m_nodePool) / sizeof(Node*);
      }

      Node * item = m_pBuckets[ind];
      while (true)
      {
        if (pItem->key == a_key)
        {
          return item - m_nodePool;
        }
        if (!pItem->pNext) break;
        pItem = pItem->pNext;
      }

      return (GetNewNode(a_key) - m_nodePool) / sizeof(Node*);
    }

    bool IsPrime(size_t a_val)
    {
      for (size_t i = 2; i*i < a_val; ++i)
      {
        if (a_val % i == 0)
        {
          return false;
        }
      }
      return true;
    }

    void init(size_t a_nBuckets)
    {
      //Update values
      while (!IsPrime(a_nBuckets))
      {
        ++a_nBuckets;
      }
      m_nBuckets = a_nBuckets;
      if (m_nBuckets < s_minBuckets)
      {
        m_nBuckets = s_minBuckets;
      }
      m_nItems = 0;
      m_poolSize = static_cast<size_t>(static_cast<float>(m_nBuckets) / s_defaultLF);


      //Update data pool
      m_dataPool = static_cast<T*>(realloc(m_dataPool, m_poolSize * sizeof(T)));
      
      //Update bucket pool
      m_nodePool = static_cast<Node*>(realloc(m_nodePool, m_poolSize * sizeof(Node)));
      for (size_t i = 0; i < m_poolSize - 1; ++i)
      {
        m_nodePool[i].pNext = &m_nodePool[i + 1];
      }
      m_nodePool[m_poolSize - 1].pNext = nullptr;

      //Flag the next free bucket
      m_nextFree = &m_nodePool[0];

      //Create a new hash table
      m_pBuckets = static_cast<Node**>(realloc(m_pBuckets, m_nBuckets * sizeof(Node*)));
      for (int i = 0; i < m_nBuckets; ++i)
      {
        m_pBuckets[i] = nullptr;
      }
    }

  private:
    float       m_maxLoadFactor;
    Hasher<K> * m_pHasher;

    T *         m_dataPool;
    Node *      m_nodePool;
    size_t      m_poolSize;

    Node *      m_nextFree;
    Node **     m_pBuckets;
    size_t      m_nBuckets; //Size of the hash table
    size_t      m_nItems;   //Number of curent elements
  };
}

#endif
