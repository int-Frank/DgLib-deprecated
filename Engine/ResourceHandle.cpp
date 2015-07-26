#include "ResourceHandle.h"

namespace Dg
{
  //--------------------------------------------------------------------------------
  //	@	hResource::~hResource()
  //--------------------------------------------------------------------------------
  hResource::~hResource()
  {
    if (m_resource != nullptr)
    {
      ResourceManager::Instance()->DeregisterUser(m_resource->GetKey());
    }
  }// End: hResource::~hResource()


  //--------------------------------------------------------------------------------
  //	@	hResource::hResource()
  //--------------------------------------------------------------------------------
  hResource::hResource(hResource const & a_other) : m_resource(nullptr)
  {
    if (a_other.m_resource != nullptr)
    {
      m_resource = ResourceManager::Instance()->RegisterUser(a_other.m_resource->GetKey());
    }
  }// End: hResource::hResource()


  //--------------------------------------------------------------------------------
  //	@	hResource::operator=()
  //--------------------------------------------------------------------------------
  hResource & hResource::operator=(hResource const & a_other)
  {
    if (m_resource != nullptr)
    {
      ResourceManager::Instance()->DeregisterUser(m_resource->GetKey());
    }

    if (a_other.m_resource != nullptr)
    {
      m_resource = ResourceManager::Instance()->RegisterUser(a_other.m_resource->GetKey());
    }
    else
    {
      m_resource = nullptr;
    }

    return *this;
  }// End: hResource::operator=()


  //--------------------------------------------------------------------------------
  //	@	hResource::operator->()
  //--------------------------------------------------------------------------------
  Resource * hResource::operator->()
  {
    return m_resource;
  }// End: hResource::operator->()


  //--------------------------------------------------------------------------------
  //	@	hResource::operator*()
  //--------------------------------------------------------------------------------
  Resource & hResource::operator*()
  {
    return *m_resource;
  }// End: hResource::operator*()
}