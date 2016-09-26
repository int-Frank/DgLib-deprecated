
#ifndef POLYGONDECOMP_H
#define POLYGONDECOMP_H

#include <cstdlib>

#include "DgVector3.h"
#include "DgTypes.h"

#include "Dg_vector.h"

namespace Dg
{

  template<typename Real>
  class PolygonDecomp
  {
  public:

    class Node
    {
      friend class PolygonDecomp<Real>;
    public:

      Node const * Next() const { return m_pNext; }
      Vector3<Real> const & Point() const { return m_point; }

    private:
      Node *          m_pNext;
      Vector3<Real>   m_point;
    };

    struct Polygon
    {
      Node *    pHead;
      size_t    size;
    };

  public:

    PolygonDecomp();
    ~PolygonDecomp();

    PolygonDecomp(PolygonDecomp const &);
    PolygonDecomp & operator=(PolygonDecomp const &);

    ErrorCode Decompose(Vector3<Real> const *, size_t);
    size_t PolygonCount() const { return m_polyCount; }
    Polygon const & GetPolygon(size_t i) const { return m_pPolys[i]; }


  private:

    ErrorCode Init(Vector3<Real> const *, size_t);
    ErrorCode Decompose(Node *);

  private:

    Node *            m_pNodePool;
    Node *            m_pNextFree;
    size_t            m_nodePoolSize;

    Polygon *         m_pPolys;
    size_t            m_polyCount;
  };

  //Must be at least 1 point
  template<typename Real>
  ErrorCode PolygonDecomp<Real>::Init(Vector3<Real> const * a_pPoints, size_t a_pointCount)
  {
    if (a_pPoints == nullptr || a_pointCount == 0)
    {
      return ErrorCode::BadInput;
    }

    m_pPolys.clear();
    m_nodePoolSize = a_pointCount * 3; //TODO work out what this number will be
    m_pNodePool = static_cast<Node*>(realloc(m_pNodePool, sizeof(Node) * m_nodePoolSize);

    //Assign pointers
    for (size_t i = 0; i < m_nodePoolSize - 1; i++)
    {
      m_pNodePool[i].m_next = &m_pNodePool[i + 1];
    }

    //Copy data, discarding successive duplicate points.
    m_pNodePool[0] = a_pPoints[0];
    size_t lastInd = 0;
    for (size_t i = 1; i < a_pointCount; i++)
    {
      if (a_pPoints[i] != m_pNodePool[lastInd].m_point)
      {
        lastInd++;
        m_pNodePool[lastInd].m_point = a_pPoints[i];
      }
    }

    m_pPolys = static_cast<Polygon*>(realloc(m_pPolys, sizeof(Polygon) * a_pointCount / 2 + 1); //TODO work out what how large this will be
    m_pPolys[0].pHead = &m_pNodePool[0];
    m_pNodePool[lastInd].m_pNext = &m_pNodePool[0];
    m_pNextFree = &m_pNodePool[lastInd + 1];
    m_polyCount = 1;

    return ErrorCode::None;
  }

  template<typename Real>
  ErrorCode PolygonDecomp<Real>::Decompose(Vector3<Real> const * a_points, size_t a_pointCount)
  {
    ErrorCode result = Init(a_points, a_pointCount);
    if (result != ErrorCode::None)
    {
      return result;
    }
    return Decompose(&m_pNodePool[0]);
  }

  template<typename Real>
  ErrorCode PolygonDecomp<Real>::Decompose(PolygonDecomp<Real>::Node * a_pHead)
  {

    return ErrorCode::None;
  }
}

#endif