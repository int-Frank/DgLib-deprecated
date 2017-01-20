#ifndef MODELSCENE_H
#define MODELSCENE_H

#include "Types.h"

class SceneObject
{
public:

  enum
  {
    NoReference = -1
  };

public:

  SceneObject();
  ~SceneObject() {}

  SceneObject(SceneObject const &);
  SceneObject & operator=(SceneObject const &);

  void SetModelReference(int);
  void SetPosition(vec4 const &);
  void SetRotation(float, float, float);
  void SetScale(float);

  void SetRetardingCoefficient(float);
  void SetVelocity(vec4 const &);
  void SetAngularVelocity(float, float, float);

  mat44 GetMatrix() const;
  int GetModelReference() const { return m_modelReference; }

  void Update(float dt);

private:

  int   m_modelReference;
  
  vec4  m_translation;
  vec4  m_rotation;
  float m_scale;

  float m_retardingCoefficient;
  vec4  m_velocity;
  vec4  m_angularRotation;
};

#endif