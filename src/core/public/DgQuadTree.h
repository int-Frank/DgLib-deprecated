

#ifndef DG_QUADTREE_H
#define DG_QUADTREE_H

#include <stdint.h>
#include <limits>

#include "DgTypes.h"
#include "DgOpenHashTable.h"
#include "DgR2AABB.h"

namespace Dg
{
  namespace impl
  {
    namespace QuadTree
    {
      enum Flag
      {
        //Node...
        IsLeaf        = 1
      };

      template<typename I>
      bool HasFlag(I a_flags, Flag a_flag)
      {
        return (a_flags & a_flag != 0);
      }


      template<typename I>
      class QuadSet
      {
      public:

        I & operator[](int i)
        {
          return m_quad[i];
        }

       I BottomLeft()  const { return m_quad[0]; }
       I BottomRight() const { return m_quad[1]; }
       I TopLeft()     const { return m_quad[2]; }
       I TopRight()    const { return m_quad[3]; }

      private:
        I m_quad[4];
      };

      template<typename Real, typename I>
      class Node
      {
      public:

        Node(R2::AABB<Real> const & a_bounds, I a_flags)
          : bounds(a_bounds)
          , flags(a_flags)
        {}

        R2::AABB<Real>   bounds;
        I                flags;
      };
    }
  }
  /*
                              |
                         2    |   3
                              |
                      -----------------
                              |
                         0    |   1
                              |                
              ^
            y |
              |----> 
                 x
  
  */
  template<typename Real, typename Handle, typename DataType, int MaxDepth = (sizeof(typename Handle) * CHAR_BIT / 2 - 1)>
  class QuadTree
  {
    static_assert(std::numeric_limits<Handle>::is_integer, "Key must be integer type.");
    static_assert( MaxDepth > 0 && MaxDepth <= (sizeof(Handle) * CHAR_BIT / 2 - 1), "Invalid depth.");

    typedef uint8_t NodeFlagType;

    static Handle const s_root;

  public:

    typedef Handle                                    Handle;
    typedef impl::QuadTree::QuadSet<Handle>           QuadSet;
    typedef R2::AABB<Real>                            AABB;
    typedef impl::QuadTree::Node<Real, NodeFlagType>  Node;

  public:

    QuadTree(AABB const & a_bounds)
    {
      m_nodes.insert(s_root, Node(a_bounds, impl::QuadTree::IsLeaf));
    }

    ErrorCode QueryPoint(Real x, Real y, Handle & a_out) const
    {
      return __QueryPoint(x, y, s_root, a_out);
    }

    static Handle GetParent(Handle a_h)
    {
      return (a_h >> 2);
    }

    static QuadSet GetChildren(Handle a_h)
    {
      QuadSet children;
      children[0] = (a_h << 2);
      children[1] = (a_h << 2) | 1;
      children[2] = (a_h << 2) | 2;
      children[3] = (a_h << 2) | 3;
      return children;
    }

    DataType * Data(Handle a_h)
    {
      return m_data.at(a_h);
    }

    DataType const * Data(Handle a_h) const
    {
      return m_data.at(a_h);
    }

    bool Subdivide(Handle a_h)
    {
      Node * pNode(m_nodes.at(a_h));
      if (!(pNode->flags & impl::QuadTree::IsLeaf) || (a_h >> (MaxDepth * 2)))
      {
        return false;
      }

      pNode->flags &= ~impl::QuadTree::IsLeaf;

      AABB parentBounds(pNode->bounds);

      Real hx(static_cast<Real>(0));
      Real hy(static_cast<Real>(0));
      R2::Vector<Real> c(parentBounds.GetCenter());
      Real hl[2];
      parentBounds.GetHalfLengths(hl);
      hl[0] /= static_cast<Real>(2);
      hl[1] /= static_cast<Real>(2);

      R2::Vector<Real> c0(c.x() - hl[0], c.y() - hl[1], static_cast<Real>(1));
      R2::Vector<Real> c1(c.x() + hl[0], c.y() - hl[1], static_cast<Real>(1));
      R2::Vector<Real> c2(c.x() - hl[0], c.y() + hl[1], static_cast<Real>(1));
      R2::Vector<Real> c3(c.x() + hl[0], c.y() + hl[1], static_cast<Real>(1));

      QuadSet qs(GetChildren(a_h));

      m_nodes.insert(qs[0], Node(AABB(c0, hl), impl::QuadTree::IsLeaf));
      m_nodes.insert(qs[1], Node(AABB(c1, hl), impl::QuadTree::IsLeaf));
      m_nodes.insert(qs[2], Node(AABB(c2, hl), impl::QuadTree::IsLeaf));
      m_nodes.insert(qs[3], Node(AABB(c3, hl), impl::QuadTree::IsLeaf));

      return true;
    }

    AABB GetAABB(Handle a_h) const
    {
      return m_nodes.at(a_h)->bounds;
    }

    bool IsLeaf(Handle a_h) const
    {
      return m_nodes.at(a_h)->flags & impl::QuadTree::IsLeaf;
    }

    bool HasData(Handle a_h) const
    {
      return m_data.at(a_h) != nullptr;
    }

    bool IsRoot(Handle a_h) const
    {
      return a_h == 1;
    }

    Handle Root() const
    {
      return s_root;
    }

    size_t Size() const
    {
      return m_nodes.size();
    }

    void Reset(AABB const & a_bounds)
    {
      m_nodes.clear();
      m_data.clear();
      m_nodes.insert(s_root, Node(a_bounds, (impl::QuadTree::IsLeaf)));
    }

  private:

    Dg::ErrorCode __QueryPoint(Real a_x, Real a_y, 
                               Handle a_handle,
                               Handle & a_out) const
    {
      Node const * pNode(m_nodes.at(a_handle));

      if (pNode->flags & impl::QuadTree::IsLeaf)
      {
        a_out = a_handle;
        return Dg::ErrorCode::None;
      }

      Handle child(0);
      R2::Vector<Real> center(pNode->bounds.GetCenter());

      if (a_x > center.x()) child |= 1;
      if (a_y > center.y()) child |= 2;

      child = (a_handle << 2) | child;

      return __QueryPoint(a_x, a_y, child, a_out);
    }

  private:

    OpenHashTable<Handle, Node>            m_nodes;
    OpenHashTable<Handle, DataType>        m_data;
  };

  template<typename Real, typename Handle, typename DataType, int MaxDepth>
  Handle const QuadTree<Real, Handle, DataType, MaxDepth>::s_root = 1;
}

#endif
