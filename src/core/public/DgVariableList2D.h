//! @file Dg_vector.h
//!
//! @author Frank Hart
//! @date 22/07/2016
//!
//! Class header: VariableList2D2D<>

#ifndef DGVARIABLELIST2D_H
#define DGVARIABLELIST2D_H

#include <sstream>

#include "DgDynamicArray.h"

#ifdef STD_COMPATIBLE
#include <iterator>
#define BASE_ITERATOR : public std::iterator<std::bidirectional_iterator_tag, T>
#else
#define BASE_ITERATOR
#endif

namespace Dg
{
  //! @ingroup DgContainers
  //!
  //! @class VariableList2D
  //!
  //! @author Frank Hart
  //! @date 7/01/2014
  template<class T>
  class VariableList2D : public ContainerBase
  {
    //! Container to hold the object and pointers
    class Node
    {
    public:

      //! Default constructor
      Node(): pNext(nullptr), pPrev(nullptr) {}

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

      //! Pre increment
      iterator& operator++()
      {
        m_pNode = m_pNode->Next();
        return *this;
      }

      //! Post increment
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
    //Constructor / destructor
    VariableList2D();

    //! Construct with a set size
    ~VariableList2D();

    //! Copy constructor
    VariableList2D(VariableList2D const &);

    //! Assignment
    VariableList2D& operator= (VariableList2D const &);

    //! Move constructor
    VariableList2D(VariableList2D &&);

    //! Move assignment
    VariableList2D& operator= (VariableList2D &&);

    iterator begin(size_t);
    iterator end(size_t);
    const_iterator cbegin(size_t) const;
    const_iterator cend(size_t) const;

    size_t size(size_t) const;
    size_t NumberBuckets() const;

    T & back(size_t);
    T & front(size_t);

    T const & back(size_t) const;
    T const & front(size_t) const;

    void push_back(size_t, T const &);
    void push_front(size_t, T const &);

    iterator insert(size_t, iterator const &, T const &);

    void pop_back(size_t);
    void pop_front(size_t);

    iterator erase(iterator const &);

    void clear();

  private:

    void Extend();

    void rangeCheck(size_t) const;

    //Data members
    struct Index
    {
      T * pBegin;
    };

    DynamicArray<T>     m_data;
    DynamicArray<Index> m_indices;
  };


  //--------------------------------------------------------------------------------
  //	@	VariableList2D<T>::VariableList2D()
  //--------------------------------------------------------------------------------
  template<class T>
  VariableList2D<T>::VariableList2D() 
  {

  }	//End: VariableList2D::VariableList2D()


    //--------------------------------------------------------------------------------
    //	@	VariableList2D<T>::~VariableList2D()
    //--------------------------------------------------------------------------------
  template<class T>
  VariableList2D<T>::~VariableList2D()
  {

  }	//End: VariableList2D::~VariableList2D()


    //--------------------------------------------------------------------------------
    //	@	VariableList2D<T>::VariableList2D()
    //--------------------------------------------------------------------------------
  template<class T>
  VariableList2D<T>::VariableList2D(VariableList2D<T> const & a_other)
    : m_data(a_other.m_data)
    , m_indices(a_other.m_indices)
  {

  }	//End: VariableList2D::VariableList2D()


    //--------------------------------------------------------------------------------
    //	@	VariableList2D<T>::VariableList2D()
    //--------------------------------------------------------------------------------
  template<class T>
  VariableList2D<T>::VariableList2D(VariableList2D<T> && a_other) 
    : m_data(std::move(a_other.m_data))
    , m_indices(std::move(a_other.indices))
  {

  }	//End: VariableList2D::VariableList2D()


    //--------------------------------------------------------------------------------
    //	@	VariableList2D<T>::VariableList2D()
    //--------------------------------------------------------------------------------
  template<class T>
  VariableList2D<T> & VariableList2D<T>::operator=(VariableList2D<T> && a_other)
  {
    if (this != &a_other)
    {
      m_data = std::move(a_other.m_data);
      m_indices = std::move(a_other.m_indices);
    }
    return *this;
  }	//End: VariableList2D::VariableList2D()


    //--------------------------------------------------------------------------------
    //	@	VariableList2D<T>::operator=()
    //--------------------------------------------------------------------------------
  template<class T>
  VariableList2D<T>& VariableList2D<T>::operator=(VariableList2D const & a_other)
  {
    if (this != &a_other)
    {
      m_data = a_other.m_data;
      m_indices = a_other.m_indices;
    }
    return *this;
  }	//End: VariableList2D::operator=()


    //--------------------------------------------------------------------------------
    //	@	VariableList2D<T>::push_back()
    //--------------------------------------------------------------------------------
  template<class T>
  void VariableList2D<T>::push_back(T const * a_pItems, size_t a_count)
  {
    Index ind;
    ind.start = m_data.size();
    ind.count = a_count;

    m_indices.push_back(ind);

    for (size_t i = 0; i < a_count; i++)
    {
      m_data.push_back(a_pItems[i]);
    }
  }	//End: VariableList2D<T>::push_back()


    //--------------------------------------------------------------------------------
    //	@	VariableList2D<T>::clear()
    //--------------------------------------------------------------------------------
  template<class T>
  void VariableList2D<T>::clear()
  {
    m_data.clear();
    m_indices.clear();

  }	//End: VariableList2D::clear()


  template<class T>
  T & VariableList2D<T>::operator()(size_t a_row, size_t a_element)
  {
    Index ind = m_indices[a_row];
    return m_data[ind.start + a_element];
  }


  template<class T>
  T const & VariableList2D<T>::operator()(size_t a_row, size_t a_element) const
  {
    Index ind = m_indices[a_row];
    return m_data[ind.start + a_element];
  }


  template<class T>
  T & VariableList2D<T>::at(size_t a_row, size_t a_element)
  {
    rangeCheck(a_row, a_element);
    Index ind = m_indices[a_row];
    return m_data[ind.start + a_element];
  }


  template<class T>
  T const & VariableList2D<T>::at(size_t a_row, size_t a_element) const
  {
    rangeCheck(a_row, a_element);
    Index ind = m_indices[a_row];
    return m_data[ind.start + a_element];
  }
}
#endif