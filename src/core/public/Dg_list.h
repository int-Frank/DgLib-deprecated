//! @file Dg_list.h
//!
//! @author: Frank B. Hart
//! @date 21/05/2016
//!
//! Class declaration: list


#ifndef DG_LIST_POD_H
#define DG_LIST_POD_H

#include <cstring>
#include <new>
#include <type_traits>

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
  //! @class list
  //!
  //! Pre-allocated Linked list. Similar to std::list with similarly named methods
  //! and functionality. The underlying array is preallocated and only change in
  //! size if extending list past that allocated, or manually resizing. This makes
  //! for fast insertion/erasing of elements.
  //!
  //! A list with a memory pool the size of 8 item might typicall look like this in memory. 
  //! The first element in the pool is always the root. The end method will return an 
  //! iterator pointing to this element. The begin method will return an iterator built 
  //! from whatever element 0 is pointing to next. For empty lists, this will be itself. 
  //! When adding items to the list, the element pointed to by the m_pNextFree free pointer 
  //! will be broken from the sub-list of free elements and added to the list of current items.
  //!     
  //!      v------------------------------------------------|   
  //!     |     |->|     |->|     |->|     |->|     |->|     ||     |  |     |  |     |  |     |
  //!     |  0  |  |  1  |  |  2  |  |  3  |  |  4  |  |  5  ||  6  |->|  7  |->|  8  |->|  9  |->NULL
  //!     |     |<-|     |<-|     |<-|     |<-|     |<-|     ||     |  |     |  |     |  |     |
  //!      |------------------------------------------------^   ^
  //!                                                           Next free
  //!
  //!     Root node = [0]. The list begins at element [1].
  //!
  //! Constructors and destructors will be called if the elements types are not pod.
  //!
  //! The pool is extended when there is only one element left in the list of free elements.
  //!
  //! @author Frank B. Hart
  //! @date 25/08/2016
  template<typename T>
  class list : public ContainerBase
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

      //! Initialise the data if a constructor exists for the type.
      inline void InitData()
      {
        if (!std::is_trivially_constructible<T>::value)
        {
          new (&data) T();
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
    //! Const iterator for the list.
    //!
    //! @author Frank B. Hart
    //! @date 21/05/2016
    class const_iterator
	  {
	  private:
		  friend class list;
		
	  private:
		  //! Special constructor, not for external use
		  const_iterator(Node const * _ptr) {m_pNode = _ptr;}

	  public:

		  const_iterator(): m_pNode(nullptr) {}
		  ~const_iterator(){}

		  //! Copy constructor
		  const_iterator(const_iterator const & it): m_pNode(it.m_pNode){}

      //! Assignment
		  const_iterator& operator= (const_iterator const &);
		
		  //! Comparison.
		  bool operator==(const_iterator const & it) const {return m_pNode == it.m_pNode;}

      //! Comparison.
		  bool operator!=(const_iterator const & it) const {return m_pNode != it.m_pNode;}

		  //! Post increment
		  const_iterator& operator++();

      //! Pre increment
		  const_iterator operator++(int);

      //! Post decrement
		  const_iterator& operator--();

      //! Pre decrement
		  const_iterator operator--(int);

		  //! Conversion. Returns pointer to data.
		  T const * operator->() const {return &(m_pNode->GetData());}

      //! Conversion. Returns reference to data.
		  T const & operator*() const {return m_pNode->GetData();}

	  private:
		  Node const * m_pNode;

	  };


    //! @class iterator
    //!
    //! Iterator for the list.
    //!
    //! @author Frank B. Hart
    //! @date 21/05/2016
    class iterator
	  {
		  friend class list;

	  private:
		  //! Special constructor, not for external use
		  iterator(Node* _ptr) {m_pNode = _ptr;}

	  public:

		  iterator(): m_pNode(nullptr) {}
		  ~iterator(){}

		  //! Copy constructor.
		  iterator(iterator const & it): m_pNode(it.m_pNode){}
		  
      //! Assignment.
      iterator& operator= (iterator const &);

		  //! Comparison.
		  bool operator==(iterator const & it) const {return m_pNode == it.m_pNode;}

      //! Comparison.
		  bool operator!=(iterator const & it) const {return m_pNode != it.m_pNode;}

		  //! Post increment
		  iterator& operator++();

      //! Pre increment
		  iterator operator++(int);

      //! Post decrement
		  iterator& operator--();

      //! Pre decrement
		  iterator operator--(int);

		  //! Conversion
		  operator const_iterator() const {return const_iterator(m_pNode);}

      //! Conversion
		  T* operator->() const {return &(m_pNode->GetData());}

      //! Conversion
		  T& operator*() const {return m_pNode->GetData();}

	  private:
		  Node *m_pNode;

	  };


  public:
    //! Constructor 
    //! If the method fails to allocate memory, the error is logged to the Dg error file and 
    //! the code aborted.
    list();

    //! Constructor 
    //! If the method fails to allocate memory, the error is logged to the Dg error file and 
    //! the code aborted.
	  list(size_t);

    //! Destructor.
	  ~list();

	  //! Copy constructor
    //! If the method fails to allocate memory, the error is logged to the Dg error file and 
    //! the code aborted.
	  list(list const &);

    //! Assignment
    //! If the method fails to allocate memory, the error is logged to the Dg error file and 
    //! the code aborted.
	  list & operator=(list const &);

    //! Move constructor
    //! If the method fails to allocate memory, the error is logged to the Dg error file and 
    //! the code aborted.
    list(list &&);

    //! Move assignment
    //! If the method fails to allocate memory, the error is logged to the Dg error file and 
    //! the code aborted.
    list & operator=(list &&);

	  //! Returns an iterator pointing to the first data in the list container.
    //! If the container is empty, the returned iterator value shall not be dereferenced.
    //!
    //! @return iterator
	  iterator begin() {return iterator(m_pData[0].Next());}
    
    //! Returns an iterator referring to the <em>past-the-end</em> data in the list container.
    //! This iterator shall not be dereferenced.
    //!
    //! @return iterator
    iterator end() {return iterator(&m_pData[0]); }
	  
    //! Returns a const iterator pointing to the first data in the list container.
    //! If the container is empty, the returned iterator value shall not be dereferenced.
    //!
    //! @return const_iterator
    const_iterator cbegin() const {return const_iterator(m_pData[0].Next());}
    
    //! Returns an iterator referring to the <em>past-the-end</em> data in the list container.
    //! This iterator shall not be dereferenced.
    //!
    //! @return const_iterator
    const_iterator cend() const {return const_iterator(&m_pData[0]); }
	  
    //! Returns number of elements in the list.
    size_t size() const {return m_nItems;}
	  
    //! Returns if the list is empty.
	  bool empty() const {return m_nItems == 0;}

    //! Returns a reference to the last data in the list container.
    //! Calling this function on an empty container causes undefined behavior.
    //!
    //! @return Reference to data type
    T & back() { return m_pData[0].Prev()->GetData(); }
     
    //! Returns a reference to the first data in the list container.
    //! Calling this function on an empty container causes undefined behavior.
    //!
    //! @return Reference to data type
    T & front() { return m_pData[0].Next()->GetData(); }

    //! Returns a const reference to the last data in the list container.
    //! Calling this function on an empty container causes undefined behavior.
    //!
    //! @return const reference to data type
    T const & back() const { return m_pData[0].Prev()->GetData(); }

    //! Returns a const reference to the first data in the list container.
    //! Calling this function on an empty container causes undefined behavior.
    //!
    //! @return const reference to data type
    T const & front() const { return m_pData[0].Next()->GetData(); }

	  //! Add an data to the back of the list
    void push_back(T const &);

    //!Add an data to the front of the list
    void push_front(T const &);
    
    //! The container is extended by inserting a new element
    //! before the element at the specified position.
    //! The input iterator may be invalid as a resize may have occured. Resizing
    //! invalidates all pointers in the iterator. The return iterator will be valid.
    //! @return iterator to the newly inserted data
    iterator insert(iterator const & position, T const & data);

    //! Erase last data
	  void pop_back();

    //! Erase last data
	  void pop_front();

    //! Erase an data from the list
    //! @return An iterator pointing to the data that followed 
    //!         the last data erased by the function call.
	  iterator erase(iterator const &);
	  
    //! Clear the list, retains allocated memory.
    void clear();

    //! Resizes the list. This function also clears the list.
	  void resize(size_t);

#ifdef DG_DEBUG
    void Print() const;
#endif
  private:

    // Increases the size of the underlying arrays by a factor of 2
    void Extend();

    // Destruct all objects in the list
    void DestructAll();

    // Initialize the list and request a size.
    void init(size_t);

    // Reset array pointers
    void AssignPointersToEmpty();

    // Break a new node off from the list of free nodes and add it to the list after the input node.
    Node * InsertNewAfter(Node * a_pNode, T const & a_data);

    // Remove a node from the list and add to the list of free nodes.
    void Remove(Node *);

  private:

	  Node *    m_pData;      //Pre-allocated block of memory to hold items
	  Node *    m_pNextFree;  //Pointer to the next free item in the list;
	  size_t    m_nItems;     //Number of items currently in the list
  };

  //--------------------------------------------------------------------------------
  //	@	list<T>::iterator::operator=()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename list<T>::iterator& list<T>::iterator::operator=
	  (typename list<T>::iterator const & other)
  {
	  m_pNode = other.m_pNode;
	  return *this;
  }

  //--------------------------------------------------------------------------------
  //	@	list<T>::iterator::operator++()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename list<T>::iterator& list<T>::iterator::operator++()
  {
	  m_pNode = m_pNode->Next();
	  return *this;
  }

  //--------------------------------------------------------------------------------
  //	@	list<T>::iterator::operator++()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename list<T>::iterator list<T>::iterator::operator++(int)
  {
	  iterator result(*this);
    ++(*this);
    return result;
  }

  //--------------------------------------------------------------------------------
  //	@	list<T>::iterator::operator--()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename list<T>::iterator& list<T>::iterator::operator--()
  {
	  m_pNode = m_pNode->Prev();
	  return *this;
  }

  //--------------------------------------------------------------------------------
  //	@	list<T>::iterator::operator--()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename list<T>::iterator list<T>::iterator::operator--(int)
  {
	  iterator result(*this);
    --(*this);
    return result;

  }

  //--------------------------------------------------------------------------------
  //	@	list<T>::const_iterator::operator=()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename list<T>::const_iterator& list<T>::const_iterator::operator=
	  (typename list<T>::const_iterator const & other)
  {
	  m_pNode = other.m_pNode;
	  return *this;
  }

  //--------------------------------------------------------------------------------
  //	@	list<T>::const_iterator::operator++()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename list<T>::const_iterator& list<T>::const_iterator::operator++()
  {
	  m_pNode = m_pNode->Next();
	  return *this;
  }

  //--------------------------------------------------------------------------------
  //	@	list<T>::const_iterator::operator=()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename list<T>::const_iterator list<T>::const_iterator::operator++(int)
  {
	  const_iterator result(*this);
    ++(*this);
    return result;
  }

  //--------------------------------------------------------------------------------
  //	@	list<T>::const_iterator::operator--()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename list<T>::const_iterator& list<T>::const_iterator::operator--()
  {
	  m_pNode = m_pNode->Prev();
	  return *this;
  }

  //--------------------------------------------------------------------------------
  //	@	list<T>::const_iterator::operator--()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename list<T>::const_iterator list<T>::const_iterator::operator--(int)
  {
	  const_iterator result(*this);
    --(*this);
    return result;
  }

  //--------------------------------------------------------------------------------
  //	@	list<T>::AssignPointersToEmpty()
  //--------------------------------------------------------------------------------
  template<typename T>
  void list<T>::AssignPointersToEmpty()
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

  //--------------------------------------------------------------------------------
  //	@	list<T>::init()
  //--------------------------------------------------------------------------------
  template<typename T>
  void list<T>::init(size_t a_size) 
  {
    pool_size(a_size + 1);
    m_pData = static_cast<Node *>(realloc(m_pData, (pool_size() + 1) * sizeof(Node)));
    DG_ASSERT(m_pData != nullptr);
	  m_nItems = 0;
    AssignPointersToEmpty();
  }

  //--------------------------------------------------------------------------------
  //	@	list<T>::list<T>()
  //--------------------------------------------------------------------------------
  template<typename T>
  list<T>::list() 
    : ContainerBase()
    , m_pData(nullptr)
    , m_pNextFree(nullptr)
  {
	  init(1);
  }

  //--------------------------------------------------------------------------------
  //	@	list<T>::list<T>()
  //--------------------------------------------------------------------------------
  template<typename T>
  list<T>::list(size_t a_size)
    : ContainerBase(a_size)
    , m_pData(nullptr)
    , m_pNextFree(nullptr)
  {
    init(pool_size());
  }

  //--------------------------------------------------------------------------------
  //	@	list<T>::~list<T>()
  //--------------------------------------------------------------------------------
  template<typename T>
  list<T>::~list()
  {
    DestructAll();
    free(m_pData);
  }

  //--------------------------------------------------------------------------------
  //	@	list<T>::list<T>()
  //--------------------------------------------------------------------------------
  template<typename T>
  list<T>::list(list const & other)
    : ContainerBase(other)
  {
	  init(other.pool_size());

	  //Assign m_pData
	  list<T>::const_iterator it = other.cbegin();
	  for (it; it != other.cend(); ++it)
	  {
		  push_back(*it);
	  }
  }

  //--------------------------------------------------------------------------------
  //	@	list<T>::operator=()
  //--------------------------------------------------------------------------------
  template<typename T>
  list<T>& list<T>::operator=(list const & other)
  {
	  if (this == &other)
		  return *this;

    ContainerBase::operator=(other);

	  resize(other.pool_size());

	  //Assign m_pData
	  list<T>::const_iterator it = other.cbegin();
	  for (it; it != other.cend(); ++it)
	  {
		  push_back(*it);
	  }

	  return *this;
  }

  //--------------------------------------------------------------------------------
  //	@	list<T>::clear()
  //--------------------------------------------------------------------------------
  template<typename T>
  void list<T>::clear()
  {
    DestructAll();
    m_nItems = 0;
    AssignPointersToEmpty();
  }

  //--------------------------------------------------------------------------------
  //	@	list<T>::resize()
  //--------------------------------------------------------------------------------
  template<typename T>
  void list<T>::resize(size_t a_newSize)
  {
    DestructAll();
	  init(a_newSize);
  }

  //--------------------------------------------------------------------------------
  //	@	list<T>::InsertNewAfter()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename list<T>::Node * list<T>::InsertNewAfter(Node * a_pNode, T const & a_data)
  {
    Node * newNode(m_pNextFree);
    if (m_nItems >= (pool_size() - 1))
    {
      size_t index = newNode - m_pData;
      Extend();
      newNode = &m_pData[index];
    }

    m_pNextFree = m_pNextFree->Next();
    newNode->InitData(a_data);
    newNode->InsertAfter(a_pNode);
    m_nItems++;

    return newNode;
  }

  //--------------------------------------------------------------------------------
  //	@	list<T>::push_back()
  //--------------------------------------------------------------------------------
  template<typename T>
  void list<T>::push_back(T const & a_item)
  {
    InsertNewAfter(m_pData[0].Prev(), a_item);
  }

  //--------------------------------------------------------------------------------
  //	@	list<T>::push_front()
  //--------------------------------------------------------------------------------
  template<typename T>
  void list<T>::push_front(T const & a_item)
  {
    InsertNewAfter(&m_pData[0], a_item);
  }

  //--------------------------------------------------------------------------------
  //	@	list<T>::Remove()
  //--------------------------------------------------------------------------------
  template<typename T>
  void list<T>::Remove(Node * a_pNode)
  {
    a_pNode->DestructData();
    a_pNode->Break();
    a_pNode->Next(m_pNextFree);
    m_pNextFree = a_pNode;
    m_nItems--;
  }

  //--------------------------------------------------------------------------------
  //	@	list<T>::pop_back()
  //--------------------------------------------------------------------------------
  template<typename T>
  void list<T>::pop_back()
  {
    Remove(m_pData[0].Prev());
  }

  //--------------------------------------------------------------------------------
  //	@	list<T>::pop_front()
  //--------------------------------------------------------------------------------
  template<typename T>
  void list<T>::pop_front()
  {
    Remove(m_pData[0].Next());
  }

  //--------------------------------------------------------------------------------
  //	@	list<T>::insert()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename list<T>::iterator list<T>::insert(typename list<T>::iterator const & it, T const & a_item)
  {
    return iterator(InsertNewAfter(it.m_pNode->Prev(), a_item));
  }

  //--------------------------------------------------------------------------------
  //	@	list<T>::erase()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename list<T>::iterator list<T>::erase(typename list<T>::iterator const & it)
  {
    iterator result(it.m_pNode->Next());
    Remove(it.m_pNode);
    return result;
  }

  //--------------------------------------------------------------------------------
  //	@	list<T>::DestructAll()
  //--------------------------------------------------------------------------------
  template<typename T>
  void list<T>::DestructAll()
  {
    for (iterator it = begin(); it != end(); ++it)
    {
      (*it).~T();
    } 
  }

  //--------------------------------------------------------------------------------
  //	@	list<T>::Extend()
  //--------------------------------------------------------------------------------
  template<typename T>
  void list<T>::Extend()
  {
    size_t oldSize(pool_size());
    Node * pOldData(m_pData);
    set_next_pool_size();

    DG_ASSERT(pool_size() > oldSize);

    //Create new array
    m_pData = static_cast<Node *>(realloc(m_pData, (pool_size() + 1) * sizeof(Node)));
    DG_ASSERT(m_pData != nullptr);

    for (size_t i = 0; i <= oldSize; ++i)
    {
      m_pData[i].Prev(&m_pData[m_pData[i].Prev() - pOldData]);
      if (m_pData[i].Next() != nullptr)
      {
        m_pData[i].Next(&m_pData[m_pData[i].Next() - pOldData]);
      }
    }

    m_pNextFree = &m_pData[oldSize]; //This assumes m_pNextFree is always valid
    
    for (size_t i = oldSize; i < pool_size(); ++i)
    {
      m_pData[i].Next(&m_pData[i + 1]);
    }
    m_pData[pool_size()].Next(nullptr);
  }

#ifdef DG_DEBUG
  void list<T>::Print() const
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

  //--------------------------------------------------------------------------------
  //		Helpful functions
  //--------------------------------------------------------------------------------

  //! @ingroup DgContainers_functions
  //!
  //! Find a value in the list, returns iterator.
  //! 
  //! @param[in] first Iterator to start searching from
  //! @param[in] last Search will stop when this iterator is found
  //! @param[in] val Object to compare
  //!
  //! @return Iterator pointing to the val in the list. If no item was found,
  //!         will return last.
  template<typename T>
  typename list<T>::iterator find (
	  typename list<T>::iterator first, 
	  typename list<T>::iterator last, 
	  T const & val)
  {
    while (first!=last) 
    {
      if (*first==val) 
		    return first;

      ++first;
    }

    return last;
  }

  //! @ingroup DgContainers_functions
  //!
  //! Find a value in the list, returns const_iterator.
  //! 
  //! @param[in] first Iterator to start searching from
  //! @param[in] last Search will stop when this iterator is found
  //! @param[in] val Object to compare
  //!
  //! @return Iterator pointing to the val in the list. If no item was found,
  //!         will return last.
  template<typename T>
  typename list<T>::const_iterator find (
	  typename list<T>::const_iterator first, 
	  typename list<T>::const_iterator last, 
	  T const & val)
  {
    while (first!=last) 
    {
      if (*first==val) 
		    return first;

      ++first;
    }

    return last;
  }
};
#endif