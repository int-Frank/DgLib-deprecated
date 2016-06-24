//! @file DgLineSegment.h
//!
//! @author: Frank B. Hart
//! @date 29/05/2016
//!
//! Class declaration: LineSegment

#ifndef PARTICLEDATA_H
#define PARTICLEDATA_H

#include "../DgVector4.h"
#include <stdint.h>

namespace Dg
{
  //! Adding / Removing Attributes SHOULD ONLY be done through the interface.
  //!
  //! Template arguments:
  //!   Real: number type
  //!   Attr: 
  template<typename Real, uint32_t Attr, uint32_t SavedAttr>
  class ParticleData
  {
  public:

    int                   * m_ID;
    Dg::Vector4<Real>     * m_position;
    Dg::Vector4<Real>     * m_velocity;
    Dg::Vector4<Real>     * m_acceleration;
    Real                  * m_force;
    Real                  * m_size;
    Real                  * m_startSize;
    Real                  * m_endSize;
    int                   * m_life;
    Dg::Vector4<float>    * m_color;
    Dg::Vector4<float>    * m_startColor;
    Dg::Vector4<float>    * m_endColor;

  public:

    explicit ParticleData(size_t a_maxCount);
    ~ParticleData();

    size_t GetCountAlive() const { return m_countAlive; }
    size_t GetCountMax() const { return m_countMax; }

    void Kill(size_t);
    size_t Wake();

    void InitID()           { (m_ID == nullptr) ? m_ID = new int[m_maxCount]; }
    void InitPosition()     { (m_position == nullptr) ? m_position = new Vector4<Real>[m_maxCount]; }
    void InitVelocity()     { (m_velocity == nullptr) ? m_velocity = new Vector4<Real>[m_maxCount]; }
    void InitAcceleration() { (m_acceleration == nullptr) ? m_acceleration = new Vector4<Real>[m_maxCount]; }
    void InitForce()        { (m_force == nullptr) ? m_force = new Real[m_maxCount]; }
    void InitSize()         { (m_size == nullptr) ? m_size = new Real[m_maxCount]; }
    void InitStartSize()    { (m_startSize == nullptr) ? m_startSize = new Real[m_maxCount]; }
    void InitEndSize()      { (m_endSize == nullptr) ? m_endSize = new Real[m_maxCount]; }
    void InitLife()         { (m_life == nullptr) ? m_life = new int[m_maxCount]; }
    void InitColor()        { (m_color == nullptr) ? m_color = new Vector4<float>[m_maxCount]; }
    void InitStartColor()   { (m_startColor == nullptr) ? m_startColor = new Vector4<float>[m_maxCount]; }
    void InitEndColor()     { (m_endColor == nullptr) ? m_endColor = new Vector4<float>[m_maxCount]; }

    void DeinitID()           { delete[] m_ID; m_ID = nullptr;}
    void DeinitPosition()     { delete[] m_position; m_position = nullptr;}
    void DeinitVelocity()     { delete[] m_velocity; m_velocity = nullptr;}
    void DeinitAcceleration() { delete[] m_acceleration; m_acceleration = nullptr;}
    void DeinitForce()        { delete[] m_force; m_force = nullptr;}
    void DeinitSize()         { delete[] m_size; m_size = nullptr;}
    void DeinitStartSize()    { delete[] m_startSize; m_startSize = nullptr;}
    void DeinitEndSize()      { delete[] m_endSize; m_endSize = nullptr;}
    void DeinitLife()         { delete[] m_life; m_life = nullptr;}
    void DeinitColor()        { delete[] m_color; m_color = nullptr;}
    void DeinitStartColor()   { delete[] m_startColor; m_startColor = nullptr;}
    void DeinitEndColor()     { delete[] m_endColor; m_endColor = nullptr;}

  private:
    size_t const            m_maxCount;
    size_t                  m_countAlive;
  };

  template<typename Real, uint32_t Opts>
  ParticleData<Real, Opts>::ParticleData(size_t a_maxCount)
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

  template<typename Real, uint32_t Opts>
  ParticleData<Real, Opts>::~ParticleData()
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