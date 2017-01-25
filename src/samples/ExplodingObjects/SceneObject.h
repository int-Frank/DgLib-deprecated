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
  void SetTranslation(vec4 const &);
  void SetRotation(float, float, float);
  void SetScale(float);

  void SetVelocity(vec4 const &);
  void SetAngularVelocity(float, float, float);

  mat44 GetMatrix() const;
  int GetModelReference() const { return m_modelReference; }

  void Update(float dt);

private:

  int   m_modelReference;
  
  vec4  m_centroid;

  vec4  m_T_M_W;
  vec4  m_R_M_W;
  float m_S_M_W;

  vec4  m_velocity;
  vec4  m_angularRotation;
};

#endif