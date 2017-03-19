#ifndef MESHBASE_H
#define MESHBASE_H

#include <vector>

#include "DgHandleBase.h"
#include "DgMapKeyIterator.h"

namespace Dg
{
  template<typename T, int A>
  class DgArray
  {
  public:

    T const & operator[](size_t i) const { return m_data[i]; }
    T & operator[](size_t i) { return m_data[i]; }

  private:
    T m_data[A];
  };

  typedef int32_t                     singleHandle;
  typedef int64_t                     doubleHandle;

  typedef singleHandle                vHandleType;
  typedef doubleHandle                eHandleType;
  typedef singleHandle                fHandleType;

  typedef HandleBase<vHandleType, 0>  vHandle;
  typedef HandleBase<eHandleType, 1>  eHandle;
  typedef HandleBase<fHandleType, 2>  fHandle;

  typedef DgArray<eHandle, 3>         eHandle_3;
  typedef DgArray<vHandle, 3>         vHandle_3;
  typedef DgArray<fHandle, 3>         fHandle_2;

  namespace MeshTools
  {
    eHandle GetEdgeHandle(vHandle, vHandle);
    bool HasVertex(eHandle, vHandle);
    vHandle Source(eHandle);
    vHandle Target(eHandle);
  }

  template<typename vData,
           typename eData,
           typename fData>
  class MeshBase
  {

  public:

    typedef const_key_iterator<vHandle, vData>                        Vertex_iterator;
    typedef const_key_iterator<eHandle, eData>                        Edge_iterator;
    typedef const_key_iterator<fHandle, std::pair<vHandle_3, fData>>  Face_iterator;

    virtual ~MeshBase() {}

    vData & GetData(vHandle a_h)
    {
      return m_vertexData.at(a_h);
    }


    eData & GetData(eHandle a_h)
    {
      return m_edgeData.at(a_h);
    }


    fData & GetData(fHandle a_h)
    {
      return m_faceData.at(a_h).second;
    }


    vData const & GetData(vHandle a_h) const
    {
      return m_vertexData.at(a_h);
    }


    eData const & GetData(eHandle a_h) const
    {
      return m_edgeData.at(a_h);
    }


    fData const & GetData(fHandle a_h) const
    {
      return m_faceData.at(a_h).second;
    }

    void Clear()
    {
      m_vertexData.clear();
      m_edgeData.clear();
      m_faceData.clear();
    }

    bool IsValid(vHandle a_vh) const
    {
      return (m_vertexData.find(a_vh) != m_vertexData.cend());
    }


    bool IsValid(eHandle a_eh) const
    {
      return (m_edgeData.find(a_eh) != m_edgeData.cend());
    }


    bool IsValid(fHandle a_ph) const
    {
      return (m_faceData.find(a_ph) != m_faceData.cend());
    }

    //Vertex methods

    std::vector<vHandle> JoiningVertices(vHandle a_h) const
    {
      std::vector<vHandle> result;
      for (auto const & kv : m_edgeData)
      {
        if (MeshTools::HasVertex(kv.first, a_h))
        {
          vHandle sh = MeshTools::Source(kv.first);
          if (sh != a_h)
          {
            result.push_back(sh);
          }
          else
          {
            result.push_back(MeshTools::Target(kv.first));
          }
        }
      }
      return result;
    }

    std::vector<eHandle> JoiningEdges(vHandle a_h) const
    {
      std::vector<eHandle> result;
      for (auto const & kv : m_edgeData)
      {
        if (MeshTools::HasVertex(kv.first, a_h))
        {
          result.push_back(kv.first);
        }
      }
      return result;
    }

    std::vector<fHandle> JoiningFaces(vHandle a_h) const
    {
      std::vector<eHandle> result;
      for (auto const & kv : m_faceData)
      {
        if (HasVertex(kv.first, a_h))
        {
          result.push_back(kv.first);
        }
      }
      return result;
    }

    size_t NumberVertices() const
    {
      return m_vertexData.size();
    }

    Vertex_iterator VerticesBegin() const
    {
      return Vertex_iterator(m_vertexData.cbegin());
    }

    Vertex_iterator VerticesEnd() const
    {
      return Vertex_iterator(m_vertexData.cend());
    }

    //Edge methods
    bool IsBoundary(eHandle a_h) const
    {
      fHandle f0, f1;
      GetAdjacentPolygons(a_h, f0, f1);
      return (IsUnbounded(f0) || IsUnbounded(f1));
    }

    size_t NumberEdges() const
    {
      return m_edgeData.size();
    }

    size_t NumberBoundaryEdges() const
    {
      size_t boundaryEdges = 0;
      for (auto eit = edges_begin();
           eit != edges_end();
           eit++)
      {
        if (IsBoundary(*eit)) boundaryEdges++;
      }
      return boundaryEdges;
    }

    bool AdjacentFaces(eHandle a_eh, fHandle_2 & a_out) const
    {
      for (auto it = m_faceData.cbegin(); it != m_faceData.cend(); it++)
      {
        if (HasEdge(it->first, a_eh))
        {
          a_out.data[0] = it->first;
          it++;
          for (; it != m_faceData.cend(); it++)
          {
            if (HasEdge(it->first, a_eh))
            {
              a_out.data[1] = it->first;
              return true;
            }
          }
          a_out.data[1] = pHandle(HandleEnum::UNBOUNDED);
          return true;
        }
      }
      return false;
    }

    Edge_iterator EdgesBegin() const
    {
      return Edge_iterator(m_edgeData.begin());
    }

    Edge_iterator EdgesEnd() const
    {
      return Edge_iterator(m_edgeData.end());
    }

    //Face methods
    vHandle_3 Vertices(fHandle a_fh) const
    {
      return m_faceData.at(a_fh).first;
    }

    eHandle_3 Edges(fHandle a_fh) const
    {
      eHandle_3 result;
      vHandle_3 const & vh3 = m_faceData.at(a_fh).first;
      result[0] = MeshTools::GetEdgeHandle(vh3[0], vh3[1]);
      result[1] = MeshTools::GetEdgeHandle(vh3[0], vh3[2]);
      result[2] = MeshTools::GetEdgeHandle(vh3[1], vh3[2]);
      return result;
    }

    int NumberFaces() const
    {
      return m_faceData.size();
    }

    bool AreNeighbours(fHandle a_h0, fHandle a_h1,
                       bool byVertex, bool byEdge) const
    {
      vHandle_3 fvh0 = Vertices(a_h0);
      vHandle_3 fvh1 = Vertices(a_h1);
      if (byVertex)
      {
        for (int i = 0; i < 3; i++)
        {
          for (int j = 0; j < 3; j++)
          {
            if (fvh0[i] == fvh1[j]) return true;
          }
        }
      }

      eHandle_3 feh0 = Edges(a_h0);
      eHandle_3 feh1 = Edges(a_h1);
      if (byEdge)
      {
        for (int i = 0; i < 3; i++)
        {
          for (int j = 0; j < 3; j++)
          {
            if (feh0[i] == feh1[j]) return true;
          }
        }
      }

      return false;
    }

    std::vector<fHandle> AdjacentFaces(fHandle a_fh,
                                       bool byVertex,
                                       bool byEdge) const
    {
      std::vector<fHandle> result;
      for (auto const & kv : m_faceData)
      {
        if (kv.first != a_fh)
        {
          if (AreNeighbours(kv.first, a_fh, byVertex, byEdge))
          {
            result.push_back(kv.first);
          }
        }
      }
      return result;
    }

    bool HasVertex(fHandle a_fh, vHandle a_vh) const
    {
      vHandle_3 fvh = Vertices(a_fh);
      for (int i = 0; i < 3; i++)
      {
        if (fvh[i] == a_vh) return true;
      }
      return false;
    }

    bool HasEdge(fHandle a_fh, eHandle a_eh) const
    {
      eHandle_3 feh = Edges(a_fh);
      for (int i = 0; i < 3; i++)
      {
        if (feh[i] == a_eh) return true;
      }
      return false;
    }

    Face_iterator FacesBegin() const
    {
      return Face_iterator(m_faceData.begin());
    }

    Face_iterator FacesEnd() const
    {
      return Face_iterator(m_faceData.begin());
    }

  protected:

    std::map<vHandle, vData>                        m_vertexData;
    std::map<eHandle, eData>                        m_edgeData;
    std::map<fHandle, std::pair<vHandle_3, fData>>  m_faceData;
  };

}

#endif