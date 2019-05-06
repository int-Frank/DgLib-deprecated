#ifndef DIRECTIONMASK_H
#define DIRECTIONMASK_H

#include "Types.h"

//All argument vectors are assumed to be normalised
class DM_Base
{
public:

  virtual ~DM_Base(){}
  virtual DM_Base * Add(vec3 const & dir) =0;
  virtual bool InMask(vec3 const &) const =0;
  virtual DM_Base * Clone() const =0;
};

class DM_None : public DM_Base
{
public:

  ~DM_None(){}
  DM_Base * Add(vec3 const & dir);
  bool InMask(vec3 const &) const;
  DM_Base * Clone() const;
};

class DM_D : public DM_Base
{
public:

  DM_D(vec3 const &);
  ~DM_D(){}
  DM_Base * Add(vec3 const & dir);
  bool InMask(vec3 const &) const;
  DM_Base * Clone() const;

private:

  vec3 m_dir;
};

class DM_LDR : public DM_Base
{
public:

  DM_LDR(vec3 const & loa, vec3 const & roa);
  ~DM_LDR(){}
  DM_Base * Add(vec3 const & dir);
  bool InMask(vec3 const &) const;
  DM_Base * Clone() const;

private:

  void Set();

private:
  vec3 m_loa;
  vec3 m_roa;
  vec3 m_dir;
  float m_proj;
};

class DM_LR : public DM_Base
{
public:

  DM_LR(vec3 const & loa);
  ~DM_LR(){}
  DM_Base * Add(vec3 const & dir);
  bool InMask(vec3 const &) const;
  DM_Base * Clone() const;

private:
  vec3 m_loa;
};

class DM_LRD : public DM_Base
{
public:

  DM_LRD(vec3 const & loa);
  ~DM_LRD(){}
  DM_Base * Add(vec3 const & dir);
  bool InMask(vec3 const &) const;
  DM_Base * Clone() const;

private:
  vec3 m_loa;
};

class DM_S : public DM_Base
{
public:

  DM_S();
  ~DM_S(){}
  DM_Base * Add(vec3 const & dir);
  bool InMask(vec3 const &) const;
  DM_Base * Clone() const;
};

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