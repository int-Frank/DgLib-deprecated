#include "ResourceManager.h"
#include "ResourcePointer.h"

namespace Dg
{
  //--------------------------------------------------------------------------------
  //	@	ResourceManager::SetOptions()
  //--------------------------------------------------------------------------------
  void ResourceManager::SetOptions(uint32_t a_options)
  {
    m_options = a_options;
  }// End: ResourceManager::SetOptions()


  //--------------------------------------------------------------------------------
  //	@	ResourceManager::CheckOption()
  //--------------------------------------------------------------------------------
  bool ResourceManager::CheckOption(Option a_option)
  {
    return ((m_options & a_option) != 0);
  }// End: ResourceManager::CheckOption()


  //--------------------------------------------------------------------------------
  //	@	ResourceManager::ClearResource()
  //--------------------------------------------------------------------------------
  void ResourceManager::ClearResource(DgRKey a_key, bool a_force)
  {
    int index(0);
    if (!m_resourceList.find(a_key, index))
    {
      return;
    }

    if (m_resourceList[index].m_nUsers == 0 || a_force)
    {
      m_resourceList[index].m_resource->DeInit();
    }
  }// End: ResourceManager::ClearResource()


  //--------------------------------------------------------------------------------
  //	@	ResourceManager::DeregisterUser()
  //--------------------------------------------------------------------------------
  void ResourceManager::DeregisterUser(DgRKey a_key)
  {
    int index(0);
    if (!m_resourceList.find(a_key, index))
    {
      return;
    }

    m_resourceList[index].m_nUsers--;
    if (m_resourceList[index].m_nUsers == 0 && CheckOption(AutomaticClear))
    {
      m_resourceList[index].m_resource->DeInit();
    }
  }// End: ResourceManager::DeregisterUser()

  
  //--------------------------------------------------------------------------------
  //	@	ResourceManager::RegisterUser()
  //--------------------------------------------------------------------------------
  Resource * ResourceManager::RegisterUser(DgRKey a_key)
  {
    int index(0);
    if (!m_resourceList.find(a_key, index))
    {
      return nullptr;
    }

    if (!m_resourceList[index].m_resource->IsInitialised())
    {
      if (!m_resourceList[index].m_resource->Init())
      {
        return nullptr;
      }
    }

    m_resourceList[index].m_nUsers++;
    return m_resourceList[index].m_resource;
  }// End: ResourceManager::RegisterUser()


  //--------------------------------------------------------------------------------
  //	@	ResourceManager::GetResource()
  //--------------------------------------------------------------------------------
  Dg_Result ResourceManager::GetResource(DgRKey a_key, pResource & a_out)
  {
    Resource * pR(nullptr);
    pR = RegisterUser(a_key);
    if (pR == nullptr)
    {
      return DgR_Failure;
    }

    DeregisterUser(a_out.m_rKey);

    a_out.m_resource = pR;
    a_out.m_rKey = a_key;
    return DgR_Success;

  }// End: ResourceManager::GetResource()

}