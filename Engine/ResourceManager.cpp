#include "ResourceManager.h"
#include "ResourceHandle.h"

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
  //	@	ResourceManager::InitResource()
  //--------------------------------------------------------------------------------
  Dg_Result ResourceManager::InitResource(DgRKey a_key)
  {
    int index(0);
    if (!m_resourceList.find(a_key, index))
    {
      return DgR_Failure;;
    }

    if (!m_resourceList[index].m_resource->IsInitialised())
    {
      return m_resourceList[index].m_resource->Init();
    }

    return DgR_Success;
  }// End: ResourceManager::InitResource()


  //--------------------------------------------------------------------------------
  //	@	ResourceManager::InitAll()
  //--------------------------------------------------------------------------------
  Dg_Result ResourceManager::InitAll()
  {
    bool isGood = true;

    for (size_t i = 0; i < m_resourceList.size(); ++i)
    {
      if (m_resourceList[i].m_resource->Init() != DgR_Success)
      {
        isGood = false;
      }
    }

    return isGood ? DgR_Success : DgR_Failure;

  }// End: ResourceManager::InitAll()


  //--------------------------------------------------------------------------------
  //	@	ResourceManager::DeinitResource()
  //--------------------------------------------------------------------------------
  void ResourceManager::DeinitResource(DgRKey a_key, bool a_force)
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
  }// End: ResourceManager::DeinitResource()


  //--------------------------------------------------------------------------------
  //	@	ResourceManager::DeinitAll()
  //--------------------------------------------------------------------------------
  void ResourceManager::DeinitAll(bool a_force)
  {
    for (size_t i = 0; i < m_resourceList.size(); ++i)
    {
      if (a_force || m_resourceList[i].m_nUsers == 0)
      {
        m_resourceList[i].m_resource->DeInit();
      }
    }
  }// End: ResourceManager::DeinitAll()


  //--------------------------------------------------------------------------------
  //	@	ResourceManager::~ResourceManager
  //--------------------------------------------------------------------------------
  ResourceManager::~ResourceManager()
  {
    for (size_t i = 0; i < m_resourceList.size(); ++i)
    {
      delete m_resourceList[i].m_resource;
    }
  }// End: ResourceManager::~ResourceManager


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
  Dg_Result ResourceManager::GetResource(DgRKey a_key, hResource & a_out)
  {
    Resource * pR(nullptr);
    pR = RegisterUser(a_key);
    if (pR == nullptr)
    {
      return DgR_Failure;
    }

    if (a_out.m_rKey.IsValid())
    {
      DeregisterUser(a_out.m_rKey);
    }

    a_out.m_resource = pR;
    a_out.m_rKey = a_key;
    return DgR_Success;

  }// End: ResourceManager::GetResource()

}