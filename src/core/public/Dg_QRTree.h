
#ifndef QRTREE_H
#define QRTREE_H

#include <stdint.h>
#include "DgTypes.h"
#include "DgR2AABB.h"

namespace Dg
{
  namespace impl
  {
    namespace QRTree
    {
      template<typename Real>
      struct Node
      {
        union
        {
          struct Items
          {
            bool isLeaf   : 1;
            bool hasData  : 1;
          } items;
          uint32_t all;
        } m_flags;


        Dg::R2::AABB<Real>      m_aabb;

        uint32_t                m_bottomLeft;
        uint32_t                m_bottomRight;
        uint32_t                m_topLeft;
        uint32_t                m_topRight;

        uint32_t                m_data;
      };
    }
  }

  //Optimized for static data.
  template
    <
      typename Real, 
      typename I, 
      typename DataType, 
      uint16_t MaxDepth
    >
  class QRTree
  {
    typedef impl::QRTree::Node Node;

  public:

    Dg::ErrorCode Build();

  private:

    size_t m_nNodes;
    Node * m_pNodes;
  };
}

#endif