//! @file Resource.h
//!
//! @author: Frank B. Hart
//! @date 25/7/2015
//!
//! Class declaration: Resource

#ifndef RESOURCE_H
#define RESOURCE_H

#include "utility.h"

namespace Dg
{
  //! Base class for all Resources
  class Resource
  {
  public:

    virtual ~Resource() = 0;

    virtual bool IsInitialised() = 0;
    virtual Dg_Result Init() = 0;
    virtual Dg_Result DeInit() = 0;

    virtual Dg_Result SetFile(char const *) = 0;

  };
}

#endif