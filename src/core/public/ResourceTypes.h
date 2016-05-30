//! @file ResourceKey.h
//!
//! @author: Frank B. Hart
//! @date 25/7/2015
//!
//! Class declaration: RKey

#ifndef RESOURCETYPES_H
#define RESOURCETYPES_H

#include <stdint.h>

#include "DgUtility.h"

namespace Dg
{
  namespace Rm
  {
    //! Resource manager key
    typedef uint32_t RKey;


    //! Options for individual resources
    enum
    {
      rNON_OPTS      = 0,
      rInitOnReg     = 1,   // Initialise the resource on registration
      rInitWhenInUse = 2,   // Initialise the resource when at least one user is registered
      rAutoDeinit    = 4,   // Deinitialise a resource once number of registered users equals 0.
      rDEFAULT       = rInitWhenInUse
    };


    //! Global options for the Resource Manager
    enum
    {
      rmDEFAULT = 0,
    };


    //! Resource base class. All resource types should inherit from this class.
    class Resource
    {
    public:

      Resource(RKey a_key, void * a_data = nullptr) : m_key(a_key) {}

      virtual ~Resource() {};

      virtual bool IsInitialised() = 0;

      virtual Dg_Error Init() = 0;
      virtual Dg_Error DeInit() = 0;

      RKey GetKey() const { return m_key; }

    private:

      RKey m_key;
    };

    namespace impl
    {
      class ResourceManager;
    }

    //! Reference counted pointer to a resource. Avoid excessive copying as this 
    //! Registers and Deregisters resources in the resource manager.
    class hResource
    {
      
      friend class impl::ResourceManager;

    public:

      hResource() : m_resource(nullptr) {}
      ~hResource();
      hResource(hResource const & a_other);
      hResource & operator=(hResource const & a_other);

      //! Conversion operator
      Resource * operator->();

      //! Conversion operator
      Resource & operator*();

    private:

      Resource * m_resource;
    };
  }
}

#endif