#ifndef DGATTRACTOR_H
#define DGATTRACTOR_H

#include "..\DgVector4.h"
#include "..\DgObject.h"

namespace Dg
{
  template<typename Real>
  class Attractor : public Object<Attractor<Real>>
  {
  public:

    Attractor();
    virtual ~Attractor() {}

    Attractor(Attractor<Real> const &);
    Attractor<Real> & operator=(Attractor<Real> const &);

    virtual void UpdateStrength(Real dt) {}

    void SetPosition(Vector4<Real> const &);
    void SetStrength(Real);

    Vector4<Real> const & GetPosition() const { return m_position; }
    Real GetStrength() const { return m_strength; }

    Attractor* Clone() const { return new Attractor(*this); }

  protected:
    Vector4<Real> m_position;
    Real          m_strength;
  };

  template<typename Real>
  Attractor<Real>::Attractor()
    : m_position(Vector4<Real>::Origin()), m_strength(static_cast<Real>(0.0))
  {

  }

  template<typename Real>
  Attractor<Real>::Attractor(Attractor<Real> const & a_other)
    : m_position(a_other.m_position), m_strength(a_other.m_strength)
  {

  }

  template<typename Real>
  Attractor<Real> & Attractor<Real>::operator=(Attractor<Real> const & a_other)
  {
    m_position = a_other.m_position;
    m_strength = a_other.m_strength;

    return *this;
  }


  template<typename Real>
  void Attractor<Real>::SetPosition(Vector4<Real> const & a_p)
  {
    m_position = a_p;
    m_position.w() = static_cast<Real>(1.0);
  }

  template<typename Real>
  void Attractor<Real>::SetStrength(Real a_s)
  {
    m_strength = a_s;
  }
}

#endif