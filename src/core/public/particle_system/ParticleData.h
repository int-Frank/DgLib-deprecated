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
  template<typename Real, uint32_t SavedAttr = 0>
  class ParticleData
  {
  public:

    enum
    {
      ID = 0,
      Position,
      Velocity,
      Acceleration,
      Force,
      Size,
      StartSize,
      EndSize,
      Life,
      Color,
      StartColor,
      EndColor
    };

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

  template<typename Real, uint32_t SavedAttr>
  ParticleData<Real, SavedAttr>::ParticleData(size_t a_maxCount)
    : m_maxCount(a_maxCount),
      m_countAlive(0),
      m_ID(nullptr),
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

  template<typename Real, uint32_t SavedAttr>
  ParticleData<Real, SavedAttr>::~ParticleData()
  {
    delete[] m_ID;
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

  template<typename Real, uint32_t SavedAttr>
  void ParticleData<Real, SavedAttr>::Kill(size_t a_index)
  {
    if (m_countAlive == 0)
    {
      return;
    }

    --m_countAlive;

    //ID
    if (m_ID)
    {
      if (SavedAttr & (1 << ID))
      {
        int temp(m_ID[a_index]);
        m_ID[a_index] = m_ID[m_countAlive];
        m_ID[m_countAlive] = temp;
      }
      else
      {
        m_ID[a_index] = m_ID[m_countAlive];
      }
    }

    //Position
    if (m_position)
    {
      if (SavedAttr & (1 << Position))
      {
        Vector4<Real> temp(m_position[a_index]);
        m_position[a_index] = m_position[m_countAlive];
        m_position[m_countAlive] = temp;
      }
      else
      {
        m_position[a_index] = m_position[m_countAlive];
      }
    }

    //Velocity
    if (m_velocity)
    {
      if (SavedAttr & (1 << Velocity))
      {
        Vector4<Real> temp(m_velocity[a_index]);
        m_velocity[a_index] = m_velocity[m_countAlive];
        m_velocity[m_countAlive] = temp;
      }
      else
      {
        m_velocity[a_index] = m_velocity[m_countAlive];
      }
    }

    //Acceleration
    if (m_acceleration)
    {
      if (SavedAttr & (1 << Acceleration))
      {
        Vector4<Real> temp(m_acceleration[a_index]);
        m_acceleration[a_index] = m_acceleration[m_countAlive];
        m_acceleration[m_countAlive] = temp;
      }
      else
      {
        m_acceleration[a_index] = m_acceleration[m_countAlive];
      }
    }

    //Force
    if (m_force)
    {
      if (SavedAttr & (1 << Force))
      {
        Real temp(m_force[a_index]);
        m_force[a_index] = m_force[m_countAlive];
        m_force[m_countAlive] = temp;
      }
      else
      {
        m_force[a_index] = m_force[m_countAlive];
      }
    }

    //Size
    if (m_size)
    {
      if (SavedAttr & (1 << Size))
      {
        Real temp(m_size[a_index]);
        m_size[a_index] = m_size[m_countAlive];
        m_size[m_countAlive] = temp;
      }
      else
      {
        m_size[a_index] = m_size[m_countAlive];
      }
    }

    //Start size
    if (m_startSize)
    {
      if (SavedAttr & (1 << StartSize))
      {
        Real temp(m_startSize[a_index]);
        m_startSize[a_index] = m_startSize[m_countAlive];
        m_startSize[m_countAlive] = temp;
      }
      else
      {
        m_startSize[a_index] = m_startSize[m_countAlive];
      }
    }

    //End size
    if (m_endSize)
    {
      if (SavedAttr & (1 << EndSize))
      {
        Real temp(m_endSize[a_index]);
        m_endSize[a_index] = m_endSize[m_countAlive];
        m_endSize[m_countAlive] = temp;
      }
      else
      {
        m_endSize[a_index] = m_endSize[m_countAlive];
      }
    }

    //Life
    if (m_life)
    {
      if (SavedAttr & (1 << Life))
      {
        int temp(m_life[a_index]);
        m_life[a_index] = m_life[m_countAlive];
        m_life[m_countAlive] = temp;
      }
      else
      {
        m_life[a_index] = m_life[m_countAlive];
      }
    }

    //Color
    if (m_color)
    {
      if (SavedAttr & (1 << Color))
      {
        Vector4<float> temp(m_color[a_index]);
        m_color[a_index] = m_color[m_countAlive];
        m_color[m_countAlive] = temp;
      }
      else
      {
        m_color[a_index] = m_color[m_countAlive];
      }
    }

    //Start color
    if (m_startColor)
    {
      if (SavedAttr & (1 << StartColor))
      {
        Vector4<float> temp(m_startColor[a_index]);
        m_startColor[a_index] = m_startColor[m_countAlive];
        m_startColor[m_countAlive] = temp;
      }
      else
      {
        m_startColor[a_index] = m_startColor[m_countAlive];
      }
    }

    //End color
    if (m_endColor)
    {
      if (SavedAttr & (1 << EndColor))
      {
        Vector4<float> temp(m_endColor[a_index]);
        m_endColor[a_index] = m_endColor[m_countAlive];
        m_endColor[m_countAlive] = temp;
      }
      else
      {
        m_endColor[a_index] = m_endColor[m_countAlive];
      }
    }
  }
}

#endif // !PARTICLEDATA_H