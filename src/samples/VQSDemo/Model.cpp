#include <fstream>

#include "DgStringFunctions.h"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "UI.h"
#include "Model.h"


#define rad(x) x / 180 * 3.14159f

template<typename T>
static std::string ToString(T const * a_data, int a_size)
{
  std::stringstream ss;
  for (int i = 0; i < a_size; ++i)
  {
    ss << a_data[i];
    if (i != a_size - 1) ss << ',';
  }
  return ss.str();
}

ArmSkeleton::ArmSkeleton()
  : m_pRoot(nullptr)
{
  m_pRoot = new Bone(&m_tBone[H]);
  InitBoneHeirarchy();
  InitBoneTransforms();

  std::ifstream fs;
  fs.open("./resources/poses.dgd");
  if (!fs.good())
  {
    printf("Failed to open file './resources/poses.dgd'!\n");
    return;
  }
  fs >> m_jsRoot;
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
  m_tBone[F].T_MP.SetS(0.30f / 0.30f);
  
  m_tBone[T0].T_MP.SetS(0.07f / 0.30f);
  m_tBone[T1].T_MP.SetS(0.03f / 0.07f);
  m_tBone[T2].T_MP.SetS(0.03f / 0.03f);

  m_tBone[I0].T_MP.SetS(0.11f / 0.30f);
  m_tBone[I1].T_MP.SetS(0.05f / 0.11f);
  m_tBone[I2].T_MP.SetS(0.025f / 0.05f);
  m_tBone[I3].T_MP.SetS(0.02f / 0.025f);

  m_tBone[M0].T_MP.SetS(0.11f / 0.30f);
  m_tBone[M1].T_MP.SetS(0.055f / 0.11f);
  m_tBone[M2].T_MP.SetS(0.03f / 0.055f);
  m_tBone[M3].T_MP.SetS(0.025f / 0.03f);

  m_tBone[R0].T_MP.SetS(0.105f / 0.30f);
  m_tBone[R1].T_MP.SetS(0.05f / 0.105f);
  m_tBone[R2].T_MP.SetS(0.025f / 0.05f);
  m_tBone[R3].T_MP.SetS(0.02f / 0.025f);

  m_tBone[L0].T_MP.SetS(0.095f / 0.30f);
  m_tBone[L1].T_MP.SetS(0.04f / 0.095f);
  m_tBone[L2].T_MP.SetS(0.02f / 0.04f);
  m_tBone[L3].T_MP.SetS(0.02f / 0.02f);
  
  //Place at the end of the parent bone
  for (int i = 1; i < BONE_COUNT; i++)
  {
    m_tBone[i].T_MP.SetV(Vec4(1.0f, 0.0f, 0.0f, 0.0f));
  }

  //Set Rotations
  Quat q;

  q.SetRotationY(Dg::Constants<float>::PI / 4.0f);
  m_tBone[T0].T_MP.SetQ(q);

  q.SetRotationY(Dg::Constants<float>::PI / 18.0f);
  m_tBone[I0].T_MP.SetQ(q);

  q.SetRotationY(-Dg::Constants<float>::PI / 18.0f);
  m_tBone[R0].T_MP.SetQ(q);

  q.SetRotationY(-Dg::Constants<float>::PI / 9.0f);
  m_tBone[L0].T_MP.SetQ(q);

  m_pRoot->Update(VQS());
}

void Bone::Update(VQS const & a_T_PV)
{
  m_pTBone->T_MV = m_pTBone->T_MP * a_T_PV;
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


float ArmSkeleton::GetRunTime() const
{
  if (m_time >= m_changeTime)
  {
    return 1.0f;
  }
  return (sin(m_time / m_changeTime * Dg::Constants<float>::PI - Dg::Constants<float>::PI / 2.0f) + 1.0f) / 2.0f;
}

void ArmSkeleton::Update(float a_dt)
{
  m_time += a_dt;
  for (int i = 0; i < BONE_COUNT; i++)
  {
    Quat q;
    Dg::Slerp(q, m_tBone[i].qBegin, m_tBone[i].qEnd, GetRunTime());
    m_tBone[i].T_MP.SetQ(q);
  }
}

void ArmSkeleton::ShowUI()
{
  static BoneID activeBone = H;
  char const * boneName[BONE_COUNT] =
  {
    "H", "F"
    , "T0", "T1", "T2"
    , "I0", "I1", "I2", "I3"
    , "M0", "M1", "M2", "M3"
    , "R0", "R1", "R2", "R3"
    , "L0", "L1", "L2", "L3"
  };
  static char nameBuf[64] = {};
  static float rotations[BONE_COUNT][3] = {};
  static bool isFirst = true;

  ImGui::Begin("Main");

  static std::string curentPose;
  std::vector<std::string> poses = m_jsRoot.getMemberNames();
  for (auto it = poses.begin(); it != poses.end(); it++)
  {
    if (ImGui::Button(it->c_str(), ImVec2(100, 20)) || isFirst)
    {
      isFirst = false;
      strcpy(nameBuf, it->c_str());
      std::vector<float> values;
      Dg::StringToNumberList(m_jsRoot[*it].asString(), ',', std::dec, values);
      memcpy(rotations, values.data(), sizeof(float) * BONE_COUNT * 3);
      for (int i = 0; i < BONE_COUNT; i++)
      {
        Quat q;
        Dg::Slerp(q, m_tBone[i].qBegin, m_tBone[i].qEnd, GetRunTime());
        m_tBone[i].qBegin = q;
        m_tBone[i].qEnd.SetRotation(rotations[i][0]
          , rotations[i][1]
          , rotations[i][2]
          , Dg::EulerOrder::XYZ);
      }
      m_time = 0.0f;
    }
  }
  ImGui::End();
  if (UI::showEditor)
  {
    ImGui::Begin("Editor", &UI::showEditor);
    
    if (ImGui::Button(boneName[H])) activeBone = H; ImGui::SameLine();
    if (ImGui::Button(boneName[F])) activeBone = F;

    if (ImGui::Button(boneName[T0])) activeBone = T0; ImGui::SameLine();
    if (ImGui::Button(boneName[T1])) activeBone = T1; ImGui::SameLine();
    if (ImGui::Button(boneName[T2])) activeBone = T2;

    if (ImGui::Button(boneName[I0])) activeBone = I0; ImGui::SameLine();
    if (ImGui::Button(boneName[I1])) activeBone = I1; ImGui::SameLine();
    if (ImGui::Button(boneName[I2])) activeBone = I2; ImGui::SameLine();
    if (ImGui::Button(boneName[I3])) activeBone = I3;

    if (ImGui::Button(boneName[M0])) activeBone = M0; ImGui::SameLine();
    if (ImGui::Button(boneName[M1])) activeBone = M1; ImGui::SameLine();
    if (ImGui::Button(boneName[M2])) activeBone = M2; ImGui::SameLine();
    if (ImGui::Button(boneName[M3])) activeBone = M3;

    if (ImGui::Button(boneName[R0])) activeBone = R0; ImGui::SameLine();
    if (ImGui::Button(boneName[R1])) activeBone = R1; ImGui::SameLine();
    if (ImGui::Button(boneName[R2])) activeBone = R2; ImGui::SameLine();
    if (ImGui::Button(boneName[R3])) activeBone = R3;

    if (ImGui::Button(boneName[L0])) activeBone = L0; ImGui::SameLine();
    if (ImGui::Button(boneName[L1])) activeBone = L1; ImGui::SameLine();
    if (ImGui::Button(boneName[L2])) activeBone = L2; ImGui::SameLine();
    if (ImGui::Button(boneName[L3])) activeBone = L3;

    ImGui::Separator();
    ImGui::Text("Active bone: %s", boneName[activeBone]);
    ImGui::Separator();

    ImGui::SliderAngle("rx", &rotations[activeBone][0]);
    ImGui::SliderAngle("ry", &rotations[activeBone][1]);
    ImGui::SliderAngle("rz", &rotations[activeBone][2]);

    m_tBone[activeBone].qEnd.SetRotation(rotations[activeBone][0]
      , rotations[activeBone][1]
      , rotations[activeBone][2]
      , Dg::EulerOrder::XYZ);

    ImGui::Separator();

    ImGui::InputText("Name: ", nameBuf, 64);
    if (ImGui::Button("Save"))
    {
      m_jsRoot[nameBuf] = ToString((float*)rotations, BONE_COUNT * 3);
      std::ofstream fs;
      fs.open("./resources/poses.dgd");
      if (fs.good())
      {
        fs << m_jsRoot;
      }
      else
      {
        printf("Failed to open file './resources/poses.dgd'!\n");
      }
    }
    ImGui::End();
    
  }
  

}

void ArmSkeleton::SetTransform(VQS const & a_T)
{
  m_pRoot->Update(a_T);
}
