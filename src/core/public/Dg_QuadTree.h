

#ifndef DG_QUADTREE_H
#define DG_QUADTREE_H

#include <stdint.h>
#include <unordered_map>
#include <vector>

#include "impl\DgContainerBase.h"

namespace Dg
{
  template<typename I, typename DataType, int MaxDepth = (sizeof(I) * CHAR_BIT / 2)>
  class Quadtree : public ContainerBase
  {
    enum NodeFlags
    {
      IsLeaf = 0,
      HasData,
      TOTAL_FLAGS
    };

  public:

    template<typename Real>
    DataType * QueryPoint(Real x, Real y);

    template<typename Real>
    DataType const * QueryPoint(Real x, Real y) const;



  private:

    //0: is leaf node
    //1: has data. Data lookup will be a hash of this node id.
    std::unordered_map<I, uint8_t>    m_nodeFlags;
    std::unordered_map<I, DataType>   m_data;
  };
}

#endif
