#include "ResourceManager.h"
#include "ResourceManager_private.h"
#include "ResourceTypes.h"

namespace Dg
{
  namespace Rm
  {
    //--------------------------------------------------------------------------------
    //	@	SetOptions()
    //--------------------------------------------------------------------------------
    void SetOptions(uint32_t a_opts)
    {
      impl::ResourceManager::Instance()->SetOptions(a_opts);
    }


    //--------------------------------------------------------------------------------
    //	@	CheckOption()
    //--------------------------------------------------------------------------------
    bool CheckOption(uint32_t a_option)
    {
      return impl::ResourceManager::Instance()->CheckOption(a_option);
    }


    //--------------------------------------------------------------------------------
    //	@	RegisterResource()
    //--------------------------------------------------------------------------------
    Dg_Error RegisterResource(Resource * a_resource, uint32_t a_options)
    {
      return impl::ResourceManager::Instance()->RegisterResource(a_resource, a_options);
    }


    //--------------------------------------------------------------------------------
    //	@	GetResourceHandle()
    //--------------------------------------------------------------------------------
    Dg_Error GetResourceHandle(RKey a_key, hResource & a_out)
    {
      return impl::ResourceManager::Instance()->GetResourceHandle(a_key, a_out);
    }


    //--------------------------------------------------------------------------------
    //	@	InitResource()
    //--------------------------------------------------------------------------------
    Dg_Error InitResource(RKey a_key)
    {
      return impl::ResourceManager::Instance()->InitResource(a_key);
    }


    //--------------------------------------------------------------------------------
    //	@	InitAll()
    //--------------------------------------------------------------------------------
    Dg_Error InitAll()
    {
      return impl::ResourceManager::Instance()->InitAll();
    }


    //--------------------------------------------------------------------------------
    //	@	DeinitResource()
    //--------------------------------------------------------------------------------
    void DeinitResource(RKey a_key, bool a_force)
    {
      impl::ResourceManager::Instance()->DeinitResource(a_key, a_force);
    }


    //--------------------------------------------------------------------------------
    //	@	DeinitAll()
    //--------------------------------------------------------------------------------
    void DeinitAll(bool a_force)
    {
      impl::ResourceManager::Instance()->DeinitAll(a_force);
    }


    namespace impl
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
      bool ResourceManager::CheckOption(uint32_t a_option)
      {
        return ((m_options & a_option) != 0);
      }// End: ResourceManager::CheckOption()


      //--------------------------------------------------------------------------------
      //	@	ResourceManager::InitResource()
      //--------------------------------------------------------------------------------
      Dg_Error ResourceManager::InitResource(RKey a_key)
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
      Dg_Error ResourceManager::InitAll()
      {
        bool isGood = true;

        for (int i = 0; i < m_resourceList.size(); ++i)
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
      void ResourceManager::DeinitResource(RKey a_key, bool a_force)
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
        for (int i = 0; i < m_resourceList.size(); ++i)
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
        for (int i = 0; i < m_resourceList.size(); ++i)
        {
          delete m_resourceList[i].m_resource;
        }
      }// End: ResourceManager::~ResourceManager


      //--------------------------------------------------------------------------------
      //	@	ResourceManager::DeregisterUser()
      //--------------------------------------------------------------------------------
      void ResourceManager::DeregisterUser(RKey a_key)
      {
        int index(0);
        if (!m_resourceList.find(a_key, index))
        {
          return;
        }

        m_resourceList[index].m_nUsers--;
        if (m_resourceList[index].m_nUsers == 0 && (m_resourceList[index].m_opts & rAutoDeinit) != 0)
        {
          m_resourceList[index].m_resource->DeInit();
        }
      }// End: ResourceManager::DeregisterUser()


      //--------------------------------------------------------------------------------
      //	@	ResourceManager::RegisterUser()
      //--------------------------------------------------------------------------------
      Resource * ResourceManager::RegisterUser(RKey a_key)
      {
        int index(0);
        if (!m_resourceList.find(a_key, index))
        {
          return nullptr;
        }

        if (!m_resourceList[index].m_resource->IsInitialised())
        {
          if ((m_resourceList[index].m_opts & rInitWhenInUse)
            && m_resourceList[index].m_resource->Init() != DgR_Success)
          {
            return nullptr;
          }
        }

        m_resourceList[index].m_nUsers++;
        return m_resourceList[index].m_resource;
      }// End: ResourceManager::RegisterUser()


      //--------------------------------------------------------------------------------
      //	@	ResourceManager::GetResourceHandle()
      //--------------------------------------------------------------------------------
      Dg_Error ResourceManager::GetResourceHandle(RKey a_key, hResource & a_out)
      {
        Resource * pR(nullptr);
        pR = RegisterUser(a_key);
        if (pR == nullptr)
        {
          return DgR_Failure;
        }

        if (a_out.m_resource != nullptr)
        {
          DeregisterUser(a_out.m_resource->GetKey());
        }

        a_out.m_resource = pR;
        return DgR_Success;

      }// End: ResourceManager::GetResourceHandle()



      //--------------------------------------------------------------------------------
      //	@	ResourceManager::RegisterResource()
      //--------------------------------------------------------------------------------
      Dg_Error ResourceManager::RegisterResource(Resource * a_resource,
                                                  uint32_t a_options)
      {
        if (a_resource == nullptr)
        {
          return DgR_Failure;
        }

        int index(0);
        if (m_resourceList.find(a_resource->GetKey(), index))
        {
          delete a_resource;
          return DgR_Duplicate;
        }

        ResourceContainer rc;
        rc.m_nUsers = 0;
        rc.m_opts = a_options;
        rc.m_resource = a_resource;

        if (a_options & rInitOnReg)
        {
          if (rc.m_resource->Init() != DgR_Success)
          {
            return DgR_Failure;
          }
        }

        m_resourceList.insert(a_resource->GetKey(), rc);

        return DgR_Success;
      }// End: ResourceManager::RegisterResource()
    }
  }
}