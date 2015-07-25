//! @file ResourceManager.h
//!
//! @author: Frank B. Hart
//! @date 25/7/2015
//!
//! Class declaration: ResourceManager

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "dg_map_p.h"
#include "singleton.h"
#include "Resource.h"
#include "ResourceKey.h"
#include "utility.h"

namespace Dg
{
  class pResource;

  //! General resource manager.
  //! Use:
  //!       1) Register all resources with RegisterResource()
  //!       2) Request Resources with GetResource()
  class ResourceManager : public Singleton<ResourceManager>
  {
    friend class pResource;
    friend class Singleton < ResourceManager > ;

  public:

    enum Option
    {
      DEFAULT         = 0,
      AutomaticClear  = 1,  // DeInitialise a resource once number of registered users equals 0.
    };

  public:

    //! Set an option.
    void SetOptions(uint32_t);

    //! Check an option.
    bool CheckOption(Option);

    //! Register a new resource. Will automatically load the resource from file if the
    //! AutomaticInit flag is set as an option.
    template<typename ResourceType>
    Dg_Result RegisterResource(DgRKey, char const * a_file, bool a_init = false);

    //! Get a pointer to a resource. Will fail if the resouce has not been 
    //! successfully registed first with RegisterResource.
    Dg_Result GetResource(DgRKey, pResource &);

    //! Clear a particular resource. 
    void ClearResource(DgRKey, bool a_force = false);

    //Clears all resources.
    void ClearAll(bool a_force = false);

  private:

    ResourceManager() : m_options(DEFAULT){}

    //! Only the pResource class should be calling this function.
    void DeregisterUser(DgRKey);

    //! Will initialised resource if not initialised.
    //! Only the pResource class should be calling this function.
    Resource * RegisterUser(DgRKey);

  private:

    struct ResourceContainer
    {
      Resource * m_resource;
      unsigned m_nUsers;
    };

  private:

    uint32_t m_options;
    Dg::map_p<DgRKey, ResourceContainer> m_resourceList;
  };


  //--------------------------------------------------------------------------------
  //	@	ResourceManager::RegisterResource()
  //--------------------------------------------------------------------------------
  template <typename ResourceType>
  Dg_Result ResourceManager::RegisterResource(DgRKey a_key, 
                                              char const * a_file,
                                              bool a_init)
  {
    int index(0);
    if (m_resourceList.find(a_key, index))
    {
      return DgR_Duplicate;
    }

    ResourceContainer rc;
    rc.m_nUsers = 0;
    rc.m_resource = new ResourceType();
    rc.m_resource.SetFile(a_file);

    if (a_init)
    {
      if (!rc.m_resource->Init())
      {
        return DgR_Failure;
      }
    }

    m_resourceList.insert(a_key, rc);

    return DgR_Success;
  }
}// End: ResourceManager::RegisterResource()


#endif
