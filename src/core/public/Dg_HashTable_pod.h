#ifndef DGHASHTABLE_POD
#define DGHASHTABLE_POD

#include <cstdlib>

#define DG_HASHTABLE_DEFAULT_POOL_SIZE 1024
#define DG_DEFAULT_HASHTABLE_SIZE 1021

namespace Dg
{
  template<typename K, typename T>
  class HashTable_pod
  {
    struct Node
    {
      K       key;
      Node *  pNext;
      Node *  pPrev;
    };

  public:

    HashTable_pod()
      : m_dataPool(nullptr)
      , m_nodePool(nullptr)
      , m_hashTable(nullptr)
      , m_nextFree(nullptr)
      , m_poolSize(0)
      , m_tableSize(0)
      , m_nItems(0)
    {
      reset(DG_DEFAULT_HASHTABLE_SIZE, DG_HASHTABLE_DEFAULT_POOL_SIZE);
    }

    HashTable_pod(size_t a_tableSize, size_t a_poolSize)
      : m_dataPool(nullptr)
      , m_nodePool(nullptr)
      , m_hashTable(nullptr)
      , m_nextFree(nullptr)
      , m_poolSize(0)
      , m_tableSize(0)
      , m_nItems(0)
    {
      reset(a_tableSize, a_poolSize);
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

    bool exists(K a_key) const
    {
      size_t ind = Hash(a_key);

      if (m_hashTable[ind] == nullptr)
      {
        return false;
      }

      Node * pItem = m_hashTable[ind];
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

    void erase(K)
    {
      size_t ind = Hash(a_key);

      if (m_hashTable[ind] == nullptr)
      {
        return;
      }

      Node * item = m_hashTable[ind];
      while (true)
      {
        if (pItem->key == a_key)
        {
          if (pItem->pPrev)
          {
            pItem->pPrev->pNext = pItem->pNext;
            pItem->pPrev = nullptr;
          }
          pItem->pNext = m_nextFree;
          m_nextFree = pItem;
          break;
        }
        if (!pItem->pNext) break;
        pItem = pItem->pNext;
      }
    }

    size_t size() const 
    {
      return m_nItems;
    }

  private:

    size_t Hash(K a_key) 
    {
      return a_key % m_tableSize;
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

      if (m_hashTable[ind] == nullptr)
      {
        m_hashTable[ind] = GetNewNode(a_key);
        return (m_hashTable[ind] - m_nodePool) / sizeof(Node*);
      }

      Node * item = m_hashTable[ind];
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

    void reset(size_t a_hashTableSize, size_t a_poolSize)
    {
      //Update values
      m_poolSize = a_poolSize;
      m_tableSize = a_hashTableSize;
      m_nItems = 0;

      //Update data pool
      m_dataPool = static_cast<T*>(realloc(m_dataPool, m_poolSize * sizeof(T)));
      
      //Update bucket pool
      m_nodePool = static_cast<Node*>(realloc(m_nodePool, m_poolSize * sizeof(Node)));
      for (size_t i = 1; i < m_poolSize; ++i)
      {
        m_nodePool[i].pPrev = &m_nodePool[i - 1];
      }
      for (size_t i = 0; i < m_poolSize - 1; ++i)
      {
        m_nodePool[i].pNext = &m_nodePool[i + 1];
      }
      m_nodePool[0].pPrev = nullptr;
      m_nodePool[0].pNext = nullptr;

      //Flag the next free bucket
      m_nextFree = &m_nodePool[0];

      //Create a new hash table
      m_hashTable = static_cast<Node**>(realloc(m_hashTable, m_tableSize * sizeof(Node*)));
      for (int i = 0; i < m_tableSize; ++i)
      {
        m_hashTable[i] = nullptr;
      }
    }

  private:
    T *       m_dataPool;
    Node *    m_nodePool;
    size_t    m_poolSize;

    Node *    m_nextFree;
    Node **   m_hashTable;
    size_t    m_tableSize; //Size of the hash table
    size_t    m_nItems; //Number of curent elements
  };
}

#endif
