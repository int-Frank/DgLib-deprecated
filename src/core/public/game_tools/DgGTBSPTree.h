#ifndef BSPTREE_H
#define BSPTREE_H

#include <stdint.h>
#include <vector>
#include <map>

#include "Dg_map.h"
#include "DgR2Vector.h"
#include "DgR2Segment.h"
#include "DgR2Disk.h"

namespace Dg
{
  namespace GT
  {
    template<typename Real, typename Key>
    class BSPTree
    {
    public:

      typedef Dg::R2::Vector<Real>  Vector;
      typedef Dg::R2::Segment<Real> Segment;
      typedef Dg::R2::Disk<Real>    Disk;

    private:

      struct Node
      {
        enum
        {
          //Flags
          isLeaf = 1
        };

        uint32_t      m_flags;
        union
        {
          struct Branch
          {
            int           m_element; //index of x or y element (0, 1)
            Real          m_offset;
            uint32_t      m_child_LESS;
            uint32_t      m_child_GREATER;
          } branch;

          struct Leaf
          {
            uint32_t      m_dataOffset;
            uint32_t      m_nEdges;
            uint32_t      m_nPolygons;
          } leaf;
        };
      };

      struct Polygon
      {
        Key      key;
        uint32_t offset;
        uint32_t nEdges;
      };

      struct Edge
      {
        //Elements 0 and 1 will be the same for boundary edges 
        uint32_t     adjacentPolygonOffsets[2];
        Segment      edge;
      };

    public:
      BSPTree();

      Dg::ErrorCode Build(PolyMap const &);

      std::vector<Key> Query(Vector const &) const;
      std::vector<Key> Query(Disk const &) const;
      std::vector<Key> Query(Capsule const &) const;

    private:

      uint32_t m_nNodes;
      uint32_t m_nEdges;
      uint32_t m_nPolygons;
      uint32_t m_ndataItens;

      Node *         m_pNodes;
      Edge *         m_pEdges;
      Polygon *      m_polygons;

      // {Leaf data, Polygon data} where:
      // Leaf data: {edgeIndex0, edgeIndex1, ..., edgeIndexN, polygonIndex0, polygonIndex1, ... }
      // Polygon data: {edgeIndex0, edgeIndex1, ... }
      uint32_t *       m_data; 
    };
  }
}

#endif // QRTREE_H
