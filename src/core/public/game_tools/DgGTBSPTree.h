#ifndef BSPTREE_H
#define BSPTREE_H

#include <stdint.h>
#include <vector>
#include <map>

#include "Dg_map.h"
#include "DgR2Vector.h"
#include "DgR2Segment.h"
#include "DgR2Disk.h"
#include "DgR2Polygon.h"

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
      typedef Dg::R2::Polygon<Real> Polygon;

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
            uint32_t      m_element; //index of x or y element (0, 1)
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

      BSPTree()
        : m_nNodes(0)
        , m_nEdges(0)
        , m_nPolygons(0)
        , m_nDataItems(0)
        , m_pNodes(nullptr)
        , m_pEdges(nullptr)
        , m_pPolygons(nullptr)
        , m_pData(nullptr)
      {}

      BSPTree(BSPTree const & a_other)
      {
        Init(a_other);
      }

      BSPTree & operator=(BSPTree const & a_other)
      {
        if (this != &a_other)
        {
          Init(a_other);
        }
        return *this;
      }

      Dg::ErrorCode Build(std::map<Key, Polygon> const &);

      void Clear()
      {
        delete[] m_pNodes;
        delete[] m_pEdges;
        delete[] m_pPolygons;
        delete[] m_pData;

        m_pNodes = nullptr;
        m_pEdges = nullptr;
        m_pPolygons = nullptr;
        m_pData = nullptr;

        m_nNodes = 0;
        m_nEdges = 0;
        m_nPolygons = 0;
        m_nDataItems = 0;
      }

      std::vector<Key> Query(Vector const &) const;
      std::vector<Key> Query(Disk const &) const;
      //std::vector<Key> Query(Capsule const &) const;

    private:

      void Init(BSPTree const & a_other)
      {
        Clear();

        m_nNodes = a_other.m_nNodes;
        m_nEdges = a_other.m_nEdges;
        m_nPolygons = a_other.m_nPolygons;
        m_nDataItems = a_other.m_nDataItems;

        m_pNodes = new Node[m_nNodes];
        m_pEdges = new Edge[m_nEdges];
        m_pPolygons = new Polygon[m_nPolygons];
        m_pData = new uint32_t[m_nDataItems];

        for (uint32_t i = 0; i < m_nNodes; ++i) m_pNodes[i] = a_other.m_pNodes[i];
        for (uint32_t i = 0; i < m_nEdges; ++i) m_pEdges[i] = a_other.m_pEdges[i];
        for (uint32_t i = 0; i < m_nPolygons; ++i) m_pPolygons[i] = a_other.m_pPolygons[i];
        for (uint32_t i = 0; i < m_nDataItems; ++i) m_pData[i] = a_other.m_pData[i];
      }

    private:

      uint32_t m_nNodes;
      uint32_t m_nEdges;
      uint32_t m_nPolygons;
      uint32_t m_nDataItems;

      Node *         m_pNodes;
      Edge *         m_pEdges;
      Polygon *      m_pPolygons;

      // {Leaf data, Polygon data} where:
      // Leaf data: {edgeIndex0, edgeIndex1, ..., edgeIndexN, polygonIndex0, polygonIndex1, ... }
      // Polygon data: {edgeIndex0, edgeIndex1, ... }
      uint32_t *       m_pData; 
    };
  }
}

#endif // QRTREE_H
