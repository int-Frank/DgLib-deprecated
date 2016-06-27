//! @file DgLineSegment.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: LineSegment

#ifndef PARTICLEDATA_H
#define PARTICLEDATA_H

#include <stdint.h>

#include "../DgVector4.h"
#include "DgVariadicMacros.h"
#include "../impl/DgParticleData_impl.inl"

#define ATTRIBUTES \
int,                  ID,\
Dg::Vector4<Real>,    Position,\
Dg::Vector4<Real>,    Velocity,\
Dg::Vector4<Real>,    Acceleration,\
Real,                 Force,\
Real,                 Size,\
Real,                 StartSize,\
Real,                 EndSize,\
int,                  Life,\
Dg::Vector4<float>,   Color,\
Dg::Vector4<float>,   StartColor,\
Dg::Vector4<float>,   EndColor



namespace Dg
{
  struct ParticleDataAttributes
  {
    enum
    {
      ADD_ENUM_ENTRIES(ATTRIBUTES)
    };
  };

  template<typename Real, uint32_t SavedAttr = 0>
  class ParticleData
  {
  public:

    explicit ParticleData(size_t a_maxCount);
    ~ParticleData();

    size_t GetCountAlive() const { return m_countAlive; }
    size_t GetCountMax() const { return m_countMax; }

    void Kill(size_t);
    size_t Wake();

    ADD_METHODS(ATTRIBUTES)

  private:
    size_t const            m_maxCount;
    size_t                  m_countAlive;

    ADD_MEMBERS(ATTRIBUTES)
  };

  template<typename Real, uint32_t SavedAttr>
  ParticleData<Real, SavedAttr>::ParticleData(size_t a_maxCount)
    : ADD_MEMBER_CONSTRUCTORS(ATTRIBUTES)
      m_maxCount(a_maxCount),
      m_countAlive(0)
  {
   
  }

  template<typename Real, uint32_t SavedAttr>
  ParticleData<Real, SavedAttr>::~ParticleData()
  {
    ADD_MEMBER_DESTRUCTORS(ATTRIBUTES)
  }

  template<typename Real, uint32_t SavedAttr>
  size_t ParticleData<Real, SavedAttr>::Wake()
  {
    ADD_WAKE_CODE(ATTRIBUTES)

    return m_countAlive++;
  }

  template<typename Real, uint32_t SavedAttr>
  void ParticleData<Real, SavedAttr>::Kill(size_t a_index)
  {
    if (m_countAlive == 0)
    {
      return;
    }

    ADD_KILL_CODE(ATTRIBUTES)

    --m_countAlive;
  }
}

#endif // !PARTICLEDATA_H