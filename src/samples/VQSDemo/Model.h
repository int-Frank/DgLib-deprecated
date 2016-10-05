#pragma once

#include "Dg_vector.h"
#include "DgVQS.h"
#include "DgMatrix44.h"

typedef float Real;
typedef Dg::Quaternion<Real>  Quat;
typedef Dg::VQS<Real>         VQS;
typedef Dg::Matrix44<Real>    Mat44;
typedef Dg::Vector4<Real>     Vec4;

struct T_Bone
{
  VQS     TPM;    //Model-to-Parent transform
  VQS     TVM;    //Model-to-Camera transform
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
  void Update();

private:
  
  T_Bone *          m_pTBone;
  Dg::vector<Bone>  m_children;
};

class ArmSkeleton
{
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

  void Update(float a_time);
  void SetPose(Pose);

private:

  void InitBoneHeirarchy();
  void InitBoneTransforms();
  void InitDefaultPose();

private:

  float     m_startPoseTime;
  bool      m_newPose;

  Mat44     m_TVM[BONE_COUNT];
  T_Bone    m_tBone[BONE_COUNT];
  Bone *    m_pRoot;
};