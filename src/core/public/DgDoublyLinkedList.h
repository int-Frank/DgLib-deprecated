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
  //! A DoublyLinkedList with a memory pool the size of 8 item might typicall look like this in memory. 
  //! The first element in the pool is always the root. The end method will return an 
  //! iterator pointing to this element. The begin method will return an iterator built 
  //! from whatever element 0 is pointing to next. For empty lists, this will be itself. 
  //! When adding items to the DoublyLinkedList, the element pointed to by the m_pNextFree free pointer 
  //! will be broken from the sub-DoublyLinkedList of free elements and added to the DoublyLinkedList of current items.
  //! The last element in the pool is never used; the pool is always extended before it is used.
  //!     
  //!      v------------------------------------------------|   
  //!     |     |->|     |->|     |->|     |->|     |->|     ||     |  |     |  |     |  |     |
  //!     |  0  |  |  1  |  |  2  |  |  3  |  |  4  |  |  5  ||  6  |->|  7  |->|  8  |->|  9  |->NULL
  //!     |     |<-|     |<-|     |<-|     |<-|     |<-|     ||     |  |     |  |     |  |     |
  //!      |------------------------------------------------^    ^
  //!                                                            Next free
  //!
  //!     Root node = [0]. The DoublyLinkedList begins at element [1].
  //!
  //! Constructors and destructors will be called if the elements types are not pod.
  //!
  //! The pool is extended when there is only one element left in the DoublyLinkedList of free elements.
  //!
  //! @author Frank B. Hart
  //! @date 25/08/2016
  template<typename T>
  class DoublyLinkedList : public ContainerBase
  {
  private:
    
    //! Container to hold the object and pointers
	  class Node
	  {
    public:

      //! Default constructor
		  Node(): pNext(nullptr), pPrev(nullptr) {}
		
      //! Copy the data in this node
      inline void InitData(T const & a_item)
      {
        if (std::is_trivially_copy_constructible<T>::value)
        {
          memcpy(&data, &a_item, sizeof(T));
        }
        else
        {
          new (&data) T(a_item);
        }
      }

      //! Destruct the data is a destructor exists for the type.
      inline void DestructData()
      {
        if (!std::is_trivially_destructible<T>::value)
        {
          data.~T();
        }
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

      inline T & GetData()              { return data;}
      inline T const & GetData() const  { return data; }

      inline Node * Next()              { return pNext; }
      inline Node const * Next() const  { return pNext; }
      inline void Next(Node * a_pNext)  { pNext = a_pNext; }

      inline Node * Prev()              { return pPrev; }
      inline Node const * Prev() const  { return pPrev; }
      inline void Prev(Node * a_pPrev)  { pPrev = a_pPrev; }

    private:

		  Node* pNext;
		  Node* pPrev;
		  T     data;
	  };

  public:

    //! @class const_iterator
    //!
    //! Const iterator for the DoublyLinkedList.
    //!
    //! @author Frank B. Hart
    //! @date 21/05/2016
    class const_iterator BASE_ITERATOR
	  {
	  private:
		  friend class DoublyLinkedList;
		
	  private:
		  //! Special constructor, not for external use
		  const_iterator(Node const * _ptr) 
      {
        m_pNode = _ptr;
      }

	  public:

		  const_iterator()
        : m_pNode(nullptr) 
      {
      
      }

		  ~const_iterator()
      {
      
      }

		  //! Copy constructor
		  const_iterator(const_iterator const & it)
        : m_pNode(it.m_pNode)
      {

      }

      //! Assignment
		  const_iterator& operator= (const_iterator const & a_other)
      {
        m_pNode = a_other.m_pNode;
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
		  const_iterator& operator++()
      {
        m_pNode = m_pNode->Next();
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
        m_pNode = m_pNode->Prev();
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
        return &(m_pNode->GetData());
      }

      //! Conversion. Returns reference to data.
		  T const & operator*() const 
      {
        return m_pNode->GetData();
      }

	  private:
		  Node const * m_pNode;
	  };


    //! @class iterator
    //!
    //! Iterator for the DoublyLinkedList.
    //!
    //! @author Frank B. Hart
    //! @date 21/05/2016
    class iterator BASE_ITERATOR
	  {
		  friend class DoublyLinkedList;

	  private:
		  //! Special constructor, not for external use
		  iterator(Node* _ptr) 
      {
        m_pNode = _ptr;
      }

	  public:

		  iterator()
        : m_pNode(nullptr) 
      {
      
      }

		  ~iterator()
      {
      
      }

		  //! Copy constructor.
		  iterator(iterator const & it)
        : m_pNode(it.m_pNode)
      {
      
      }
		  
      //! Assignment.
      iterator& operator= (iterator const & a_other)
      {
        m_pNode = a_other.m_pNode;
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
		  iterator& operator++()
      {
        m_pNode = m_pNode->Next();
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
        m_pNode = m_pNode->Prev();
        return *this;
      }

      //! Pre decrement
		  iterator operator--(int)
      {
        iterator result(*this);
        --(*this);
        return result;
      }

		  //! Conversion
		  operator const_iterator() const 
      {
        return const_iterator(m_pNode);
      }

      //! Conversion
		  T* operator->() const 
      {
        return &(m_pNode->GetData());
      }

      //! Conversion
		  T& operator*() const 
      {
        return m_pNode->GetData();
      }

	  private:
		  Node *m_pNode;
	  };


  public:
    //! Constructor 
    //! If the method fails to allocate memory, the error is logged to the Dg error file and 
    //! the code aborted.
    DoublyLinkedList()
      : ContainerBase()
      , m_pData(nullptr)
      , m_pNextFree(nullptr)
    {
      Init(1);
    }

    //! Constructor 
    //! If the method fails to allocate memory, the error is logged to the Dg error file and 
    //! the code aborted.
	  DoublyLinkedList(size_t a_size)
      : ContainerBase(a_size)
      , m_pData(nullptr)
      , m_pNextFree(nullptr)
    {
      Init(pool_size());
    }

    //! Destructor.
	  ~DoublyLinkedList()
    {
      DestructAll();
      free(m_pData);
    }

	  //! Copy constructor
    //! If the method fails to allocate memory, the error is logged to the Dg error file and 
    //! the code aborted.
	  DoublyLinkedList(DoublyLinkedList const & a_other)
      : ContainerBase(a_other)
    {
      Init(a_other.pool_size());

      //Assign m_pData
      DoublyLinkedList<T>::const_iterator it = a_other.cbegin();
      for (it; it != a_other.cend(); it++)
      {
        push_back(*it);
      }
    }

    //! Assignment
    //! If the method fails to allocate memory, the error is logged to the Dg error file and 
    //! the code aborted.
	  DoublyLinkedList & operator=(DoublyLinkedList const & a_other)
    {
      if (this == &a_other)
        return *this;

      ContainerBase::operator=(a_other);

      resize(a_other.pool_size());

      //Assign m_pData
      DoublyLinkedList<T>::const_iterator it = a_other.cbegin();
      for (it; it != a_other.cend(); it++)
      {
        push_back(*it);
      }

      return *this;
    }

    //! Move constructor
    //! If the method fails to allocate memory, the error is logged to the Dg error file and 
    //! the code aborted.
    DoublyLinkedList(DoublyLinkedList &&);

    //! Move assignment
    //! If the method fails to allocate memory, the error is logged to the Dg error file and 
    //! the code aborted.
    DoublyLinkedList & operator=(DoublyLinkedList &&);

	  //! Returns an iterator pointing to the first data in the DoublyLinkedList container.
    //! If the container is empty, the returned iterator value shall not be dereferenced.
    //!
    //! @return iterator
	  iterator begin() 
    {
      return iterator(m_pData[0].Next());
    }
    
    //! Returns an iterator referring to the <em>past-the-end</em> data in the DoublyLinkedList container.
    //! This iterator shall not be dereferenced.
    //!
    //! @return iterator
    iterator end() 
    {
      return iterator(&m_pData[0]); 
    }
	  
    //! Returns a const iterator pointing to the first data in the DoublyLinkedList container.
    //! If the container is empty, the returned iterator value shall not be dereferenced.
    //!
    //! @return const_iterator
    const_iterator cbegin() const 
    {
      return const_iterator(m_pData[0].Next());
    }
    
    //! Returns an iterator referring to the <em>past-the-end</em> data in the DoublyLinkedList container.
    //! This iterator shall not be dereferenced.
    //!
    //! @return const_iterator
    const_iterator cend() const 
    {
      return const_iterator(&m_pData[0]); 
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
      return m_pData[0].Prev()->GetData(); 
    }
     
    //! Returns a reference to the first data in the DoublyLinkedList container.
    //! Calling this function on an empty container causes undefined behavior.
    //!
    //! @return Reference to data type
    T & front() 
    { 
      return m_pData[0].Next()->GetData(); 
    }

    //! Returns a const reference to the last data in the DoublyLinkedList container.
    //! Calling this function on an empty container causes undefined behavior.
    //!
    //! @return const reference to data type
    T const & back() const 
    { 
      return m_pData[0].Prev()->GetData(); 
    }

    //! Returns a const reference to the first data in the DoublyLinkedList container.
    //! Calling this function on an empty container causes undefined behavior.
    //!
    //! @return const reference to data type
    T const & front() const 
    { 
      return m_pData[0].Next()->GetData(); 
    }

	  //! Add an data to the back of the DoublyLinkedList
    void push_back(T const & a_item)
    {
      InsertNewAfter(m_pData[0].Prev(), a_item);
    }

    //!Add an data to the front of the DoublyLinkedList
    void push_front(T const & a_item)
    {
      InsertNewAfter(&m_pData[0], a_item);
    }
    
    //! The container is extended by inserting a new element
    //! before the element at the specified position.
    //! The input iterator may be invalid as a resize may have occured. Resizing
    //! invalidates all pointers in the iterator. The return iterator will be valid.
    //! @return iterator to the newly inserted data
    iterator insert(iterator const & a_position, T const & a_item)
    {
      return iterator(InsertNewAfter(a_position.m_pNode->Prev(), a_item));
    }

    //! Erase last data
	  void pop_back()
    {
      Remove(m_pData[0].Prev());
    }

    //! Erase last data
	  void pop_front()
    {
      Remove(m_pData[0].Next());
    }

    //! Erase an data from the DoublyLinkedList
    //! @return An iterator pointing to the data that followed 
    //!         the last data erased by the function call.
	  iterator erase(iterator const & a_position)
    {
      iterator result(a_position.m_pNode->Next());
      Remove(a_position.m_pNode);
      return result;
    }
	  
    //! Clear the DoublyLinkedList, retains allocated memory.
    void clear()
    {
      DestructAll();
      m_nItems = 0;
      AssignPointersToEmpty();
    }

    //! Resizes the DoublyLinkedList. This function also clears the DoublyLinkedList.
	  void resize(size_t a_newSize)
    {
      DestructAll();
      Init(a_newSize);
    }

#ifdef DG_DEBUG
    void Print() const
    {
      std::cout << "\n\nSize: " << m_nItems;
      for (size_t i = 0; i < pool_size() + 1; ++i)
      {
        std::cout << "\n";
        if (m_pData[i].Prev()) std::cout << "[" << (m_pData[i].Prev() - m_pData) << "]";
        else std::cout << "NULL";
        std::cout << "\t<-[" << i << "]-> \t";
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
    }
#endif
  private:

    // Increases the size of the underlying arrays by a factor of 2
    void Extend()
    {
      size_t oldSize(pool_size());
      Node * pOldData(m_pData);
      size_t nextFreeInd(m_pNextFree - m_pData); //This assumes m_pNextFree is always valid
      set_next_pool_size();

      DG_ASSERT(pool_size() > oldSize);

      //Create new array
      m_pData = static_cast<Node *>(realloc(m_pData, (pool_size() + 1) * sizeof(Node)));
      DG_ASSERT(m_pData != nullptr);

      for (size_t i = 0; i < oldSize; i++)
      {
        m_pData[i].Prev(&m_pData[m_pData[i].Prev() - pOldData]);
        m_pData[i].Next(&m_pData[m_pData[i].Next() - pOldData]);
      }

      m_pData[oldSize].Prev(&m_pData[m_pData[oldSize].Prev() - pOldData]);
      m_pData[oldSize].Next(&m_pData[oldSize + 1]);

      m_pNextFree = &m_pData[nextFreeInd];

      for (size_t i = oldSize + 1; i < pool_size(); ++i)
      {
        m_pData[i].Next(&m_pData[i + 1]);
      }
      m_pData[pool_size()].Next(nullptr);
    }

    // Destruct all objects in the DoublyLinkedList
    void DestructAll()
    {
      for (iterator it = begin(); it != end(); it++)
      {
        it.m_pNode->DestructData();
      }
    }

    // Initialize the DoublyLinkedList and request a size.
    void Init(size_t a_size)
    {
      pool_size(a_size + 1);
      m_pData = static_cast<Node *>(realloc(m_pData, (pool_size() + 1) * sizeof(Node)));
      DG_ASSERT(m_pData != nullptr);
      m_nItems = 0;
      AssignPointersToEmpty();
    }

    // Reset array pointers
    void AssignPointersToEmpty()
    {
      m_pNextFree = &m_pData[1];

      m_pData[0].Next(&m_pData[0]);
      m_pData[0].Prev(&m_pData[0]);

      for (size_t i = 1; i < pool_size(); i++)
      {
        m_pData[i].Next(&m_pData[i + 1]);
      }
      m_pData[pool_size()].Next(nullptr);
    }

    // Break a new node off from the DoublyLinkedList of free nodes and add it to the DoublyLinkedList after the input node.
    Node * InsertNewAfter(Node * a_pNode, T const & a_data)
    {
      if (m_nItems >= (pool_size() - 1))
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

    // Remove a node from the DoublyLinkedList and add to the DoublyLinkedList of free nodes.
    void Remove(Node * a_pNode)
    {
      a_pNode->DestructData();
      a_pNode->Break();
      a_pNode->Next(m_pNextFree);
      m_pNextFree = a_pNode;
      m_nItems--;
    }

  private:

	  Node *    m_pData;      //Pre-allocated block of memory to hold items
	  Node *    m_pNextFree;  //Pointer to the next free item in the DoublyLinkedList;
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