//! @file DgParticleData.h
//!
//! @author: Frank B. Hart
//! @date 22/07/2016
//!
//! Class declaration: ParticleData

#ifndef PARTICLEDATA_H
#define PARTICLEDATA_H

#include <stdint.h>

#include "../DgR3Vector4.h"
#include "DgVariadicMacros.h"
#include "../impl/DgParticleData_impl.inl"


//! Attribute must be a list on name, type pairs:
//!
//!   \#define ATTIBUTES ID, int, Position, Dg::Vector4<Real>, Life, float
//!
//! Be sure there are no spaces after the final '\' on each line.
#define ATTRIBUTES ID,                  int,\
                   Position,            Dg::R3::Vector4<Real>,\
                   Velocity,            Dg::R3::Vector4<Real>,\
                   Acceleration,        Dg::R3::Vector4<Real>,\
                   Force,               Real,\
                   Size,                Real,\
                   StartSize,           Real,\
                   DSize,               Real,\
                   Life,                Real,\
                   LifeMax,             Real,\
                   DLife,               Real,\
                   TimeSinceBirth,      Real,\
                   Color,               Dg::R3::Vector4<float>,\
                   StartColor,          Dg::R3::Vector4<float>,\
                   DColor,              Dg::R3::Vector4<float>


namespace Dg
{
  //! @ingroup DgEngine_ParticleSystem
  //!
  //! @class ParticleData
  //!
  //! A particle is an aggregation on attributes. The data is kept as an SoA.
  //! Particle attributes are defined in the ATTRIBUTES macro.
  //!
  //! @author Frank Hart
  //! @date 22/07/2016
  template<typename Real>
  class ParticleData
  {
  public:

    //! Enum enrty names are taken from ATTRIBUTES names
    enum Attr
    {
      ADD_ENUM_ENTRIES(ATTRIBUTES)
      COUNT
    };

  public:

    ParticleData(int a_maxCount);
    ~ParticleData();

    //! Are all available particles active?
    bool IsFull() const { return m_countAlive == m_countMax; }

    //! Get the number of particles currently active.
    int GetCountAlive() const { return m_countAlive; }

    //! Get the maximum number of particles the ParticleData object can hold.
    int GetCountMax() const { return m_countMax; }

    //! Are there any unused particles?
    bool HasUnusedParticles() const { return m_countAlive < m_countMax; }

    //! Kill a particle at index. 
    int Kill(int);

    //! Kill all particles.
    void KillAll();

    //! Request to wake a particle. New particles are located at the end of the lists.
    //!
    //! @param[in] index Index of the new particle.
    //! @return true if there are still available particles.
    bool Wake(int & index);

    //! Initialize a particular attribute.
    void InitAttribute(Attr);

    //! Deinitialize a particular attribute.
    void DeinitAttribute(Attr);

    //! Initialise all attributes.
    void InitAll();

    //! Deinitialise all attributes.
    void DeinitAll();

    //! For each entry in ATTRIBUTES, there exists a Get function.
    //! For example, for the Position attribute:
    //!   Dg::Vector4<Real> * GetPosition();
    //!  An uninitialized attributed will return a null pointer.
    ADD_METHODS(ATTRIBUTES)

  private:
    int const            m_countMax;
    int                  m_countAlive;

    //! Members are built from ATTRIBUTES name-type pairs
    ADD_MEMBERS(ATTRIBUTES)
  };


  //--------------------------------------------------------------------------------
  //	@	ParticleData::ParticleData()
  //--------------------------------------------------------------------------------
  template<typename Real>
  ParticleData<Real>::ParticleData(int a_maxCount)
    : ADD_MEMBER_CONSTRUCTORS(ATTRIBUTES)
      m_countMax(a_maxCount)
    , m_countAlive(0)
  {
   
  }	//End: ParticleData::ParticleData()


  //--------------------------------------------------------------------------------
  //	@	ParticleData::~ParticleData()
  //--------------------------------------------------------------------------------
  template<typename Real>
  ParticleData<Real>::~ParticleData()
  {
    ADD_MEMBER_DESTRUCTORS(ATTRIBUTES)
  }	//End: ParticleData::~ParticleData()


  //--------------------------------------------------------------------------------
  //	@	ParticleData::Wake()
  //--------------------------------------------------------------------------------
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
  }	//End: ParticleData::Wake()


  //--------------------------------------------------------------------------------
  //	@	ParticleData::Kill()
  //--------------------------------------------------------------------------------
  template<typename Real>
  int ParticleData<Real>::Kill(int a_index)
  {
    if (m_countAlive > 0)
    {
      --m_countAlive;

      ADD_KILL_CODE(ATTRIBUTES)
    }

    return m_countAlive;
  }	//End: ParticleData::Kill()


  //--------------------------------------------------------------------------------
  //	@	ParticleData::KillAll()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void ParticleData<Real>::KillAll()
  {
    m_countAlive = 0;
  }	//End: ParticleData::KillAll()


  //--------------------------------------------------------------------------------
  //	@	ParticleData::InitAttribute()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void ParticleData<Real>::InitAttribute(Attr a_val)
  {
    switch (a_val)
    {
      ADD_INIT_CODE(ATTRIBUTES)
    }
  }	//End: ParticleData::InitAttribute()


  //--------------------------------------------------------------------------------
  //	@	ParticleData::DeinitAttribute()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void ParticleData<Real>::DeinitAttribute(Attr a_val)
  {
    switch (a_val)
    {
      ADD_DEINIT_CODE(ATTRIBUTES)
    }
  }	//End: ParticleData::DeinitAttribute()


  //--------------------------------------------------------------------------------
  //	@	ParticleData::InitAll()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void ParticleData<Real>::InitAll()
  {
    ADD_INITALL_CODE(ATTRIBUTES)
  }	//End: ParticleData::InitAll()


  //--------------------------------------------------------------------------------
  //	@	ParticleData::DeinitAll()
  //--------------------------------------------------------------------------------
  template<typename Real>
  void ParticleData<Real>::DeinitAll()
  {
    ADD_DEINITALL_CODE(ATTRIBUTES)
  }	//End: ParticleData::DeinitAll()

}

#endif // !PARTICLEDATA_H