#ifndef BSPTREE_H
#define BSPTREE_H

#include <stdint.h>
#include <vector>
#include <map>

#include "Dg_map.h"
#include "DgR2Vector.h"
#include "DgR2Polygon.h"

namespace Dg
{
  namespace GT
  {
    template<typename Real, typename Key>
    class BSPTree
    {
      struct Node
      {
        enum
        {
          //Values
          NoData = -1,

          //Flags
          isLeaf = 1
        };

        static uint32_t const s_noData = -1;

        // The index of the relavent element in the point (geometry) to be tested against.
        // vertical split: 0 (x)
        // horizontal split: 1 (y)
        int           m_element;
        Real          m_offset;

        size_t        m_dataOffset;
        uint32_t      m_nDataElemets;

        uint32_t      m_flags;
      };

      struct KV
      {
        Key key;
        Dg::R2::Polygon<Real> polygon;
      };

    public:

      

    public:
      BSPTree();

      Dg::ErrorCode Build(PolyMap const &);

      std::vector<Key> Query(Dg::R2::Vector<Real> const &) const;
      std::vector<Key> Query(Dg::R2::Disk<Real> const &) const;
      std::vector<Key> Query(Dg::R2::Capsule<Real> const &) const;

    private:

      std::vector<Node>         m_nodes;
      std::vector<KV>           m_polygons;
      std::vector<size_t>       m_leafData;
    };
  }
}

#endif // QRTREE_H
