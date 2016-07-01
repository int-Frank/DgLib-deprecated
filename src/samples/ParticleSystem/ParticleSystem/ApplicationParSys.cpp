#include "Application.h"

#include "AttractorPoint.h"
#include "AttractorPointPulse.h"
#include "AttractorLine.h"
#include "AttractorPlane.h"
#include "AttractorPlaneRepel.h"
#include "AttractorGlobal.h"
#include "EmitterLinear.h"
#include "EmitterPoisson.h"
#include "GenPosPoint.h"
#include "GenVelCone.h"
#include "GenColor.h"
#include "UpdaterColor.h"
#include "UpdaterEuler.h"
#include "UpdaterLife.h"
#include "UpdaterZeroAccel.h"

using namespace Dg;

typedef Dg::Matrix44<float>     mat44;
typedef Dg::Vector4<float>      vec4;
typedef Dg::Quaternion<float>   quat;
typedef Dg::VQS<float>          Vqs;

static EmitterLinear<float> CreateEmitter_1()
{
  Vqs vqs;
  quat q;
  vec4 v;

  //Create Emitter 1
  GenPosPoint<float> genPos;
  v.Set(1.0f, 1.0f, 0.0f, 0.0f);
  vqs.SetV(v);
  genPos.SetTransformation(vqs);
  vqs.Identity();

  GenVelCone<float>  genVel;
  q.SetRotationY(-PI / 2.0f);
  vqs.SetQ(q);
  genVel.SetTransformation(vqs);
  genVel.SetAngle(PI / 8.0f);

  GenColor<float> genColor;
  genColor.SetColors(vec4(1.0f, 1.0f, 1.0f, 1.0f), vec4(1.0f, 0.1f, 0.1f, 0.5f));

  EmitterLinear<float> emitter;
  emitter.SetRate(100.0f);
  emitter.AddGenerator(Application::E_GenPosPoint, genPos);
  emitter.AddGenerator(Application::E_GenVelCone, genVel);
  emitter.AddGenerator(Application::E_GenColor, genColor);

  return emitter;
}

static EmitterLinear<float> CreateEmitter_2()
{
  Vqs vqs;
  quat q;
  vec4 v;

  //Create Emitter 1
  GenPosPoint<float> genPos;
  v.Set(-1.0f, 1.0f, 0.0f, 0.0f);
  vqs.SetV(v);
  genPos.SetTransformation(vqs);
  vqs.Identity();

  GenVelCone<float>  genVel;
  q.SetRotationY(-PI / 2.0f);
  vqs.SetQ(q);
  genVel.SetTransformation(vqs);
  genVel.SetAngle(PI / 8.0f);

  GenColor<float> genColor;
  genColor.SetColors(vec4(1.0f, 1.0f, 1.0f, 1.0f), vec4(0.1f, 1.0f, 0.1f, 0.5f));

  EmitterLinear<float> emitter;
  emitter.SetRate(100.0f);
  emitter.AddGenerator(Application::E_GenPosPoint, genPos);
  emitter.AddGenerator(Application::E_GenVelCone, genVel);
  emitter.AddGenerator(Application::E_GenColor, genColor);

  return emitter;
}

static EmitterLinear<float> CreateEmitter_3()
{
  Vqs vqs;
  quat q;
  vec4 v;

  //Create Emitter 1
  GenPosPoint<float> genPos;
  v.Set(0.0f, -1.0f, 0.0f, 0.0f);
  vqs.SetV(v);
  genPos.SetTransformation(vqs);
  vqs.Identity();

  GenVelCone<float>  genVel;
  q.SetRotationY(-PI / 2.0f);
  vqs.SetQ(q);
  genVel.SetTransformation(vqs);
  genVel.SetAngle(PI / 8.0f);

  GenColor<float> genColor;
  genColor.SetColors(vec4(1.0f, 1.0f, 1.0f, 1.0f), vec4(0.1f, 0.1f, 1.0f, 0.5f));

  EmitterLinear<float> emitter;
  emitter.SetRate(100.0f);
  emitter.AddGenerator(Application::E_GenPosPoint, genPos);
  emitter.AddGenerator(Application::E_GenVelCone, genVel);
  emitter.AddGenerator(Application::E_GenColor, genColor);

  return emitter;
}

static Attractor<float, AttractorForce::Constant> CreateAttractor_1()
{
  Vqs vqs;
  quat q;

  q.SetRotationY(-PI / 2.0f);
  vqs.SetQ(q);
  AttractorGlobal<float, AttractorForce::Constant> attr;
  attr.SetTransformation(vqs);
  attr.SetStrength(1.0f);

  return attr;
}

static Attractor<float, AttractorForce::InverseSquare> CreateAttractor_2()
{
  Vqs vqs;
  vec4 v;

  v.Set(2.0f, 2.0f, 2.0f, 0.0f);
  vqs.SetV(v);
  AttractorPoint<float, AttractorForce::InverseSquare> attr;
  attr.SetTransformation(vqs);
  attr.SetStrength(1.0f);

  return attr;
}

void Application::InitParticleSystem()
{
  //Init Particle Data attributes
  m_particleSystem.InitAllParticleAttr();

  //Add emitters
  m_particleSystem.AddEmitter(E_Emitter_1, CreateEmitter_1());
  m_particleSystem.AddEmitter(E_Emitter_2, CreateEmitter_2());
  m_particleSystem.AddEmitter(E_Emitter_3, CreateEmitter_3());

  //Add Updaters
  m_particleSystem.AddUpdater(E_UpdaterLife, UpdaterLife<float>());
  m_particleSystem.AddUpdater(E_UpdaterZeroAccel, UpdaterZeroAccel<float>());
  m_particleSystem.AddUpdater(E_UpdaterAttractor_1, CreateAttractor_1());
  m_particleSystem.AddUpdater(E_UpdaterAttractor_2, CreateAttractor_2());
  m_particleSystem.AddUpdater(E_UpdaterEuler, UpdaterEuler<float>());
  m_particleSystem.AddUpdater(E_UpdaterColor, UpdaterColor<float>());

}