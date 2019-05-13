#include "DirectionMask.h"
#include "DgMath.h"


//------------------------------------------------------------------------------------------
// States...
//------------------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------------------
// DM_None
//------------------------------------------------------------------------------------------
bool DM_None::InMask(vec3 const & a_vec) const
{
  return false;
}

DM_Base *  DM_None::Add(vec3 const & a_vec)
{
  return new DM_D(a_vec);
}

DM_Base *  DM_None::Clone() const
{
  return new DM_None();
}

//------------------------------------------------------------------------------------------
// DM_D
//------------------------------------------------------------------------------------------
DM_D::DM_D(vec3 const & a_vec)
  : m_dir(a_vec)
{

}

bool DM_D::InMask(vec3 const & a_vec) const
{
  return a_vec == m_dir;
}

DM_Base *  DM_D::Add(vec3 const & a_vec)
{
  if (a_vec == m_dir)
  {
    return nullptr;
  }

  if (a_vec == -m_dir)
  {
    return new DM_LR(m_dir);
  }

  if (m_dir.PerpDot(a_vec) < 0.0f)
  {
    return new DM_LDR(m_dir, a_vec);
  }

  return new DM_LDR(a_vec, m_dir);
}

DM_Base *  DM_D::Clone() const
{
  return new DM_D(m_dir);
}

//------------------------------------------------------------------------------------------
// DM_LDR
//------------------------------------------------------------------------------------------
DM_LDR::DM_LDR(vec3 const & a_loa, vec3 const & a_roa)
  : m_loa(a_loa)
  , m_roa(a_roa)
  , m_dir(vec3::ZeroVector())
  , m_proj(0.0f)
{
  Set();
}

bool DM_LDR::InMask(vec3 const & a_vec) const
{
  float epsilon = 0.0001f;
  return m_dir.Dot(a_vec) > (m_proj - epsilon);
}

DM_Base *  DM_LDR::Add(vec3 const & a_vec)
{
  float lPv = m_loa.PerpDot(a_vec);
  float rPv = m_roa.PerpDot(a_vec);

  if (lPv > 0.0f)
  {
    if (rPv >= 0.0f)
    {
      m_loa = a_vec;
      Set();
      return nullptr;
    }
    return new DM_S();
  }
  if (rPv < 0.0f)
  {
    if (lPv <= 0.0f)
    {
      m_roa = a_vec;
      Set();
      return nullptr;
    }
    return new DM_S();
  }
  return nullptr;
}

void DM_LDR::Set()
{
  if (m_loa == -m_roa)
  {
    m_dir.Set(m_loa.y(), -m_loa.x(), 0.0f);
    m_proj = 0.0f;
  }
  else
  {
    m_dir = m_loa + m_roa;
    m_dir.Normalize();
    m_proj = m_loa.Dot(m_dir);
  }
}

DM_Base * DM_LDR::Clone() const
{
  return new DM_LDR(m_loa, m_roa);
}

//------------------------------------------------------------------------------------------
// DM_LR
//------------------------------------------------------------------------------------------
DM_LR::DM_LR(vec3 const & a_loa)
  : m_loa(a_loa)
{
}

bool DM_LR::InMask(vec3 const & a_vec) const
{
  return (a_vec == m_loa) || (a_vec == -m_loa);
}

DM_Base *  DM_LR::Add(vec3 const & a_vec)
{
  if (a_vec == m_loa || a_vec == -m_loa)
  {
    return nullptr;
  }

  float lPv = m_loa.PerpDot(a_vec);

  if (lPv > 0.0f)
  {
    return new DM_LRD(m_loa);
  }
  
  return new DM_LDR(m_loa, -m_loa);
}

DM_Base *  DM_LR::Clone() const
{
  return new DM_LR(m_loa);
}

//------------------------------------------------------------------------------------------
// DM_LRD
//------------------------------------------------------------------------------------------
DM_LRD::DM_LRD(vec3 const & a_loa)
  : m_loa(a_loa)
{
}

bool DM_LRD::InMask(vec3 const & a_vec) const
{
  float lPv = m_loa.PerpDot(a_vec);

  return lPv >= 0.0f;
}

DM_Base *  DM_LRD::Add(vec3 const & a_vec)
{
  float lPv = m_loa.PerpDot(a_vec);

  if (lPv >= 0.0f)
  {
    return nullptr;
  }

  return new DM_S();
}

DM_Base *  DM_LRD::Clone() const
{
  return new DM_LRD(m_loa);
}

//------------------------------------------------------------------------------------------
// DM_S
//------------------------------------------------------------------------------------------
DM_S::DM_S()
{
}

bool DM_S::InMask(vec3 const & a_vec) const
{
  return true;
}

DM_Base *  DM_S::Add(vec3 const & a_vec)
{
  return nullptr;
}

DM_Base *  DM_S::Clone() const
{
  return new DM_S();
}

//------------------------------------------------------------------------------------------
// DirMask
//------------------------------------------------------------------------------------------
DirMask::DirMask()
  : m_pState(new DM_None())
{

}

DirMask::DirMask(DirMask const & a_other)
  : m_pState(a_other.m_pState->Clone())
{

}

DirMask & DirMask::operator=(DirMask const & a_other)
{
  if (this != &a_other)
  {
    delete m_pState;
    m_pState = a_other.m_pState->Clone();
  }
  return *this;
}

DirMask::~DirMask()
{
  delete m_pState;
}

void DirMask::Add(vec3 const & a_vec)
{
  DM_Base * pNewState = m_pState->Add(a_vec);
  if (pNewState != nullptr)
  {
    delete m_pState;
    m_pState = pNewState;
  }
}

bool DirMask::InMask(vec3 const & a_vec) const
{
  return m_pState->InMask(a_vec);
}

void DirMask::Reset()
{
  delete m_pState;
  m_pState = new DM_None();
}