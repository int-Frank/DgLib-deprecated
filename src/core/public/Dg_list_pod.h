//! @file Dg_list_pod.h
//!
//! @author: Frank B. Hart
//! @date 21/05/2016
//!
//! Class declaration: list_podod


#ifndef DG_LIST_POD_H
#define DG_LIST_POD_H

#include <exception>

#include "container_common.h"

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
  template<typename T>
  class list_pod
  {
  private:
    
    //Container to hold the object and pointers
	  struct DataContainer
	  {
		  DataContainer(): next(nullptr), previous(nullptr) {}
		
		  DataContainer* next;
		  DataContainer* previous;
		  T element;
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
		  const_iterator(DataContainer const * _ptr) {ptr = _ptr;}

	  public:

		  const_iterator(): ptr(nullptr) {}
		  ~const_iterator(){}

		  //! Copy constructor
		  const_iterator(const_iterator const & it): ptr(it.ptr){}

      //! Assignment
		  const_iterator& operator= (const_iterator const &);
		
		  //! Comparison.
		  bool operator==(const_iterator const & it) const {return ptr == it.ptr;}

      //! Comparison.
		  bool operator!=(const_iterator const & it) const {return ptr != it.ptr;}

		  //! Post increment
		  const_iterator& operator++();

      //! Pre increment
		  const_iterator operator++(int);

      //! Post decrement
		  const_iterator& operator--();

      //! Pre decrement
		  const_iterator operator--(int);

		  //! Conversion. Returns pointer to element.
		  T const * operator->() const {return &(ptr->element);}

      //! Conversion. Returns reference to element.
		  T const & operator*() const {return ptr->element;}

	  private:
		  DataContainer const * ptr;

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
		  iterator(DataContainer* _ptr) {ptr = _ptr;}

	  public:

		  iterator(): ptr(nullptr) {}
		  ~iterator(){}

		  //! Copy constructor.
		  iterator(iterator const & it): ptr(it.ptr){}
		  
      //! Assignment.
      iterator& operator= (iterator const &);

		  //! Comparison.
		  bool operator==(iterator const & it) const {return ptr == it.ptr;}

      //! Comparison.
		  bool operator!=(iterator const & it) const {return ptr != it.ptr;}

		  //! Post increment
		  iterator& operator++();

      //! Pre increment
		  iterator operator++(int);

      //! Post decrement
		  iterator& operator--();

      //! Pre decrement
		  iterator operator--(int);

		  //! Conversion
		  operator const_iterator() const {return const_iterator(ptr);}

      //! Conversion
		  T* operator->() const {return &(ptr->element);}

      //! Conversion
		  T& operator*() const {return ptr->element;}

	  private:
		  DataContainer *ptr;

	  };


  public:
    //! Constructor 
    //! If the constructor fails to allocate the map_p, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
    list_pod();

    //! Constructor 
    //! If the constructor fails to allocate the map_p, the function throws a <a href="http://www.cplusplus.com/reference/new/bad_alloc/">bad_alloc</a> exception.
	  list_pod(size_t);

	  ~list_pod();

	  //! Copy constructor
	  list_pod(list_pod const &);

    //! Assignment
	  list_pod& operator= (list_pod const &);

	  //! Returns an iterator pointing to the first element in the list container.
    //! If the container is empty, the returned iterator value shall not be dereferenced.
    //!
    //! @return iterator
	  iterator			  begin()		  const {return iterator(m_rootContainer.next);}
    
    //! Returns an iterator referring to the <em>past-the-end</em> element in the list container.
    //! This iterator shall not be dereferenced.
    //!
    //! @return iterator
    iterator			  end()		    const {return iterator(const_cast<DataContainer*>(&m_endContainer)); }
	  
    //! Returns a const iterator pointing to the first element in the list container.
    //! If the container is empty, the returned iterator value shall not be dereferenced.
    //!
    //! @return const_iterator
    const_iterator	cbegin()	  const {return const_iterator(m_rootContainer.next);}
    
    //! Returns an iterator referring to the <em>past-the-end</em> element in the list container.
    //! This iterator shall not be dereferenced.
    //!
    //! @return const_iterator
    const_iterator	cend()		  const {return const_iterator(const_cast<DataContainer*>(&m_endContainer)); }
	  
    //! Returns number of elements in the list.
    size_t			    size()		  const {return m_currentSize;}
	  
    //! Returns the maximum number of elements that can be held in the current allocated memory.
    size_t 			    max_size()	const {return m_arraySize;}

    //! Returns if the list is empty.
	  bool 			      empty()		  const {return m_currentSize == 0;}

    //! Returns a reference to the last element in the list container.
    //! Calling this function on an empty container causes undefined behavior.
    //!
    //! @return Reference to data type
    T &             back()		  const { return m_endContainer.previous->element; }

    //! Returns a reference to the first element in the list container.
    //! Calling this function on an empty container causes undefined behavior.
    //!
    //! @return Reference to data type
    T &             front()		  const { return m_rootContainer.next->element; }

    //! Returns a const reference to the last element in the list container.
    //! Calling this function on an empty container causes undefined behavior.
    //!
    //! @return const reference to data type
    T const &				back()		  const { return m_endContainer.previous->element; }

    //! Returns a const reference to the first element in the list container.
    //! Calling this function on an empty container causes undefined behavior.
    //!
    //! @return const reference to data type
    T const &				front()		  const { return m_rootContainer.next->element; }

	  //! Add an element to the back of the list
    void push_back(T const &);

    //! Add an element to the back of the list, but does not assign, nor
    //!	resize the array.
    //!
    //! @return false if allocated memory is full
	  bool push_back();

    //!Add an element to the front of the list
    void push_front(T const &);
    
    //! Add an element to the front of the list, but does not assign, nor
    //! resize the array.
    //!
    //! @return false if allocated memory is full
    bool push_front();

    //! Add an element to the list at position. 
    void insert(iterator const &, T const &);

    //! Erase last element
	  void pop_back();

    //! Erase last element
	  void pop_front();

    //! Erase an element from the list
	  void erase(iterator&); //TODO Needs to return iterator
	  
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
	  DataContainer *m_data;

	  //Root and end objects, and pointers
	  DataContainer m_rootContainer;
	  DataContainer m_endContainer;

	  //Next free element in the list;
	  DataContainer* m_nextFree;		

	  //Sizes
	  size_t m_currentSize;
	  size_t m_arraySize;

  };


  //--------------------------------------------------------------------------------
  //	@	list_pod<T>::iterator::operator=()
  //--------------------------------------------------------------------------------
  template<class T>
  typename list_pod<T>::iterator& list_pod<T>::iterator::operator=
	  (typename list_pod<T>::iterator const & other)
  {
	  ptr = other.ptr;

	  return *this;
  }	//End:: list_pod<T>::iterator::operator=()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T>::iterator::operator++()
  //--------------------------------------------------------------------------------
  template<class T>
  typename list_pod<T>::iterator& list_pod<T>::iterator::operator++()
  {
	  ptr = ptr->next;

	  return *this;
  }	//End: list_pod<T>::iterator::operator++()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T>::iterator::operator++()
  //--------------------------------------------------------------------------------
  template<class T>
  typename list_pod<T>::iterator list_pod<T>::iterator::operator++(int)
  {
	  iterator result(*this);	// make a copy for result
      ++(*this);              // Now use the prefix version to do the work
      return result;			// return the copy (the old) value.

  }	//End: list_pod<T>::iterator::operator++()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T>::iterator::operator--()
  //--------------------------------------------------------------------------------
  template<class T>
  typename list_pod<T>::iterator& list_pod<T>::iterator::operator--()
  {
	  ptr = ptr->previous;

	  return *this;

  }	//End: list_pod<T>::iterator::operator++()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T>::iterator::operator--()
  //--------------------------------------------------------------------------------
  template<class T>
  typename list_pod<T>::iterator list_pod<T>::iterator::operator--(int)
  {
	  iterator result(*this);	// make a copy for result
      --(*this);              // Now use the prefix version to do the work
      return result;			// return the copy (the old) value.

  }	//End: list_pod<T>::iterator::operator--()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T>::const_iterator::operator=()
  //--------------------------------------------------------------------------------
  template<class T>
  typename list_pod<T>::const_iterator& list_pod<T>::const_iterator::operator=
	  (typename list_pod<T>::const_iterator const & other)
  {
	  ptr = other.ptr;

	  return *this;

  }	//End::list_pod<T>::const_iterator::operator=()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T>::const_iterator::operator++()
  //--------------------------------------------------------------------------------
  template<class T>
  typename list_pod<T>::const_iterator& list_pod<T>::const_iterator::operator++()
  {
	  ptr = ptr->next;

	  return *this;

  }	//End: list_pod<T>::const_iterator::operator++()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T>::const_iterator::operator=()
  //--------------------------------------------------------------------------------
  template<class T>
  typename list_pod<T>::const_iterator list_pod<T>::const_iterator::operator++(int)
  {
	  const_iterator result(*this);	// make a copy for result
      ++(*this);              // Now use the prefix version to do the work
      return result;			// return the copy (the old) value.

  }	//End: list_pod<T>::const_iterator::operator++()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T>::const_iterator::operator--()
  //--------------------------------------------------------------------------------
  template<class T>
  typename list_pod<T>::const_iterator& list_pod<T>::const_iterator::operator--()
  {
	  ptr = ptr->previous;

	  return *this;

  }	//End: list_pod<T>::const_iterator::operator--()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T>::const_iterator::operator--()
  //--------------------------------------------------------------------------------
  template<class T>
  typename list_pod<T>::const_iterator list_pod<T>::const_iterator::operator--(int)
  {
	  const_iterator result(*this);	// make a copy for result
      --(*this);              // Now use the prefix version to do the work
      return result;			// return the copy (the old) value.

  }	//End: list_pod<T>::const_iterator::operator--()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T>::init()
  //--------------------------------------------------------------------------------
  template<class T>
  void list_pod<T>::init(size_t a_size) 
  {
    T * tempPtr = static_cast<DataContainer *>(realloc(m_data, a_size * sizeof(DataContainer)));

    if (tempPtr == nullptr)
    {
      throw std::bad_alloc;
    }

    m_data = tempPtr;

	  //Assign sizes
    m_arraySize = a_size;
	  m_currentSize = 0;

	  //Initialise m_data
	  m_nextFree = &m_data[0];

	  //Set outer container pointers
    m_rootContainer.next = &m_endContainer;
    m_endContainer.previous = &m_rootContainer;

	  //Only need to assign forward pointers
	  for (size_t i = 0; i < m_arraySize-1; i++)
	  {
		  m_data[i].next = &m_data[i+1];
	  }

  }	//End: list_pod::init()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T>::list_pod<T>()
  //--------------------------------------------------------------------------------
  template<class T>
  list_pod<T>::list_pod() : m_data(nullptr), m_nextFree(nullptr)
  {
	  //Set m_data
	  init(DG_CONTAINER_DEFAULT_SIZE);

  }	//End: list_pod::list_pod()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T>::list_pod<T>()
  //--------------------------------------------------------------------------------
  template<class T>
  list_pod<T>::list_pod(size_t a_size): m_data(nullptr), m_nextFree(nullptr)
  {
	  //Size must be at least 1
    assert(a_size > 0);

	  //Set up the list
    init(a_size);

  }	//End: list_pod::list_pod()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T>::~list_pod<T>()
  //--------------------------------------------------------------------------------
  template<class T>
  list_pod<T>::~list_pod()
  {
	  free(m_data)

  }	//End: list_pod::~list_pod()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T>::list_pod<T>()
  //--------------------------------------------------------------------------------
  template<class T>
  list_pod<T>::list_pod(list_pod const & other)
  {
	  //Initialise m_data
	  init(other.m_arraySize);

	  //Assign m_data
	  list_pod<T>::const_iterator it = other.begin();
	  for (it; it != other.end(); ++it)
	  {
		  push_back(*it);
	  }

  }	//End: list_pod::list_pod()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T>::operator=()
  //--------------------------------------------------------------------------------
  template<class T>
  list_pod<T>& list_pod<T>::operator=(list_pod const & other)
  {
	  if (this == &other)
		  return *this;

	  //resize array
	  resize(other.m_arraySize);

	  //Assign m_data
	  list_pod<T>::const_iterator it = other.begin();
	  for (it; it != other.end(); ++it)
	  {
		  push_back(*it);
	  }

	  return *this;
  }	//End: list_pod::operator=()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T>::clear()
  //--------------------------------------------------------------------------------
  template<class T>
  void list_pod<T>::clear()
  {
	  //Reset next free
	  m_nextFree = &m_data[0];

	  //Set outer container pointers
    m_rootContainer.next = &m_endContainer;
    m_endContainer.previous = &m_rootContainer;

	  //close the last element in the list
	  m_data[m_arraySize-1].next = nullptr;

	  //Assign pointers
	  for (size_t i = 0; i < m_arraySize-1; i++)
	  {
		  m_data[i].next = &m_data[i+1];
	  }

	  m_currentSize = 0;

  }	//End: list_pod<T>::clear()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T>::resize()
  //--------------------------------------------------------------------------------
  template<class T>
  void list_pod<T>::resize(size_t a_newSize)
  {
	  //Size must be at least 1
    assert(a_newSize > 0);

	  //Initialise m_data
	  init(new_size);

  }	//End: list_pod<T>::resize()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T>::push_back()
  //--------------------------------------------------------------------------------
  template<class T>
  void list_pod<T>::push_back(T const & a_item)
  {
	  //Is the list full?
	  if (m_currentSize == m_arraySize)
		  extend();

	  //Get the list node to work on
	  DataContainer *new_element = m_nextFree;
	
	  //Move m_nextFree pointer to the next DataContainer
	  m_nextFree = m_nextFree->next;

	  //Assign the element
    memcpy(new_element->element, &a_item, sizeof(T));

	  //Add the current element to the back of the active list
	  m_endContainer.previous->next = new_element;
	  new_element->previous = m_endContainer.previous;
    new_element->next = &m_endContainer;
	  m_endContainer.previous = new_element;

	  //Increment m_currentSize
	  m_currentSize++;

  }	//End: list_pod::push_back()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T>::push_back()
  //--------------------------------------------------------------------------------
  template<class T>
  bool list_pod<T>::push_back()
  {
	  //Is the list full?
	  if (m_currentSize == m_arraySize)
		  return false;

	  //Get the list node to work on
	  DataContainer *new_element = m_nextFree;

	  //Move m_nextFree pointer to the next DataContainer
	  m_nextFree = m_nextFree->next;

	  //Add the current element to the back of the active list
	  m_endContainer.previous->next = new_element;
	  new_element->previous = m_endContainer.previous;
    new_element->next = &m_endContainer;
	  m_endContainer.previous = new_element;

	  //Increment m_currentSize
	  m_currentSize++;

	  return true;

  }	//End: list_pod::push_back()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T>::push_front()
  //--------------------------------------------------------------------------------
  template<class T>
  bool list_pod<T>::push_front()
  {
      //Is the list full?
      if (m_currentSize == m_arraySize)
          return false;

      //Get the list node to work on
      DataContainer *new_element = m_nextFree;

      //Move m_nextFree pointer to the next DataContainer
      m_nextFree = m_nextFree->next;

      //Add the current element to the back of the active list
      m_rootContainer.next->previous = new_element;
      new_element->previous = &m_rootContainer;
      new_element->next = m_rootContainer.next;
      m_rootContainer.next = new_element;

      //Increment m_currentSize
      m_currentSize++;

      return true;

  }	//End: list_pod::push_back()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T>::push_front()
  //--------------------------------------------------------------------------------
  template<class T>
  void list_pod<T>::push_front(T const & val)
  {
	  //Is the list full?
	  if (m_currentSize == m_arraySize)
		  extend();

	  //Get the list node to work on
	  DataContainer *new_element = m_nextFree;
	
	  //Move m_nextFree pointer to the next DataContainer
	  m_nextFree = m_nextFree->next;

	  //Assign the element
    memcpy(new_element->element, &a_item, sizeof(T));

	  //Add the current element to the back of the active list
	  m_rootContainer.next->previous = new_element;
	  new_element->previous = &m_rootContainer;
	  new_element->next = m_rootContainer.next;
	  m_rootContainer.next = new_element;

	  //Increment m_currentSize
	  m_currentSize++;

  }	//End: list_pod::push_front()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T>::pop_back()
  //--------------------------------------------------------------------------------
  template<class T>
  void list_pod<T>::pop_back()
  {
	  //Range check
	  assert(m_currentSize != 0);
	
	  //Get new last element
    DataContainer* last = m_endContainer.previous->previous;

	  //Assign next free
	  m_endContainer.previous->next = m_nextFree;
	  m_nextFree = m_endContainer.previous;

	  //Break element from chain
    last->next = &m_endContainer;			//prev points to next
	  m_endContainer.previous = last;	//next points to previous

	  //Deincrement m_currentSize
	  m_currentSize--;

  }	//End: list_pod::pop_back()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T>::pop_front()
  //--------------------------------------------------------------------------------
  template<class T>
  void list_pod<T>::pop_front()
  {
	  //Range check
	  assert(m_currentSize != 0);
	
	  //Get new first element
    DataContainer* first = m_rootContainer.next->next;

	  //Assign next free
	  m_rootContainer.next->next = m_nextFree;
	  m_nextFree = m_rootContainer.next;

	  //Break element from chain
    first->previous = &m_rootContainer;		//prev points to next
	  m_rootContainer.next = first;	//next points to previous

	  //Deincrement m_currentSize
	  m_currentSize--;

  }	//End: list_pod::pop_front()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T>::insert()
  //--------------------------------------------------------------------------------
  template<class T>
  void list_pod<T>::insert(iterator const & it, T const & a_item)
  {
    assert(it.ptr != &m_rootContainer);

	  //Is the list full?
	  if (m_currentSize == m_arraySize)
		  extend();
	
	  //Get the list node to work on
	  DataContainer *new_element = m_nextFree;
	
	  //Move m_nextFree pointer to the next DataContainer
	  m_nextFree = m_nextFree->next;

	  //Insert next free
	  it.ptr->previous->next = new_element;
	  new_element->previous = it.ptr->previous;
	  it.ptr->previous = new_element;
	  new_element->next = it.ptr;

	  //Set the element
    memcpy(new_element->element, &a_item, sizeof(T));

	  //Increment m_currentSize
	  m_currentSize++;

  }	//End: list_pod::insert()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T>::erase()
  //--------------------------------------------------------------------------------
  template<class T>
  void list_pod<T>::erase(iterator& it)
  {
	  //Remember previous element
	  DataContainer* next = it.ptr->next;

	  //Break element from chain
	  it.ptr->previous->next = it.ptr->next;		//prev points to next
	  it.ptr->next->previous = it.ptr->previous;	//next points to previous

	  //Add this broken item to the begining of the free list
	  it.ptr->next = m_nextFree;				//put item in between m_nextFree and end of the list
	
	  //Reset m_nextFree
	  m_nextFree = it.ptr;
	
	  //Deincrement m_currentSize
	  m_currentSize--;

	  //Return iterator to the next container
	  it = iterator(next);

  }	//End: list_pod::erase()


  //--------------------------------------------------------------------------------
  //	@	list_pod<T>::extend()
  //--------------------------------------------------------------------------------
  template<class T>
  void list_pod<T>::extend()
  {
	  //Calculate new size
	  size_t new_size = m_arraySize << 1;
	
    if (new_size < m_arraySize)
    {
      throw std::overflow_error("m_arraySize");
    }

	  //Create new array
	  DataContainer* new_data = static_cast<DataContainer *>(malloc(new_size * sizeof(DataContainer)));

    if (new_data == nullptr)
    {
      throw std::bad_alloc;
    }

	  //Assign pointers
	  for (size_t i = 0; i < new_size-1; i++)
	  {
		  new_data[i].next = &new_data[i+1];
	  }
	  for (size_t i = 1; i < m_currentSize; i++)
	  {
		  new_data[i].previous = &new_data[i-1];
	  }

	  //Assign values
	  iterator it = begin();
	  for (size_t i = 0; it != end(); ++it, ++i)
	  {
		  new_data[i].element = *it;
	  }

	  //Assign m_data pointer
	  free(m_data);
	  m_data = new_data;
	
	  //Assign next free pointer
	  m_nextFree = &new_data[m_currentSize];
	
	  //Adjust sizes
	  m_arraySize = new_size;

	  //Determine root and end pointers
	  if (m_currentSize == 0)
	  {
      m_rootContainer.next = &m_endContainer;
      m_endContainer.previous = &m_rootContainer;
	  }
	  else
	  {
		  m_rootContainer.next = &m_data[0];
		  m_endContainer.previous = &m_data[m_currentSize-1];
      new_data[0].previous = &m_rootContainer;
      new_data[m_currentSize - 1].next = &m_endContainer;
	  }
	
  }	//End: list_pod<T>::extend()


  //--------------------------------------------------------------------------------
  //		Helpful functions
  //--------------------------------------------------------------------------------

  //! @ingroup Containers_functions
  //!
  //! Find a value in the list, returns iterator.
  //! 
  //! @param[in] first Iterator to start searching from
  //! @param[in] last Search will stop when this iterator is found
  //! @param[in] val Object to compare
  //!
  //! @return Iterator pointing to the val in the list_pod. If no item was found,
  //!         will return last.
  template<class T>
  typename list_pod<T>::iterator find (
	  typename list_pod<T>::iterator first, 
	  typename list_pod<T>::iterator last, 
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


  //! @ingroup Containers_functions
  //!
  //! Find a value in the list, returns const_iterator.
  //! 
  //! @param[in] first Iterator to start searching from
  //! @param[in] last Search will stop when this iterator is found
  //! @param[in] val Object to compare
  //!
  //! @return Iterator pointing to the val in the list_pod. If no item was found,
  //!         will return last.
  template<class T>
  typename list_pod<T>::const_iterator find (
	  typename list_pod<T>::const_iterator first, 
	  typename list_pod<T>::const_iterator last, 
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