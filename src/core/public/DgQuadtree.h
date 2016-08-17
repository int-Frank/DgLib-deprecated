#ifndef DGQUADTREE_H
#define DGQUADTREE_H

#include <cstdint>
#include <limits>

#include "DgVector4.h"
#include "Dg_vector_pod.h"
#include "Dg_shared_ptr.h"
#include "DgIDManager.h"

namespace Dg
{
  template<typename Real, typename I>
  class Quadtree
  {
    static_assert(std::numeric_limits<I>::is_integer 
      & std::numeric_limits<I>::is_signed
      , "Quadtree ref type must be a signed integer");

  public:

    class Node
    {
    public:

      Node()
        : m_pChild{ -1, -1, -1, -1 }
        , m_pObjects(nullptr)
      {}

      Node(Vector4<Real> const & a_center, Real a_hw)
        : a_center(a_center)
        , m_halfWidth(a_hw)
        , m_pChild{ -1, -1, -1, -1 }
        , m_pObjects(nullptr)
      {}

      Node(Node const & a_other)
        : a_center(a_other.m_center)
        , m_halfWidth(a_other.m_halfWidth)
        , m_pChild{ a_other.m_pChild[0]
        , a_other.m_pChild[1]
        , a_other.m_pChild[2]
        , a_other.m_pChild[3] }
        , m_pObjects(m_pObjects)
      {}

      Node & operator=(Node const & a_other)
      {
        if (this != &a_other)
        {
          m_center = a_other.m_center;
          m_halfWidth = a_other.m_halfWidth;
          memcpy(m_Child, a_other.m_Child, 4 * sizeof(int));
          m_pObjects = a_other.m_pObjects;
        }
        return *this;
      }

      Vector4<Real>     m_center;
      Real              m_halfWidth;
      I                 m_Child[4];
    };

  public:

    Quadtree()
      : m_nodeIndexHandler(0, std::numeric_limits<I>::max())
    {}

  private:

    IDManager           m_nodeIndexHandler;
    vector_pod<Node>    m_nodes;
  };
}

#endif