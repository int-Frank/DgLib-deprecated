#ifndef PULSE_H
#define PULSE_H

#include <cmath>
#include "DgMath.h"

template<typename Real>
class Pulse
{
public:

  Pulse()
    : m_isOn(false)
    , m_amplitude(static_cast<Real>(1.0))
    , m_frequency(static_cast<Real>(1.0))
    , m_time(static_cast<Real>(0.0))
    , m_offset(static_cast<Real>(0.0))
  {}

  virtual ~Pulse() {}

  Pulse(Pulse<Real> const & a_other)
    : m_isOn(a_other.m_isOn)
    , m_amplitude(a_other.m_amplitude)
    , m_frequency(a_other.m_frequency)
    , m_time(a_other.m_time)
    , m_offset(a_other.m_offset)
  {}

  Pulse<Real> & operator=(Pulse<Real> const &);

  void SetOnState(bool a_state) { m_isOn = a_state; }
  void GetOnState() const { return m_isOn; }

  void SetOffset(Real offset) { m_offset = offset; }
  void SetAmplitute(Real ampl) { m_amplitude = ampl; }
  void SetPeriod(Real);
  void Reset() { m_time = static_cast<Real>(0.0); }
  void Update(Real dt);
  Real GetCurrentValue() const;

private:
  bool    m_isOn;
  Real    m_time;
  Real    m_amplitude;
  Real    m_freq;
  Real    m_offset;
};

template<typename Real>
Pulse<Real> & Pulse<Real>::operator=(Pulse<Real> const & a_other)
{
  m_isOn = a_other.m_isOn;
  m_time = a_other.m_time;
  m_amplitude = a_other.m_amplitude;
  m_freq = a_other.m_freq;
  m_offset = a_other.m_offset;
  return *this;
}

template<typename Real>
void Pulse<Real>::SetPeriod(Real a_per)
{
  if (Dg::IsZero(a_per))
  {
    a_per = static_cast<Real>(1.0);
  }
  m_freq = static_cast<Real>(1.0) / a_per;
}

template<typename Real>
void Pulse<Real>::Update(Real a_dt)
{
  m_time += a_dt;
  Dg::WrapAngle(m_time);
}

template<typename Real>
Real Pulse<Real>::GetCurrentValue() const
{
  return m_amplitude * sin(mm_time * static_cast<Real>(2.0 * Dg::Constants<double>::PI) * m_freq);
}
#endif