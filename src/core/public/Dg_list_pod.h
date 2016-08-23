//! @file Dg_list_pod.h
//!
//! @author: Frank B. Hart
//! @date 21/05/2016
//!
//! Class declaration: list_pod


#ifndef DG_LIST_POD_H
#define DG_LIST_POD_H

#include <cstdlib>
#include <cstring>

#include "DgErrorHandler.h"

#define DG_CONTAINER_DEFAULT_SIZE 1024

namespace Dg
{
  //! @ingroup DgContainers
  //!
  //! @class list_pod
  //!
  //! Pre-allocated Linked list. Similar to std::list with similarly named methods
  //! and functionality. The underlying arrays are preallocated and only change in
  //! size if extending list past that allocated, or manually resizing. This makes
  //! for fast insertion/erasing of elements.
  //!
  //! Assumed types are POD, so no construction / assignment operators called
  //!
  //! @author Frank B. Hart
  //! @date 21/05/2016
  template<typename T, bool POD>
  class list_pod
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
		  friend class list_pod;
		
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
		  friend class list_pod;

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
    list_pod();

    //! Constructor 
    //! If the constructor fails to allocate the list, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
	  list_pod(size_t);

	  ~list_pod();

	  //! Copy constructor
	  list_pod(list_pod const &);

    //! Assignment
	  list_pod & operator=(list_pod const &);

	  //! Returns an iterator pointing to the first data in the list container.
    //! If the container is empty, the returned iterator value shall not be dereferenced.
    //!
    //! @return iterator
	  iterator			  begin()		        {return iterator(m_root.pNext);}
    
    //! Returns an iterator referring to the <em>past-the-end</em> data in the list container.
    //! This iterator shall not be dereferenced.
    //!
    //! @return iterator
    iterator			  end()		          {return iterator(const_cast<Node*>(&m_root)); }
	  
    //! Returns a const iterator pointing to the first data in the list container.
    //! If the container is empty, the returned iterator value shall not be dereferenced.
    //!
    //! @return const_iterator
    const_iterator	cbegin()	  const {return const_iterator(m_root.pNext);}
    
    //! Returns an iterator referring to the <em>past-the-end</em> data in the list container.
    //! This iterator shall not be dereferenced.
    //!
    //! @return const_iterator
    const_iterator	cend()		  const {return const_iterator(const_cast<Node*>(&m_root)); }
	  
    //! Returns number of elements in the list.
    size_t			    size()		  const {return m_nItems;}
	  
    //! Returns the maximum number of elements that can be held in the current allocated memory.
    size_t 			    max_size()	const {return m_poolSize;}

    //! Returns if the list is empty.
	  bool 			      empty()		  const {return m_nItems == 0;}

    //! Returns a reference to the last data in the list container.
    //! Calling this function on an empty container causes undefined behavior.
    //!
    //! @return Reference to data type
    T &             back()	 	        { return m_root.pPrev->data; }
     
    //! Returns a reference to the first data in the list container.
    //! Calling this function on an empty container causes undefined behavior.
    //!
    //! @return Reference to data type
    T &             front()		        { return m_root.pNext->data; }

    //! Returns a const reference to the last data in the list container.
    //! Calling this function on an empty container causes undefined behavior.
    //!
    //! @return const reference to data type
    T const &				back()		  const { return m_root.pPrev->data; }

    //! Returns a const reference to the first data in the list container.
    //! Calling this function on an empty container causes undefined behavior.
    //!
    //! @return const reference to data type
    T const &				front()		  const { return m_root.pNext->data; }

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

  private:
    // Increases the size of the underlying arrays by a factor of 2
    void extend();
    void init(size_t new_size);

  private:
	  //Data members

	  //Pre-allocated block of memory to hold elements
	  Node *    m_pData;

	  //Root and end objects, and pointers
    Node      m_root;

	  //Next free data in the list;
	  Node *    m_pNextFree;		

	  //Sizes
	  size_t    m_nItems;
	  size_t    m_poolSize;

  };


  //--------------------------------------------------------------------------------
  //	@	list_pod<T, POD>::iterator::operator=()
  //--------------------------------------------------------------------------------
  template<typename T, bool POD>
  typename list_pod<T, POD>::iterator& list_pod<T, POD>::iterator::operator=
	  (typename list_pod<T, POD>::iterator const & other)
  {
	  m_pNode = other.m_pNode;

	  return *this;
  }	//End:: list_pod<T, POD>::iterator::operator=()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T, POD>::iterator::operator++()
  //--------------------------------------------------------------------------------
  template<typename T, bool POD>
  typename list_pod<T, POD>::iterator& list_pod<T, POD>::iterator::operator++()
  {
	  m_pNode = m_pNode->pNext;

	  return *this;
  }	//End: list_pod<T, POD>::iterator::operator++()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T, POD>::iterator::operator++()
  //--------------------------------------------------------------------------------
  template<typename T, bool POD>
  typename list_pod<T, POD>::iterator list_pod<T, POD>::iterator::operator++(int)
  {
	  iterator result(*this);	// make a copy for result
      ++(*this);              // Now use the prefix version to do the work
      return result;			// return the copy (the old) value.

  }	//End: list_pod<T, POD>::iterator::operator++()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T, POD>::iterator::operator--()
  //--------------------------------------------------------------------------------
  template<typename T, bool POD>
  typename list_pod<T, POD>::iterator& list_pod<T, POD>::iterator::operator--()
  {
	  m_pNode = m_pNode->pPrev;

	  return *this;

  }	//End: list_pod<T, POD>::iterator::operator++()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T, POD>::iterator::operator--()
  //--------------------------------------------------------------------------------
  template<typename T, bool POD>
  typename list_pod<T, POD>::iterator list_pod<T, POD>::iterator::operator--(int)
  {
	  iterator result(*this);	// make a copy for result
      --(*this);              // Now use the prefix version to do the work
      return result;			// return the copy (the old) value.

  }	//End: list_pod<T, POD>::iterator::operator--()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T, POD>::const_iterator::operator=()
  //--------------------------------------------------------------------------------
  template<typename T, bool POD>
  typename list_pod<T, POD>::const_iterator& list_pod<T, POD>::const_iterator::operator=
	  (typename list_pod<T, POD>::const_iterator const & other)
  {
	  m_pNode = other.m_pNode;

	  return *this;

  }	//End::list_pod<T, POD>::const_iterator::operator=()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T, POD>::const_iterator::operator++()
  //--------------------------------------------------------------------------------
  template<typename T, bool POD>
  typename list_pod<T, POD>::const_iterator& list_pod<T, POD>::const_iterator::operator++()
  {
	  m_pNode = m_pNode->pNext;

	  return *this;

  }	//End: list_pod<T, POD>::const_iterator::operator++()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T, POD>::const_iterator::operator=()
  //--------------------------------------------------------------------------------
  template<typename T, bool POD>
  typename list_pod<T, POD>::const_iterator list_pod<T, POD>::const_iterator::operator++(int)
  {
	  const_iterator result(*this);	// make a copy for result
      ++(*this);              // Now use the prefix version to do the work
      return result;			// return the copy (the old) value.

  }	//End: list_pod<T, POD>::const_iterator::operator++()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T, POD>::const_iterator::operator--()
  //--------------------------------------------------------------------------------
  template<typename T, bool POD>
  typename list_pod<T, POD>::const_iterator& list_pod<T, POD>::const_iterator::operator--()
  {
	  m_pNode = m_pNode->pPrev;

	  return *this;

  }	//End: list_pod<T, POD>::const_iterator::operator--()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T, POD>::const_iterator::operator--()
  //--------------------------------------------------------------------------------
  template<typename T, bool POD>
  typename list_pod<T, POD>::const_iterator list_pod<T, POD>::const_iterator::operator--(int)
  {
	  const_iterator result(*this);	// make a copy for result
      --(*this);              // Now use the prefix version to do the work
      return result;			// return the copy (the old) value.

  }	//End: list_pod<T, POD>::const_iterator::operator--()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T, POD>::init()
  //--------------------------------------------------------------------------------
  template<typename T, bool POD>
  void list_pod<T, POD>::init(size_t a_size) 
  {
    m_pData = static_cast<Node *>(realloc(m_pData, a_size * sizeof(Node)));

    DG_ASSERT((m_pData == nullptr) ? 0 : 1);

	  //Assign sizes
    m_poolSize = a_size;
	  m_nItems = 0;

	  //Initialise m_pData
	  m_pNextFree = &m_pData[0];

	  //Set outer container pointers
    m_root.pNext = &m_root;
    m_root.pPrev = &m_root;

	  //Only need to assign forward pointers
	  for (size_t i = 0; i < m_poolSize-1; i++)
	  {
		  m_pData[i].pNext = &m_pData[i+1];
	  }

  }	//End: list_pod::init()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T, POD>::list_pod<T, POD>()
  //--------------------------------------------------------------------------------
  template<typename T, bool POD>
  list_pod<T, POD>::list_pod() : m_pData(nullptr), m_pNextFree(nullptr)
  {
	  //Set m_pData
	  init(DG_CONTAINER_DEFAULT_SIZE);

  }	//End: list_pod::list_pod()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T, POD>::list_pod<T, POD>()
  //--------------------------------------------------------------------------------
  template<typename T, bool POD>
  list_pod<T, POD>::list_pod(size_t a_size): m_pData(nullptr), m_pNextFree(nullptr)
  {
	  //Set up the list
    init(a_size);

  }	//End: list_pod::list_pod()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T, POD>::~list_pod<T, POD>()
  //--------------------------------------------------------------------------------
  template<typename T, bool POD>
  list_pod<T, POD>::~list_pod()
  {
    free(m_pData);

  }	//End: list_pod::~list_pod()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T, POD>::list_pod<T, POD>()
  //--------------------------------------------------------------------------------
  template<typename T, bool POD>
  list_pod<T, POD>::list_pod(list_pod const & other)
  {
	  //Initialise m_pData
	  init(other.m_poolSize);

	  //Assign m_pData
	  list_pod<T, POD>::const_iterator it = other.cbegin();
	  for (it; it != other.cend(); ++it)
	  {
		  push_back(*it);
	  }

  }	//End: list_pod::list_pod()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T, POD>::operator=()
  //--------------------------------------------------------------------------------
  template<typename T, bool POD>
  list_pod<T, POD>& list_pod<T, POD>::operator=(list_pod const & other)
  {
	  if (this == &other)
		  return *this;

	  //resize array
	  resize(other.m_poolSize);

	  //Assign m_pData
	  list_pod<T, POD>::const_iterator it = other.cbegin();
	  for (it; it != other.cend(); ++it)
	  {
		  push_back(*it);
	  }

	  return *this;
  }	//End: list_pod::operator=()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T, POD>::clear()
  //--------------------------------------------------------------------------------
  template<typename T, bool POD>
  void list_pod<T, POD>::clear()
  {
	  //Reset pNext free
	  m_pNextFree = &m_pData[0];

	  //Set outer container pointers
    m_root.pNext = &m_root;
    m_root.pPrev = &m_root;

	  //close the last data in the list
	  m_pData[m_poolSize-1].pNext = nullptr;

	  //Assign pointers
	  for (size_t i = 0; i < m_poolSize-1; i++)
	  {
		  m_pData[i].pNext = &m_pData[i+1];
	  }

	  m_nItems = 0;

  }	//End: list_pod<T, POD>::clear()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T, POD>::resize()
  //--------------------------------------------------------------------------------
  template<typename T, bool POD>
  void list_pod<T, POD>::resize(size_t a_newSize)
  {
	  //Size must be at least 1
    if (a_newSize == 0) a_newSize = 1;

	  //Initialise m_pData
	  init(a_newSize);

  }	//End: list_pod<T, POD>::resize()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T, POD>::push_back()
  //--------------------------------------------------------------------------------
  template<typename T, bool POD>
  void list_pod<T, POD>::push_back(T const & a_item)
  {
	  //Is the list full?
	  if (m_nItems == m_poolSize)
		  extend();

	  //Get the list node to work on
	  Node *new_element = m_pNextFree;
	
	  //Move m_pNextFree pointer to the pNext Node
	  m_pNextFree = m_pNextFree->pNext;

	  //Assign the data
    memcpy(&new_element->data, &a_item, sizeof(T));

	  //Add the current data to the back of the active list
	  m_root.pPrev->pNext = new_element;
	  new_element->pPrev = m_root.pPrev;
    new_element->pNext = &m_root;
	  m_root.pPrev = new_element;

	  //Increment m_nItems
	  m_nItems++;

  }	//End: list_pod::push_back()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T, POD>::push_back()
  //--------------------------------------------------------------------------------
  template<typename T, bool POD>
  bool list_pod<T, POD>::push_back()
  {
	  //Is the list full?
	  if (m_nItems == m_poolSize)
		  return false;

	  //Get the list node to work on
	  Node *new_element = m_pNextFree;

	  //Move m_pNextFree pointer to the pNext Node
	  m_pNextFree = m_pNextFree->pNext;

	  //Add the current data to the back of the active list
    m_root.pPrev->pNext = new_element;
    new_element->pPrev = m_root.pPrev;
    new_element->pNext = &m_root;
    m_root.pPrev = new_element;

	  //Increment m_nItems
	  m_nItems++;

	  return true;

  }	//End: list_pod::push_back()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T, POD>::push_front()
  //--------------------------------------------------------------------------------
  template<typename T, bool POD>
  bool list_pod<T, POD>::push_front()
  {
      //Is the list full?
      if (m_nItems == m_poolSize)
          return false;

      //Get the list node to work on
      Node *new_element = m_pNextFree;

      //Move m_pNextFree pointer to the pNext Node
      m_pNextFree = m_pNextFree->pNext;

      //Add the current data to the back of the active list
      m_root.pNext->pPrev = new_element;
      new_element->pPrev = &m_root;
      new_element->pNext = m_root.pNext;
      m_root.pNext = new_element;

      //Increment m_nItems
      m_nItems++;

      return true;

  }	//End: list_pod::push_back()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T, POD>::push_front()
  //--------------------------------------------------------------------------------
  template<typename T, bool POD>
  void list_pod<T, POD>::push_front(T const & a_item)
  {
	  //Is the list full?
	  if (m_nItems == m_poolSize)
		  extend();

	  //Get the list node to work on
	  Node *new_element = m_pNextFree;
	
	  //Move m_pNextFree pointer to the pNext Node
	  m_pNextFree = m_pNextFree->pNext;

	  //Assign the data
    memcpy(&new_element->data, &a_item, sizeof(T));

	  //Add the current data to the back of the active list
    m_root.pNext->pPrev = new_element;
    new_element->pPrev = &m_root;
    new_element->pNext = m_root.pNext;
    m_root.pNext = new_element;

	  //Increment m_nItems
	  m_nItems++;

  }	//End: list_pod::push_front()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T, POD>::pop_back()
  //--------------------------------------------------------------------------------
  template<typename T, bool POD>
  void list_pod<T, POD>::pop_back()
  {
	  //Range check
    if (m_nItems == 0) return;
	
	  //Get new last data
    Node* last = m_root.pPrev->pPrev;

	  //Assign pNext free
	  m_root.pPrev->pNext = m_pNextFree;
	  m_pNextFree = m_root.pPrev;

	  //Break data from chain
    last->pNext = &m_root;			//prev points to pNext
	  m_root.pPrev = last;	//pNext points to pPrev

	  //Deincrement m_nItems
	  m_nItems--;

  }	//End: list_pod::pop_back()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T, POD>::pop_front()
  //--------------------------------------------------------------------------------
  template<typename T, bool POD>
  void list_pod<T, POD>::pop_front()
  {
	  //Range check
    if (m_nItems == 0) return;
	
	  //Get new first data
    Node* first = m_root.pNext->pNext;

	  //Assign pNext free
	  m_root.pNext->pNext = m_pNextFree;
	  m_pNextFree = m_root.pNext;

	  //Break data from chain
    first->pPrev = &m_root;		//prev points to pNext
	  m_root.pNext = first;	//pNext points to pPrev

	  //Deincrement m_nItems
	  m_nItems--;

  }	//End: list_pod::pop_front()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T, POD>::insert()
  //--------------------------------------------------------------------------------
  template<typename T, bool POD>
  typename list_pod<T, POD>::iterator list_pod<T, POD>::insert(typename list_pod<T, POD>::iterator const & it, T const & a_item)
  {
	  //Is the list full?
	  if (m_nItems == m_poolSize)
		  extend();
	
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
    memcpy(&new_element->data, &a_item, sizeof(T));

	  //Increment m_nItems
	  m_nItems++;

    return iterator(new_element);

  }	//End: list_pod::insert()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T, POD>::erase()
  //--------------------------------------------------------------------------------
  template<typename T, bool POD>
  typename list_pod<T, POD>::iterator list_pod<T, POD>::erase(typename list_pod<T, POD>::iterator const & it)
  {
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

  }	//End: list_pod::erase()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T, POD>::extend()
  //--------------------------------------------------------------------------------
  template<typename T, bool POD>
  void list_pod<T, POD>::extend()
  {
	  //Calculate new size
	  size_t new_size = m_poolSize << 1;
	
    if (new_size < m_poolSize)
    {
      //TODO
    }

	  //Create new array
	  Node* new_data = static_cast<Node *>(malloc(new_size * sizeof(Node)));

    if (new_data == nullptr)
    {
      //TODO
    }

	  //Assign pointers
	  for (size_t i = 0; i < new_size-1; i++)
	  {
		  new_data[i].pNext = &new_data[i+1];
	  }
	  for (size_t i = 1; i < m_nItems; i++)
	  {
		  new_data[i].pPrev = &new_data[i-1];
	  }

	  //Assign values
	  iterator it = begin();
	  for (size_t i = 0; it != end(); ++it, ++i)
	  {
		  new_data[i].data = *it;
	  }

	  //Assign m_pData pointer
	  free(m_pData);
	  m_pData = new_data;
	
	  //Assign pNext free pointer
	  m_pNextFree = &new_data[m_nItems];
	
	  //Adjust sizes
	  m_poolSize = new_size;

	  //Determine root and end pointers
	  if (m_nItems == 0)
	  {
      m_root.pNext = &m_root;
      m_root.pPrev = &m_root;
	  }
	  else
	  {
		  m_root.pNext = &m_pData[0];
		  m_root.pPrev = &m_pData[m_nItems-1];
      new_data[0].pPrev = &m_root;
      new_data[m_nItems - 1].pNext = &m_root;
	  }
	
  }	//End: list_pod<T, POD>::extend()


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
  //! @return Iterator pointing to the val in the list_pod. If no item was found,
  //!         will return last.
  template<typename T, bool POD>
  typename list_pod<T, POD>::iterator find (
	  typename list_pod<T, POD>::iterator first, 
	  typename list_pod<T, POD>::iterator last, 
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
  //! @return Iterator pointing to the val in the list_pod. If no item was found,
  //!         will return last.
  template<typename T, bool POD>
  typename list_pod<T, POD>::const_iterator find (
	  typename list_pod<T, POD>::const_iterator first, 
	  typename list_pod<T, POD>::const_iterator last, 
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