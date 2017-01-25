#include "SceneObject.h"

SceneObject::SceneObject()
  : m_modelReference(SceneObject::NoReference)
  , m_T_M_W(vec4::ZeroVector())
  , m_R_M_W(vec4::ZeroVector())
  , m_S_M_W(1.f)
  , m_velocity(vec4::ZeroVector())
  , m_angularRotation(vec4::ZeroVector())
{

}


SceneObject::SceneObject(SceneObject const & a_other)
  : m_modelReference(a_other.m_modelReference)
  , m_T_M_W(a_other.m_T_M_W)
  , m_R_M_W(a_other.m_R_M_W)
  , m_S_M_W(a_other.m_S_M_W)
  , m_velocity(a_other.m_velocity)
  , m_angularRotation(a_other.m_angularRotation)
{

}


SceneObject & SceneObject::operator=(SceneObject const & a_other)
{
  if (this != &a_other)
  {
    m_modelReference = a_other.m_modelReference;
    m_T_M_W = a_other.m_T_M_W;
    m_R_M_W = a_other.m_R_M_W;
    m_S_M_W = a_other.m_S_M_W;
    m_velocity = a_other.m_velocity;
    m_angularRotation = a_other.m_angularRotation;
  }
  return *this;
}


void SceneObject::SetModelReference(int a_id)
{
  m_modelReference = a_id;
}

void SceneObject::SetTranslation(vec4 const & a_vec)
{
  m_T_M_W = a_vec;
}

void SceneObject::SetRotation(float a_rx, float a_ry, float a_rz)
{
  m_R_M_W.Set(a_rx, a_ry, a_rz, 0.f);
}

void SceneObject::SetScale(float a_scale)
{
  m_S_M_W = a_scale;
}

void SceneObject::SetVelocity(vec4 const & a_vec)
{
  m_velocity = a_vec;
}

void SceneObject::SetAngularVelocity(float a_rx, float a_ry, float a_rz)
{
  m_angularRotation.Set(a_rx, a_ry, a_rz, 0.f);
}

mat44 SceneObject::GetMatrix() const
{
  mat44 translate, rotate, scale;
  translate.Translation(m_T_M_W);
  rotate.Rotation(m_R_M_W.x(), m_R_M_W.y(), m_R_M_W.z(),
                  Dg::EulerOrder::ZYX);
  scale.Scaling(m_S_M_W);
  return scale * rotate * translate;
}

void SceneObject::Update(float a_dt)
{
  m_T_M_W += m_velocity * a_dt;
  m_R_M_W += m_angularRotation * a_dt;
}