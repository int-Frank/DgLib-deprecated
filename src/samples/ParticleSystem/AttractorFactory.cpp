#include "AttractorFactory.h"
#include "AttractorGlobal.h"
#include "AttractorPoint.h"
#include "AttractorLine.h"
#include "AttractorPlane.h"
#include "Types.h"


typedef Dg::R3::Vector<float>      vec4;
typedef Dg::R3::Quaternion<float>   quat;
typedef Dg::R3::VQS<float>          Vqs;

Dg::Attractor<float> * AttractorFactory::operator()(AttractorData const & a_data) const
{
  Dg::Attractor<float> * pAttractor(nullptr);
  switch (a_data.type)
  {
  case E_AttGlobal: pAttractor = new AttractorGlobal<float>(); break;
  case E_AttPoint:  pAttractor = new AttractorPoint<float>(); break;
  case E_AttLine:   pAttractor = new AttractorLine<float>; break;
  case E_AttPlane:  pAttractor = new AttractorPlane<float>; break;
  }

  if (pAttractor)
  {
    Vqs vqs;
    quat qy, qz;
    vec4 v;
    v.Set(a_data.transform[0], a_data.transform[1], a_data.transform[2], 0.0f);
    qz.SetRotationZ(a_data.transform[3]);
    qy.SetRotationY(a_data.transform[4] - Dg::Constants<float>::PI / 2.0f);
    vqs.SetQ(qy * qz);
    vqs.SetV(v);
    vqs.SetS(a_data.transform[5]);

    pAttractor->SetTransformation(vqs);
    pAttractor->SetAttenuationMethod(a_data.attenuationMethod);
    pAttractor->SetMaxAppliedAccelMagnitude(a_data.maxAppliedAccelMag);
    pAttractor->SetStrength(a_data.strength);
  }

  return pAttractor;
}
