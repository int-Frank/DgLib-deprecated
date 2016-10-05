#include "Model.h"

ArmSkeleton::ArmSkeleton()
  : m_pRoot(nullptr)
{
  m_pRoot = new Bone(&m_tBone[H]);
  InitBoneHeirarchy();
  InitBoneTransforms();
  InitDefaultPose();
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
  m_tBone[H].TPM.SetS(0.23f);

  m_tBone[F].TPM.SetS(0.25f);

  m_tBone[T0].TPM.SetS(0.7f);
  m_tBone[T1].TPM.SetS(0.3f);
  m_tBone[T2].TPM.SetS(0.3f);

  m_tBone[I0].TPM.SetS(0.11f);
  m_tBone[I1].TPM.SetS(0.05f);
  m_tBone[I2].TPM.SetS(0.025f);
  m_tBone[I3].TPM.SetS(0.02f);

  m_tBone[M0].TPM.SetS(0.11f);
  m_tBone[M1].TPM.SetS(0.055f);
  m_tBone[M2].TPM.SetS(0.03f);
  m_tBone[M3].TPM.SetS(0.025f);

  m_tBone[R0].TPM.SetS(0.105f);
  m_tBone[R1].TPM.SetS(0.05f);
  m_tBone[R2].TPM.SetS(0.025f);
  m_tBone[R3].TPM.SetS(0.02f);

  m_tBone[L0].TPM.SetS(0.095f);
  m_tBone[L1].TPM.SetS(0.04f);
  m_tBone[L2].TPM.SetS(0.02f);
  m_tBone[L3].TPM.SetS(0.02f);

  //Place at the end of the parent bone
  m_tBone[H].TPM.SetV(Vec4(0.0f, 0.0f, 0.0f, 0.0f));

  m_tBone[F].TPM.SetV(Vec4(m_tBone[H].TPM.S(), 0.0f, 0.0f, 0.0f));

  m_tBone[T0].TPM.SetV(Vec4(m_tBone[F].TPM.S(), 0.0f, 0.0f, 0.0f));
  m_tBone[T1].TPM.SetV(Vec4(m_tBone[T0].TPM.S(), 0.0f, 0.0f, 0.0f));
  m_tBone[T2].TPM.SetV(Vec4(m_tBone[T1].TPM.S(), 0.0f, 0.0f, 0.0f));

  m_tBone[I0].TPM.SetV(Vec4(m_tBone[F].TPM.S(), 0.0f, 0.0f, 0.0f));
  m_tBone[I1].TPM.SetV(Vec4(m_tBone[I0].TPM.S(), 0.0f, 0.0f, 0.0f));
  m_tBone[I2].TPM.SetV(Vec4(m_tBone[I1].TPM.S(), 0.0f, 0.0f, 0.0f));
  m_tBone[I3].TPM.SetV(Vec4(m_tBone[I2].TPM.S(), 0.0f, 0.0f, 0.0f));

  m_tBone[M0].TPM.SetV(Vec4(m_tBone[F].TPM.S(), 0.0f, 0.0f, 0.0f));
  m_tBone[M1].TPM.SetV(Vec4(m_tBone[M0].TPM.S(), 0.0f, 0.0f, 0.0f));
  m_tBone[M2].TPM.SetV(Vec4(m_tBone[M1].TPM.S(), 0.0f, 0.0f, 0.0f));
  m_tBone[M3].TPM.SetV(Vec4(m_tBone[M2].TPM.S(), 0.0f, 0.0f, 0.0f));

  m_tBone[R0].TPM.SetV(Vec4(m_tBone[F].TPM.S(), 0.0f, 0.0f, 0.0f));
  m_tBone[R1].TPM.SetV(Vec4(m_tBone[R0].TPM.S(), 0.0f, 0.0f, 0.0f));
  m_tBone[R2].TPM.SetV(Vec4(m_tBone[R1].TPM.S(), 0.0f, 0.0f, 0.0f));
  m_tBone[R3].TPM.SetV(Vec4(m_tBone[R2].TPM.S(), 0.0f, 0.0f, 0.0f));

  m_tBone[L0].TPM.SetV(Vec4(m_tBone[F].TPM.S(), 0.0f, 0.0f, 0.0f));
  m_tBone[L1].TPM.SetV(Vec4(m_tBone[L0].TPM.S(), 0.0f, 0.0f, 0.0f));
  m_tBone[L2].TPM.SetV(Vec4(m_tBone[L1].TPM.S(), 0.0f, 0.0f, 0.0f));
  m_tBone[L3].TPM.SetV(Vec4(m_tBone[L2].TPM.S(), 0.0f, 0.0f, 0.0f));

  //Set Rotations
  Quat q;
  q.SetRotationY(Dg::PI_f / 4.0f);
  m_tBone[T0].TPM.SetQ(q);

  q.SetRotationY(Dg::PI_f / 18.0f);
  m_tBone[I0].TPM.SetQ(q);

  q.SetRotationY(-Dg::PI_f / 18.0f);
  m_tBone[R0].TPM.SetQ(q);

  q.SetRotationY(-Dg::PI_f / 9.0f);
  m_tBone[L0].TPM.SetQ(q);
}