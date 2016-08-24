//! @file DgContainerBase.h
//!
//! @author: Frank B. Hart
//! @date 2/5/2015
//!
//! Class declaration: ContainerBase

#ifndef DG_CONTAINERBASE_H
#define DG_CONTAINERBASE_H

#define ARRAY_SIZE(a) sizeof(a) / sizeof(*a)

namespace Dg
{
  class ContainerBase
  {
  public:

    ContainerBase();
    ContainerBase(size_t a_nItems);
    virtual ~ContainerBase();
    ContainerBase(ContainerBase const &);
    ContainerBase & operator=(ContainerBase const &);
    ContainerBase(ContainerBase &&);
    ContainerBase & operator=(ContainerBase &&);

    size_t pool_size() const;

    size_t pool_size(size_t a_nItems);

    size_t set_next_pool_size();

  private:
    int   m_poolSizeIndex;
  };
}

#endif