#include "Model.h"

#define rad(x) x / 180 * 3.14159f

ArmSkeleton::ArmSkeleton()
  : m_pRoot(nullptr)
{
  m_pRoot = new Bone(&m_tBone[H]);
  InitBoneHeirarchy();
  InitBoneTransforms();
}

ArmSkeleton::~ArmSkeleton()
{
  delete m_pRoot;
}

void ArmSkeleton::InitBoneHeirarchy()
{
  Bone bF(&m_tBone[F]);

  Bone bT0(&m_tBone[T0]);
  Bone bT1(&m_tBone[T1]);
  Bone bT2(&m_tBone[T2]);

  Bone bI0(&m_tBone[I0]);
  Bone bI1(&m_tBone[I1]);
  Bone bI2(&m_tBone[I2]);
  Bone bI3(&m_tBone[I3]);

  Bone bM0(&m_tBone[M0]);
  Bone bM1(&m_tBone[M1]);
  Bone bM2(&m_tBone[M2]);
  Bone bM3(&m_tBone[M3]);

  Bone bR0(&m_tBone[R0]);
  Bone bR1(&m_tBone[R1]);
  Bone bR2(&m_tBone[R2]);
  Bone bR3(&m_tBone[R3]);

  Bone bL0(&m_tBone[L0]);
  Bone bL1(&m_tBone[L1]);
  Bone bL2(&m_tBone[L2]);
  Bone bL3(&m_tBone[L3]);

  bT1.AddChild(bT2);
  bT0.AddChild(bT1);

  bI2.AddChild(bI3);
  bI1.AddChild(bI2);
  bI0.AddChild(bI1);

  bM2.AddChild(bM3);
  bM1.AddChild(bM2);
  bM0.AddChild(bM1);

  bR2.AddChild(bR3);
  bR1.AddChild(bR2);
  bR0.AddChild(bR1);

  bL2.AddChild(bL3);
  bL1.AddChild(bL2);
  bL0.AddChild(bL1);

  bF.AddChild(bT0);
  bF.AddChild(bI0);
  bF.AddChild(bM0);
  bF.AddChild(bR0);
  bF.AddChild(bL0);

  m_pRoot->AddChild(bF);
}

void ArmSkeleton::InitBoneTransforms()
{
  //Sizes
  m_tBone[F].T_MP.SetS(0.25f / 0.25f);
  
  m_tBone[T0].T_MP.SetS(0.07f / 0.25f);
  m_tBone[T1].T_MP.SetS(0.03f / 0.07f);
  m_tBone[T2].T_MP.SetS(0.03f / 0.03f);

  m_tBone[I0].T_MP.SetS(0.11f / 0.25f);
  m_tBone[I1].T_MP.SetS(0.05f / 0.11f);
  m_tBone[I2].T_MP.SetS(0.025f / 0.05f);
  m_tBone[I3].T_MP.SetS(0.02f / 0.025f);

  m_tBone[M0].T_MP.SetS(0.11f / 0.25f);
  m_tBone[M1].T_MP.SetS(0.055f / 0.11f);
  m_tBone[M2].T_MP.SetS(0.03f / 0.055f);
  m_tBone[M3].T_MP.SetS(0.025f / 0.03f);

  m_tBone[R0].T_MP.SetS(0.105f / 0.25f);
  m_tBone[R1].T_MP.SetS(0.05f / 0.105f);
  m_tBone[R2].T_MP.SetS(0.025f / 0.05f);
  m_tBone[R3].T_MP.SetS(0.02f / 0.025f);

  m_tBone[L0].T_MP.SetS(0.095f / 0.25f);
  m_tBone[L1].T_MP.SetS(0.04f / 0.095f);
  m_tBone[L2].T_MP.SetS(0.02f / 0.04f);
  m_tBone[L3].T_MP.SetS(0.02f);
  
  //Place at the end of the parent bone
  for (int i = 1; i < BONE_COUNT; i++)
  {
    m_tBone[i].T_MP.SetV(Vec4(1.0f, 0.0f, 0.0f, 0.0f));
  }

  //Set Rotations
  Quat q;

  q.SetRotationY(Dg::PI_f / 4.0f);
  m_tBone[T0].T_MP.SetQ(q);

  q.SetRotationY(Dg::PI_f / 18.0f);
  m_tBone[I0].T_MP.SetQ(q);

  q.SetRotationY(-Dg::PI_f / 18.0f);
  m_tBone[R0].T_MP.SetQ(q);

  q.SetRotationY(-Dg::PI_f / 9.0f);
  m_tBone[L0].T_MP.SetQ(q);

  m_pRoot->Update(VQS());
}

void Bone::Update(VQS const & a_T_PW)
{
  m_pTBone->T_MV = m_pTBone->T_MP * a_T_PW;
  for (size_t i = 0; i < m_children.size(); ++i)
  {
    m_children[i].Update(m_pTBone->T_MV);
  }
}

void ArmSkeleton::SetMatrices(Mat44 a_out[BONE_COUNT])
{
  for (int i = 0; i < BONE_COUNT; ++i)
  {
     m_tBone[i].T_MV.GetMatrix(a_out[i]);
  }
}

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

void ArmSkeleton::SetTransform(VQS const & a_T)
{
  m_pRoot->Update(a_T);

  ImGui::Begin("Editor");
  ImGui::End();
}

void ArmSkeleton::SetPose(Pose a_pose)
{
  switch (a_pose)
  {
  case Relaxed:
  {
    SetPoseRelaxed();
    break;
  }
  case Muscle:
  {
    SetPoseMuscle();
    break;
  }
  case Salute:
  {
    SetPoseSalute();
    break;
  }
  case ThumbsUp:
  {
    SetPoseThumbsUp();
    break;
  }
  case ThumbsDown:
  {
    SetPoseThumbsDown();
    break;
  }
  case OK:
  {
    SetPoseOK();
    break;
  }
  default:
  {
    SetPoseStop();
  }
  }
}

void ArmSkeleton::SetPoseRelaxed()
{
  Quat q;

  q.SetRotationY(rad(80.0f));
  m_tBone[H].T_MP.SetQ(q);

  m_pRoot->Update(VQS());
}

void ArmSkeleton::SetPoseMuscle()
{
  Quat q;

  q.SetRotationY(rad(80.0f));
  m_tBone[H].T_MP.SetQ(q);

  m_pRoot->Update(VQS());
}