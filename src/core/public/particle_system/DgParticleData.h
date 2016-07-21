//! @file DgLineSegment.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: ParticleData

#ifndef PARTICLEDATA_H
#define PARTICLEDATA_H

#include <stdint.h>

#include "../DgVector4.h"
#include "DgVariadicMacros.h"
#include "../impl/DgParticleData_impl.inl"


//! Attribute must be a list on name, type pairs:
//!   #define ATTIBUTES ID, int, Position, Dg::Vector4<Real>, Life, float
//! Be sure there are no spaces after the final '\' on each line.
#define ATTRIBUTES ID,                  int,\
                   Position,            Dg::Vector4<Real>,\
                   Velocity,            Dg::Vector4<Real>,\
                   Acceleration,        Dg::Vector4<Real>,\
                   Force,               Real,\
                   Size,                Real,\
                   StartSize,           Real,\
                   DSize,               Real,\
                   Life,                Real,\
                   LifeMax,             Real,\
                   DLife,               Real,\
                   TimeSinceBirth,      Real,\
                   Color,               Dg::Vector4<float>,\
                   StartColor,          Dg::Vector4<float>,\
                   DColor,              Dg::Vector4<float>

namespace Dg
{
  template<typename Real>
  class ParticleData
  {
  public:

    enum Attr
    {
      //Enum enrty names are taken from ATTRIBUTES names
      ADD_ENUM_ENTRIES(ATTRIBUTES)
      COUNT
    };

  public:

    explicit ParticleData(int a_maxCount);
    ~ParticleData();

    bool IsFull() const { return m_countAlive == m_countMax; }

    int GetCountAlive() const { return m_countAlive; }
    int GetCountMax() const { return m_countMax; }

    bool HasUnusedParticles() const { return m_countAlive < m_countMax; }

    int Kill(int);
    void KillAll();

    //New particles are always added to the end of the list;
    bool Wake(int &);

    void InitAttribute(Attr);
    void DeinitAttribute(Attr);

    void InitAll();
    void DeinitAll();

    //! For each entry in ATTRIBUTES, there exists a Get function.
    //! For example, for the Position attribute:
    //!   Dg::Vector4<Real> * GetPosition();
    ADD_METHODS(ATTRIBUTES)

  private:
    int const            m_countMax;
    int                  m_countAlive;

    //! Members are built from ATTRIBUTES name-type pairs
    ADD_MEMBERS(ATTRIBUTES)
  };

  template<typename Real>
  ParticleData<Real>::ParticleData(int a_maxCount)
    : ADD_MEMBER_CONSTRUCTORS(ATTRIBUTES)
      m_countMax(a_maxCount)
    , m_countAlive(0)
  {
   
  }

  template<typename Real>
  ParticleData<Real>::~ParticleData()
  {
    ADD_MEMBER_DESTRUCTORS(ATTRIBUTES)
  }

  template<typename Real>
  bool ParticleData<Real>::Wake(int & a_index)
  {
    if (m_countAlive == m_countMax)
    {
      return false;
    }

    a_index = m_countAlive;
    ++m_countAlive;
    return true;
  }

  template<typename Real>
  int ParticleData<Real>::Kill(int a_index)
  {
    if (m_countAlive > 0)
    {
      --m_countAlive;

      ADD_KILL_CODE(ATTRIBUTES)
    }

    return m_countAlive;
  }

  template<typename Real>
  void ParticleData<Real>::KillAll()
  {
    m_countAlive = 0;
  }

  template<typename Real>
  void ParticleData<Real>::InitAttribute(Attr a_val)
  {
    switch (a_val)
    {
      ADD_INIT_CODE(ATTRIBUTES)
    }
  }

  template<typename Real>
  void ParticleData<Real>::DeinitAttribute(Attr a_val)
  {
    switch (a_val)
    {
      ADD_DEINIT_CODE(ATTRIBUTES)
    }
  }

  template<typename Real>
  void ParticleData<Real>::InitAll()
  {
    ADD_INITALL_CODE(ATTRIBUTES)
  }

  template<typename Real>
  void ParticleData<Real>::DeinitAll()
  {
    ADD_DEINITALL_CODE(ATTRIBUTES)
  }

}

#endif // !PARTICLEDATA_H