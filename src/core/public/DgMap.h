//! @file DgMap.h
//!
//! @author: Frank B. Hart
//! @date 2/5/2015
//!
//! Class declaration: map

#ifndef DGMAP_H
#define DGMAP_H

#include <stdint.h>

#include "impl/DgContainerBase.h"
#include "DgErrorHandler.h"

namespace Dg
{
  template<typename Key, typename T>
  class Map : public ContainerBase
  {
    class Node
    {
      static uint32_t const s_none = 0xFFFFFFFF;
    public:

    private:
      Key       m_key;
      T         m_data;
      //Leaf when both children are -1
      union
      {
        struct Active
        {
          uint32_t   m_indexChildLess;    //s_none denotes no child
          uint32_t   m_indexChildGreater; //s_none denotes no child
          uint32_t   m_indexParent;       //s_none denotes the root node
        } active;

        struct Inactive
        {
          uint32_t   m_indexNext; //s_none denotes end of the free list
        } inactive;
      };
    };

  public:

    class const_iterator
    {
    public:

    private:
      Node const * m_pNodes;
      int32_t      m_nodeIndex;
    };

    class iterator
    {
    public:

    private:
      Node * m_pNodes;
      int32_t m_nodeIndex;
    };

  public:

    Map() {}
    ~Map() {}

    Map(Map const & a_other)
    {

    }

    Map & operator=(Map const & a_other)
    {

    }

    T & operator[](Key const & a_key)
    {

    }

    T const & operator[](Key const & a_key) const
    {

    }

    iterator begin()
    {

    }

    iterator end()
    {

    }

    const_iterator cbegin() const
    {

    }

    const_iterator cend() const
    {

    }

    uint32_t Size() const
    {

    }

    bool Empty() const
    {

    }

    bool Exists(Key const & a_key) const
    {

    }

    T const * At(Key const & a_key) const
    {

    }

    T * At(Key const & a_key)
    {

    }

    void Insert(Key const & a_key, T const & a_data)
    {

    }

    void Erase(Key const & a_key)
    {

    }

    void Clear()
    {

    }

  private:

    Node *     m_pNodes;
    uint32_t   m_root;
    uint32_t   m_nextFree;
  };
};

#endif