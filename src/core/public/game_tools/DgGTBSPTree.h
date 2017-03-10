#ifndef BSPTREE_H
#define BSPTREE_H

#include <stdint.h>
#include <vector>
#include <map>

#include "Dg_map.h"
#include "DgR2Vector.h"
#include "DgR2Segment.h"
#include "DgR2Line.h"
#include "DgR2Disk.h"
#include "DgR2Polygon.h"
#include "DgR2Regression.h"

namespace Dg
{
  namespace GT
  {
    template<typename Real, typename Key>
    class BSPTree
    {
    public:

      typedef Dg::R2::Vector<Real>  DgVector;
      typedef Dg::R2::Segment<Real> DgSegment;
      typedef Dg::R2::Line<Real>    DgLine;
      typedef Dg::R2::Disk<Real>    DgDisk;
      typedef Dg::R2::Polygon<Real> DgPolygon;

    private:

      struct Node
      {
        enum
        {
          //Flags
          Leaf = 1
        };

        uint32_t      flags;
        union
        {
          struct Branch
          {
            uint32_t      element; //index of x or y element (0, 1)
            Real          offset;
            uint32_t      child_BELOW;
            uint32_t      child_ABOVE;
          } branch;

          struct Leaf
          {
            uint32_t      dataOffset;
            uint32_t      nPolygons;
          } leaf;
        };
      };

      struct Polygon
      {
        Key      key;
        uint32_t offset;
        uint32_t nEdges;
      };

      class Edge
      {
        static uint32_t const NONE = 0xFFFFFFFF;

        Edge()
          : adjacentPolygonOffsets{ NONE, NONE }
        {}

        //Elements 0 and 1 will be the same for boundary edges 
        uint32_t     adjacentPolygonOffsets[2];
        DgSegment    edge;
      };

      struct SplitData
      {
        std::vector<uint32_t> belowPolys;
        std::vector<uint32_t> abovePolys;
        uint32_t              element;
        Real                  offset;
      };

    public:

      class Criteria
      {
      public:

      };

      struct DataInput
      {
        std::vector<Vector>                 points;
        std::map<Key, std::vector<size_t>>  polygons;
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

      Dg::ErrorCode Build(DataInput const & a_input, Criteria a_criteria = Criteria())
      {
        Clear();
        LoadPolygons(a_input);

        std::vector<Node>     nodes;
        std::vector<uint32_t> edgeIndices;
        std::vector<uint32_t> polygonIndices;

        //Our first node will contain every polygon and edge.
        for (uint32_t i = 0; i < m_nEdges; ++i)     edgeIndices.push_back(i);
        for (uint32_t i = 0; i < m_nPolygons; ++i)  polygonIndices.push_back(i);

        Split(edgeIndices, polygonIndices, nodes, a_criteria);
        AssignTree(polygonIndices, nodes);
      }

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

      std::vector<Key> Query(DgVector const &) const;
      std::vector<Key> Query(DgDisk const &) const;
      //std::vector<Key> Query(Capsule const &) const;

    private:

      void LoadPolygons(DataInput const & a_input)
      {
        delete[] m_pPolygons;
        delete[] m_pEdges;
        m_pPolygons = nullptr;
        m_pEdges = nullptr;

        std::vector<DgSegment>  edges;
        std::vector<Polygon>    polygons;

        for (auto const & kv : a_input.polygons)
        {
          DgPolygon dgpolygon;
          for (auto const & index : kv.second)
          {
            dgpolygon.push_back(a_input.points[index]);
          }

          //Make sure all our polygons are CCW
          if (dgpolygon.Orientation() != Dg::Orientation::CCW)
          {
            dgpolygon.reverse();
          }

          Polygon polygon;
          polygon.key = kv.first;
          polygon.offset = edges.size();
          polygon.nEdges = dgpolygon.size();
          polygons.push_back(polygon);

          DgPolygon::const_iterator it = dgpolygon.chead();
          DgPolygon::const_iterator it_next = ++dgpolygon.chead();
          for (size_t i = 0; i < dgpolygon.size(); ++i, ++it, ++it_next)
          {
            Edge e;
            GetAdjacentPolygons(e, a_input, *it, *it_next);
            e.segment = Segment(a_input.points[*it], a_input.points[*it_next]);
            edges.push_back(e);
          }
        }

        m_pEdges = new Edge[edges.size()];
        for (size_t i = 0; i < edges.size(); ++i) m_pEdges[i] = edges[i];

        m_pPolygons = new Polygon[polygons.size()];
        for (size_t i = 0; i < polygons.size(); ++i) m_pPolygons[i] = polygons[i];
      }
      
      bool GetAdjacentPolygons(Edge & a_out, DataInput const & a_dataIn, uint32_t a_i0, uint32_t a_i1)
      {
        int ind = 0;
        size_t polyInd = 0;
        for (auto const & kv : a_input.polygons)
        {
          for (size_t i = 0; i < kv.second.size(); ++i)
          {
            i_next = (i + 1) % kv.second.size();
            if ((kv.second[i] == a_i0 && kv.second[i_next] == a_i1) ||
                (kv.second[i] == a_i1 && kv.second[i_next] == a_i0))
            {
              a_out.adjacentPolygons[ind] = polyInd;
              ind++;
              if (ind == 2)
              {
                goto endLoop;
              }
              break;
            }
          }
          polyInd++;
        }
        endLoop:
        return ind > 0;
      }

      SplitData PartitionPolygons(std::vector<uint32_t> a_parentPolygons)
      {
        std::vector<DgVector> centroids;
        for (auto p : a_parentPolygons)
        {
          DgPolygon polygon(GetPolygon(p));
          centroids.push_back(polygon.Centroid());
        }

        SplitData data;

        DgLine line = Dg::R2::LineOfBestFit(centroids.data(), centroids.size());
        DgVector slope = line.Direction();
        data.element = (abs(slope[0]) > abs(slope[1])) ? 0 : 1;
        data.offset = line.Origin()[a_out.element];

        for (auto p : a_parentPolygons)
        {
          bool isBelow = false;
          bool isAbove = false;

          for (uint32_t i = m_pPolygons[p].offset; 
               i < m_pPolygons[p].offset + m_pPolygons[p].nEdges;
               ++i)
          {
            if (m_pEdges[i].GetP0()[data.element] < data.offset)
            {
              isBelow = true;
            }
            else if (m_pEdges[i].GetP0()[data.element] > data.offset)
            {
              isAbove = true;
            }
          }

          if (isBelow)
          {
            data.belowPolys.push_back(p);
          }
          if (isAbove)
          {
            data.abovePolys.push_back(p);
          }
        }

        return data;
      }

      void Split(std::vector<uint32_t> const & a_parentPolygons,
                 std::vector<uint32_t> & a_polyList,
                 uint32_t a_parentIndex,
                 std::vector<Node> & a_output,
                 Criteria const & a_criteria)
      {
        SplitData sdata = PartitionPolygons(a_parentPolygons);

        bool validPartition = true;
        validPartition &= (sdata.belowPolys.size() < a_parentPolygons.size());
        validPartition &= (sdata.abovePolys.size() < a_parentPolygons.size());
        //...other criteria here...

        if (validPartition)
        {
          a_output[a_parentIndex].flags = 0;
          a_output[a_parentIndex].branch.element = sdata.element;
          a_output[a_parentIndex].branch.offset = sdata.offset;

          uint32_t childBelowInd = a_output.size();
          a_output[a_parentIndex].branch.child_BELOW = childBelowInd;
          a_output.push_back(Node());
          Split(sdata.belowPolys, childBelowInd, a_output, a_criteria);

          uint32_t childAboveInd = a_output.size();
          a_output[a_parentIndex].branch.child_ABOVE = a_output.size();
          a_output.push_back(Node());
          Split(sdata.abovePolys, childAboveInd, a_output, a_criteria);
        }
        else
        {
          a_output[a_parentIndex].flags = Node::Leaf;
          a_output[a_parentIndex].leaf.dataOffset = a_polylist.size();
          a_output[a_parentIndex].leaf.nPolygons = a_parentPolygons.size();

          a_polyList.insert(a_polyList.end(), a_parentPolygons.begin(), a_parentPolygons.end());
        }
      }
      
      void AssignTree(std::vector<uint32_t> const & a_polygonIndices, 
                      std::vector<Node> const & a_nodes )
      {
        delete[] m_pNodes;
        m_pNodes = new Node[a_nodes.size()];
        for (size_t i = 0; i < a_nodes.size(); ++i) m_pNodes[i] = a_nodes[i];

        delete[] m_pData;
        m_pData = new uint32_t[a_polygonIndices.size()];
        for (size_t i = 0; i < a_polygonIndices.size(); ++i) m_pData[i] = a_polygonIndices[i];
      }

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
      // Leaf data: {polygonIndex0, polygonIndex1, ... }
      uint32_t *       m_pData; 
    };
  }
}

#endif // QRTREE_H
