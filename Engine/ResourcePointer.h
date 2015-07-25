//! @file ResourcePointer.h
//!
//! @author: Frank B. Hart
//! @date 25/7/2015
//!
//! Class declaration: pResource

#ifndef RESOURCEPOINTER_H
#define RESOURCEPOINTER_H

#include "ResourceManager.h"

namespace Dg
{
  //! Reference counted pointer to a resource.
  class pResource
  {
    friend class ResourceManager;

  public:

    pResource() : m_resource(nullptr) {}

    ~pResource() { ResourceManager::Instance()->DeregisterUser(m_rKey); }

    pResource(pResource const & a_other) : m_rKey(a_other.m_rKey)
                                         , m_resource(ResourceManager::Instance()->RegisterUser(a_other.m_rKey))
                                         {}

    pResource & operator=(pResource const & a_other)
    {
      ResourceManager::Instance()->DeregisterUser(m_rKey);
      m_rKey = a_other.m_rKey;
      m_resource = ResourceManager::Instance()->RegisterUser(m_rKey);

      return *this;
    }

    Resource * operator->()
    {
      return m_resource;
    }

    Resource & operator*()
    {
      return *m_resource;
    }

  private:

    Resource * m_resource;
    DgRKey m_rKey;

  };
}

#endif