
#include <chrono>
#include <thread>
#include <sstream>

#include "CollisionApp.h"
#include "imgui.h"
#include "GLFW\glfw3.h"
#include "Tests.h"


void Log(std::string const & a_message, int a_logLevel)
{
  DgApp::GetInstance()->LogToOutputWindow(a_message, a_logLevel);
}

static Context::LineMesh GenerateCircle(float a_radius)
{
  float pi = 3.14159f;
  unsigned nSides = 20;

  Context::LineMesh   result;
  Context::LineVertex vertex;
  Context::Line       line;
  vertex.point[2] =   0.0f;
  vertex.point[3] =   1.0f;

  for (unsigned i = 0; i < nSides; i++)
  {
    float angle = (2.0f * pi) * float(i) / float(nSides);

    vertex.point[0] = a_radius * cos(angle);
    vertex.point[1] = a_radius * sin(angle);
    result.verts.push_back(vertex);
  }

  for (unsigned i = 0; i < nSides; i++)
  {
    line.indices[0] = i;
    line.indices[1] = (i + 1) % nSides;
    result.lines.push_back(line);
  }

  return result;
}

static Context::LineMesh GenerateArrow()
{
  Context::LineMesh   result;
  Context::LineVertex v;
  Context::Line       line;
  v.point[2] =   0.0f;
  v.point[3] =   1.0f;

  float tip = 0.7f;
  float offsetX = 0.2f;
  float offsetY = 0.15f;

  v.point[0] = 0.0f;
  v.point[1] = 0.0f;
  result.verts.push_back(v);

  v.point[0] = tip;
  v.point[1] = 0.0f;
  result.verts.push_back(v);

  v.point[0] = tip - offsetX;
  v.point[1] = offsetY;
  result.verts.push_back(v);

  v.point[0] = tip - offsetX;
  v.point[1] = -offsetY;
  result.verts.push_back(v);

  line.indices[0] = 0;
  line.indices[1] = 1;
  result.lines.push_back(line);

  line.indices[0] = 1;
  line.indices[1] = 2;
  result.lines.push_back(line);

  line.indices[0] = 1;
  line.indices[1] = 3;
  result.lines.push_back(line);

  return result;
}

CollisionApp::CollisionApp()
  : m_dTurn(0.0f)
  , m_moveRate(2.0f)
  , m_turnRate(2.0f)
  , m_playerMoving(false)
{
  int tstDM = TestDirectionMask();
  if (tstDM != 0)
  {
    char brk(0);
  }

  this->ToggleOutputWindow(false);
  Context::Init(Log);

  Window::WindowFactory windowFactory;

  Window::InitData windowData;
  windowData.isImGuiWindow = false;
  int appW(0), appH(0);
  GetWindowDimensions(appW, appH);
  windowData.width = appW;
  windowData.height = appH;
  windowData.clearColor = (vec4(0.0f, 0.0f, 0.0f, 1.0f));

  m_pViewport = windowFactory.GetNewWindow(windowData);

  float margin = 0.5f;
  m_canvasDim = 10.0f;

  //Add boundary lines
  BoundaryLine bl;
  bl.line = Line(vec3(margin, margin, 1.0f), vec3(1.0f, 0.0f, 0.0f));
  bl.length = m_canvasDim - 2.0f * margin;
  m_boundaryLines.push_back(bl);

  bl.line = Line(vec3(m_canvasDim - margin, margin, 1.0f), vec3(0.0f, 1.0f, 0.0f));
  m_boundaryLines.push_back(bl);

  bl.line = Line(vec3(m_canvasDim - margin, m_canvasDim - margin, 1.0f), vec3(-1.0f, 0.0f, 0.0f));
  m_boundaryLines.push_back(bl);

  bl.line = Line(vec3(margin, m_canvasDim - margin, 1.0f), vec3(0.0f, -1.0f, 0.0f));
  m_boundaryLines.push_back(bl);

  float crossDim = 0.6f;
  vec3 crossCenter(5.0f, 5.0f, 1.0f);

  // Upper arm
  vec3 lo = crossCenter + vec3(crossDim, crossDim, 0.0f);
  bl.length = 3.0f * crossDim;
  bl.line = Line(lo, vec3::yAxis());
  m_boundaryLines.push_back(bl);

  lo = crossCenter + vec3(crossDim, 4.0f * crossDim, 0.0f);
  bl.length = 2.0f * crossDim;
  bl.line = Line(lo, -vec3::xAxis());
  m_boundaryLines.push_back(bl);

  lo = crossCenter + vec3(-crossDim, 4.0f * crossDim, 0.0f);
  bl.length = 3.0f * crossDim;
  bl.line = Line(lo, -vec3::yAxis());
  m_boundaryLines.push_back(bl);

  // Left arm
  lo = crossCenter + vec3(-crossDim, crossDim, 0.0f);
  bl.length = 3.0f * crossDim;
  bl.line = Line(lo, -vec3::xAxis());
  m_boundaryLines.push_back(bl);

  lo = crossCenter + vec3(-4.0f * crossDim, crossDim, 0.0f);
  bl.length = 2.0f * crossDim;
  bl.line = Line(lo, -vec3::yAxis());
  m_boundaryLines.push_back(bl);

  lo = crossCenter + vec3(-4.0f * crossDim, -crossDim, 0.0f);
  bl.length = 3.0f * crossDim;
  bl.line = Line(lo, vec3::xAxis());
  m_boundaryLines.push_back(bl);

  // Bottom arm
  lo = crossCenter + vec3(-crossDim, -crossDim, 0.0f);
  bl.length = 3.0f * crossDim;
  bl.line = Line(lo, -vec3::yAxis());
  m_boundaryLines.push_back(bl);

  lo = crossCenter + vec3(-crossDim, -4.0f * crossDim, 0.0f);
  bl.length = 2.0f * crossDim;
  bl.line = Line(lo, vec3::xAxis());
  m_boundaryLines.push_back(bl);

  lo = crossCenter + vec3(crossDim, -4.0f * crossDim, 0.0f);
  bl.length = 3.0f * crossDim;
  bl.line = Line(lo, vec3::yAxis());
  m_boundaryLines.push_back(bl);

  // Right arm
  lo = crossCenter + vec3(crossDim, -crossDim, 0.0f);
  bl.length = 3.0f * crossDim;
  bl.line = Line(lo, vec3::xAxis());
  m_boundaryLines.push_back(bl);

  lo = crossCenter + vec3(4.0f * crossDim, -crossDim, 0.0f);
  bl.length = 2.0f * crossDim;
  bl.line = Line(lo, vec3::yAxis());
  m_boundaryLines.push_back(bl);

  lo = crossCenter + vec3(4.0f * crossDim, crossDim, 0.0f);
  bl.length = 3.0f * crossDim;
  bl.line = Line(lo, -vec3::xAxis());
  m_boundaryLines.push_back(bl);

  //Boundary points
  BoundaryPoint bp;
  bp.origin = crossCenter + vec3(crossDim, 4.0f * crossDim, 0.0f);
  bp.loa.Set(0.0f, 1.0f, 0.0f);
  bp.roa.Set(1.0f, 0.0f, 0.0f);
  m_boundaryPoints.push_back(bp);

  bp.origin = crossCenter + vec3(-crossDim, 4.0f * crossDim, 0.0f);
  bp.loa.Set(-1.0f, 0.0f, 0.0f);
  bp.roa.Set(0.0f, 1.0f, 0.0f);
  m_boundaryPoints.push_back(bp);

  bp.origin = crossCenter + vec3(-4.0f * crossDim, crossDim, 0.0f);
  bp.loa.Set(-1.0f, 0.0f, 0.0f);
  bp.roa.Set(0.0f, 1.0f, 0.0f);
  m_boundaryPoints.push_back(bp);

  bp.origin = crossCenter + vec3(-4.0f * crossDim, -crossDim, 0.0f);
  bp.loa.Set(0.0f, -1.0f, 0.0f);
  bp.roa.Set(-1.0f, 0.0f, 0.0f);
  m_boundaryPoints.push_back(bp);

  bp.origin = crossCenter + vec3(-crossDim, -4.0f * crossDim, 0.0f);
  bp.loa.Set(0.0f, -1.0f, 0.0f);
  bp.roa.Set(-1.0f, 0.0f, 0.0f);
  m_boundaryPoints.push_back(bp);

  bp.origin = crossCenter + vec3(crossDim, -4.0f * crossDim, 0.0f);
  bp.loa.Set(1.0f, 0.0f, 0.0f);
  bp.roa.Set(0.0f, -1.0f, 0.0f);
  m_boundaryPoints.push_back(bp);

  bp.origin = crossCenter + vec3(4.0f * crossDim, -crossDim, 0.0f);
  bp.loa.Set(1.0f, 0.0f, 0.0f);
  bp.roa.Set(0.0f, -1.0f, 0.0f);
  m_boundaryPoints.push_back(bp);

  bp.origin = crossCenter + vec3(4.0f * crossDim, crossDim, 0.0f);
  bp.loa.Set(0.0f, 1.0f, 0.0f);
  bp.roa.Set(1.0f, 0.0f, 0.0f);
  m_boundaryPoints.push_back(bp);

  //Boundary disks
  Disk dsk(vec3(1.0f, 8.0f, 1.0f), 0.3f);
  m_boundaryDisks.push_back(dsk);

  //Player puck
  float playerRadius = 0.3f;
  m_player.disk.SetRadius(playerRadius);
  float playerOffset = margin + playerRadius + 0.1f;
  m_player.disk.SetCenter(vec3(playerOffset, playerOffset, 1.0f));
  m_player.angle =  0.0f;
  //m_player.disk.SetCenter(vec3(5.0f, 5.0f, 1.0f));
  //m_player.angle = 0.0f;
  //m_player.disk.SetCenter(vec3(0.799999952f, playerOffset, 1.0f));
  //m_player.angle = 2.46666574f;
  m_player.speed = 0.0f;



  //------------------------------------------------------------------------------------
  //  Add drawables
  //------------------------------------------------------------------------------------
  ScreenObject so;
  so.draw = true;
  Context::LineMesh player_lm = GenerateCircle(1.0f);
  player_lm.color.Set(1.0f, 0.0f, 0.0f, 1.0f);
  m_circleHandle = m_contextLine.AddObject(player_lm);
  m_drawables.insert(dItem(m_circleHandle, so));

  player_lm.color.Set(1.0f, 0.5f, 0.0f, 1.0f);
  so.draw = false;
  m_circleHandleInt = m_contextLine.AddObject(player_lm);
  m_drawables.insert(dItem(m_circleHandleInt, so));

  Context::LineMesh arrow_lm = GenerateArrow();
  so.draw = true;
  arrow_lm.color.Set(1.0f, 0.0f, 0.0f, 1.0f);
  m_arrowHandle = m_contextLine.AddObject(arrow_lm);
  m_drawables.insert(dItem(m_arrowHandle, so));

  arrow_lm.color.Set(1.0f, 0.5f, 0.0f, 1.0f);
  so.draw = false;
  m_arrowHandleInt = m_contextLine.AddObject(arrow_lm);
  m_drawables.insert(dItem(m_arrowHandleInt, so));

  Context::LineMesh lm;
  lm.color.Set(1.0f, 0.0f, 0.0f, 1.0f);
  for (auto const & l : m_boundaryLines)
  {
    Context::LineVertex lv0, lv1;

    lv0.point[0] = l.line.Origin()[0];
    lv0.point[1] = l.line.Origin()[1];
    lv0.point[2] = 0.0f;
    lv0.point[3] = 1.0f;

    lv1.point[0] = l.line.Origin()[0] + l.length * l.line.Direction()[0];
    lv1.point[1] = l.line.Origin()[1] + l.length * l.line.Direction()[1];
    lv1.point[2] = 0.0f;
    lv1.point[3] = 1.0f;

    lm.verts.push_back(lv0);
    lm.verts.push_back(lv1);

    Context::Line l;
    l.indices[0] = (unsigned)lm.verts.size() - 1;
    l.indices[1] = (unsigned)lm.verts.size() - 2;
    lm.lines.push_back(l);
  }

  mat4 scale, translation;
  scale.Scaling(2.0f / m_canvasDim);

  vec4 tvec(-1.0f, -1.0f, 0.0f, 0.0f);
  translation.Translation(tvec);
  m_transform = scale * translation;

  so.draw = true;
  m_drawables.insert(dItem(m_contextLine.AddObject(lm), so));

  m_contextLine.CommitLoadList();
}

void CollisionApp::KeyEvent(int a_key, int a_action)
{
  switch (a_key)
  {
    case GLFW_KEY_RIGHT:
    {
      if (a_action == GLFW_PRESS)
      {
        m_dTurn = -m_turnRate;
      }
      else if (a_action == GLFW_RELEASE)
      {
        m_dTurn = 0.0f;
      }
      break;
    }
    case GLFW_KEY_LEFT:
    {
      if (a_action == GLFW_PRESS)
      {
        m_dTurn = m_turnRate;
      }
      else if (a_action == GLFW_RELEASE)
      {
        m_dTurn = 0.0f;
      }
      break;
    }
    case GLFW_KEY_UP:
    {
      if (a_action == GLFW_PRESS)
      {
        m_player.speed = m_moveRate;
        m_playerMoving = true;
      }
      else if (a_action == GLFW_RELEASE)
      {
        m_player.speed = 0.0f;
        m_playerMoving = false;
      }
      break;
    }
    case GLFW_KEY_DOWN:
    {
      if (a_action == GLFW_PRESS)
      {
        m_player.speed = -m_moveRate;
        m_playerMoving = true;
      }
      else if (a_action == GLFW_RELEASE)
      {
        m_player.speed = 0.0f;
        m_playerMoving = false;
      }
      break;
    }
    case GLFW_KEY_X:
    {
      if (a_action == GLFW_PRESS)
      {
        m_moveRate += 0.5f;
        if (m_playerMoving)
        {
          m_player.speed = m_moveRate;
        }
      }
      break;
    }
    case GLFW_KEY_Z:
    {
      if (a_action == GLFW_PRESS)
      {
        m_moveRate -= 0.5f;
        if (m_playerMoving)
        {
          m_player.speed = m_moveRate;
        }
      }
      break;
    }
    case GLFW_KEY_R:
    {
      if (a_action == GLFW_PRESS)
      {
        m_player.disk.SetCenter(vec3(0.799999952f, 1.0f, 1.0f));
        m_player.angle = 2.46666574f;
      }
      break;
    }
    default:
    {
      break;
    }
  }
}

void CollisionApp::Render()
{
  mat4 scale, translation, rotation;
  scale.Scaling(m_player.disk.Radius());

  vec4 tvec;
  tvec[0] = m_player.disk.Center()[0];
  tvec[1] = m_player.disk.Center()[1];
  tvec[2] = 0.0f;
  tvec[3] = 0.0f;

  translation.Translation(tvec);

  m_drawables[m_circleHandle].transform = scale * translation;

  rotation.RotationZ(m_player.angle);
  m_drawables[m_arrowHandle].transform = rotation * translation;

  m_pViewport->BeginDraw();
  m_contextLine.Bind();
  for (auto const & kv : m_drawables)
  {
    if (!kv.second.draw) continue;
    m_contextLine.SetMatrix(kv.second.transform * m_transform);
    m_contextLine.Draw(kv.first);
  }

  for (auto const disk : m_boundaryDisks)
  {
    scale.Scaling(disk.Radius());

    tvec[0] = disk.Center()[0];
    tvec[1] = disk.Center()[1];
    tvec[2] = 0.0f;
    tvec[3] = 0.0f;

    translation.Translation(tvec);

    mat4 transform = scale * translation;
    m_contextLine.SetMatrix(transform * m_transform);
    m_contextLine.Draw(m_circleHandle);
  }

  m_contextLine.Unbind();

  m_pViewport->EndDraw();
  m_pViewport->Update();
}

void CollisionApp::DoFrame(double a_dt)
{
  double frameRate = 1.0 / 60.0;

  DoPhysics(frameRate);
  Render();

  double remaining = frameRate - a_dt;
  if (remaining > 0.0)
  {
    int ms = int(remaining * 1000.0);
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
  }

}