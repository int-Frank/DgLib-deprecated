//! @file DgPolygon2D.h
//!
//! @author: Frank B. Hart
//! @date 21/05/2016
//!
//! Class declaration: Polygon2D


#ifndef DGPOLYGON2D_H
#define DGPOLYGON2D_H

#include <cstring>
#include <new>
#include <type_traits>

#include "impl/DgContainerBase.h"
#include "DgErrorHandler.h"

#include "DgVector3.h"

//#define DG_DEBUG
#ifdef DG_DEBUG
#include <iostream>
#include <cstddef>
#endif

namespace Dg
{
  //! @ingroup DgContainers
  //!
  //! @class Polygon2D
  //!
  //! Circular, pre-allocated doubly linked Polygon2D. 
  //!
  //! @author Frank B. Hart
  //! @date 25/08/2016
  template<typename Real>
  class Polygon2D : public ContainerBase
  {
    typedef Vector3<Real> vec2D;

  private:

    //! Container to hold the object and pointers
    class Node
    {
    public:

      //! Default constructor
      Node() : pNext(nullptr), pPrev(nullptr) {}

      //! Copy the vertex in this node
      inline void InitData(vec2D const & a_vertex)
      {
        new (&vertex) vec2D(a_vertex);
      }

      //! Destruct the vertex is a destructor exists for the type.
      inline void DestructData()
      {
        vertex.~vec2D();
      }

      //! Breaks a node out its chain.
      //! Assumes pNext and pPrev are valid pointers.
      inline void Break()
      {
        pNext->pPrev = pPrev;
        pPrev->pNext = pNext;
      }

      //! Add node to the chain after the input node.
      inline void InsertAfter(Node * a_pNode)
      {
        pNext = a_pNode->pNext;
        pPrev = a_pNode;
        a_pNode->pNext->pPrev = this;
        a_pNode->pNext = this;
      }

      inline vec2D & GetVertex() { return vertex; }
      inline vec2D const & GetVertex() const { return vertex; }

      inline Node * Next() { return pNext; }
      inline Node const * Next() const { return pNext; }
      inline void Next(Node * a_pNext) { pNext = a_pNext; }

      inline Node * Prev() { return pPrev; }
      inline Node const * Prev() const { return pPrev; }
      inline void Prev(Node * a_pPrev) { pPrev = a_pPrev; }

    private:

      Node*     pNext;
      Node*     pPrev;
      vec2D     vertex;
    };

  public:

    //! @class const_iterator
    //!
    //! Const iterator for the Polygon2D.
    //!
    //! @author Frank B. Hart
    //! @date 21/05/2016
    class const_iterator
    {
    private:
      friend class Polygon2D;

    private:
      //! Special constructor, not for external use
      const_iterator(Node const * _ptr) { m_pNode = _ptr; }

    public:

      const_iterator() : m_pNode(nullptr) {}
      ~const_iterator() {}

      //! Copy constructor
      const_iterator(const_iterator const & it) : m_pNode(it.m_pNode) {}

      //! Assignment
      const_iterator& operator= (const_iterator const &);

      //! Comparison.
      bool operator==(const_iterator const & it) const { return m_pNode == it.m_pNode; }

      //! Comparison.
      bool operator!=(const_iterator const & it) const { return m_pNode != it.m_pNode; }

      //! Post increment
      const_iterator& operator++();

      //! Pre increment
      const_iterator operator++(int);

      //! Post decrement
      const_iterator& operator--();

      //! Pre decrement
      const_iterator operator--(int);

      //! Conversion. Returns pointer to data.
      vec2D const * operator->() const { return &(m_pNode->GetData()); }

      //! Conversion. Returns reference to data.
      vec2D const & operator*() const { return m_pNode->GetData(); }

    private:
      Node const * m_pNode;

    };


    //! @class iterator
    //!
    //! Iterator for the Polygon2D.
    //!
    //! @author Frank B. Hart
    //! @date 21/05/2016
    class iterator
    {
      friend class Polygon2D;

    private:
      //! Special constructor, not for external use
      iterator(Node* _ptr) { m_pNode = _ptr; }

    public:

      iterator() : m_pNode(nullptr) {}
      ~iterator() {}

      //! Copy constructor.
      iterator(iterator const & it) : m_pNode(it.m_pNode) {}

      //! Assignment.
      iterator& operator= (iterator const &);

      //! Comparison.
      bool operator==(iterator const & it) const { return m_pNode == it.m_pNode; }

      //! Comparison.
      bool operator!=(iterator const & it) const { return m_pNode != it.m_pNode; }

      //! Post increment
      iterator& operator++();

      //! Pre increment
      iterator operator++(int);

      //! Post decrement
      iterator& operator--();

      //! Pre decrement
      iterator operator--(int);

      //! Conversion
      operator const_iterator() const { return const_iterator(m_pNode); }

      //! Conversion
      vec2D* operator->() const { return &(m_pNode->GetData()); }

      //! Conversion
      vec2D& operator*() const { return m_pNode->GetData(); }

    private:
      Node *m_pNode;

    };


  public:
    //! Constructor 
    //! If the method fails to allocate memory, the error is logged to the Dg error file and 
    //! the code aborted.
    Polygon2D();

    //! Destructor.
    ~Polygon2D();

    //! Copy constructor
    //! If the method fails to allocate memory, the error is logged to the Dg error file and 
    //! the code aborted.
    Polygon2D(Polygon2D const &);

    //! Assignment
    //! If the method fails to allocate memory, the error is logged to the Dg error file and 
    //! the code aborted.
    Polygon2D & operator=(Polygon2D const &);

    //! Returns an iterator pointing to the first data in the Polygon2D container.
    //! If the container is empty, the returned iterator value shall not be dereferenced.
    //!
    //! @return iterator
    iterator head() { return iterator(m_pHead); }

    //! Returns a const iterator pointing to the first data in the Polygon2D container.
    //! If the container is empty, the returned iterator value shall not be dereferenced.
    //!
    //! @return const_iterator
    const_iterator chead() const { return const_iterator(m_pHead); }

    //! Returns number of elements in the Polygon2D.
    size_t size() const { return m_nItems; }

    //! Returns if the Polygon2D is empty.
    bool empty() const { return m_nItems == 0; }

    //! Add an data to the back of the Polygon2D
    void push_back(vec2D const &);

    //!Add an data to the front of the Polygon2D
    void push_front(vec2D const &);

    //! The container is extended by inserting a new element
    //! before the element at the specified position.
    //! The input iterator may be invalid as a resize may have occured. Resizing
    //! invalidates all pointers in the iterator. The return iterator will be valid.
    //! @return iterator to the newly inserted data
    iterator insert(iterator const & position, vec2D const & data);

    //! Erase last data
    void pop_back();

    //! Erase last data
    void pop_front();

    //! Erase an data from the Polygon2D
    //! @return An iterator pointing to the data that followed 
    //!         the last data erased by the function call.
    iterator erase(iterator const &);

    //! Clear the Polygon2D, retains allocated memory.
    void clear();

#ifdef DG_DEBUG
    void Print() const;
#endif
  private:

    // Increases the size of the underlying arrays by a factor of 2
    void Extend();

    // Destruct all objects in the Polygon2D
    void DestructAll();

    // Initialize the Polygon2D and request a size.
    void init(size_t);

    // Reset array pointers
    void AssignPointersToEmpty();

    // Break a new node off from the Polygon2D of free nodes and add it to the Polygon2D after the input node.
    Node * InsertNewAfter(Node * a_pNode, vec2D const & a_data);

    // Remove a node from the Polygon2D and add to the Polygon2D of free nodes.
    void Remove(Node *);

  private:

    Node *    m_pNodePool;      //Pre-allocated block of memory to hold items
    Node *    m_pNextFree;  //Pointer to the next free item in the Polygon2D;
    Node *    m_pHead;  
    size_t    m_nItems;     //Number of items currently in the Polygon2D
  };

  //--------------------------------------------------------------------------------
  //	@	Polygon2D<vec2D>::iterator::operator=()
  //--------------------------------------------------------------------------------
  template<typename vec2D>
  typename Polygon2D<vec2D>::iterator& Polygon2D<vec2D>::iterator::operator=
    (typename Polygon2D<vec2D>::iterator const & a_other)
  {
    m_pNode = a_other.m_pNode;
    return *this;
  }

  //--------------------------------------------------------------------------------
  //	@	Polygon2D<vec2D>::iterator::operator++()
  //--------------------------------------------------------------------------------
  template<typename vec2D>
  typename Polygon2D<vec2D>::iterator& Polygon2D<vec2D>::iterator::operator++()
  {
    m_pNode = m_pNode->Next();
    return *this;
  }

  //--------------------------------------------------------------------------------
  //	@	Polygon2D<vec2D>::iterator::operator++()
  //--------------------------------------------------------------------------------
  template<typename vec2D>
  typename Polygon2D<vec2D>::iterator Polygon2D<vec2D>::iterator::operator++(int)
  {
    iterator result(*this);
    ++(*this);
    return result;
  }

  //--------------------------------------------------------------------------------
  //	@	Polygon2D<vec2D>::iterator::operator--()
  //--------------------------------------------------------------------------------
  template<typename vec2D>
  typename Polygon2D<vec2D>::iterator& Polygon2D<vec2D>::iterator::operator--()
  {
    m_pNode = m_pNode->Prev();
    return *this;
  }

  //--------------------------------------------------------------------------------
  //	@	Polygon2D<vec2D>::iterator::operator--()
  //--------------------------------------------------------------------------------
  template<typename vec2D>
  typename Polygon2D<vec2D>::iterator Polygon2D<vec2D>::iterator::operator--(int)
  {
    iterator result(*this);
    --(*this);
    return result;

  }

  //--------------------------------------------------------------------------------
  //	@	Polygon2D<vec2D>::const_iterator::operator=()
  //--------------------------------------------------------------------------------
  template<typename vec2D>
  typename Polygon2D<vec2D>::const_iterator& Polygon2D<vec2D>::const_iterator::operator=
    (typename Polygon2D<vec2D>::const_iterator const & a_other)
  {
    m_pNode = a_other.m_pNode;
    return *this;
  }

  //--------------------------------------------------------------------------------
  //	@	Polygon2D<vec2D>::const_iterator::operator++()
  //--------------------------------------------------------------------------------
  template<typename vec2D>
  typename Polygon2D<vec2D>::const_iterator& Polygon2D<vec2D>::const_iterator::operator++()
  {
    m_pNode = m_pNode->Next();
    return *this;
  }

  //--------------------------------------------------------------------------------
  //	@	Polygon2D<vec2D>::const_iterator::operator=()
  //--------------------------------------------------------------------------------
  template<typename vec2D>
  typename Polygon2D<vec2D>::const_iterator Polygon2D<vec2D>::const_iterator::operator++(int)
  {
    const_iterator result(*this);
    ++(*this);
    return result;
  }

  //--------------------------------------------------------------------------------
  //	@	Polygon2D<vec2D>::const_iterator::operator--()
  //--------------------------------------------------------------------------------
  template<typename vec2D>
  typename Polygon2D<vec2D>::const_iterator& Polygon2D<vec2D>::const_iterator::operator--()
  {
    m_pNode = m_pNode->Prev();
    return *this;
  }

  //--------------------------------------------------------------------------------
  //	@	Polygon2D<vec2D>::const_iterator::operator--()
  //--------------------------------------------------------------------------------
  template<typename vec2D>
  typename Polygon2D<vec2D>::const_iterator Polygon2D<vec2D>::const_iterator::operator--(int)
  {
    const_iterator result(*this);
    --(*this);
    return result;
  }

  //--------------------------------------------------------------------------------
  //	@	Polygon2D<vec2D>::AssignPointersToEmpty()
  //--------------------------------------------------------------------------------
  template<typename vec2D>
  void Polygon2D<vec2D>::AssignPointersToEmpty()
  {
    m_pNextFree = &m_pNodePool[0];
    m_pHead = nullptr;

    for (size_t i = 0; i < pool_size() - 1; i++)
    {
      m_pNodePool[i].Next(&m_pNodePool[i + 1]);
    }
  }

  //--------------------------------------------------------------------------------
  //	@	Polygon2D<vec2D>::init()
  //--------------------------------------------------------------------------------
  template<typename vec2D>
  void Polygon2D<vec2D>::init(size_t a_size)
  {
    pool_size(a_size);
    m_pNodePool = static_cast<Node *>(realloc(m_pNodePool, pool_size() * sizeof(Node)));
    DG_ASSERT(m_pNodePool != nullptr);
    m_nItems = 0;
    AssignPointersToEmpty();
  }

  //--------------------------------------------------------------------------------
  //	@	Polygon2D<vec2D>::Polygon2D<vec2D>()
  //--------------------------------------------------------------------------------
  template<typename vec2D>
  Polygon2D<vec2D>::Polygon2D()
    : ContainerBase()
    , m_pNodePool(nullptr)
    , m_pNextFree(nullptr)
    , m_pHead(nullptr)
    , m_nItems(0)
  {
    init(1);
  }

  //--------------------------------------------------------------------------------
  //	@	Polygon2D<vec2D>::Polygon2D<vec2D>()
  //--------------------------------------------------------------------------------
  template<typename vec2D>
  Polygon2D<vec2D>::Polygon2D(size_t a_size)
    : ContainerBase(a_size)
    , m_pNodePool(nullptr)
    , m_pNextFree(nullptr)
    , m_pHead(nullptr)
    , m_nItems(0)
  {
    init(pool_size());
  }

  //--------------------------------------------------------------------------------
  //	@	Polygon2D<vec2D>::~Polygon2D<vec2D>()
  //--------------------------------------------------------------------------------
  template<typename vec2D>
  Polygon2D<vec2D>::~Polygon2D()
  {
    DestructAll();
    free(m_pNodePool);
  }

  //--------------------------------------------------------------------------------
  //	@	Polygon2D<vec2D>::Polygon2D<vec2D>()
  //--------------------------------------------------------------------------------
  template<typename vec2D>
  Polygon2D<vec2D>::Polygon2D(Polygon2D const & other)
    : ContainerBase(other)
  {
    init(other.pool_size());

    //Assign m_pNodePool
    Polygon2D<vec2D>::const_iterator it = other.cbegin();
    for (it; it != other.cend(); ++it)
    {
      push_back(*it);
    }
  }

  //--------------------------------------------------------------------------------
  //	@	Polygon2D<vec2D>::operator=()
  //--------------------------------------------------------------------------------
  template<typename vec2D>
  Polygon2D<vec2D>& Polygon2D<vec2D>::operator=(Polygon2D const & other)
  {
    if (this == &other)
      return *this;

    ContainerBase::operator=(other);

    resize(other.pool_size());

    //Assign m_pNodePool
    Polygon2D<vec2D>::const_iterator it = other.cbegin();
    for (it; it != other.cend(); ++it)
    {
      push_back(*it);
    }

    return *this;
  }

  //--------------------------------------------------------------------------------
  //	@	Polygon2D<vec2D>::clear()
  //--------------------------------------------------------------------------------
  template<typename vec2D>
  void Polygon2D<vec2D>::clear()
  {
    DestructAll();
    m_nItems = 0;
    AssignPointersToEmpty();
  }

  //--------------------------------------------------------------------------------
  //	@	Polygon2D<vec2D>::resize()
  //--------------------------------------------------------------------------------
  template<typename vec2D>
  void Polygon2D<vec2D>::resize(size_t a_newSize)
  {
    DestructAll();
    init(a_newSize);
  }

  //--------------------------------------------------------------------------------
  //	@	Polygon2D<vec2D>::InsertNewAfter()
  //--------------------------------------------------------------------------------
  template<typename vec2D>
  typename Polygon2D<vec2D>::Node * Polygon2D<vec2D>::InsertNewAfter(Node * a_pNode, vec2D const & a_data)
  {
    if (m_nItems == pool_size())
    {
      //Extending might invalidate a_pNode, so we need to record its index in the pool.
      size_t index(a_pNode - m_pNodePool);
      Extend();
      a_pNode = &m_pNodePool[index]; //Reset the pointer in case we have extended.
    }

    Node * newNode(m_pNextFree);
    m_pNextFree = m_pNextFree->Next();
    newNode->InitData(a_data);
    newNode->InsertAfter(a_pNode);
    m_nItems++;

    return newNode;
  }

  //--------------------------------------------------------------------------------
  //	@	Polygon2D<vec2D>::push_back()
  //--------------------------------------------------------------------------------
  template<typename vec2D>
  void Polygon2D<vec2D>::push_back(vec2D const & a_item)
  {
    InsertNewAfter(m_pNodePool[0].Prev(), a_item);
  }

  //--------------------------------------------------------------------------------
  //	@	Polygon2D<vec2D>::push_front()
  //--------------------------------------------------------------------------------
  template<typename vec2D>
  void Polygon2D<vec2D>::push_front(vec2D const & a_item)
  {
    InsertNewAfter(&m_pNodePool[0], a_item);
  }

  //--------------------------------------------------------------------------------
  //	@	Polygon2D<vec2D>::Remove()
  //--------------------------------------------------------------------------------
  template<typename vec2D>
  void Polygon2D<vec2D>::Remove(Node * a_pNode)
  {
    a_pNode->DestructData();
    a_pNode->Break();
    a_pNode->Next(m_pNextFree);
    m_pNextFree = a_pNode;
    m_nItems--;
  }

  //--------------------------------------------------------------------------------
  //	@	Polygon2D<vec2D>::pop_back()
  //--------------------------------------------------------------------------------
  template<typename vec2D>
  void Polygon2D<vec2D>::pop_back()
  {
    Remove(m_pNodePool[0].Prev());
  }

  //--------------------------------------------------------------------------------
  //	@	Polygon2D<vec2D>::pop_front()
  //--------------------------------------------------------------------------------
  template<typename vec2D>
  void Polygon2D<vec2D>::pop_front()
  {
    Remove(m_pNodePool[0].Next());
  }

  //--------------------------------------------------------------------------------
  //	@	Polygon2D<vec2D>::insert()
  //--------------------------------------------------------------------------------
  template<typename vec2D>
  typename Polygon2D<vec2D>::iterator Polygon2D<vec2D>::insert(typename Polygon2D<vec2D>::iterator const & it, vec2D const & a_item)
  {
    return iterator(InsertNewAfter(it.m_pNode->Prev(), a_item));
  }

  //--------------------------------------------------------------------------------
  //	@	Polygon2D<vec2D>::erase()
  //--------------------------------------------------------------------------------
  template<typename vec2D>
  typename Polygon2D<vec2D>::iterator Polygon2D<vec2D>::erase(typename Polygon2D<vec2D>::iterator const & it)
  {
    iterator result(it.m_pNode->Next());
    Remove(it.m_pNode);
    return result;
  }

  //--------------------------------------------------------------------------------
  //	@	Polygon2D<vec2D>::DestructAll()
  //--------------------------------------------------------------------------------
  template<typename vec2D>
  void Polygon2D<vec2D>::DestructAll()
  {
    for (iterator it = begin(); it != end(); ++it)
    {
      it.m_pNode->DestructData();
    }
  }

  //--------------------------------------------------------------------------------
  //	@	Polygon2D<vec2D>::Extend()
  //--------------------------------------------------------------------------------
  template<typename vec2D>
  void Polygon2D<vec2D>::Extend()
  {
    size_t oldSize(pool_size());
    Node * pOldData(m_pNodePool);
    size_t nextFreeInd(m_pNextFree - m_pNodePool); //This assumes m_pNextFree is always valid
    set_next_pool_size();

    DG_ASSERT(pool_size() > oldSize);

    //Create new array
    m_pNodePool = static_cast<Node *>(realloc(m_pNodePool, (pool_size() + 1) * sizeof(Node)));
    DG_ASSERT(m_pNodePool != nullptr);

    for (size_t i = 0; i < oldSize; ++i)
    {
      m_pNodePool[i].Prev(&m_pNodePool[m_pNodePool[i].Prev() - pOldData]);
      m_pNodePool[i].Next(&m_pNodePool[m_pNodePool[i].Next() - pOldData]);
    }

    m_pNodePool[oldSize].Prev(&m_pNodePool[m_pNodePool[oldSize].Prev() - pOldData]);
    m_pNodePool[oldSize].Next(&m_pNodePool[oldSize + 1]);

    m_pNextFree = &m_pNodePool[nextFreeInd];

    for (size_t i = oldSize + 1; i < pool_size(); ++i)
    {
      m_pNodePool[i].Next(&m_pNodePool[i + 1]);
    }
    m_pNodePool[pool_size()].Next(nullptr);
  }

#ifdef DG_DEBUG
  void Polygon2D<vec2D>::Print() const
  {
    std::cout << "\n\nSize: " << m_nItems;
    for (size_t i = 0; i < pool_size() + 1; ++i)
    {
      std::cout << "\n";
      if (m_pNodePool[i].Prev()) std::cout << "[" << (m_pNodePool[i].Prev() - m_pNodePool) << "]";
      else std::cout << "NULL";
      std::cout << "\t<-[" << i << "]-> \t";
      if (m_pNodePool[i].Next()) std::cout << "[" << (m_pNodePool[i].Next() - m_pNodePool) << "]";
      else std::cout << "NULL";
      std::cout << "\t: " << m_pNodePool[i].GetData();
    }
    std::cout << "\nNext free: ";
    if (m_pNextFree == nullptr)
    {
      std::cout << "NULL\n";
    }
    else
    {
      std::cout << "[" << (m_pNextFree - m_pNodePool) << "]\n";
    }
  }
#endif

  //--------------------------------------------------------------------------------
  //		Helpful functions
  //--------------------------------------------------------------------------------

  //! @ingroup DgContainers_functions
  //!
  //! Find a value in the Polygon2D, returns iterator.
  //! 
  //! @param[in] first Iterator to start searching from
  //! @param[in] last Search will stop when this iterator is found
  //! @param[in] val Object to compare
  //!
  //! @return Iterator pointing to the val in the Polygon2D. If no item was found,
  //!         will return last.
  template<typename vec2D>
  typename Polygon2D<vec2D>::iterator find(
    typename Polygon2D<vec2D>::iterator first,
    typename Polygon2D<vec2D>::iterator last,
    vec2D const & val)
  {
    while (first != last)
    {
      if (*first == val)
        return first;

      ++first;
    }

    return last;
  }

  //! @ingroup DgContainers_functions
  //!
  //! Find a value in the Polygon2D, returns const_iterator.
  //! 
  //! @param[in] first Iterator to start searching from
  //! @param[in] last Search will stop when this iterator is found
  //! @param[in] val Object to compare
  //!
  //! @return Iterator pointing to the val in the Polygon2D. If no item was found,
  //!         will return last.
  template<typename vec2D>
  typename Polygon2D<vec2D>::const_iterator find(
    typename Polygon2D<vec2D>::const_iterator first,
    typename Polygon2D<vec2D>::const_iterator last,
    vec2D const & val)
  {
    while (first != last)
    {
      if (*first == val)
        return first;

      ++first;
    }

    return last;
  }
};
#endif