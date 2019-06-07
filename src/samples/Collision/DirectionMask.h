#ifndef DIRECTIONMASK_H
#define DIRECTIONMASK_H

#include "Types.h"

//All argument vectors are assumed to be normalised

class DM_Base;

//TODO instead of new stated being created on the heap,
//     states should return the address of a static state.
//     The only downside is this will then not be thread safe.
class DirMask
{
public:

  DirMask();
  ~DirMask();

  DirMask(DirMask const &);
  DirMask & operator=(DirMask const &);

  void Add(vec3 const & dir);
  bool InMask(vec3 const &) const;
  void Reset();

private:

  DM_Base * m_pState;
};

#endif