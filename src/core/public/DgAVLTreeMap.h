#ifndef DGAVLTREEMAP_H
#define DGAVLTREEMAP_H

#include <exception>
#include <new>
#include <cstring>
#include "DgPair.h"
#include "impl/DgContainerBase.h"
#include "DgErrorHandler.h"

#define DEBUG
#ifdef DEBUG
#include <sstream>
#include <iostream>
#endif

namespace Dg
{
  namespace impl
  {
    template<typename T>
    bool Less(T const & t0, T const & t1)
    {
      return t0 < t1;
    }

    template<typename T>
    T Max(T a, T b)
    {
      return a > b ? a : b;
    }
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &) = impl::Less<K>>
  class AVLTreeMap : public ContainerBase
  {
  public:

    typedef Pair<K const, V> ValueType;

  private:

    typedef size_t sizeType;

    struct Node
    {
      Node *       pParent;
      Node *       pLeft;
      Node *       pRight;
      ValueType *  pKV;
      int          height;
    };

  public:

    //Iterates through the map as it appears in memory. 
    //Faster but assume order is random.
    class const_iterator_rand
    {
      friend class AVLTreeMap;
      friend class iterator_rand;

    private:

      const_iterator_rand(ValueType const *  a_pKV);

    public:

      const_iterator_rand();
      ~const_iterator_rand();

      const_iterator_rand(const_iterator_rand const & a_it);
      const_iterator_rand& operator= (const_iterator_rand const & a_other);

      bool operator==(const_iterator_rand const & a_it) const;
      bool operator!=(const_iterator_rand const & a_it) const;

      const_iterator_rand& operator++();
      const_iterator_rand operator++(int);
      const_iterator_rand& operator--();
      const_iterator_rand operator--(int);

      ValueType const * operator->() const;
      ValueType const & operator*() const;

    private:
      ValueType const *  m_pKV;
    };

    //Iterates through the map as it appears in memory. 
    //Faster but assume order is random.
    class iterator_rand
    {
      friend class AVLTreeMap;

    private:

      iterator_rand(ValueType *  a_pKV);

    public:

      iterator_rand();
      ~iterator_rand();

      iterator_rand(iterator_rand const & a_it);
      iterator_rand& operator= (iterator_rand const & a_other);

      bool operator==(iterator_rand const & a_it) const;
      bool operator!=(iterator_rand const & a_it) const;

      iterator_rand& operator++();
      iterator_rand operator++(int);
      iterator_rand& operator--();
      iterator_rand operator--(int);

      operator const_iterator_rand() const;

      ValueType * operator->();
      ValueType & operator*();

    private:
      ValueType *  m_pKV;
    };

    //Iterates through the map as sorted by the criterion
    class const_iterator
    {
      friend class AVLTreeMap;
      friend class iterator;
    private:

      const_iterator(Node const * a_pNode);

    public:

      const_iterator();
      ~const_iterator();

      const_iterator(const_iterator const & a_it);
      const_iterator& operator=(const_iterator const & a_other);

      bool operator==(const_iterator const & a_it) const;
      bool operator!=(const_iterator const & a_it) const;

      ValueType const * operator->() const;
      ValueType const & operator*() const;

      const_iterator & operator++();
      const_iterator operator++(int);
      const_iterator & operator--();
      const_iterator operator--(int);

    private:
      Node const * m_pNode;
    };

    //Iterates through the map as sorted by the criterion
    class iterator
    {
      friend class AVLTreeMap;
    private:

      iterator(Node * a_pNode);

    public:

      iterator();
      ~iterator();

      iterator(iterator const & a_it);
      iterator& operator=(iterator const & a_other);

      bool operator==(iterator const & a_it) const;
      bool operator!=(iterator const & a_it) const;

      ValueType * operator->();
      ValueType & operator*();

      iterator & operator++();
      iterator operator++(int);
      iterator & operator--();
      iterator operator--(int);

      operator const_iterator() const;

    private:
      Node * m_pNode;
    };

  public:

    AVLTreeMap();
    AVLTreeMap(sizeType requestSize);
    ~AVLTreeMap();

    AVLTreeMap(AVLTreeMap const &);
    AVLTreeMap & operator=(AVLTreeMap const & a_other);

    AVLTreeMap(AVLTreeMap && a_other);
    AVLTreeMap & operator=(AVLTreeMap && a_other);

    sizeType size() const;
    bool empty() const;

    iterator_rand begin_rand();
    iterator_rand end_rand();
    const_iterator_rand cbegin_rand();
    const_iterator_rand cend_rand();

    iterator begin();
    iterator end();
    const_iterator cbegin();
    const_iterator cend();

    //If the key already exists in the map, the data is 
    //inserted at this key
    iterator insert(K const & a_key, V const & a_data);

    void erase(K const &);

    //Returns an iterator to the element that follows the element removed
    //(or end(), if the last element was removed).
    iterator erase(iterator);

    //Searches the container for an element with a key equivalent to a_key and returns 
    //a handle to it if found, otherwise it returns an iterator to end().
    const_iterator find(K const &) const;

    //Searches the container for an element with a key equivalent to a_key and returns 
    //a handle to it if found, otherwise it returns an iterator to end().
    iterator find(K const & a_key);

    //If k matches the key of an element in the container, the function returns 
    //a reference to its mapped value.
    //If k does not match the key of any element in the container, the function 
    //inserts a new element with that key and returns a reference to its mapped value.
    V & operator[](K const &);

    //Returns a reference to the mapped value of the element identified with key k.
    //If k does not match the key of any element in the container, the function 
    //throws an out_of_range exception.
    V & at(K const &);

    //Returns a reference to the mapped value of the element identified with key k.
    //If k does not match the key of any element in the container, the function 
    //throws an out_of_range exception.
    V const & at(K const &) const;

    void clear();
#ifdef DEBUG
    sizeType Ind(Node * a_pNode) const;
    void PrintNode(Node * a_pNode) const;
    void Print() const;
#endif
  private:

    void DestructAll();
    void InitMemory();
    void InitDefaultNode();
    void Init(AVLTreeMap const &);

    //Sets a_out to the node index which references the key, or
    //if the key does not exist, the node at which the key should be
    //added
    bool KeyExists(K const & a_key, Node *& a_out) const;

    Node * RawKeyIndex(K const &) const;
    void Extend();
    int GetBalance(Node *);

    // A utility function to get height  
    // of the tree  
    int Height(Node *);
    Node * LeftRotate(Node *);
    Node * RightRotate(Node * a_y);

    //Constructs a new node after a_pParent. New node will point back to a_pParent.
    Node * NewNode(Node * a_pParent, K const & a_key, V const & a_data);
    Node * EndNode();

    //Returns the pointer to the new node.
    //Assumes tree has at least 1 element
    Node * __Insert(Node * a_pNode, Node * a_pParent, 
                    bool a_childIsRight, 
                    K const & a_key, V const & a_data);
  private:

    Node *        m_pRoot;
    Node *        m_pNodes;
    ValueType *   m_pKVs;
    sizeType      m_nItems;
  };

  //------------------------------------------------------------------------------------------------
  // const_iterator_rand
  //------------------------------------------------------------------------------------------------
  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  AVLTreeMap<K, V, Compare>::const_iterator_rand::const_iterator_rand(ValueType const * a_pKV)
    : m_pKV(a_pKV)
  {

  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  AVLTreeMap<K, V, Compare>::const_iterator_rand::const_iterator_rand()
    : m_pKV(nullptr)
  {

  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  AVLTreeMap<K, V, Compare>::const_iterator_rand::~const_iterator_rand()
  {

  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  AVLTreeMap<K, V, Compare>::const_iterator_rand::const_iterator_rand(const_iterator_rand const & a_it)
    : m_pKV(a_it.m_pKV)
  {

  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::const_iterator_rand & 
    AVLTreeMap<K, V, Compare>::const_iterator_rand::operator=(const_iterator_rand const & a_it)
  {
    m_pKV = a_it.m_pKV;
    return *this;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  bool AVLTreeMap<K, V, Compare>::const_iterator_rand::operator==(const_iterator_rand const & a_it) const
  {
    return m_pKV == a_it.m_pKV;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  bool AVLTreeMap<K, V, Compare>::const_iterator_rand::operator!=(const_iterator_rand const & a_it) const
  {
    return m_pKV != a_it.m_pKV;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::const_iterator_rand & 
    AVLTreeMap<K, V, Compare>::const_iterator_rand::operator++()
  {
    m_pKV++;
    return *this;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::const_iterator_rand
    AVLTreeMap<K, V, Compare>::const_iterator_rand::operator++(int)
  {
    const_iterator_rand result(*this);
    ++(*this);
    return result;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::const_iterator_rand & 
    AVLTreeMap<K, V, Compare>::const_iterator_rand::operator--()
  {
    m_pKV--;
    return *this;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::const_iterator_rand
    AVLTreeMap<K, V, Compare>::const_iterator_rand::operator--(int)
  {
    const_iterator_rand result(*this);
    --(*this);
    return result;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::ValueType const * 
    AVLTreeMap<K, V, Compare>::const_iterator_rand::operator->() const
  {
    return m_pKV;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::ValueType const & 
    AVLTreeMap<K, V, Compare>::const_iterator_rand::operator*() const
  {
    return *m_pKV;
  }

  //------------------------------------------------------------------------------------------------
  // iterator_rand
  //------------------------------------------------------------------------------------------------
  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  AVLTreeMap<K, V, Compare>::iterator_rand::iterator_rand(ValueType * a_pKV)
    : m_pKV(a_pKV)
  {

  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  AVLTreeMap<K, V, Compare>::iterator_rand::iterator_rand()
    : m_pKV(nullptr)
  {

  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  AVLTreeMap<K, V, Compare>::iterator_rand::~iterator_rand()
  {

  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  AVLTreeMap<K, V, Compare>::iterator_rand::iterator_rand(iterator_rand const & a_it)
    : m_pKV(a_it.m_pKV)
  {

  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::iterator_rand & 
    AVLTreeMap<K, V, Compare>::iterator_rand::operator=(iterator_rand const & a_it)
  {
    m_pKV = a_it.m_pKV;
    return *this;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  bool AVLTreeMap<K, V, Compare>::iterator_rand::operator==(iterator_rand const & a_it) const
  {
    return m_pKV == a_it.m_pKV;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  bool AVLTreeMap<K, V, Compare>::iterator_rand::operator!=(iterator_rand const & a_it) const
  {
    return m_pKV != a_it.m_pKV;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::iterator_rand & 
    AVLTreeMap<K, V, Compare>::iterator_rand::operator++()
  {
    m_pKV++;
    return *this;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::iterator_rand
    AVLTreeMap<K, V, Compare>::iterator_rand::operator++(int)
  {
    iterator_rand result(*this);
    ++(*this);
    return result;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::iterator_rand & 
    AVLTreeMap<K, V, Compare>::iterator_rand::operator--()
  {
    m_pKV--;
    return *this;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::iterator_rand
    AVLTreeMap<K, V, Compare>::iterator_rand::operator--(int)
  {
    iterator_rand result(*this);
    --(*this);
    return result;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  AVLTreeMap<K, V, Compare>::iterator_rand::operator
    typename AVLTreeMap<K, V, Compare>::const_iterator_rand() const
  {
    return AVLTreeMap<K, V, Compare>::const_iterator_rand(m_pKV);
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::ValueType * 
    AVLTreeMap<K, V, Compare>::iterator_rand::operator->()
  {
    return m_pKV;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::ValueType & 
    AVLTreeMap<K, V, Compare>::iterator_rand::operator*()
  {
    return *m_pKV;
  }


  //------------------------------------------------------------------------------------------------
  // const_iterator
  //------------------------------------------------------------------------------------------------
  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  AVLTreeMap<K, V, Compare>::const_iterator::const_iterator(Node const * a_pNode)
    : m_pNode(a_pNode)
  {

  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  AVLTreeMap<K, V, Compare>::const_iterator::const_iterator()
    : m_pNode(nullptr)
  {

  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  AVLTreeMap<K, V, Compare>::const_iterator::~const_iterator()
  {

  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  AVLTreeMap<K, V, Compare>::const_iterator::const_iterator(const_iterator const & a_it)
    : m_pNode(a_it.m_pNode)
  {

  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::const_iterator & 
    AVLTreeMap<K, V, Compare>::const_iterator::operator=(const_iterator const & a_it)
  {
    m_pNode = a_it.m_pNode;
    return *this;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  bool AVLTreeMap<K, V, Compare>::const_iterator::operator==(const_iterator const & a_it) const
  {
    return m_pNode == a_it.m_pNode;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  bool AVLTreeMap<K, V, Compare>::const_iterator::operator!=(const_iterator const & a_it) const
  {
    return m_pNode != a_it.m_pNode;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::const_iterator & 
    AVLTreeMap<K, V, Compare>::const_iterator::operator++()
  {
    if (m_pNode->pRight != nullptr)
    {
      //If right, take right, then take left all the way you can, then return.
      m_pNode = m_pNode->pRight;
      while (m_pNode->pLeft != nullptr)
      {
        m_pNode = m_pNode->pLeft;
      }
      return *this;
    }

    //If no right, go up
    Node const * pOldNode;
    do
    {
      pOldNode = m_pNode;
      m_pNode = m_pNode->pParent;
    } while (pOldNode == m_pNode->pRight);
    return *this;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::const_iterator
    AVLTreeMap<K, V, Compare>::const_iterator::operator++(int)
  {
    const_iterator result(*this);
    ++(*this);
    return result;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::const_iterator & 
    AVLTreeMap<K, V, Compare>::const_iterator::operator--()
  {
    //Try left
    if (m_pNode->pLeft != nullptr)
    {
      //If left, take left, then take left all the way you can, then return.
      m_pNode = m_pNode->pLeft;
      while (m_pNode->pRight != nullptr)
      {
        m_pNode = m_pNode->pRight;
      }
      return *this;
    }

    //If no right, go up
    Node const * pOldNode;
    do
    {
      pOldNode = m_pNode;
      m_pNode = m_pNode->pParent;
    } while (pOldNode == m_pNode->pLeft);
    return *this;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::const_iterator
    AVLTreeMap<K, V, Compare>::const_iterator::operator--(int)
  {
    const_iterator result(*this);
    --(*this);
    return result;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::ValueType const * 
    AVLTreeMap<K, V, Compare>::const_iterator::operator->() const
  {
    return m_pNode->pKV;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::ValueType const & 
    AVLTreeMap<K, V, Compare>::const_iterator::operator*() const
  {
    return &(*m_pNode->pKV);
  }

  //------------------------------------------------------------------------------------------------
  // iterator
  //------------------------------------------------------------------------------------------------
  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  AVLTreeMap<K, V, Compare>::iterator::iterator(Node * a_pNode)
    : m_pNode(a_pNode)
  {

  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  AVLTreeMap<K, V, Compare>::iterator::iterator()
    : m_pNode(nullptr)
  {

  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  AVLTreeMap<K, V, Compare>::iterator::~iterator()
  {

  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  AVLTreeMap<K, V, Compare>::iterator::iterator(iterator const & a_it)
    : m_pNode(a_it.m_pNode)
  {

  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::iterator & 
    AVLTreeMap<K, V, Compare>::iterator::operator=(iterator const & a_it)
  {
    m_pNode = a_it.m_pNode;
    return *this;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  bool AVLTreeMap<K, V, Compare>::iterator::operator==(iterator const & a_it) const
  {
    return m_pNode == a_it.m_pNode;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  bool AVLTreeMap<K, V, Compare>::iterator::operator!=(iterator const & a_it) const
  {
    return m_pNode != a_it.m_pNode;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::iterator & 
    AVLTreeMap<K, V, Compare>::iterator::operator++()
  {
    if (m_pNode->pRight != nullptr)
    {
      //If right, take right, then take left all the way you can, then return.
      m_pNode = m_pNode->pRight;
      while (m_pNode->pLeft != nullptr)
      {
        m_pNode = m_pNode->pLeft;
      }
      return *this;
    }

    //If no right, go up
    Node * pOldNode;
    do
    {
      pOldNode = m_pNode;
      m_pNode = m_pNode->pParent;
    } while (pOldNode == m_pNode->pRight);
    return *this;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::iterator
    AVLTreeMap<K, V, Compare>::iterator::operator++(int)
  {
    iterator result(*this);
    ++(*this);
    return result;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::iterator & 
    AVLTreeMap<K, V, Compare>::iterator::operator--()
  {
    //Try left
    if (m_pNode->pLeft != nullptr)
    {
      //If left, take left, then take left all the way you can, then return.
      m_pNode = m_pNode->pLeft;
      while (m_pNode->pRight != nullptr)
      {
        m_pNode = m_pNode->pRight;
      }
      return *this;
    }

    //If no right, go up
    Node * pOldNode;
    do
    {
      pOldNode = m_pNode;
      m_pNode = m_pNode->pParent;
    } while (pOldNode == m_pNode->pLeft);
    return *this;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::iterator
    AVLTreeMap<K, V, Compare>::iterator::operator--(int)
  {
    iterator result(*this);
    --(*this);
    return result;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::ValueType * 
    AVLTreeMap<K, V, Compare>::iterator::operator->()
  {
    return m_pNode->pKV;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::ValueType & 
    AVLTreeMap<K, V, Compare>::iterator::operator*()
  {
    return *m_pNode->pKV;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  AVLTreeMap<K, V, Compare>::iterator::operator
    typename AVLTreeMap<K, V, Compare>::const_iterator() const
  {
    return AVLTreeMap<K, V, Compare>::const_iterator(m_pNode);
  }

  //------------------------------------------------------------------------------------------------
  // AVLTreeMap
  //------------------------------------------------------------------------------------------------
  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  AVLTreeMap<K, V, Compare>::AVLTreeMap()
    : ContainerBase()
    , m_pKVs(nullptr)
    , m_pNodes(nullptr)
    , m_nItems(0)
    , m_pRoot(nullptr)
  {
    InitMemory();
    InitDefaultNode();
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  AVLTreeMap<K, V, Compare>::AVLTreeMap(sizeType a_request)
    : ContainerBase(a_request)
    , m_pKVs(nullptr)
    , m_pNodes(nullptr)
    , m_nItems(0)
    , m_pRoot(nullptr)
  {
    InitMemory();
    InitDefaultNode();
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  AVLTreeMap<K, V, Compare>::~AVLTreeMap()
  {
    DestructAll();
    free(m_pKVs);
    free(m_pNodes);
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  AVLTreeMap<K, V, Compare>::AVLTreeMap(AVLTreeMap const & a_other)
    : ContainerBase(a_other.pool_size())
    , m_pKVs(nullptr)
    , m_pNodes(nullptr)
    , m_nItems(0)
    , m_pRoot(nullptr)
  {
    Init(a_other);
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  AVLTreeMap<K, V, Compare> & 
    AVLTreeMap<K, V, Compare>::operator=(AVLTreeMap const & a_other)
  {
    if (this != &a_other)
    {
      DestructAll();
      Init(a_other);
    }
    return *this;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  AVLTreeMap<K, V, Compare>::AVLTreeMap(AVLTreeMap && a_other)
    : m_pKVs(a_other.m_pKVs)
    , m_pNodes(a_other.m_pNodes)
    , m_nItems(a_other.m_nItems)
    , m_pRoot(a_other.m_pRoot)
  {
    a_other.m_pKVs = nullptr;
    a_other.m_pNodes = nullptr;
    a_other.m_nItems = 0;
    a_other.m_pRoot = s_nullValue;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  AVLTreeMap<K, V, Compare> &
    AVLTreeMap<K, V, Compare>::operator=(AVLTreeMap && a_other)
  {
    if (this != &a_other)
    {
      m_pKVs = a_other.m_pKVs;
      m_pNodes = a_other.m_pNodes;
      m_nItems = a_other.m_nItems;
      m_pRoot = a_other.m_pRoot;

      a_other.m_pKVs = nullptr;
      a_other.m_pNodes = nullptr;
      a_other.m_nItems = 0;
      a_other.m_pRoot = s_nullValue;
    }
    return *this;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::sizeType
    AVLTreeMap<K, V, Compare>::AVLTreeMap::size() const
  {
    return m_nItems;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  bool AVLTreeMap<K, V, Compare>::AVLTreeMap::empty() const
  {
    return m_nItems == 0;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::AVLTreeMap::iterator_rand
    AVLTreeMap<K, V, Compare>::AVLTreeMap::begin_rand()
  {
    return iterator_rand(m_pKVs);
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::AVLTreeMap::iterator_rand
    AVLTreeMap<K, V, Compare>::AVLTreeMap::end_rand()
  {
    return iterator_rand(m_pKVs + m_nItems);
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::AVLTreeMap::const_iterator_rand
    AVLTreeMap<K, V, Compare>::AVLTreeMap::cbegin_rand()
  {
    return const_iterator_rand(m_pKVs);
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::AVLTreeMap::const_iterator_rand
    AVLTreeMap<K, V, Compare>::AVLTreeMap::cend_rand()
  {
    return const_iterator_rand(m_pKVs + m_nItems);
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::AVLTreeMap::iterator
    AVLTreeMap<K, V, Compare>::AVLTreeMap::begin()
  {
    Node * pNode = m_pRoot;
    while (pNode->pLeft != nullptr)
    {
      pNode = pNode->pLeft;
    }
    return iterator(pNode);
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::AVLTreeMap::iterator
    AVLTreeMap<K, V, Compare>::AVLTreeMap::end()
  {
    return iterator(m_pNodes + m_nItems);
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::AVLTreeMap::const_iterator
    AVLTreeMap<K, V, Compare>::AVLTreeMap::cbegin()
  {
    Node * pNode = m_pRoot;
    while (pNode->pLeft != nullptr)
    {
      pNode = pNode->pLeft;
    }
    return const_iterator(pNode);
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::AVLTreeMap::const_iterator
    AVLTreeMap<K, V, Compare>::AVLTreeMap::cend()
  {
    return const_iterator(m_pNodes + m_nItems);
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::AVLTreeMap::const_iterator
    AVLTreeMap<K, V, Compare>::AVLTreeMap::find(K const & a_key) const
  {
    Node * pNode;
    if (KeyExists(a_key, pNode))
    {
      return const_iterator(pNode);
    }
    return end();
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::AVLTreeMap::iterator
    AVLTreeMap<K, V, Compare>::AVLTreeMap::find(K const & a_key)
  {
    Node * pNode;
    if (KeyExists(a_key, pNode))
    {
      return iterator(pNode);
    }
    return end();
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  V & AVLTreeMap<K, V, Compare>::AVLTreeMap::operator[](K const & a_key)
  {
    Node * pNode = insert(a_key, V());
    return pNode->pKV;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::iterator AVLTreeMap<K, V, Compare>::insert(K const & a_key, V const & a_data)
  {
    if ((m_nItems + 1) == pool_size())
      Extend();

    if (m_nItems == 0)
    {
      NewNode(nullptr, a_key, a_data);
      m_pRoot->pRight = EndNode();
      EndNode()->pParent = m_pRoot;
    }
    else
    {
      m_pRoot = __Insert(m_pRoot, nullptr, false, a_key, a_data);
    }

    return iterator(&m_pNodes[m_nItems - 1]);
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  V & AVLTreeMap<K, V, Compare>::AVLTreeMap::at(K const & a_key)
  {
    return RawKeyIndex(a_key)->second;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  V const & AVLTreeMap<K, V, Compare>::AVLTreeMap::at(K const & a_key) const
  {
    return RawKeyIndex(a_key)->second;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  void AVLTreeMap<K, V, Compare>::AVLTreeMap::clear()
  {
    DestructAll();
    m_nItems = 0;
    InitDefaultNode();
  }

#ifdef DEBUG
  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::sizeType
    AVLTreeMap<K, V, Compare>::AVLTreeMap::Ind(Node * a_pNode) const
  {
    return a_pNode - m_pNodes;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  void AVLTreeMap<K, V, Compare>::PrintNode(Node * a_pNode) const
  {
    std::cout << "Index: ";
    if (a_pNode == nullptr)
      std::cout << "NULL";
    else
      std::cout << Ind(a_pNode);

    std::cout << ", Left: ";
    if (a_pNode->pLeft == nullptr)
      std::cout << "NULL";
    else
      std::cout << Ind(a_pNode->pLeft);

    std::cout << ", Right: ";
    if (a_pNode->pRight == nullptr)
      std::cout << "NULL";
    else
      std::cout << Ind(a_pNode->pRight);

    std::cout << ", Parent: ";
    if (a_pNode->pParent == nullptr)
      std::cout << "NULL";
    else
      std::cout << Ind(a_pNode->pParent);

    std::cout << ", Key: ";
    if (a_pNode->pKV == nullptr)
      std::cout << "NULL";
    else
      std::cout << a_pNode->pKV->first;

    std::cout << ", Height: " << a_pNode->height << "\n\n";
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  void AVLTreeMap<K, V, Compare>::Print() const
  {
    std::cout << "nItems: " << m_nItems << '\n';
    std::cout << "Root:\n";
    PrintNode(m_pRoot);
    for (sizeType i = 0; i < m_nItems; i++)
    {
      PrintNode(&m_pNodes[i]);
    } 
    std::cout << "End:\n";
    PrintNode(EndNode());
  }
#endif

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  void AVLTreeMap<K, V, Compare>::DestructAll()
  {
    for (sizeType i = 0; i < m_nItems; i++)
    {
      m_pKVs[i].~ValueType();
    }
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  void AVLTreeMap<K, V, Compare>::InitMemory()
  {
    m_pNodes = static_cast<Node*> (realloc(m_pNodes, pool_size() * sizeof(Node)));
    m_pKVs = static_cast<ValueType*> (realloc(m_pKVs, pool_size() * sizeof(ValueType)));
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  void AVLTreeMap<K, V, Compare>::InitDefaultNode()
  {
    m_pRoot = &m_pNodes[0];
    Node endNode;
    endNode.pParent = nullptr;
    endNode.pLeft = nullptr;
    endNode.pRight = nullptr;
    endNode.pKV = nullptr;
    endNode.height = 0;
    new (&m_pNodes[0]) Node(endNode);
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  void AVLTreeMap<K, V, Compare>::Init(AVLTreeMap const & a_other)
  {
    pool_size(a_other.pool_size());
    InitMemory();

    m_nItems = a_other.m_nItems;
    m_pRoot = m_pNodes + (a_other.m_pRoot - a_other.m_pNodes);

    for (sizeType i = 0; i < m_nItems; i++)
    {
      Node newNode;
      newNode.pParent = m_pNodes + (a_other.m_pNodes[i].pParent - a_other.m_pNodes);
      newNode.pLeft = m_pNodes + (a_other.m_pNodes[i].pLeft - a_other.m_pNodes);
      newNode.pRight = m_pNodes + (a_other.m_pNodes[i].pRight - a_other.m_pNodes);
      newNode.pKV = m_pNodes + (a_other.m_pNodes[i].pKV - a_other.m_pKVs);

      new (&m_pNodes[i]) Node(newNode);
      new (&m_pKVs[i]) ValueType(a_other.m_pKVs[i]);
    }
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  bool AVLTreeMap<K, V, Compare>::KeyExists(K const & a_key, Node *& a_out) const
  {
    a_out = m_pRoot;

    if (m_nItems == 0)
    {
      return false;
    }

    ValueType * pKV = a_out->pKV;

    bool result = false;
    do
    {
      if (pKV->first == a_key)
      {
        result = true;
        break;
      }

      bool left = Compare(a_key, pKV->first);

      if (left)
      {
        if (a_out->pLeft != nullptr)
        {
          a_out = a_out->Left;
        }
        else
        {
          break;
        }
      }
      else
      {
        if (a_out->pRight != nullptr)
        {
          a_out = a_out->pRight;
        }
        else
        {
          break;
        }
      }
    } while (true);
    return result;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::Node * 
    AVLTreeMap<K, V, Compare>::RawKeyIndex(K const & a_key) const
  {
    Node * pResult;
    if (!KeyExists(a_key, pResult))
    {
      throw std::out_of_range("Invalid key!");
    }
    return pResult;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  void AVLTreeMap<K, V, Compare>::Extend()
  {
    set_next_pool_size();

    Node * oldNodes = m_pNodes;
    ValueType * oldKVs = m_pKVs;

    m_pNodes = static_cast<Node*> (realloc(m_pNodes, pool_size() * sizeof(Node)));
    m_pKVs = static_cast<ValueType*> (realloc(m_pKVs, pool_size() * sizeof(ValueType)));

    if (oldNodes != m_pNodes)
    {
      m_pRoot = m_pNodes + (m_pRoot - oldNodes);
      for (sizeType i = 0; i < m_nItems + 1; i++)
      {
        if (m_pNodes[i].pParent)
          m_pNodes[i].pParent = m_pNodes + (m_pNodes[i].pParent - oldNodes);

        if (m_pNodes[i].pLeft)
          m_pNodes[i].pLeft = m_pNodes + (m_pNodes[i].pLeft - oldNodes);

        if (m_pNodes[i].pRight)
          m_pNodes[i].pRight = m_pNodes + (m_pNodes[i].pRight - oldNodes);

        m_pNodes[i].height = m_pNodes[i].height;
        m_pNodes[i].pKV = m_pNodes[i].pKV;
      }
    }

    if (oldKVs != m_pKVs)
    {
      for (sizeType i = 0; i < m_nItems; i++)
      {
        m_pNodes[i].pKV = m_pKVs + (m_pNodes[i].pKV - oldKVs);
      }
    }
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  int AVLTreeMap<K, V, Compare>::GetBalance(Node * a_pNode)
  {  
    if (a_pNode == nullptr)
    {
      return 0;  
    }
    return Height(a_pNode->pLeft) - Height(a_pNode->pRight);  
  } 

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  int AVLTreeMap<K, V, Compare>::Height(Node * a_pNode)  
  {  
    if (a_pNode == nullptr)  
      return 0;  
    return a_pNode->height;  
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::Node * 
    AVLTreeMap<K, V, Compare>::LeftRotate(Node * a_x)
  {  
    Node * y = a_x->pRight;  
    Node * T2 = y->pLeft;  
    Node * xParent = a_x->pParent;

    // Perform rotation  
    y->pLeft = a_x;  
    y->pParent = xParent;
    a_x->pParent = y;
    a_x->pRight = T2; 
    if (T2)
      T2->pParent = a_x;

    if (xParent)
    {
      if (xParent->pLeft == a_x)
        xParent->pLeft = y;
      else if (xParent->pRight == a_x)
        xParent->pRight = y;
    }

    // Update heights  
    a_x->height = impl::Max(Height(a_x->pLeft),  
      Height(a_x->pRight)) + 1;  
    y->height = impl::Max(Height(y->pLeft),  
      Height(y->pRight)) + 1;  

    // Return new root  
    return y;  
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::Node * 
    AVLTreeMap<K, V, Compare>::RightRotate(Node * a_y)
  {  
    Node * x = a_y->pLeft;  
    Node * T2 = x->pRight;  
    Node * yParent = a_y->pParent;

    // Perform rotation
    x->pRight = a_y;
    x->pParent = yParent;
    a_y->pParent = x;
    a_y->pLeft = T2;
    if (T2)
      T2->pParent = a_y;

    if (yParent)
    {
      if (yParent->pLeft == a_y)
        yParent->pLeft = x;
      else if (yParent->pRight == a_y)
        yParent->pRight = x;
    }

    // Update heights  
    a_y->height = impl::Max(Height(a_y->pLeft),  
      Height(a_y->pRight)) + 1;  
    x->height = impl::Max(Height(x->pLeft),  
      Height(x->pRight)) + 1;  

    // Return new root  
    return x;  
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::Node * 
    AVLTreeMap<K, V, Compare>::NewNode(Node * a_pParent, K const & a_key, V const & a_data)
  {
    //Insert data
    new (&m_pKVs[m_nItems]) ValueType{a_key, a_data};

    //Shift end node over one
    memcpy(&m_pNodes[m_nItems + 1], &m_pNodes[m_nItems], sizeof(Node));
    Node * endNode = &m_pNodes[m_nItems + 1];

    if (endNode->pParent)
    {
      //Notify the last element in the tree that the end node has shifted
      endNode->pParent->pRight = endNode;
    }

    //Insert new node
    Node * newNode = &m_pNodes[m_nItems];
    newNode->pLeft = nullptr;
    newNode->pRight = nullptr;
    newNode->pKV = &m_pKVs[m_nItems];
    newNode->pParent = a_pParent;
    newNode->height = 1;

    m_nItems++;
    return newNode;
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::Node * 
    AVLTreeMap<K, V, Compare>::EndNode()
  {
    return &m_pNodes[m_nItems];
  }

  template<typename K, typename V, bool (*Compare)(K const &, K const &)>
  typename AVLTreeMap<K, V, Compare>::Node * 
    AVLTreeMap<K, V, Compare>::__Insert(Node * a_pNode, Node * a_pParent, 
      bool a_childIsRight, 
      K const & a_key, V const & a_data)
  {
    //Check if leaf node or final node in the tree. The final node will point
    //to the end node.

    bool isEndNode = a_pNode == EndNode();

    if (a_pNode == nullptr || isEndNode )
    {
      Node * pNode = NewNode(a_pParent, a_key, a_data);

      //The right child of the parent might be the end node.
      if (a_pParent && a_childIsRight)
      {
        pNode->pRight = a_pParent->pRight;
      }

      if (isEndNode)
      {
        EndNode()->pParent = pNode;
      }

      return pNode;
    }

    //Same key
    if (a_key == a_pNode->pKV->first)
      return a_pNode;

    if (Compare(a_key, a_pNode->pKV->first))
      a_pNode->pLeft = __Insert(a_pNode->pLeft, a_pNode, false, a_key, a_data);
    else
      a_pNode->pRight = __Insert(a_pNode->pRight, a_pNode, true, a_key, a_data);

    a_pNode->height = 1 + impl::Max(Height(a_pNode->pLeft), Height(a_pNode->pRight));
    int balance = GetBalance(a_pNode);

    // Left Left Case  
    if (balance > 1 && Compare(a_key, a_pNode->pLeft->pKV->first))  
      return RightRotate(a_pNode);  

    // Right Right Case  
    if (balance < -1 && !Compare(a_key, a_pNode->pRight->pKV->first))  
      return LeftRotate(a_pNode);  

    // Left Right Case  
    if (balance > 1 && !Compare(a_key, a_pNode->pLeft->pKV->first))  
    {  
      a_pNode->pLeft = LeftRotate(a_pNode->pLeft);  
      return RightRotate(a_pNode);  
    }  

    // Right Left Case  
    if (balance < -1 && Compare(a_key, a_pNode->pRight->pKV->first))  
    {
      a_pNode->pRight = RightRotate(a_pNode->pRight);
      return LeftRotate(a_pNode);
    }  

    /* return the (unchanged) node pointer */
    return a_pNode;
  }
}

#endif