//! @file Dg_listCircular.h
//!
//! @author: Frank B. Hart
//! @date 21/05/2016
//!
//! Class declaration: CircularDoublyLinkedList


#ifndef DG_CIRCULARDOUBLYLINKEDLIST_H
#define DG_CIRCULARDOUBLYLINKEDLIST_H

#include <cstring>
#include <new>
#include <type_traits>

#ifdef STD_COMPATIBLE
#include <iterator>
#define BASE_ITERATOR : public std::iterator<std::bidirectional_iterator_tag, T>
#else
#define BASE_ITERATOR
#endif

#include "impl/DgContainerBase.h"
#include "DgErrorHandler.h"

//#define DG_DEBUG
#ifdef DG_DEBUG
#include <iostream>
#include <cstddef>
#endif

//TODO check for nullptr returns in realloc and throw
namespace Dg
{
  //! @ingroup DgContainers
  //!
  //! @class CircularDoublyLinkedList
  //!
  //! Circular, pre-allocated doubly linked CircularDoublyLinkedList. The underlying array is preallocated and only change in
  //! size if extending CircularDoublyLinkedList past that allocated, or manually resizing. This makes
  //! for fast insertion/erasing of elements.
  //!
  //! A CircularDoublyLinkedList with a memory pool the size of 8 item might typicall look like this in memory.
  //! When adding items to the CircularDoublyLinkedList, the element pointed to by the m_pNextFree free pointer 
  //! will be broken from the sub-CircularDoublyLinkedList of free elements and added to the CircularDoublyLinkedList of current items.
  //!     
  //!      v------------------------------------------------|   
  //!     |     |->|     |->|     |->|     |->|     |->|     ||     |  |     |  |     |  |     |
  //!     |  0  |  |  1  |  |  2  |  |  3  |  |  4  |  |  5  ||  6  |->|  7  |->|  8  |->|  9  |->NULL
  //!     |     |<-|     |<-|     |<-|     |<-|     |<-|     ||     |  |     |  |     |  |     |
  //!      |------------------------------------------------^    ^
  //!                                                            Next free
  //!
  //!     The 'head' node could be any of 0 - 5.
  //!
  //! Constructors and destructors will be called if the elements types are not pod.
  //!
  //! @author Frank B. Hart
  //! @date 25/08/2016
  template<typename T>
  class CircularDoublyLinkedList : public ContainerBase
  {
  private:

    //! Container to hold the object and pointers
    class Node
    {
    public:

      //! Default constructor
      Node() : pNext(nullptr), pPrev(nullptr) {}

      //! Copy the data in this node
      inline void InitData(T const & a_item)
      {
        new (&data) T(a_item);
      }

      //! Destruct the data is a destructor exists for the type.
      inline void DestructData()
      {
        data.~T();
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

      inline void MakeSingle()
      {
        pNext = pPrev = this;
      }

      inline T & GetData() { return data; }
      inline T const & GetData() const { return data; }

      inline Node * Next() { return pNext; }
      inline Node const * Next() const { return pNext; }
      inline void Next(Node * a_pNext) { pNext = a_pNext; }

      inline Node * Prev() { return pPrev; }
      inline Node const * Prev() const { return pPrev; }
      inline void Prev(Node * a_pPrev) { pPrev = a_pPrev; }

    private:

      Node* pNext;
      Node* pPrev;
      T     data;
    };

  public:

    //! @class const_iterator
    //!
    //! Const iterator for the CircularDoublyLinkedList.
    //!
    //! @author Frank B. Hart
    //! @date 21/05/2016
    class const_iterator BASE_ITERATOR
    {
    private:
      friend class CircularDoublyLinkedList;

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
      T const * operator->() const { return &(m_pNode->GetData()); }

      //! Conversion. Returns reference to data.
      T const & operator*() const { return m_pNode->GetData(); }

    private:
      Node const * m_pNode;

    };


    //! @class iterator
    //!
    //! Iterator for the CircularDoublyLinkedList.
    //!
    //! @author Frank B. Hart
    //! @date 21/05/2016
    class iterator BASE_ITERATOR
    {
      friend class CircularDoublyLinkedList;

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
      T* operator->() const { return &(m_pNode->GetData()); }

      //! Conversion
      T& operator*() const { return m_pNode->GetData(); }

    private:
      Node *m_pNode;

    };


  public:
    //! Constructor 
    //! If the method fails to allocate memory, the error is logged to the Dg error file and 
    //! the code aborted.
    CircularDoublyLinkedList();

    //! Constructor 
    //! If the method fails to allocate memory, the error is logged to the Dg error file and 
    //! the code aborted.
    CircularDoublyLinkedList(size_t);

    //! Destructor.
    ~CircularDoublyLinkedList();

    //! Copy constructor
    //! If the method fails to allocate memory, the error is logged to the Dg error file and 
    //! the code aborted.
    CircularDoublyLinkedList(CircularDoublyLinkedList const &);

    //! Assignment
    //! If the method fails to allocate memory, the error is logged to the Dg error file and 
    //! the code aborted.
    CircularDoublyLinkedList & operator=(CircularDoublyLinkedList const &);

    //! Returns an iterator pointing to the first data in the CircularDoublyLinkedList container.
    //! If the container is empty, the returned iterator value shall not be dereferenced.
    //!
    //! @return iterator
    iterator head() { return iterator(m_pData[0].Next()); }

    //! Returns a const iterator pointing to the first data in the CircularDoublyLinkedList container.
    //! If the container is empty, the returned iterator value shall not be dereferenced.
    //!
    //! @return const_iterator
    const_iterator chead() const { return const_iterator(m_pData[0].Next()); }

    //! Returns number of elements in the CircularDoublyLinkedList.
    size_t size() const { return m_nItems; }

    //! Returns if the CircularDoublyLinkedList is empty.
    bool empty() const { return m_nItems == 0; }

    //! Add an data to the back of the CircularDoublyLinkedList
    void push_back(T const &);

    //! The container is extended by inserting a new element
    //! before the element at the specified position.
    //! The input iterator may be invalid as a resize may have occured. Resizing
    //! invalidates all pointers in the iterator. The return iterator will be valid.
    //! @return iterator to the newly inserted data
    iterator insert(iterator const & position, T const & data);

    void reverse()
    {
      if (size() < 2)
      {
        return;
      }

      Node * pBack(m_pHead->pPrev);
      Node * pForward(m_pHead);

      if (size() % 2 != 0)
      {
        pForward = m_pHead->pNext;
      }

      for (size_t i = 0; i < size() / 2; i++)
      {
        T temp = pBack->GetData();
        pBack->InitData(pForward->GetData());
        pForward->InitData(temp);

        pBack = pBack->Prev();
        pForward = pForward->Next();
      }
    }

    //! Erase an data from the CircularDoublyLinkedList
    //! @return An iterator pointing to the data that followed 
    //!         the last data erased by the function call.
    iterator erase(iterator const &);

    //! Clear the CircularDoublyLinkedList, retains allocated memory.
    void clear();

    //! Resizes the CircularDoublyLinkedList. This function also clears the CircularDoublyLinkedList.
    void resize(size_t);

#ifdef DG_DEBUG
    void Print() const;
#endif
  private:

    // Increases the size of the underlying arrays by a factor of 2
    void Extend();

    // Destruct all objects in the CircularDoublyLinkedList
    void DestructAll();

    // Initialize the CircularDoublyLinkedList and request a size.
    void init(size_t);

    // Reset array pointers
    void AssignPointersToEmpty();

    // Break a new node off from the CircularDoublyLinkedList of free nodes and add it to the CircularDoublyLinkedList after the input node.
    Node * InsertNewAfter(Node * a_pNode, T const & a_data);

    // Remove a node from the CircularDoublyLinkedList and add to the CircularDoublyLinkedList of free nodes.
    void Remove(Node *);

  private:

    Node *    m_pData;      //Pre-allocated block of memory to hold items
    Node *    m_pNextFree;  //Pointer to the next free item in the CircularDoublyLinkedList;
    Node *    m_pHead;      //Pointer to the root of the list;
    size_t    m_nItems;     //Number of items currently in the CircularDoublyLinkedList
  };

  //--------------------------------------------------------------------------------
  //	@	CircularDoublyLinkedList<T>::iterator::operator=()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename CircularDoublyLinkedList<T>::iterator& CircularDoublyLinkedList<T>::iterator::operator=
    (typename CircularDoublyLinkedList<T>::iterator const & other)
  {
    m_pNode = other.m_pNode;
    return *this;
  }

  //--------------------------------------------------------------------------------
  //	@	CircularDoublyLinkedList<T>::iterator::operator++()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename CircularDoublyLinkedList<T>::iterator& CircularDoublyLinkedList<T>::iterator::operator++()
  {
    m_pNode = m_pNode->Next();
    return *this;
  }

  //--------------------------------------------------------------------------------
  //	@	CircularDoublyLinkedList<T>::iterator::operator++()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename CircularDoublyLinkedList<T>::iterator CircularDoublyLinkedList<T>::iterator::operator++(int)
  {
    iterator result(*this);
    ++(*this);
    return result;
  }

  //--------------------------------------------------------------------------------
  //	@	CircularDoublyLinkedList<T>::iterator::operator--()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename CircularDoublyLinkedList<T>::iterator& CircularDoublyLinkedList<T>::iterator::operator--()
  {
    m_pNode = m_pNode->Prev();
    return *this;
  }

  //--------------------------------------------------------------------------------
  //	@	CircularDoublyLinkedList<T>::iterator::operator--()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename CircularDoublyLinkedList<T>::iterator CircularDoublyLinkedList<T>::iterator::operator--(int)
  {
    iterator result(*this);
    --(*this);
    return result;

  }

  //--------------------------------------------------------------------------------
  //	@	CircularDoublyLinkedList<T>::const_iterator::operator=()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename CircularDoublyLinkedList<T>::const_iterator& CircularDoublyLinkedList<T>::const_iterator::operator=
    (typename CircularDoublyLinkedList<T>::const_iterator const & other)
  {
    m_pNode = other.m_pNode;
    return *this;
  }

  //--------------------------------------------------------------------------------
  //	@	CircularDoublyLinkedList<T>::const_iterator::operator++()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename CircularDoublyLinkedList<T>::const_iterator& CircularDoublyLinkedList<T>::const_iterator::operator++()
  {
    m_pNode = m_pNode->Next();
    return *this;
  }

  //--------------------------------------------------------------------------------
  //	@	CircularDoublyLinkedList<T>::const_iterator::operator=()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename CircularDoublyLinkedList<T>::const_iterator CircularDoublyLinkedList<T>::const_iterator::operator++(int)
  {
    const_iterator result(*this);
    ++(*this);
    return result;
  }

  //--------------------------------------------------------------------------------
  //	@	CircularDoublyLinkedList<T>::const_iterator::operator--()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename CircularDoublyLinkedList<T>::const_iterator& CircularDoublyLinkedList<T>::const_iterator::operator--()
  {
    m_pNode = m_pNode->Prev();
    return *this;
  }

  //--------------------------------------------------------------------------------
  //	@	CircularDoublyLinkedList<T>::const_iterator::operator--()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename CircularDoublyLinkedList<T>::const_iterator CircularDoublyLinkedList<T>::const_iterator::operator--(int)
  {
    const_iterator result(*this);
    --(*this);
    return result;
  }

  //--------------------------------------------------------------------------------
  //	@	CircularDoublyLinkedList<T>::AssignPointersToEmpty()
  //--------------------------------------------------------------------------------
  template<typename T>
  void CircularDoublyLinkedList<T>::AssignPointersToEmpty()
  {
    m_pNextFree = &m_pData[0];

    for (size_t i = 0; i < pool_size(); i++)
    {
      m_pData[i].Next(&m_pData[i + 1]);
    }
    m_pData[pool_size() - 1].Next(nullptr);
  }

  //--------------------------------------------------------------------------------
  //	@	CircularDoublyLinkedList<T>::init()
  //--------------------------------------------------------------------------------
  template<typename T>
  void CircularDoublyLinkedList<T>::init(size_t a_size)
  {
    pool_size(a_size);
    m_pData = static_cast<Node *>(realloc(m_pData, (pool_size()) * sizeof(Node)));
    DG_ASSERT(m_pData != nullptr);
    m_nItems = 0;
    AssignPointersToEmpty();
  }

  //--------------------------------------------------------------------------------
  //	@	CircularDoublyLinkedList<T>::CircularDoublyLinkedList<T>()
  //--------------------------------------------------------------------------------
  template<typename T>
  CircularDoublyLinkedList<T>::CircularDoublyLinkedList()
    : ContainerBase()
    , m_pData(nullptr)
    , m_pNextFree(nullptr)
    , m_pHead(nullptr)
  {
    init(1);
  }

  //--------------------------------------------------------------------------------
  //	@	CircularDoublyLinkedList<T>::CircularDoublyLinkedList<T>()
  //--------------------------------------------------------------------------------
  template<typename T>
  CircularDoublyLinkedList<T>::CircularDoublyLinkedList(size_t a_size)
    : ContainerBase(a_size)
    , m_pData(nullptr)
    , m_pNextFree(nullptr)
    , m_pHead(nullptr)
  {
    init(pool_size());
  }

  //--------------------------------------------------------------------------------
  //	@	CircularDoublyLinkedList<T>::~CircularDoublyLinkedList<T>()
  //--------------------------------------------------------------------------------
  template<typename T>
  CircularDoublyLinkedList<T>::~CircularDoublyLinkedList()
  {
    DestructAll();
    free(m_pData);
  }

  //--------------------------------------------------------------------------------
  //	@	CircularDoublyLinkedList<T>::CircularDoublyLinkedList<T>()
  //--------------------------------------------------------------------------------
  template<typename T>
  CircularDoublyLinkedList<T>::CircularDoublyLinkedList(CircularDoublyLinkedList const & other)
    : ContainerBase(other)
    , m_pData(nullptr)
    , m_pNextFree(nullptr)
    , m_pHead(nullptr)
  {
    init(other.pool_size());

    //Assign m_pData
    CircularDoublyLinkedList<T>::const_iterator it = other.chead();
    for (size_t i = 0; i < other.size(); i++, it++)
    {
      push_back(*it);
    }
  }

  //--------------------------------------------------------------------------------
  //	@	CircularDoublyLinkedList<T>::operator=()
  //--------------------------------------------------------------------------------
  template<typename T>
  CircularDoublyLinkedList<T>& CircularDoublyLinkedList<T>::operator=(CircularDoublyLinkedList const & other)
  {
    if (this == &other)
      return *this;

    ContainerBase::operator=(other);

    resize(other.pool_size());

    //Assign m_pData
    CircularDoublyLinkedList<T>::const_iterator it = other.chead();
    for (size_t i = 0; i < other.size(); i++, it++)
    {
      push_back(*it);
    }

    return *this;
  }

  //--------------------------------------------------------------------------------
  //	@	CircularDoublyLinkedList<T>::clear()
  //--------------------------------------------------------------------------------
  template<typename T>
  void CircularDoublyLinkedList<T>::clear()
  {
    DestructAll();
    m_nItems = 0;
    AssignPointersToEmpty();
  }

  //--------------------------------------------------------------------------------
  //	@	CircularDoublyLinkedList<T>::resize()
  //--------------------------------------------------------------------------------
  template<typename T>
  void CircularDoublyLinkedList<T>::resize(size_t a_newSize)
  {
    DestructAll();
    init(a_newSize);
  }

  //--------------------------------------------------------------------------------
  //	@	CircularDoublyLinkedList<T>::InsertNewAfter()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename CircularDoublyLinkedList<T>::Node * CircularDoublyLinkedList<T>::InsertNewAfter(Node * a_pNode, T const & a_data)
  {
    if (m_nItems >= (pool_size()))
    {
      //Extending might invalidate a_pNode, so we need to record its index in the pool.
      size_t index(a_pNode - m_pData);
      Extend();
      a_pNode = &m_pData[index]; //Reset the pointer in case we have extended.
    }

    Node * newNode(m_pNextFree);
    m_pNextFree = m_pNextFree->Next();
    newNode->InitData(a_data);
    newNode->InsertAfter(a_pNode);
    m_nItems++;

    return newNode;
  }

  //--------------------------------------------------------------------------------
  //	@	CircularDoublyLinkedList<T>::push_back()
  //--------------------------------------------------------------------------------
  template<typename T>
  void CircularDoublyLinkedList<T>::push_back(T const & a_item)
  {
    if (m_nItems > 0)
    {
      InsertNewAfter(m_pHead->Prev(), a_item);
    }
    else
    {
      Node * newNode(m_pNextFree);
      m_pNextFree = m_pNextFree->Next();
      newNode->InitData(a_item);
      newNode->MakeSingle();
      m_pHead = newNode;
      m_nItems++;
    }
  }

  //--------------------------------------------------------------------------------
  //	@	CircularDoublyLinkedList<T>::Remove()
  //--------------------------------------------------------------------------------
  template<typename T>
  void CircularDoublyLinkedList<T>::Remove(Node * a_pNode)
  {
   a_pNode->DestructData();
   a_pNode->Break();
   a_pNode->Next(m_pNextFree);
   m_pNextFree = a_pNode;
   m_pHead = a_pNode->Prev();
   m_nItems--;
  }

  //--------------------------------------------------------------------------------
  //	@	CircularDoublyLinkedList<T>::insert()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename CircularDoublyLinkedList<T>::iterator CircularDoublyLinkedList<T>::insert(typename CircularDoublyLinkedList<T>::iterator const & it, T const & a_item)
  {
    return iterator(InsertNewAfter(it.m_pNode->Prev(), a_item));
  }

  //--------------------------------------------------------------------------------
  //	@	CircularDoublyLinkedList<T>::erase()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename CircularDoublyLinkedList<T>::iterator CircularDoublyLinkedList<T>::erase(typename CircularDoublyLinkedList<T>::iterator const & it)
  {
    iterator result(it.m_pNode->Next());
    Remove(it.m_pNode);
    return result;
  }

  //--------------------------------------------------------------------------------
  //	@	CircularDoublyLinkedList<T>::DestructAll()
  //--------------------------------------------------------------------------------
  template<typename T>
  void CircularDoublyLinkedList<T>::DestructAll()
  {
    CircularDoublyLinkedList<T>::iterator it = head();
    for (size_t i = 0; i < size(); i++, it++)
    {
      it.m_pNode->DestructData();
    }
  }

  //--------------------------------------------------------------------------------
  //	@	CircularDoublyLinkedList<T>::Extend()
  //--------------------------------------------------------------------------------
  template<typename T>
  void CircularDoublyLinkedList<T>::Extend()
  {
    size_t oldSize(pool_size());
    Node * pOldData(m_pData);
    set_next_pool_size();

    DG_ASSERT(pool_size() > oldSize);

    //Create new array
    m_pData = static_cast<Node *>(realloc(m_pData, (pool_size()) * sizeof(Node)));
    DG_ASSERT(m_pData != nullptr);

    for (size_t i = 0; i < oldSize; i++)
    {
      m_pData[i].Prev(&m_pData[m_pData[i].Prev() - pOldData]);
      m_pData[i].Next(&m_pData[m_pData[i].Next() - pOldData]);
    }

    m_pHead = &m_pData[0];
    m_pNextFree = &m_pData[oldSize];

    for (size_t i = oldSize; i < pool_size() - 1; i++)
    {
      m_pData[i].Next(&m_pData[i + 1]);
    }
    m_pData[pool_size() - 1].Next(nullptr);
  }

#ifdef DG_DEBUG
  template<typename T>
  void CircularDoublyLinkedList<T>::Print() const
  {
    std::cout << "\n\nSize: " << m_nItems;
    for (size_t i = 0; i < pool_size(); ++i)
    {
      std::cout << "\n";
      if (m_pData[i].Prev()) std::cout << "[" << (m_pData[i].Prev() - m_pData) << "]";
      else std::cout << "NULL";
      std::cout << "\t<-[" << i << "]-> \t";
      size_t sdfg = (m_pData[i].Next() - m_pData);
      if (m_pData[i].Next()) std::cout << "[" << (m_pData[i].Next() - m_pData) << "]";
      else std::cout << "NULL";
      std::cout << "\t: " << m_pData[i].GetData();
    }
    std::cout << "\nNext free: ";
    if (m_pNextFree == nullptr)
    {
      std::cout << "NULL\n";
    }
    else
    {
      std::cout << "[" << (m_pNextFree - m_pData) << "]\n";
    }
    std::cout << "\nHead: ";
    std::cout << "[" << (m_pHead - m_pData) << "]\n";
  }
#endif
};
#endif