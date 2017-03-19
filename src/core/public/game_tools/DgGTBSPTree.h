#ifndef DGGTBSPTREE_H
#define DGGTBSPTREE_H

#include <stdint.h>
#include <vector>
#include <map>
#include <limits>

#include "DgGTBSPTreePath.h"
#include "Dg_map.h"
#include "DgR2Vector.h"
#include "DgR2Segment.h"
#include "DgR2Line.h"
#include "DgR2Disk.h"
#include "DgR2Polygon.h"
#include "DgR2Regression.h"

//TODO Once subdividing returns the same polygon set, maybe subdivide like a 
//     by just cutting the region in half.

namespace Dg
{
  namespace GT
  {
    //Key must have a constructor such that Key(-1) constructs a key
    //flagged as invalid. For integer types this is trivial.
    template<typename Real, typename Key>
    class BSPTree
    {
    public:

      typedef std::vector<Key>      KeyList;
      typedef Dg::R2::Vector<Real>  DgVector;
      typedef Dg::R2::Segment<Real> DgSegment;
      typedef Dg::R2::Line<Real>    DgLine;
      typedef Dg::R2::Disk<Real>    DgDisk;
      typedef Dg::R2::Polygon<Real> DgPolygon;

    private:

      struct Node
      {
        bool isLeaf;
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

      class HalfEdge
      {
      public:
        static uint32_t const NONE = 0xFFFFFFFF;

        HalfEdge()
          : adjacentPolygons{ NONE, NONE }
        {}

        //Elements 0 and 1 will be the same for boundary edges 
        uint32_t     adjacentPolygons[2];
        DgSegment    segment;
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
        std::vector<DgVector>                 points;
        std::map<Key, std::vector<uint32_t>>  polygons;
      };

      //DEBUG
      class NodeData
      {
      public:

        NodeData()
          : upperBounds{ std::numeric_limits<Real>::max, std::numeric_limits<Real>::max }
          , lowerBounds{ -std::numeric_limits<Real>::max, -std::numeric_limits<Real>::max }
        {}

        Real      upperBounds[2];
        Real      lowerBounds[2];
        KeyList   polygons;
      };

      //DEBUG
      struct DebugQueryData
      {
        BSPTreePath       path;
        std::vector<Key>  intersectingPolygons;
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
        std::vector<uint32_t> nodePolygons;
        std::vector<uint32_t> nodeData;

        //Our first node will contain every polygon.
        nodes.push_back(Node());
        for (uint32_t i = 0; i < m_nPolygons; ++i)  nodePolygons.push_back(i);

        Split(nodes, 0, nodePolygons, nodeData, a_criteria);
        AssignTree(nodeData, nodes);

        return Dg::ErrorCode::None;
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

      Key     Query(DgVector const &) const;
      KeyList Query(DgDisk const &) const;

      bool GetBranchData(std::vector<bool> const & a_path, NodeData & a_out)
      {
        uint32_t nodeInd = 0;
        for (bool isAbove : a_path)
        {
          if (m_pNodes[nodeInd].IsLeaf())
          {
            return false;
          }

          if (isAbove)
          {
            a_out.lowerBounds[m_pNodes[nodeInd].branch.element] = m_pNodes[nodeInd].branch.offset;
          }
          else
          {
            a_out.upperBounds[m_pNodes[nodeInd].branch.element] = m_pNodes[nodeInd].branch.offset;
          }
        }

        AddPolygons(nodeInd, a_out.polygons);

        return true;
      }

      //DEBUG
      //Returns path to the leaf.
      DebugQueryData DebugQuery(DgVector const & a_point) const
      {
        DebugQueryData data;
        std::vector<uint32_t> polyIndices;
        DebugGetPolygons(a_point, 0, polyIndices, &data.path);
        data.intersectingPolygons.push_back(GetEnclosingPolygon(a_point, polyIndices));
        return data;
      }

      //std::vector<Key> Query(Capsule const &) const;

    private:

      static inline void UniqueInsert(Key a_target, KeyList & a_keys)
      {
        for (auto const & key : a_keys)
        {
          if (key == a_target) return;
        }
        a_keys.push_back(a_target);
      }

      static inline void Exists(Key a_target, KeyList const & a_keys)
      {
        for (auto const & key : a_keys)
        {
          if (key == a_target) return true;
        }
        return false;
      }

      Key GetEnclosingPolygon(DgVector const & a_point,
                              std::vector<uint32_t> const & a_polygons) const
      {
        Key result(-1);
        for (uint32_t p : a_polygons)
        {
          bool isInside = true;
          for (uint32_t e = m_pPolygons[p].offset,
               e < m_pPolygons[p].offset + m_pPolygons[p].nEdges,
               ++e)
          {
            DgVector v(a_point - m_pEdges[e].edge.Origin());
            if (m_pEdges[e].edge.Direction().PerpDot(v) < static_cast<Real>(0))
            {
              isInside = false;
              break;
            }
          }
          if (isInside)
          {
            result = m_pPolygons[p].key;
            break;
          }
        }
        return result;
      }

      void DebugGetPolygons(DgVector const & a_point,
                            uint32_t a_ind, 
                            std::vector<uint32_t> & a_out,
                            BSPTreePath * a_pPath) const
      {
        if (m_pNodes[a_ind].IsLeaf())
        {
          for (uint32_t i = m_pNodes[a_ind].leaf.offset;
               i < m_pNodes[a_ind].leaf.offset + m_pNodes[a_ind].leaf.nPolygons;
               ++i)
          {
            UniqueInsert(m_pData[i], a_out);
          }
        }
        else
        {
          uint32_t i = m_pNodes[a_ind].branch.element;
          Real offset = m_pNodes[a_ind].branch.offset;
          if (a_point[i] <= offset)
          {
            GetPolygons(a_point, m_pNodes[a_ind].branch.child_BELOW, a_out, a_pPath->NewBelow());
          }
          if (a_point[i] >= offset)
          {
            GetPolygons(a_point, m_pNodes[a_ind].branch.child_ABOVE, a_out, a_pPath->NewAbove());
          }
        }
      }

      void AddPolygons(uint32_t a_nodeInd, KeyList & a_out) const
      {
        Node const & node(m_pNodes[a_nodeInd]);
        if (node.IsLeaf())
        {
          for (uint32_t i = node.leaf.dataOffset;
               i < node.leaf.dataOffset + node.leaf.nPolygons;
               ++i)
          {
            UniqueInsert(m_data[i].key, a_out);
          }
        }
        else
        {
          AddPolygons(node.branch.child_BELOW, a_out);
          AddPolygons(node.branch.child_ABOVE, a_out);
        }
      }

      bool IsCCW(Key a_key, DataInput const & a_data)
      {
        float area = 0.0f;
        std::vector<uint32_t> const & polygon(a_data.polygons.at(a_key));
        for (size_t i0 = 0; i0 <= polygon.size(); ++i0)
        {
          size_t i1 = (i0 + 1) % polygon.size();
          Vector p0 = a_data.points[polygon[i0]];
          Vector p1 = a_data.points[polygon[i1]];

          area += (p1.x() - p0.x()) * (p1.y() + p1.y());
        }
        return area < 0.0f;
      }

      void LoadPolygons(DataInput const & a_input)
      {
        delete[] m_pPolygons;
        delete[] m_pEdges;
        m_pPolygons = nullptr;
        m_pEdges = nullptr;

        std::vector<HalfEdge>   edges;
        std::vector<Polygon>    polygons;

        for (auto const & kv : a_input.polygons)
        {
          std::vector<uint32_t> points;
          if (!IsCCW(kv.first, a_input))
          {
            for (auto it = kv.second.crbegin(); 
                 it != kv.second.crend(); 
                 ++it)
            {
              points.push_back(*it);
            }
          }
          else
          {
            points = kv.second;
          }

          Polygon polygon;
          polygon.key = kv.first;
          polygon.offset = static_cast<uint32_t>(edges.size());
          polygon.nEdges = static_cast<uint32_t>(points.size());
          polygons.push_back(polygon);

          for (size_t i = 0; i < points.size(); ++i)
          {
            size_t i_next = i;
            i_next = (i_next + 1) % points.size();

            HalfEdge e;
            GetAdjacentPolygons(e, a_input, static_cast<uint32_t>(i), static_cast<uint32_t>(i_next));
            e.segment = Segment(a_input.points[i], a_input.points[i_next]);
            edges.push_back(e);
          }
        }

        m_pEdges = new HalfEdge[edges.size()];
        for (size_t i = 0; i < edges.size(); ++i) m_pEdges[i] = edges[i];

        m_pPolygons = new Polygon[polygons.size()];
        for (size_t i = 0; i < polygons.size(); ++i) m_pPolygons[i] = polygons[i];
      }
      
      bool GetAdjacentPolygons(HalfEdge & a_out, DataInput const & a_data, uint32_t a_i0, uint32_t a_i1)
      {
        int ind = 0;
        uint32_t polyInd = 0;
        for (auto const & kv : a_data.polygons)
        {
          for (size_t i0 = 0; i0 < kv.second.size(); ++i0)
          {
            size_t i1 = (i0 + 1) % kv.second.size();
            if ((kv.second[i0] == a_i0 && kv.second[i1] == a_i1) ||
                (kv.second[i0] == a_i1 && kv.second[i1] == a_i0))
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

      DgVector Center(uint32_t a_index) const
      {
        Real minx =  std::numeric_limits<Real>::max();
        Real maxx = -std::numeric_limits<Real>::max();
        Real miny =  std::numeric_limits<Real>::max();
        Real maxy = -std::numeric_limits<Real>::max();

        for (uint32_t i = m_pPolygons[a_index].offset;
             i < m_pPolygons[a_index].offset + m_pPolygons[a_index].nEdges;
             ++i)
        {
          DgVector p(m_pEdges[i].segment.Origin());
          if (p.x() < minx)  minx = p.x();
          else if (p.x() > maxx)  maxx = p.x();

          if (p.y() < miny)  miny = p.y();
          else if (p.y() > maxy)  maxy = p.y();
        }

        Real xhl = (maxx - minx) / static_cast<Real>(2);
        Real yhl = (maxy - miny) / static_cast<Real>(2);

        return DgVector(minx + xhl, miny + yhl, 1.0f);
      }


      SplitData PartitionPolygons(std::vector<uint32_t> a_parentPolygons)
      {
        std::vector<DgVector> centers;
        for (uint32_t p : a_parentPolygons)
        {
          centers.push_back(Center(p));
        }

        SplitData data;

        DgLine line = Dg::R2::LineOfBestFit(centers.data(), centers.size());
        DgVector slope = line.Direction();
        data.element = (abs(slope[0]) > abs(slope[1])) ? 0 : 1;

        //TODO Ensure the offset is balanced, ie that is has equal polygons either side
        //            of the partition.
        data.offset = line.Origin()[data.element];

        for (uint32_t p : a_parentPolygons)
        {
          bool isBelow = false;
          bool isAbove = false;

          for (uint32_t i = m_pPolygons[p].offset; 
               i < m_pPolygons[p].offset + m_pPolygons[p].nEdges;
               ++i)
          {
            if (m_pEdges[i].segment.GetP0()[data.element] < data.offset)
            {
              isBelow = true;
            }
            else if (m_pEdges[i].segment.GetP0()[data.element] > data.offset)
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

      void Split(std::vector<Node> & a_nodes,
                 uint32_t a_parentIndex,
                 std::vector<uint32_t> const & a_parentPolygons,
                 std::vector<uint32_t> & a_nodeDataOut,
                 Criteria const & a_criteria)
      {
        SplitData sdata = PartitionPolygons(a_parentPolygons);

        bool validPartition = true;
        validPartition &= (sdata.belowPolys.size() < a_parentPolygons.size());
        validPartition &= (sdata.abovePolys.size() < a_parentPolygons.size());
        //...other criteria here...

        Node & parentNode(a_nodes[a_parentIndex]);

        if (validPartition)
        {
          parentNode.isLeaf = false;
          parentNode.branch.element = sdata.element;
          parentNode.branch.offset = sdata.offset;

          uint32_t childBelowInd = static_cast<uint32_t>(a_nodes.size());
          parentNode.branch.child_BELOW = childBelowInd;
          a_nodes.push_back(Node());
          Split(a_nodes, childBelowInd, sdata.belowPolys, a_nodeDataOut, a_criteria);

          uint32_t childAboveInd = static_cast<uint32_t>(a_nodes.size());
          parentNode.branch.child_ABOVE = childAboveInd;
          a_nodes.push_back(Node());
          Split(a_nodes, childAboveInd, sdata.abovePolys, a_nodeDataOut, a_criteria);
        }
        else
        {
          parentNode.isLeaf = true;
          parentNode.leaf.dataOffset = static_cast<uint32_t>(a_nodeDataOut.size());
          parentNode.leaf.nPolygons = static_cast<uint32_t>(a_parentPolygons.size());

          a_nodeDataOut.insert(a_nodeDataOut.end(), a_parentPolygons.begin(), a_parentPolygons.end());
        }
      }
      
      void AssignTree(std::vector<uint32_t> const & a_nodeData, 
                      std::vector<Node> const & a_nodes )
      {
        delete[] m_pNodes;
        m_pNodes = new Node[a_nodes.size()];
        for (size_t i = 0; i < a_nodes.size(); ++i) m_pNodes[i] = a_nodes[i];

        delete[] m_pData;
        m_pData = new uint32_t[a_nodeData.size()];
        for (size_t i = 0; i < a_nodeData.size(); ++i) m_pData[i] = a_nodeData[i];
      }

      void Init(BSPTree const & a_other)
      {
        Clear();

        m_nNodes = a_other.m_nNodes;
        m_nEdges = a_other.m_nEdges;
        m_nPolygons = a_other.m_nPolygons;
        m_nDataItems = a_other.m_nDataItems;

        m_pNodes = new Node[m_nNodes];
        m_pEdges = new HalfEdge[m_nEdges];
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
      HalfEdge *         m_pEdges;
      Polygon *      m_pPolygons;

      // {Leaf data, Polygon data} where:
      // Leaf data: {polygonIndex0, polygonIndex1, ... }
      uint32_t *       m_pData; 
    };
  }
}

#endif // QRTREE_H
