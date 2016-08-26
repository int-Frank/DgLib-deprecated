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
#define COMMENT
#else
#define COMMENT / ## /
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
  //!     |     |->|     |->|     |->|     |->|     |->|     |     |  |     |  |     |  |     |
  //!     |  0  |  |  1  |  |  2  |  |  3  |  |  4  |  |  5  |  6  |->|  7  |->|  8  |->|  9  |->NULL
  //!     |     |<-|     |<-|     |<-|     |<-|     |<-|     |     |  |     |  |     |  |     |
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
    
    //Container to hold the object and pointers
	  struct Node
	  {
		  Node(): pNext(nullptr), pPrev(nullptr) {}
		
		  Node* pNext;
		  Node* pPrev;

		  T data;
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
		  T const * operator->() const {return &(m_pNode->data);}

      //! Conversion. Returns reference to data.
		  T const & operator*() const {return m_pNode->data;}

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
		  T* operator->() const {return &(m_pNode->data);}

      //! Conversion
		  T& operator*() const {return m_pNode->data;}

	  private:
		  Node *m_pNode;

	  };


  public:
    //! Constructor 
    //! If the constructor fails to allocate the list, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
    list();

    //! Constructor 
    //! If the constructor fails to allocate the list, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
	  list(size_t);

	  ~list();

	  //! Copy constructor
	  list(list const &);

    //! Assignment
	  list & operator=(list const &);

    //! Move constructor
    list(list &&);

    //! Move assignment
    list & operator=(list &&);

	  //! Returns an iterator pointing to the first data in the list container.
    //! If the container is empty, the returned iterator value shall not be dereferenced.
    //!
    //! @return iterator
	  iterator			  begin()		        {return iterator(m_pData->pNext);}
    
    //! Returns an iterator referring to the <em>past-the-end</em> data in the list container.
    //! This iterator shall not be dereferenced.
    //!
    //! @return iterator
    iterator			  end()		          {return iterator(const_cast<Node*>(m_pData)); }
	  
    //! Returns a const iterator pointing to the first data in the list container.
    //! If the container is empty, the returned iterator value shall not be dereferenced.
    //!
    //! @return const_iterator
    const_iterator	cbegin()	  const {return const_iterator(m_pData->pNext);}
    
    //! Returns an iterator referring to the <em>past-the-end</em> data in the list container.
    //! This iterator shall not be dereferenced.
    //!
    //! @return const_iterator
    const_iterator	cend()		  const {return const_iterator(const_cast<Node*>(m_pData)); }
	  
    //! Returns number of elements in the list.
    size_t			    size()		  const {return m_nItems;}
	  
    //! Returns if the list is empty.
	  bool 			      empty()		  const {return m_nItems == 0;}

    //! Returns a reference to the last data in the list container.
    //! Calling this function on an empty container causes undefined behavior.
    //!
    //! @return Reference to data type
    T &             back()	 	        { return m_pData->pPrev->data; }
     
    //! Returns a reference to the first data in the list container.
    //! Calling this function on an empty container causes undefined behavior.
    //!
    //! @return Reference to data type
    T &             front()		        { return m_pData->pNext->data; }

    //! Returns a const reference to the last data in the list container.
    //! Calling this function on an empty container causes undefined behavior.
    //!
    //! @return const reference to data type
    T const &				back()		  const { return m_pData->pPrev->data; }

    //! Returns a const reference to the first data in the list container.
    //! Calling this function on an empty container causes undefined behavior.
    //!
    //! @return const reference to data type
    T const &				front()		  const { return m_pData->pNext->data; }

	  //! Add an data to the back of the list
    void push_back(T const &);

    //! Add an data to the back of the list, but does not assign, nor
    //!	resize the array.
    //!
    //! @return false if allocated memory is full
	  bool push_back();

    //!Add an data to the front of the list
    void push_front(T const &);
    
    //! Add an data to the front of the list, but does not assign, nor
    //! resize the array.
    //!
    //! @return false if allocated memory is full
    bool push_front();

    //! Add an data to the list at position. 
    //! The input iterator may be invalid as a resize may have occured. Resizing
    //! invalidates all pointers in the iterator. The return iterator will be valid.
    //! @return iterator to the newly inserted data
    iterator insert(iterator const &, T const &);

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
    void Print(char const * a_title) const
    {
      std::cout << "\n\t" << a_title << "\n\n pNext:\n";
      for (size_t i = 0; i < pool_size() + 1; ++i)
      {
        std::cout << "[" << i << "] -> ";
        if (m_pData[i].pNext == nullptr)
        {
          std::cout << "NULL\n";
        }
        else
        {
          std::cout << "[" << (m_pData[i].pNext - m_pData) << "]\n";
        }
      }
      std::cout << "\n\npPrev:\n";
      for (size_t i = 0; i < pool_size() + 1; ++i)
      {
        std::cout << "[" << i << "] -> ";
        if (m_pData[i].pPrev == nullptr)
        {
          std::cout << "NULL\n";
        }
        else
        {
          std::cout << "[" << (m_pData[i].pPrev - m_pData) << "]\n";
        }
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
    void Extend();

    void DestructAll();

    void init(size_t new_size);

    void AssignPointersToEmpty();

  private:
	  //Data members

	  //Pre-allocated block of memory to hold elements
	  Node *    m_pData;

	  //Next free data in the list;
	  Node *    m_pNextFree;		

	  //Sizes
	  size_t    m_nItems;
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
  }	//End:: list<T>::iterator::operator=()


  //--------------------------------------------------------------------------------
  //	@	list<T>::iterator::operator++()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename list<T>::iterator& list<T>::iterator::operator++()
  {
	  m_pNode = m_pNode->pNext;

	  return *this;
  }	//End: list<T>::iterator::operator++()


  //--------------------------------------------------------------------------------
  //	@	list<T>::iterator::operator++()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename list<T>::iterator list<T>::iterator::operator++(int)
  {
	  iterator result(*this);	// make a copy for result
      ++(*this);              // Now use the prefix version to do the work
      return result;			// return the copy (the old) value.

  }	//End: list<T>::iterator::operator++()


  //--------------------------------------------------------------------------------
  //	@	list<T>::iterator::operator--()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename list<T>::iterator& list<T>::iterator::operator--()
  {
	  m_pNode = m_pNode->pPrev;

	  return *this;

  }	//End: list<T>::iterator::operator++()


  //--------------------------------------------------------------------------------
  //	@	list<T>::iterator::operator--()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename list<T>::iterator list<T>::iterator::operator--(int)
  {
	  iterator result(*this);	// make a copy for result
      --(*this);              // Now use the prefix version to do the work
      return result;			// return the copy (the old) value.

  }	//End: list<T>::iterator::operator--()


  //--------------------------------------------------------------------------------
  //	@	list<T>::const_iterator::operator=()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename list<T>::const_iterator& list<T>::const_iterator::operator=
	  (typename list<T>::const_iterator const & other)
  {
	  m_pNode = other.m_pNode;

	  return *this;

  }	//End::list<T>::const_iterator::operator=()


  //--------------------------------------------------------------------------------
  //	@	list<T>::const_iterator::operator++()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename list<T>::const_iterator& list<T>::const_iterator::operator++()
  {
	  m_pNode = m_pNode->pNext;

	  return *this;

  }	//End: list<T>::const_iterator::operator++()


  //--------------------------------------------------------------------------------
  //	@	list<T>::const_iterator::operator=()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename list<T>::const_iterator list<T>::const_iterator::operator++(int)
  {
	  const_iterator result(*this);	// make a copy for result
      ++(*this);              // Now use the prefix version to do the work
      return result;			// return the copy (the old) value.

  }	//End: list<T>::const_iterator::operator++()


  //--------------------------------------------------------------------------------
  //	@	list<T>::const_iterator::operator--()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename list<T>::const_iterator& list<T>::const_iterator::operator--()
  {
	  m_pNode = m_pNode->pPrev;

	  return *this;

  }	//End: list<T>::const_iterator::operator--()


  //--------------------------------------------------------------------------------
  //	@	list<T>::const_iterator::operator--()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename list<T>::const_iterator list<T>::const_iterator::operator--(int)
  {
	  const_iterator result(*this);	// make a copy for result
      --(*this);              // Now use the prefix version to do the work
      return result;			// return the copy (the old) value.

  }	//End: list<T>::const_iterator::operator--()


  //--------------------------------------------------------------------------------
  //	@	list<T>::AssignPointersToEmpty()
  //--------------------------------------------------------------------------------
  template<typename T>
  void list<T>::AssignPointersToEmpty()
  {
    //Initialise m_pData
    m_pNextFree = &m_pData[1];

    //Set outer container pointers
    m_pData->pNext = m_pData;
    m_pData->pPrev = m_pData;

    for (size_t i = 1; i < pool_size(); i++)
    {
      m_pData[i].pNext = &m_pData[i + 1];
    }
    m_pData[pool_size()].pNext = nullptr;
  }	//End: list::AssignPointersToEmpty()


  //--------------------------------------------------------------------------------
  //	@	list<T>::init()
  //--------------------------------------------------------------------------------
  template<typename T>
  void list<T>::init(size_t a_size) 
  {
    pool_size(a_size + 1);
    m_pData = static_cast<Node *>(realloc(m_pData, (pool_size() + 1) * sizeof(Node)));

    DG_ASSERT(m_pData != nullptr);

	  //Assign sizes
	  m_nItems = 0;
    AssignPointersToEmpty();
    COMMENT Print("init");
  }	//End: list::init()


  //--------------------------------------------------------------------------------
  //	@	list<T>::list<T>()
  //--------------------------------------------------------------------------------
  template<typename T>
  list<T>::list() 
    : ContainerBase()
    , m_pData(nullptr)
    , m_pNextFree(nullptr)
  {
	  //Set m_pData
	  init(pool_size());

  }	//End: list::list()


  //--------------------------------------------------------------------------------
  //	@	list<T>::list<T>()
  //--------------------------------------------------------------------------------
  template<typename T>
  list<T>::list(size_t a_size)
    : ContainerBase(a_size)
    , m_pData(nullptr)
    , m_pNextFree(nullptr)
  {
	  //Set up the list
    init(pool_size());

  }	//End: list::list()


  //--------------------------------------------------------------------------------
  //	@	list<T>::~list<T>()
  //--------------------------------------------------------------------------------
  template<typename T>
  list<T>::~list()
  {
    if (!std::is_trivially_destructible<T>::value)
    {
      DestructAll();
    }

    free(m_pData);

  }	//End: list::~list()


  //--------------------------------------------------------------------------------
  //	@	list<T>::list<T>()
  //--------------------------------------------------------------------------------
  template<typename T>
  list<T>::list(list const & other)
    : ContainerBase(other)
  {
	  //Initialise m_pData
	  init(other.pool_size());

	  //Assign m_pData
	  list<T>::const_iterator it = other.cbegin();
	  for (it; it != other.cend(); ++it)
	  {
		  push_back(*it);
	  }

  }	//End: list::list()


  //--------------------------------------------------------------------------------
  //	@	list<T>::operator=()
  //--------------------------------------------------------------------------------
  template<typename T>
  list<T>& list<T>::operator=(list const & other)
  {
	  if (this == &other)
		  return *this;

    ContainerBase::operator=(other);

	  //resize array
	  resize(other.pool_size());

	  //Assign m_pData
	  list<T>::const_iterator it = other.cbegin();
	  for (it; it != other.cend(); ++it)
	  {
		  push_back(*it);
	  }

    COMMENT Print("operator =");
	  return *this;
  }	//End: list::operator=()


  //--------------------------------------------------------------------------------
  //	@	list<T>::clear()
  //--------------------------------------------------------------------------------
  template<typename T>
  void list<T>::clear()
  {
    if (!std::is_trivially_destructible<T>::value)
    {
      DestructAll();
    }

    m_nItems = 0;
    AssignPointersToEmpty();
    COMMENT Print("Clear");
  }	//End: list<T>::clear()


  //--------------------------------------------------------------------------------
  //	@	list<T>::resize()
  //--------------------------------------------------------------------------------
  template<typename T>
  void list<T>::resize(size_t a_newSize)
  {
    if (!std::is_trivially_destructible<T>::value)
    {
      DestructAll();
    }

	  //Size must be at least 1
    if (a_newSize == 0) a_newSize = 1;

	  //Initialise m_pData
	  init(a_newSize);
    COMMENT Print("Resize");

  }	//End: list<T>::resize()


  //--------------------------------------------------------------------------------
  //	@	list<T>::push_back()
  //--------------------------------------------------------------------------------
  template<typename T>
  void list<T>::push_back(T const & a_item)
  {
	  //Is the list full?
    if (m_nItems >= (pool_size() - 1))
    {
      Extend();
    }

	  //Get the list node to work on
	  Node *new_element = m_pNextFree;
	
	  //Move m_pNextFree pointer to the pNext Node
	  m_pNextFree = m_pNextFree->pNext;

	  //Assign the data
    if (std::is_trivially_copy_constructible<T>::value)
    {
      memcpy(&new_element->data, &a_item, sizeof(T));
    }
    else
    {
      new (&(new_element->data)) T(a_item);
    }

	  //Add the current data to the back of the active list
	  m_pData->pPrev->pNext = new_element;
	  new_element->pPrev = m_pData->pPrev;
    new_element->pNext = m_pData;
	  m_pData->pPrev = new_element;

	  //Increment m_nItems
	  m_nItems++;
    COMMENT Print("push_back");

  }	//End: list::push_back()


  //--------------------------------------------------------------------------------
  //	@	list<T>::push_back()
  //--------------------------------------------------------------------------------
  template<typename T>
  bool list<T>::push_back()
  {
	  //Is the list full?
	  if (m_nItems >= (pool_size() - 1))
		  return false;

	  //Get the list node to work on
	  Node *new_element = m_pNextFree;

	  //Move m_pNextFree pointer to the pNext Node
	  m_pNextFree = m_pNextFree->pNext;

	  //Add the current data to the back of the active list
    m_pData->pPrev->pNext = new_element;
    new_element->pPrev = m_pData->pPrev;
    new_element->pNext = m_pData;
    m_pData->pPrev = new_element;

    if (!std::is_trivial<T>::value)
    {
      new(&(new_element->data)) T();
    }

	  //Increment m_nItems
	  m_nItems++;

	  return true;

  }	//End: list::push_back()


  //--------------------------------------------------------------------------------
  //	@	list<T>::push_front()
  //--------------------------------------------------------------------------------
  template<typename T>
  bool list<T>::push_front()
  {
      //Is the list full?
      if (m_nItems >= (pool_size() - 1))
          return false;

      //Get the list node to work on
      Node *new_element = m_pNextFree;

      //Move m_pNextFree pointer to the pNext Node
      m_pNextFree = m_pNextFree->pNext;

      //Add the current data to the back of the active list
      m_pData->pNext->pPrev = new_element;
      new_element->pPrev = m_pData;
      new_element->pNext = m_pData->pNext;
      m_pData->pNext = new_element;

      if (!std::is_trivial<T>::value)
      {
        new(&(new_element->data)) T();
      }

      //Increment m_nItems
      m_nItems++;

      return true;

  }	//End: list::push_back()


  //--------------------------------------------------------------------------------
  //	@	list<T>::push_front()
  //--------------------------------------------------------------------------------
  template<typename T>
  void list<T>::push_front(T const & a_item)
  {
	  //Is the list full?
    if (m_nItems >= (pool_size() - 1))
    {
      Extend();
    }

	  //Get the list node to work on
	  Node *new_element = m_pNextFree;
	
	  //Move m_pNextFree pointer to the pNext Node
	  m_pNextFree = m_pNextFree->pNext;

	  //Assign the data
    if (std::is_trivially_copy_constructible<T>::value)
    {
      memcpy(&new_element->data, &a_item, sizeof(T));
    }
    else
    {
      new (&new_element->data) T(a_item);
    }

	  //Add the current data to the back of the active list
    m_pData->pNext->pPrev = new_element;
    new_element->pPrev = m_pData;
    new_element->pNext = m_pData->pNext;
    m_pData->pNext = new_element;

	  //Increment m_nItems
	  m_nItems++;
    COMMENT Print("push_front");

  }	//End: list::push_front()


  //--------------------------------------------------------------------------------
  //	@	list<T>::pop_back()
  //--------------------------------------------------------------------------------
  template<typename T>
  void list<T>::pop_back()
  {
    if (!std::is_trivially_destructible<T>::value)
    {
      m_pData->pPrev->data.~T();
    }

	  //Get new last data
    Node* last = m_pData->pPrev->pPrev;

	  //Assign pNext free
	  m_pData->pPrev->pNext = m_pNextFree;
	  m_pNextFree = m_pData->pPrev;

	  //Break data from chain
    last->pNext = m_pData;			//prev points to pNext
	  m_pData->pPrev = last;	//pNext points to pPrev

	  //Deincrement m_nItems
	  m_nItems--;
    COMMENT Print("pop_back");

  }	//End: list::pop_back()


  //--------------------------------------------------------------------------------
  //	@	list<T>::pop_front()
  //--------------------------------------------------------------------------------
  template<typename T>
  void list<T>::pop_front()
  {
    if (!std::is_trivially_destructible<T>::value)
    {
      m_pData->pNext->data.~T();
    }

	  //Get new first data
    Node* first = m_pData->pNext->pNext;

	  //Assign pNext free
	  m_pData->pNext->pNext = m_pNextFree;
	  m_pNextFree = m_pData->pNext;

	  //Break data from chain
    first->pPrev = m_pData;		//prev points to pNext
	  m_pData->pNext = first;	//pNext points to pPrev

	  //Deincrement m_nItems
	  m_nItems--;
    COMMENT Print("pop_front");

  }	//End: list::pop_front()


  //--------------------------------------------------------------------------------
  //	@	list<T>::insert()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename list<T>::iterator list<T>::insert(typename list<T>::iterator const & it, T const & a_item)
  {
	  //Get the list node to work on
	  Node *new_element = m_pNextFree;
	
	  //Move m_pNextFree pointer to the pNext Node
	  m_pNextFree = m_pNextFree->pNext;

	  //Insert pNext free
	  it.m_pNode->pPrev->pNext = new_element;
	  new_element->pPrev = it.m_pNode->pPrev;
	  it.m_pNode->pPrev = new_element;
	  new_element->pNext = it.m_pNode;

	  //Set the data
    if (std::is_trivially_copy_constructible<T>::value)
    {
      memcpy(&new_element->data, &a_item, sizeof(T));
    }
    else
    {
      new (&(new_element->data))  T(a_item);
    }

	  //Increment m_nItems
	  m_nItems++;

    //Is the list full?
    size_t index(new_element - m_pData);
    if (m_nItems >= (pool_size() - 1))
    {
      Extend();
    }
    return iterator(&m_pData[index]);
    COMMENT Print("insert");

  }	//End: list::insert()


  //--------------------------------------------------------------------------------
  //	@	list<T>::erase()
  //--------------------------------------------------------------------------------
  template<typename T>
  typename list<T>::iterator list<T>::erase(typename list<T>::iterator const & it)
  {
    if (!std::is_trivially_destructible<T>::value)
    {
      it.m_pNode->data.~T();
    }

	  //Remember pPrev data
	  Node* pNext = it.m_pNode->pNext;

	  //Break data from chain
	  it.m_pNode->pPrev->pNext = it.m_pNode->pNext;		//prev points to pNext
	  it.m_pNode->pNext->pPrev = it.m_pNode->pPrev;	//pNext points to pPrev

	  //Add this broken item to the begining of the free list
	  it.m_pNode->pNext = m_pNextFree;				//put item in between m_pNextFree and end of the list
	
	  //Reset m_pNextFree
	  m_pNextFree = it.m_pNode;
	
	  //Deincrement m_nItems
	  m_nItems--;

	  //Return iterator to the pNext container
	  return iterator(pNext);
    COMMENT Print("erase");

  }	//End: list::erase()


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
  }	//End: list<T>::DestructAll()


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
      m_pData[i].pPrev = &m_pData[m_pData[i].pPrev - pOldData];
      if (m_pData[i].pNext == nullptr)
      {
        m_pData[i].pNext = nullptr;
      }
      else
      {
        m_pData[i].pNext = &m_pData[m_pData[i].pNext - pOldData];
      }
    }
    if (m_pNextFree)
    {
      m_pNextFree = &m_pData[m_pNextFree - pOldData];
      Node * pTail(m_pNextFree);
      while (pTail->pNext)
      {
        pTail = pTail->pNext;
      }
      pTail->pNext = &m_pData[oldSize];
    }
    else
    {
      m_pNextFree = &m_pData[oldSize];
    }

    for (size_t i = oldSize; i < pool_size(); ++i)
    {
      m_pData[i].pNext = &m_pData[i + 1];
    }
    m_pData[pool_size()].pNext = nullptr;
    COMMENT Print("Extend");
  }	//End: list<T>::Extend()


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

  }	//End find()


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

  }	//End find()

};
#endif