//! @file ResourceManager.h
//!
//! @author: Frank B. Hart
//! @date 25/7/2015
//!
//! Resource Manager API

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "ResourceManager_private.h"
#include "ResourceTypes.h"

namespace Dg
{
  namespace Rm
  {
    //! Set an option.
    void SetOptions(uint32_t);

    //! Check an option.
    bool CheckOption(uint32_t);

    //! Register a new resource. The call will look like this:
    //!
    //! ReqisterResource(new ResourceType(key), opts);
    //!
    //! @param a_key Associate a unique key with this resource
    //! @param a_options Options for this resource
    Dg_Result RegisterResource(Resource * a_resource, uint32_t a_options = rDEFAULT);

    //! Get a pointer to a resource. Will fail if the resouce has not been 
    //! successfully registed first with RegisterResource().
    Dg_Result GetResourceHandle(RKey, hResource &);

    //! Initialise a particular resource. 
    Dg_Result InitResource(RKey);

    //! Initialises all resources.
    Dg_Result InitAll();

    //! Deinitialise a particular resource. 
    void DeinitResource(RKey, bool a_force = false);

    //! Deinitialises all resources.
    void DeinitAll(bool a_force = false);
  }
}

#endif