//! @file DgLineSegment.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: LineSegment

#ifndef PARTICLEDATA_H
#define PARTICLEDATA_H

#include "../DgVector4.h"

namespace Dg
{
  template<typename Real>
  class ParticleData
  {
  public:

    Dg::Vector4<Real> * m_position;
    Dg::Vector4<Real> * m_velocity;
    Dg::Vector4<Real> * m_acceleration;
    Real              * m_force;
    Real              * m_size;
    Real              * m_startSize;
    Real              * m_endSize;
    int               * m_life;
    Dg::Vector4<Real> * m_color;
    Dg::Vector4<Real> * m_startColor;
    Dg::Vector4<Real> * m_endColor;

  public:

    explicit ParticleData(size_t a_maxCount);
    ~ParticleData();

    size_t GetCountAlive() const { return m_countAlive; }

  private:
    size_t const            m_maxCount;
    size_t                  m_countAlive;
  };

  template<typename Real>
  ParticleData<Real>::ParticleData(size_t a_maxCount)
    : m_maxCount(a_maxCount),
      m_countAlive(0),
      m_position(nullptr),
      m_velocity(nullptr),
      m_acceleration(nullptr),
      m_force(nullptr),
      m_size(nullptr),
      m_startSize(nullptr),
      m_endSize(nullptr),
      m_life(nullptr),
      m_color(nullptr),
      m_startColor(nullptr),
      m_endColor(nullptr)
  {
   
  }

  template<typename Real>
  ParticleData<Real>::~ParticleData()
  {
    delete[] m_position;
    delete[] m_velocity;
    delete[] m_acceleration;
    delete[] m_force;
    delete[] m_size;
    delete[] m_startSize;
    delete[] m_endSize;
    delete[] m_life;
    delete[] m_color;
    delete[] m_startColor;
    delete[] m_endColor;
  }

}

#endif // !PARTICLEDATA_H