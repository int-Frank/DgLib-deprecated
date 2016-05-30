//! @file ResourceManager.h
//!
//! @author: Frank B. Hart
//! @date 25/7/2015
//!
//! Class declaration: ResourceManager

#ifndef RESOURCEMANAGER_PRIVATE_H
#define RESOURCEMANAGER_PRIVATE_H

#include <stdint.h>

#include "Dg_map_pod.h"
#include "DgSingleton.h"
#include "ResourceTypes.h"
#include "DgUtility.h"

namespace Dg
{
  namespace Rm
  {
    namespace impl
    {

      //! General resource manager.
      //! Use:
      //!       1) Register all resources with RegisterResource()
      //!       2) Request Resources with GetResource()
      class ResourceManager : public Singleton<ResourceManager>
      {
        friend class hResource;
        friend class Singleton < ResourceManager >;

      public:

        //! Set an option.
        void SetOptions(uint32_t);

        //! Check an option.
        bool CheckOption(uint32_t);

        //! Register a new resource.
        //! @param a_key Associate a unique key with this resource
        //! @param a_options Options for this resource
        Dg_Error RegisterResource(Resource * a_resource,
                                   uint32_t a_options = rDEFAULT);

        //! Get a pointer to a resource. Will fail if the resouce has not been 
        //! successfully registed first with RegisterResource().
        Dg_Error GetResourceHandle(RKey, hResource &);

        //! Initialise a particular resource. 
        Dg_Error InitResource(RKey);

        //! Initialises all resources.
        Dg_Error InitAll();

        //! Deinitialise a particular resource. 
        void DeinitResource(RKey, bool a_force = false);

        //! Deinitialises all resources.
        void DeinitAll(bool a_force = false);

      private:

        ResourceManager() : m_options(rmDEFAULT){}
        ~ResourceManager();

        //! Only the hResource class should be calling this function.
        void DeregisterUser(RKey);

        //! Will initialised resource if not initialised.
        //! Only the hResource class should be calling this function.
        Resource * RegisterUser(RKey);

      private:

        struct ResourceContainer
        {
          Resource * m_resource;
          unsigned   m_nUsers;
          uint32_t   m_opts;
        };

        uint32_t m_options;
        Dg::map_pod<RKey, ResourceContainer> m_resourceList;
      };
    }
  }
}


#endif
