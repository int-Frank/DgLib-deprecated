#include "SceneObject.h"

SceneObject::SceneObject()
  : m_modelReference(SceneObject::NoReference)
  , m_translation(vec4::Origin())
  , m_rotation(vec4::ZeroVector())
  , m_scale(1.f)
  , m_retardingCoefficient(1.f)
  , m_velocity(vec4::ZeroVector())
  , m_angularRotation(vec4::ZeroVector())
{

}


SceneObject::SceneObject(SceneObject const & a_other)
  : m_modelReference(a_other.m_modelReference)
  , m_translation(a_other.m_translation)
  , m_rotation(a_other.m_rotation)
  , m_scale(a_other.m_scale)
  , m_retardingCoefficient(a_other.m_retardingCoefficient)
  , m_velocity(a_other.m_velocity)
  , m_angularRotation(a_other.m_angularRotation)
{

}


SceneObject & SceneObject::operator=(SceneObject const & a_other)
{
  if (this != &a_other)
  {
    m_modelReference = a_other.m_modelReference;
    m_translation = a_other.m_translation;
    m_rotation = a_other.m_rotation;
    m_scale = a_other.m_scale;
    m_retardingCoefficient = a_other.m_retardingCoefficient;
    m_velocity = a_other.m_velocity;
    m_angularRotation = a_other.m_angularRotation;
  }
  return *this;
}


void SceneObject::SetModelReference(int a_id)
{
  m_modelReference = a_id;
}

void SceneObject::SetPosition(vec4 const & a_vec)
{
  m_translation = a_vec;
  m_translation.w() = 0.f;
}

void SceneObject::SetRotation(float a_rx, float a_ry, float a_rz)
{
  m_rotation.Set(a_rx, a_ry, a_rz, 0.f);
}

void SceneObject::SetScale(float a_scale)
{
  m_scale = a_scale;
}

void SceneObject::SetRetardingCoefficient(float a_val)
{
  m_retardingCoefficient = a_val;
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
  translate.Translation(m_translation);
  rotate.Rotation(m_rotation.x(), m_rotation.y(), m_rotation.z(),
                  Dg::EulerOrder::ZYX);
  scale.Scaling(m_scale);
  return scale * rotate * translate;
}

void SceneObject::Update(float a_dt)
{
  float mult = std::pow(m_retardingCoefficient, a_dt);
  m_velocity *= mult;
  m_angularRotation *= mult;

  m_translation += m_velocity * a_dt;
  m_rotation += m_angularRotation * a_dt;
}