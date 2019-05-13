#ifndef DIRECTIONMASK_H
#define DIRECTIONMASK_H

#include "Types.h"

//All argument vectors are assumed to be normalised

class DM_Base;

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