//! @file Dg_list.h
//!
//! @author: Frank B. Hart
//! @date 21/05/2016
//!
//! Class declaration: DoublyLinkedList

// old length : 855 lines

#ifndef DGDOUBLYLINKEDLIST_H
#define DGDOUBLYLINKEDLIST_H

#include <cstring>
#include <new>
#include <type_traits>

#include "impl/DgContainerBase.h"

#ifdef DEBUG
#include <iostream>
#include <cstddef>
#endif

//TODO change implementation to pack objects to the front of the object pool.
namespace Dg
{
  //! @ingroup DgContainers
  //!
  //! @class DoublyLinkedList
  //!
  //! Circular, pre-allocated doubly linked DoublyLinkedList. Similar to std::DoublyLinkedList with similarly named methods
  //! and functionality. The underlying array is preallocated and only change in
  //! size if extending DoublyLinkedList past that allocated, or manually resizing. This makes
  //! for fast insertion/erasing of elements.
  //!
  //! @author Frank B. Hart
  //! @date 25/08/2016
  template<typename T>
  class DoublyLinkedList : public ContainerBase
  {
  private:
    
	  struct Node
	  {
		  Node* pNext;
		  Node* pPrev;
	  };

  public:

    //! @class const_iterator
    //!
    //! Const iterator for the DoublyLinkedList.
    //!
    //! @author Frank B. Hart
    //! @date 21/05/2016
    class const_iterator
	  {
	  private:
		  friend class DoublyLinkedList;
		
	  private:
		  //! Special constructor, not for external use
		  const_iterator(Node const * a_pNode, Node const * a_pNodeBegin, T const * a_pData)
        : m_pNode(a_pNode)
        , m_pNodeBegin(a_pNodeBegin)
        , m_pData(a_pData)
      {

      }

	  public:

		  const_iterator()
        : m_pNode(nullptr) 
        , m_pNodeBegin(nullptr)
        , m_pData(nullptr)
      {
      
      }

		  ~const_iterator()
      {
      
      }

		  //! Copy constructor
		  const_iterator(const_iterator const & a_it)
        : m_pNode(a_it.m_pNode)
        , m_pData(a_it.m_pData)
        , m_pNodeBegin(a_it.m_pNodeBegin)
      {

      }

      //! Assignment
		  const_iterator& operator= (const_iterator const & a_other)
      {
        m_pNode = a_other.m_pNode;
        m_pNodeBegin = a_other.m_pNodeBegin;
        m_pData = a_other.m_pData;
        return *this;
      }
		
		  //! Comparison.
		  bool operator==(const_iterator const & a_it) const 
      {
        return m_pNode == a_it.m_pNode;
      }

      //! Comparison.
		  bool operator!=(const_iterator const & a_it) const 
      {
        return m_pNode != a_it.m_pNode;
      }

		  //! Post increment
		  const_iterator& operator++()
      {
        m_pNode = m_pNode->pNext;
        return *this;
      }

      //! Pre increment
		  const_iterator operator++(int)
      {
        const_iterator result(*this);
        ++(*this);
        return result;
      }

      //! Post decrement
		  const_iterator& operator--()
      {
        m_pNode = m_pNode->pPrev;
        return *this;
      }

      //! Pre decrement
		  const_iterator operator--(int)
      {
        const_iterator result(*this);
        --(*this);
        return result;
      }

		  //! Conversion. Returns pointer to data.
		  T const * operator->() const 
      {
        return m_pData + (m_pNode - m_pNodeBegin - 1);
      }

      //! Conversion. Returns reference to data.
		  T const & operator*() const 
      {
        return *(m_pData + (m_pNode - m_pNodeBegin - 1));
      }

	  private:
      Node const * m_pNode;
      Node const * m_pNodeBegin;
      T const * m_pData;
	  };


    //! @class iterator
    //!
    //! Iterator for the DoublyLinkedList.
    //!
    //! @author Frank B. Hart
    //! @date 21/05/2016
    class iterator
    {
    private:
      friend class DoublyLinkedList;

    private:
      //! Special constructor, not for external use
      iterator(Node * a_pNode, Node * a_pNodeBegin, T * a_pData)
        : m_pNode(a_pNode)
        , m_pNodeBegin(a_pNodeBegin)
        , m_pData(a_pData)
      {

      }

    public:

      iterator()
        : m_pNode(nullptr) 
        , m_pNodeBegin(nullptr)
        , m_pData(nullptr)
      {

      }

      ~iterator()
      {

      }

      //! Copy constructor
      iterator(iterator const & a_it)
        : m_pNode(a_it.m_pNode)
        , m_pData(a_it.m_pData)
        , m_pNodeBegin(a_it.m_pNodeBegin)
      {

      }

      //! Assignment
      iterator& operator= (iterator const & a_other)
      {
        m_pNode = a_other.m_pNode;
        m_pNodeBegin = a_other.m_pNodeBegin;
        m_pData = a_other.m_pData;
        return *this;
      }

      //! Comparison.
      bool operator==(iterator const & a_it) const 
      {
        return m_pNode == a_it.m_pNode;
      }

      //! Comparison.
      bool operator!=(iterator const & a_it) const 
      {
        return m_pNode != a_it.m_pNode;
      }

      //! Post increment
      iterator& operator++()
      {
        m_pNode = m_pNode->pNext;
        return *this;
      }

      //! Pre increment
      iterator operator++(int)
      {
        iterator result(*this);
        ++(*this);
        return result;
      }

      //! Post decrement
      iterator& operator--()
      {
        m_pNode = m_pNode->pPrev;
        return *this;
      }

      //! Pre decrement
      iterator operator--(int)
      {
        iterator result(*this);
        --(*this);
        return result;
      }

      //! Conversion. Returns pointer to data.
      T * operator->()
      {
        return m_pData + (m_pNode - m_pNodeBegin - 1);
      }

      //! Conversion. Returns reference to data.
      T & operator*()
      {
        return *(m_pData + (m_pNode - m_pNodeBegin - 1));
      }

      //! Conversion
      operator const_iterator() const 
      {
        return const_iterator(m_pNode);
      }

    private:
      Node * m_pNode;
      Node * m_pNodeBegin;
      T * m_pData;
    };

  public:

    //! Constructor 
    //! If the method fails to allocate memory, the error is logged to the Dg error file and 
    //! the code aborted.
    DoublyLinkedList()
      : ContainerBase()
      , m_nItems(0)
      , m_pNodes(nullptr)
      , m_pData(nullptr)
    {
      InitMemory();
      InitEndNode();
    }

    //! Constructor 
    //! If the method fails to allocate memory, the error is logged to the Dg error file and 
    //! the code aborted.
	  DoublyLinkedList(size_t a_size)
      : ContainerBase(a_size)
      , m_nItems(0)
      , m_pNodes(nullptr)
      , m_pData(nullptr)
    {
      InitMemory();
      InitEndNode();
    }

    //! Destructor.
	  ~DoublyLinkedList()
    {
      DestructAll();
      free(m_pData);
      free(m_pNodes);
    }

	  //! Copy constructor
    //! If the method fails to allocate memory, the error is logged to the Dg error file and 
    //! the code aborted.
	  DoublyLinkedList(DoublyLinkedList const & a_other)
      : ContainerBase(a_other)
      , m_nItems(0)
      , m_pNodes(nullptr)
      , m_pData(nullptr)
    {
      InitMemory();
      Init(a_other);
    }

    //! Assignment
    //! If the method fails to allocate memory, the error is logged to the Dg error file and 
    //! the code aborted.
	  DoublyLinkedList & operator=(DoublyLinkedList const & a_other)
    {
      if (this != &a_other)
      {
        DestructAll();

        if (pool_size() < a_other.pool_size())
        {
          pool_size(a_other.pool_size());
          InitMemory();
        }

        Init(a_other);
      }
      return *this;
    }

    //! Move constructor
    //! If the method fails to allocate memory, the error is logged to the Dg error file and 
    //! the code aborted.
    DoublyLinkedList(DoublyLinkedList && a_other)
      : ContainerBase(a_other)
      , m_nItems(a_other.m_nItems)
      , m_pNodes(a_other.m_pNodes)
      , m_pData(a_other.m_pData)
    {
      a_other.m_pNodes = nullptr;
      a_other.m_pData = nullptr;
      a_other.m_nItems = 0;
    }

    //! Move assignment
    //! If the method fails to allocate memory, the error is logged to the Dg error file and 
    //! the code aborted.
    DoublyLinkedList & operator=(DoublyLinkedList && a_other)
    {
      if (this != &a_other)
      {
        ContainerBase::operator=(a_other);
        m_pData = a_other.m_pData;
        m_pNodes = a_other.m_pNodes;
        m_nItems = a_other.m_nItems;

        a_other.m_pNodes = nullptr;
        a_other.m_pData = nullptr;
        a_other.m_nItems = 0;
      }
      return *this;
    }

	  //! Returns an iterator pointing to the first data in the DoublyLinkedList container.
    //! If the container is empty, the returned iterator value shall not be dereferenced.
    //!
    //! @return iterator
	  iterator begin() 
    {
      return iterator(m_pNodes[0].pNext, m_pNodes, m_pData);
    }
    
    //! Returns an iterator referring to the <em>past-the-end</em> data in the DoublyLinkedList container.
    //! This iterator shall not be dereferenced.
    //!
    //! @return iterator
    iterator end() 
    {
      return iterator(&m_pNodes[0], m_pNodes, m_pData); 
    }
	  
    //! Returns a const iterator pointing to the first data in the DoublyLinkedList container.
    //! If the container is empty, the returned iterator value shall not be dereferenced.
    //!
    //! @return const_iterator
    const_iterator cbegin() const 
    {
      return const_iterator(m_pNodes[0].pNext, m_pNodes, m_pData);
    }
    
    //! Returns an iterator referring to the <em>past-the-end</em> data in the DoublyLinkedList container.
    //! This iterator shall not be dereferenced.
    //!
    //! @return const_iterator
    const_iterator cend() const 
    {
      return const_iterator(&m_pNodes[0], m_pNodes, m_pData); 
    }
	  
    //! Returns number of elements in the DoublyLinkedList.
    size_t size() const 
    {
      return m_nItems;
    }
	  
    //! Returns if the DoublyLinkedList is empty.
	  bool empty() const 
    {
      return m_nItems == 0;
    }

    //! Returns a reference to the last data in the DoublyLinkedList container.
    //! Calling this function on an empty container causes undefined behavior.
    //!
    //! @return Reference to data type
    T & back() 
    { 
      return *GetDataFromNode(m_pNodes[0].pPrev);
    }
     
    //! Returns a reference to the first data in the DoublyLinkedList container.
    //! Calling this function on an empty container causes undefined behavior.
    //!
    //! @return Reference to data type
    T & front() 
    { 
      return *GetDataFromNode(m_pNodes[0].pNext); 
    }

    //! Returns a const reference to the last data in the DoublyLinkedList container.
    //! Calling this function on an empty container causes undefined behavior.
    //!
    //! @return const reference to data type
    T const & back() const 
    { 
      return GetDataFromNode(m_pNodes[0].pPrev);
    }

    //! Returns a const reference to the first data in the DoublyLinkedList container.
    //! Calling this function on an empty container causes undefined behavior.
    //!
    //! @return const reference to data type
    T const & front() const 
    { 
      return GetDataFromNode(m_pNodes[0].pNext); 
    }

	  //! Add an data to the back of the DoublyLinkedList
    void push_back(T const & a_item)
    {
      InsertNewAfter(m_pNodes[0].pPrev, a_item);
    }

    //!Add an data to the front of the DoublyLinkedList
    void push_front(T const & a_item)
    {
      InsertNewAfter(&m_pNodes[0], a_item);
    }
    
    //! The container is extended by inserting a new element
    //! before the element at the specified position.
    //! The input iterator may be invalid as a resize may have occured. Resizing
    //! invalidates all pointers in the iterator. The return iterator will be valid.
    //! @return iterator to the newly inserted data
    iterator insert(iterator const & a_position, T const & a_item)
    {
      Node * pNode = InsertNewAfter(a_position.m_pNode->pPrev, a_item);
      return iterator(pNode, m_pNodes, m_pData);
    }

    //! Erase last data
	  void pop_back()
    {
      Remove(m_pNodes[0].pPrev);
    }

    //! Erase last data
	  void pop_front()
    {
      Remove(m_pNodes[0].pNext);
    }

    //! Erase an data from the DoublyLinkedList
    //! @return An iterator pointing to the data that followed 
    //!         the last data erased by the function call.
	  iterator erase(iterator const & a_position)
    {
      Node * pNode = Remove(a_position.m_pNode);
      return iterator(pNode, m_pNodes, m_pData);
    }
	  
    //! Clear the DoublyLinkedList, retains allocated memory.
    void clear()
    {
      DestructAll();
      m_nItems = 0;
      InitEndNode();
    }

    //! Resizes the DoublyLinkedList. This function also clears the DoublyLinkedList.
	  void resize(size_t a_newSize)
    {
      DestructAll();
      Init(a_newSize);
    }

#ifdef DEBUG
    void Print() const
    {
      std::cout << "\n\nSize: " << m_nItems;
      for (size_t i = 0; i < pool_size() + 1; ++i)
      {
        std::cout << "\n";
        if (m_pData[i]pPrev) std::cout << "[" << (m_pData[i].Prev - m_pData) << "]";
        else std::cout << "NULL";
        std::cout << "\t<-[" << i << "]-> \t";
        if (m_pData[i].Next()) std::cout << "[" << (m_pData[i].Next() - m_pData) << "]";
        else std::cout << "NULL";
        std::cout << "\t: " << m_pData[i].GetData();
      }
    }
#endif
  private:

    // Increases the size of the underlying arrays by a factor of 2
    void Extend()
    {
      Node * pOldNodes(m_pNodes);
      set_next_pool_size();

      m_pNodes = static_cast<Node *>(realloc(m_pNodes, (pool_size()) * sizeof(Node)));
      m_pData = static_cast<T *>(realloc(m_pData, (pool_size()) * sizeof(T)));

      if (pOldNodes != m_pNodes)
      {
        for (size_t i = 0; i <= m_nItems; i++)
        {
          m_pNodes[i].pPrev = m_pNodes + (m_pNodes[i].pPrev - pOldNodes);
          m_pNodes[i].pNext= m_pNodes + (m_pNodes[i].pNext - pOldNodes);
        }
      }
    }

    Node * InsertNewAfter(Node * a_pNode, T const & a_data)
    {
      if (m_nItems == (pool_size() - 1))
      {
        //Extending might invalidate a_pNode, so we need to record its index in the pool.
        size_t index(a_pNode - m_pNodes);
        Extend();
        a_pNode = &m_pNodes[index]; //Reset the pointer in case we have extended.
      }

      new (&m_pData[m_nItems]) T(a_data);
      m_nItems++;

      Node * newNode = &m_pNodes[m_nItems];
      newNode->pPrev = a_pNode;
      newNode->pNext = a_pNode->pNext;
      a_pNode->pNext->pPrev = newNode;
      a_pNode->pNext = newNode;

      return newNode;
    }

    void DestructAll()
    {
      for (size_t i = 0; i < m_nItems; i++)
        m_pData[i].~T();
    }

    void InitMemory()
    {
      m_pNodes = static_cast<Node*> (realloc(m_pNodes, pool_size() * sizeof(Node)));
      m_pData = static_cast<T*> (realloc(m_pData, pool_size() * sizeof(T)));
    }

    void Init(DoublyLinkedList const & a_other)
    {
      m_nItems = a_other.m_nItems;

      //We might as well sort the data in list order as we copy.
      //It will cost us nothing.
      Node const * node = &a_other.m_pNodes[0];
      for (size_t i = 0; i < m_nItems; i++)
      {
        node = node->pNext;
        size_t indOther = (node - a_other.m_pNodes);
        new (&m_pData[i]) T(a_other.m_pData[indOther - 1]);
      }

      for (size_t i = 1; i <= m_nItems; i++)
        m_pNodes[i].pPrev = &m_pNodes[i - 1];

      for (size_t i = 0; i < m_nItems; i++)
        m_pNodes[i].pNext = &m_pNodes[i + 1];

      m_pNodes[0].pPrev = &m_pNodes[m_nItems];
      m_pNodes[m_nItems].pNext = m_pNodes;
    }

    void InitEndNode()
    {
      m_pNodes[0].pNext = &m_pNodes[0];
      m_pNodes[0].pPrev = &m_pNodes[0];
    }

    Node * Remove(Node * a_pNode)
    {
      Node * pNext(a_pNode->pNext);

      //Break node form list
      a_pNode->pPrev->pNext = a_pNode->pNext;
      a_pNode->pNext->pPrev = a_pNode->pPrev;

      T * pData = GetDataFromNode(a_pNode);
      pData->~T();

      if (&m_pNodes[m_nItems] != a_pNode)
      {
        //Move last node to fill gap
        *a_pNode = m_pNodes[m_nItems];
        a_pNode->pPrev->pNext = a_pNode;
        a_pNode->pNext->pPrev = a_pNode;
        memcpy(pData, &m_pData[m_nItems - 1], sizeof (T));
      }

      if (pNext == &m_pNodes[m_nItems])
        pNext = a_pNode;

      m_nItems--;
      return pNext;
    }

    T * GetDataFromNode(Node * a_pNode)
    {
      return m_pData + (a_pNode - m_pNodes - 1);
    }

    T const * GetDataFromNode(Node * a_pNode) const
    {
      return m_pData + (a_pNode - m_pNodes - 1);
    }

  private:

	  Node *    m_pNodes;      //Pre-allocated block of memory to hold items
    T    *    m_pData;
	  size_t    m_nItems;     //Number of items currently in the DoublyLinkedList
  };

  //--------------------------------------------------------------------------------
  //		Helpful functions
  //--------------------------------------------------------------------------------

  //! @ingroup DgContainers_functions
  //!
  //! Find a value in the DoublyLinkedList, returns iterator.
  //! 
  //! @param[in] first Iterator to start searching from
  //! @param[in] last Search will stop when this iterator is found
  //! @param[in] val Object to compare
  //!
  //! @return Iterator pointing to the val in the DoublyLinkedList. If no item was found,
  //!         will return last.
  template<typename T>
  typename DoublyLinkedList<T>::iterator find (
	  typename DoublyLinkedList<T>::iterator first, 
	  typename DoublyLinkedList<T>::iterator last, 
	  T const & val)
  {
    while (first!=last) 
    {
      if (*first==val) 
		    return first;

      first++;
    }

    return last;
  }

  //! @ingroup DgContainers_functions
  //!
  //! Find a value in the DoublyLinkedList, returns const_iterator.
  //! 
  //! @param[in] first Iterator to start searching from
  //! @param[in] last Search will stop when this iterator is found
  //! @param[in] val Object to compare
  //!
  //! @return Iterator pointing to the val in the DoublyLinkedList. If no item was found,
  //!         will return last.
  template<typename T>
  typename DoublyLinkedList<T>::const_iterator find (
	  typename DoublyLinkedList<T>::const_iterator first, 
	  typename DoublyLinkedList<T>::const_iterator last, 
	  T const & val)
  {
    while (first!=last) 
    {
      if (*first==val) 
		    return first;

      first++;
    }

    return last;
  }
};
#endif