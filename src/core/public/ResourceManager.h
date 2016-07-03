//! @file ResourceManager.h
//!
//! @author: Frank B. Hart
//! @date 25/7/2015
//!
//! Resource Manager API

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "ResourceTypes.h"

namespace Dg
{
  namespace Rm
  {
    //! Set an option.
    void SetOptions(uint32_t);

    //! Check an option.
    bool CheckOption(uint32_t);

    //! @ingroup DgEngine_Rm
    //!
    //! Register a new resource. The call will look like this:
    //!
    //! ReqisterResource(new ResourceType(key, pData), opts);
    //!
    //! @param a_resource New instance of a resource
    //! @param a_options Options for this resource
    //!
    //! @return Error code
    ErrorCode RegisterResource(Resource * a_resource, uint32_t a_options = rDEFAULT);

    //! @ingroup DgEngine_Rm
    //!
    //! Get a pointer to a resource. Will fail if the resouce has not been 
    //! successfully registed first with RegisterResource().
    //!
    //! @return Error code
    ErrorCode GetResourceHandle(RKey, hResource &);

    //! @ingroup DgEngine_Rm
    //!
    //! Initialise a particular resource. 
    //!
    //! @return Error code
    ErrorCode InitResource(RKey);

    //! @ingroup DgEngine_Rm
    //!
    //! Initialises all resources.
    //!
    //! @return Error code
    ErrorCode InitAll();

    //! @ingroup DgEngine_Rm
    //!
    //! Deinitialise a particular resource. 
    //!
    //! @param a_k Key of the resource
    //! @param a_force Force a resource even if there are active handles to this resource.
    void DeinitResource(RKey a_k, bool a_force = false);

    //! @ingroup DgEngine_Rm
    //!
    //! Deinitialises all resources.
    //!
    //! @param a_force Force deinit of all resources even if there
    //!                there are active handles.
    void DeinitAll(bool a_force = false);
  }
}

#endif