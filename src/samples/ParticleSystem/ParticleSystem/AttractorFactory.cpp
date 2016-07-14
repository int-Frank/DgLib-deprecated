#include "AttractorFactory.h"
#include "AttractorGlobal.h"
#include "AttractorPoint.h"
#include "AttractorLine.h"
#include "AttractorPlane.h"
#include "Types.h"


typedef Dg::Vector4<float>      vec4;
typedef Dg::Quaternion<float>   quat;
typedef Dg::VQS<float>          Vqs;

Dg::Attractor<float> * AttractorFactory::Create(AttractorData const & a_data) const
{
  Dg::Attractor<float> * pAttractor(nullptr);
  switch (a_data.shape)
  {
  case E_AttGlobal: pAttractor = new AttractorGlobal<float>(); break;
  case E_AttPoint:  pAttractor = new AttractorPoint<float>(); break;
    //case E_AttLine:   pAttractor = new AttractorLine<float>; break;
    //case E_AttPlane:  pAttractor = new AttractorPlane<float>; break;
  }

  if (pAttractor)
  {
    Vqs vqs;
    quat qy, qz;
    vec4 v;
    v.Set(a_data.transform[0], a_data.transform[1], a_data.transform[2], 0.0f);
    qz.SetRotationZ(a_data.transform[3]);
    qy.SetRotationY(a_data.transform[4] - Dg::PI_f / 2.0f);
    vqs.SetQ(qy * qz);
    vqs.SetV(v);
    vqs.SetS(a_data.transform[5]);

    pAttractor->SetTransformation(vqs);
    pAttractor->SetAccelType(a_data.forceType);
    pAttractor->SetMaxAccelMagnitude(a_data.maxAccelMag);
    pAttractor->SetStrength(a_data.strength);
  }

  return pAttractor;
}
