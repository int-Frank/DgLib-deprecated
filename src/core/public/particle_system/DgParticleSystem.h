//! @file DgParticleSystem.h
//!
//! @author Frank Hart
//! @date 23/07/2016
//!
//! Clas declartion: ParticleSystem

#ifndef DGPARTICLESYSTEM_H
#define DGPARTICLESYSTEM_H

#include "DgParticleEmitter.h"
#include "DgParticleUpdater.h"
#include "DgParticleData.h"
#include "DgAttractor.h"


namespace Dg
{
  //! @ingroup DgEngine_ParticleSystem
  //!
  //! @class ParticleSystem
  //!
  //! Particles systems contain three main components
  //!   - Emitters
  //!   - Updaters
  //!   - Particle data
  //! To set up a particle system:
  //!   - Initialize the particle attributes you will use
  //!   - Add Emitters 
  //!   - Add updaters
  //!
  //! @author Frank Hart
  //! @date 23/07/2016
  template<typename Real>
  class ParticleSystem
  {
  public:
    ParticleSystem(int);
    ~ParticleSystem() {}

    ParticleSystem(ParticleSystem<Real> const & a_other);
    ParticleSystem<Real> & operator=(ParticleSystem<Real> const & a_other);

    //! This function will take control of the emitter.
    //!
    //! @param[in] id ID of the emitter.
    //! @param[in] emitter Particle emitter to add.
    void AddEmitter(int id, ParticleEmitter<Real> * emitter);

    //! This function will take control of the updater.
    //!
    //! @param[in] id ID of the updater.
    //! @param[in] updater Particle updater to add.
    void AddUpdater(int id, ParticleUpdater<Real> * updater);
    
    //! Remove emitter by ID.
    void RemoveEmitter(int id);

    //! Remove Updater by ID.
    void RemoveUpdater(int id);

    //! Initialize particle attribute by id, as define in ParticleData::Attr.
    void InitParticleAttr(int id);
    
    //! Deinitialize particle attribute by id, as define in ParticleData::Attr.
    void DeinitParticleAttr(int);

    //! Initialize all particle attributes.
    void InitAllParticleAttr();

    //! Deinitialize all particle attributes.
    void DeinitAllParticleAttr();

    void StartAllEmitters();
    void StopAllEmitters();

    //! Get Emitter by ID
    ParticleEmitter<Real> * GetEmitter(int id);
    
    //! Get Updater by ID
    ParticleUpdater<Real> * GetUpdater(int);
    
    //! Get Pointer to particle data
    ParticleData<Real> * GetParticleData() { return &m_particleData; }

    //! Update the particle system.
    void Update(Real dt);

    //! Deletes all emitters and updaters, also kills all particles.
    void Clear();

  private:
    Dg::AVLTreeMap<int, ObjectWrapper<ParticleEmitter<Real>>>   m_emitters;
    ParticleData<Real>                                   m_particleData;
    Dg::AVLTreeMap<int, ObjectWrapper<ParticleUpdater<Real>>>   m_updaters;
  };


  //--------------------------------------------------------------------------------
  //	@	ParticleSystem::ParticleSystem()
  //--------------------------------------------------------------------------------
  template<typename Real>
  ParticleSystem<Real>::ParticleSystem(int a_nPar)
    : m_particleData(a_nPar)
    , m_emitters(16)
    , m_updaters(32)
  {

  }	//End: ParticleSystem::ParticleSystem()


  //--------------------------------------------------------------------------------
  //	@	ParticleSystem::ParticleSystem()
  //--------------------------------------------------------------------------------
  template<typename Real>
  ParticleSystem<Real>::ParticleSystem(ParticleSystem<Real> const & a_other) 
    : m_particleData(a_other.m_particleData),
      m_emitters(a_other.m_emitters),
      m_updaters(a_other.m_updaters)
  {

  }	//End: ParticleSystem::ParticleSystem()


  //--------------------------------------------------------------------------------
  //	@	ParticleSystem::operator=()
  //--------------------------------------------------------------------------------
  template<typename Real>
  ParticleSystem<Real> & ParticleSystem<Real>::operator=(ParticleSystem<Real> const & a_other)
  {
    if (this == &a_other)
    {
      return *this;
    }

    m_emitters = a_other.m_emitters;
    m_updaters = a_other.m_updaters;
    m_particleData = a_other.m_particleData;

    return *this;
  }	//End: ParticleSystem::operator=()


  //--------------------------------------------------------------------------------
  //	@	ParticleSystem::AddEmitter()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void ParticleSystem<Real>::AddEmitter(int a_key, ParticleEmitter<Real> * a_pEmitter)
  {
    if (a_pEmitter)
    {
      ObjectWrapper<ParticleEmitter<Real>> newEmitter(a_pEmitter, true);
      m_emitters.insert(a_key, newEmitter);
    }
  }	//End: ParticleSystem::AddEmitter()


  //--------------------------------------------------------------------------------
  //	@	ParticleSystem::AddUpdater()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void ParticleSystem<Real>::AddUpdater(int a_key, ParticleUpdater<Real> * a_pUpdater)
  {
    if (a_pUpdater)
    {
      ObjectWrapper<ParticleUpdater<Real>> newUpdater(a_pUpdater, true);
      m_updaters.insert(a_key, newUpdater);
    }
  }	//End: ParticleSystem::AddUpdater()


  //--------------------------------------------------------------------------------
  //	@	ParticleSystem::RemoveEmitter()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void ParticleSystem<Real>::RemoveEmitter(int a_key)
  {
    m_emitters.erase(a_key);
  }	//End: ParticleSystem::RemoveEmitter()


  //--------------------------------------------------------------------------------
  //	@	ParticleSystem::RemoveUpdater()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void ParticleSystem<Real>::RemoveUpdater(int a_key)
  {
    m_updaters.erase(a_key);
  }	//End: ParticleSystem::RemoveUpdater()


  //--------------------------------------------------------------------------------
  //	@	ParticleSystem::GetEmitter()
  //--------------------------------------------------------------------------------
  template<typename Real>
  ParticleEmitter<Real> * ParticleSystem<Real>::GetEmitter(int a_key)
  {
    auto it = m_emitters.find(a_key);
    if (it != m_emitters.end())
    {
      return it->second;
    }
    return nullptr;
  }	//End: ParticleSystem::GetEmitter()


  //--------------------------------------------------------------------------------
  //	@	ParticleSystem::GetUpdater()
  //--------------------------------------------------------------------------------
  template<typename Real>
  ParticleUpdater<Real> * ParticleSystem<Real>::GetUpdater(int a_key)
  {
    auto it = m_updaters.find(a_key);
    if (it != m_updaters.end())
    {
      return it->second;
    }
    return nullptr;
  }	//End: ParticleSystem::GetUpdater()


  //--------------------------------------------------------------------------------
  //	@	ParticleSystem::InitParticleAttr()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void ParticleSystem<Real>::InitParticleAttr(int a_val)
  {
    m_particleData.InitAttribute(a_val);
  }	//End: ParticleSystem::InitParticleAttr()


  //--------------------------------------------------------------------------------
  //	@	ParticleSystem::DeinitParticleAttr()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void ParticleSystem<Real>::DeinitParticleAttr(int a_val)
  {
    m_particleData.DeinitAttribute(a_val);
  }	//End: ParticleSystem::DeinitParticleAttr()


  //--------------------------------------------------------------------------------
  //	@	ParticleSystem::InitAllParticleAttr()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void ParticleSystem<Real>::InitAllParticleAttr()
  {
    m_particleData.InitAll();
  }	//End: ParticleSystem::InitAllParticleAttr()


  //--------------------------------------------------------------------------------
  //	@	ParticleSystem::DeinitAllParticleAttr()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void ParticleSystem<Real>::DeinitAllParticleAttr()
  {
    m_particleData.DeinitAll();
  }	//End: ParticleSystem::DeinitAllParticleAttr()


  //--------------------------------------------------------------------------------
  //	@	ParticleSystem::StartAllEmitters()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void ParticleSystem<Real>::StartAllEmitters()
  {
    for (auto it = m_emitters.begin_rand(); it != m_emitters.end_rand(); it++)
      it->second->Start();
  }	//End: ParticleSystem::StartAllEmitters()


  //--------------------------------------------------------------------------------
  //	@	ParticleSystem::StopAllEmitters()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void ParticleSystem<Real>::StopAllEmitters()
  {
    for (auto it = m_emitters.begin_rand(); it != m_emitters.end_rand(); it++)
      it->second->Stop();
  }	//End: ParticleSystem::StopAllEmitters()


  //--------------------------------------------------------------------------------
  //	@	ParticleSystem::Clear()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void ParticleSystem<Real>::Clear()
  {
    m_emitters.clear();
    m_updaters.clear();
    m_particleData.KillAll();
  }	//End: ParticleSystem::Clear()


  //--------------------------------------------------------------------------------
  //	@	ParticleSystem::Update()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void ParticleSystem<Real>::Update(Real a_dt)
  {
    //Update all particles
    for (auto it = m_updaters.begin_rand(); it != m_updaters.end_rand(); it++)
      it->second->Update(m_particleData, 0,  a_dt);

    //Emit new particles, keep tally of particles emitted
    //from the various emitters
    int nNewParticles = 0;
    for (auto it = m_emitters.begin_rand(); it != m_emitters.end_rand(); it++)
      nNewParticles += it->second->EmitParticles(m_particleData, a_dt);

    //Update all newly emitted particles. Since new particles are added to the end of the 
    //particle data, we simply update the last set of new particles.
    int startIndex = m_particleData.GetCountAlive() - nNewParticles;
    if (nNewParticles)
    {
      for (auto it = m_updaters.begin_rand(); it != m_updaters.end_rand(); it++)
        it->second->UpdateNew(m_particleData, startIndex, a_dt);
    }
  }	//End: ParticleSystem::Update()
}

#endif
