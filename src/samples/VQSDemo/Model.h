#pragma once

#include "DgDynamicArray.h"
#include "DgR3VQS.h"
#include "DgR3Matrix.h"
#include "./json/json.h"

typedef float Real;
typedef Dg::R3::Quaternion<Real>  Quat;
typedef Dg::R3::VQS<Real>         VQS;
typedef Dg::R3::Matrix<Real>    Mat44;
typedef Dg::R3::Vector<Real>     Vec4;

struct T_Bone
{
  VQS     T_MP;    //Model-to-Parent transform
  VQS     T_MV;    //Model-to-View transform
  Quat    qBegin;
  Quat    qEnd;
};

class Bone
{
public:

  Bone(T_Bone * a_pTBone)
    : m_pTBone(a_pTBone)
  {}

  ~Bone() {}

  Bone(Bone const & a_other)
    : m_pTBone(a_other.m_pTBone)
    , m_children(a_other.m_children)
  {}

  void AddChild(Bone const & a_bone) { m_children.push_back(a_bone); }
  void Update(VQS const & a_TVP);

private:
  
  T_Bone *          m_pTBone;
  Dg::DynamicArray<Bone>  m_children;
};

class ArmSkeleton
{
public:
  enum BoneID
  {
    //Humerus
    H,

    //Forearm
    F,

    //Thumb
    T0,
    T1,
    T2,

    //Index finger
    I0,
    I1,
    I2,
    I3,

    //Middle finger
    M0,
    M1,
    M2,
    M3,

    //Ring finger
    R0,
    R1,
    R2,
    R3,

    //Little finger
    L0,
    L1,
    L2,
    L3,

    BONE_COUNT
  };

public:

  enum Pose
  {
    Relaxed,
    Muscle,
    Salute,
    ThumbsUp,
    ThumbsDown,
    OK,
    Stop
  };

public:

  ArmSkeleton();
  ~ArmSkeleton();

  void Update(float a_dt);
  void SetTransform(VQS const &);
  void SetMatrices(Mat44[BONE_COUNT]);
  void ShowUI();

private:

  void InitBoneHeirarchy();
  void InitBoneTransforms();
  float GetRunTime() const;

private:

  float const m_changeTime = 1.5f;

private:

  Json::Value               m_jsRoot;

  float     m_time;
  bool      m_newPose;

  Mat44     m_TVM[BONE_COUNT];
  T_Bone    m_tBone[BONE_COUNT];
  Bone *    m_pRoot;
};