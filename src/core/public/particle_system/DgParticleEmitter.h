//! @file DgParticleEmitter.h
//!
//! @author Frank Hart
//! @date 4/10/2015
//!
//! Class declaration: ParticleEmitter

#ifndef DGPARTICLEEMITTER_H
#define DGPARTICLEEMITTER_H

#include "Dg_map.h"
#include "DgObjectWrapper.h"
#include "DgParticleGenerator.h"

namespace Dg
{
  template<typename Real>
  class ParticleData;

  //! @ingroup DgEngine_ParticleSystem
  //!
  //! @class ParticleEmitter
  //!
  //! Particle Emitters are responsible for birthing particles in a particle system.
  //! They are made up of one or more generators.
  //!
  //! @author Frank Hart
  //! @date 22/07/2016
  template<typename Real>
  class ParticleEmitter : public Object<ParticleEmitter<Real>>
  {
  public:
    ParticleEmitter(): m_isOn(false), m_generators(32){}
    virtual ~ParticleEmitter() {}

    ParticleEmitter(ParticleEmitter<Real> const & a_other)
      : m_isOn(a_other.m_isOn)
      , m_generators(a_other.m_generators){}

    ParticleEmitter<Real> & operator=(ParticleEmitter<Real> const & a_other) 
    { 
      m_isOn = a_other.m_isOn;
      m_generators = a_other.m_generators;
      return *this; 
    }

    //! Emission rate (per sec)
    virtual void SetRate(Real) {}

    //! This function will calculate how many particles should be emitted
    //! based of the emission rate and input time. It will then iterate over
    //! all new particles with any generators it contains.
    virtual int EmitParticles(ParticleData<Real> & data, Real dt) { return 0; }

    //! Set the emitter to start producing particles.
    void Start() { m_isOn = true; }

    //! Stop producing particles.
    void Stop() { m_isOn = false; }

    //! Toggle on state.
    void ToggleOn() { m_isOn = !m_isOn; }

    //! Query on state.
    bool IsOn() const { return m_isOn; }

    //! Add a generator to the internal list of generators. The Emitter takes ownership of
    //! the input generator.
    //! 
    //! @param id Identifier - must be unique to this emitter
    //! @param pGen Pointer to the generator to add.
    void AddGenerator(int id, ParticleGenerator<Real> * pGen);
    
    //! Remove generator by ID
    void RemoveGenerator(int id);

    //! Get a pointer to an internal generator.
    //!
    //! @return nullptr is id not found.
    ParticleGenerator<Real> * GetGenerator(int id);

    //! Create a deep copy of this object.
    virtual ParticleEmitter<Real> * Clone() const { return new ParticleEmitter<Real>(*this); }

  protected:
    Dg::map<int, ObjectWrapper<ParticleGenerator<Real>>>   m_generators;

  private:
    bool m_isOn;
  };


  //--------------------------------------------------------------------------------
  //	@	ParticleEmitter::AddGenerator()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void ParticleEmitter<Real>::AddGenerator(int a_key, ParticleGenerator<Real> * a_pGen)
  {
    if (a_pGen)
    {
      ObjectWrapper<ParticleGenerator<Real>> newGenerator(a_pGen, true);
      m_generators.insert(a_key, newGenerator);
    }
  } //End: ParticleEmitter::AddGenerator()


  //--------------------------------------------------------------------------------
  //	@	ParticleEmitter::RemoveGenerator()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void ParticleEmitter<Real>::RemoveGenerator(int a_key)
  {
    m_generators.erase(a_key);
  } //End: ParticleEmitter::RemoveGenerator()


  //--------------------------------------------------------------------------------
  //	@	ParticleEmitter::GetGenerator()
  //--------------------------------------------------------------------------------
  template<typename Real>
  ParticleGenerator<Real> * ParticleEmitter<Real>::GetGenerator(int a_key)
  {
    int index = 0;
    if (!m_generators.find(a_key, index))
    {
      return nullptr;
    }
    return m_generators[index];
  } //End: ParticleEmitter::GetGenerator()
}

#endif
