
#include <chrono>
#include <thread>
#include <sstream>

#include "CollisionApp.h"
#include "imgui.h"
#include "GLFW\glfw3.h"
#include "query\DgR2QueryDiskLine.h"
#include "query\DgR2QueryPointDisk.h"
#include "query\DgR2QueryDiskDisk.h"
#include "query\DgR2QueryPointLine.h"
#include "Tests.h"
#include "DgDynamicArray.h"

#define EPSILON_INTERSECTS 0.0001f
#define EPSILON_SPEED 0.0001f
#define EPSILON_RADIUS 0.0001f
#define EPSILON_TIME 0.00001f

bool WillCollide(float a_dt, 
                 Disk const & a_disk, 
                 vec3 const & a_vel, 
                 Line const & a_line, 
                 float a_lineLength, 
                 float & a_outTime)
{
  Dg::R2::FPCDiskLine<float> fpc;
  Dg::R2::FPCDiskLine<float>::Result result_fpc = fpc(a_disk, a_vel, a_line, vec3::ZeroVector());

  bool willCollide = result_fpc.code == Dg::QC_Intersecting;
  willCollide = willCollide && (result_fpc.t <= a_dt);

  //TODO Even though the querty accounts for this, we still need to check for some reason
  willCollide = willCollide && (result_fpc.t >= 0.0f);
  if (willCollide)
  {
    vec3 p = a_disk.Center() + result_fpc.t * a_vel;

    Dg::R2::CPPointLine<float> cp;
    Dg::R2::CPPointLine<float>::Result result_cp = cp(p, a_line);

    willCollide = willCollide && (result_cp.u <= a_lineLength);
    willCollide = willCollide && (result_cp.u >= 0.0f);
    a_outTime = result_fpc.t;
  }

  return willCollide;
}

bool WillCollide(float a_dt, 
                 Disk const & a_disk, 
                 vec3 const & a_vel, 
                 vec3 const & a_point,
                 float & a_outTime)
{
  Dg::R2::FPCPointDisk<float> fpc;
  Dg::R2::FPCPointDisk<float>::Result result_fpc = fpc(a_disk, a_vel, a_point, vec3::ZeroVector());

  bool willCollide = result_fpc.code == Dg::QC_Intersecting;
  willCollide = willCollide && (result_fpc.t <= a_dt);
  willCollide = willCollide && (result_fpc.t >= 0.0f);

  return willCollide;
}

bool WillCollide(float a_dt, 
                 Disk const & a_disk0, 
                 vec3 const & a_vel0, 
                 Disk const & a_disk1, 
                 vec3 const & a_vel1, 
                 float & a_outTime)
{
  Dg::R2::FPCDiskDisk<float> fpc;
  Dg::R2::FPCDiskDisk<float>::Result result_fpc = fpc(a_disk0, a_vel0, a_disk1, a_vel1);

  bool willCollide = result_fpc.code == Dg::QC_Intersecting;
  willCollide = willCollide && (result_fpc.t <= a_dt);
  willCollide = willCollide && (result_fpc.t >= 0.0f);

  return willCollide;
}

void Log(std::string const & a_message, int a_logLevel)
{
  DgApp::GetInstance()->LogToOutputWindow(a_message, a_logLevel);
}

static Context::LineMesh GenerateCircle(float a_radius)
{
  float pi = 3.14159f;
  int nSides = 20;

  Context::LineMesh   result;
  Context::LineVertex vertex;
  Context::Line       line;
  vertex.point[2] =   0.0f;
  vertex.point[3] =   1.0f;

  for (int i = 0; i < nSides; i++)
  {
    float angle = (2.0f * pi) * float(i) / float(nSides);

    vertex.point[0] = a_radius * cos(angle);
    vertex.point[1] = a_radius * sin(angle);
    result.verts.push_back(vertex);
  }

  for (size_t i = 0; i < nSides; i++)
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

  float tip = 0.7;
  float offsetX = 0.2;
  float offsetY = 0.15;

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
  vec3 bp = crossCenter + vec3(crossDim, 4.0f * crossDim, 0.0f);
  m_boundaryPoints.push_back(bp);
  bp = crossCenter + vec3(-crossDim, 4.0f * crossDim, 0.0f);
  m_boundaryPoints.push_back(bp);
  bp = crossCenter + vec3(-4.0f * crossDim, crossDim, 0.0f);
  m_boundaryPoints.push_back(bp);
  bp = crossCenter + vec3(-4.0f * crossDim, -crossDim, 0.0f);
  m_boundaryPoints.push_back(bp);
  bp = crossCenter + vec3(-crossDim, -4.0f * crossDim, 0.0f);
  m_boundaryPoints.push_back(bp);
  bp = crossCenter + vec3(crossDim, -4.0f * crossDim, 0.0f);
  m_boundaryPoints.push_back(bp);
  bp = crossCenter + vec3(4.0f * crossDim, -crossDim, 0.0f);
  m_boundaryPoints.push_back(bp);
  bp = crossCenter + vec3(4.0f * crossDim, crossDim, 0.0f);
  m_boundaryPoints.push_back(bp);

  //Boundary disks
  Disk dsk(vec3(1.0f, 8.0f, 1.0f), 0.3f);
  m_boundaryDisks.push_back(dsk);

  //Player puck
  float playerRadius = 0.3f;
  m_player.disk.SetRadius(playerRadius);
  float playerOffset = margin + playerRadius + 0.1f;
  //m_player.disk.SetCenter(vec3(playerOffset, playerOffset, 1.0f));
  //m_player.angle =  0.0f;
  m_player.disk.SetCenter(vec3(0.942994535f, 7.31161976, 1.0f));
  m_player.angle = 2.36666584;
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
    l.indices[0] = lm.verts.size() - 1;
    l.indices[1] = lm.verts.size() - 2;
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

bool CollisionApp::AllCPData::Empty() const
{
  return (lines.size() == 0)
    && (points.size() == 0)
    && (disks.size() == 0);
}

bool CollisionApp::AdjacentGeometry::Empty() const
{
  return (lines.size() == 0)
    && (points.size() == 0)
    && (disks.size() == 0);
}

void CollisionApp::AdjacentGeometry::Clear()
{
  lines.clear();
  points.clear();
  disks.clear();
}

//Develop a Potentially Collidable Set
void CollisionApp::GetPCS(Disk const & a_disk, float a_maxDistSq, AllCPData & a_out) const
{
  for (size_t i = 0; i < m_boundaryPoints.size(); i++)
  {
    CPDataPoint data;
    data.vToPoint = m_boundaryPoints[i] - a_disk.Center();
    data.distSqToPoint = data.vToPoint.LengthSquared();
    data.index = i;

    if (data.distSqToPoint < a_maxDistSq)
    {
      a_out.points.push_back(data);
    }
  }

  for (size_t i = 0; i < m_boundaryDisks.size(); i++)
  {
  }

  for (size_t i = 0; i < m_boundaryLines.size(); i++)
  {
    CPDataLine data;
    Dg::R2::CPPointLine<float> cp;
    Dg::R2::CPPointLine<float>::Result result = cp(a_disk.Center(), m_boundaryLines[i].line);

    data.u = result.u;
    data.vToLine = result.cp - a_disk.Center();
    data.distSqToLine = data.vToLine.LengthSquared();
    data.index = i;

    float distSq = FLT_MAX;
    if (result.u < 0.0f)
    {
      vec3 v = m_boundaryLines[i].line.Origin() - a_disk.Center();
      distSq = v.LengthSquared();
    }
    else if (result.u > m_boundaryLines[i].length)
    {
      vec3 v = (m_boundaryLines[i].line.Origin() + m_boundaryLines[i].length * m_boundaryLines[i].line.Direction())- a_disk.Center();
      distSq = v.LengthSquared();
    }
    else
    {
      distSq = data.distSqToLine;
    }

    if (distSq < a_maxDistSq)
    {
      a_out.lines.push_back(data);
    }
  }
}

//Set the potentially collidable set for a new puck position
void CollisionApp::SetCPData(Disk const & a_disk, float a_maxDistSq, AllCPData & a_out) const
{
  for (size_t i = 0; i < a_out.points.size(); i++)
  {
    a_out.points[i].vToPoint = m_boundaryPoints[a_out.points[i].index] - a_disk.Center();
    a_out.points[i].distSqToPoint = a_out.points[i].vToPoint.LengthSquared();
  }

  for (size_t i = 0; i < a_out.disks.size(); i++)
  {
  }

  for (size_t i = 0; i < a_out.lines.size(); i++)
  {
    Dg::R2::CPPointLine<float> cp;
    Dg::R2::CPPointLine<float>::Result result = cp(a_disk.Center(), m_boundaryLines[a_out.lines[i].index].line);

    a_out.lines[i].u = result.u;
    a_out.lines[i].vToLine = result.cp - a_disk.Center();
    a_out.lines[i].distSqToLine = a_out.lines[i].vToLine.LengthSquared();
  }
}

void CollisionApp::MovePuck(float a_dt, ModifiedPuck & a_out) const
{
  vec3 trajectory = a_dt * a_out.speed * a_out.v;
  a_out.disk.SetCenter(a_out.disk.Center() + trajectory);
}


/*
  Go through the potentially collidable set. If the puck is already intersecting an object,
  adjust the puck's velocity vector to run along the object and then discard the object. 
  Otherwise output the object.
*/
void CollisionApp::RemoveIntersectingGeometry(AllCPData const & a_cpData, 
                                              AdjacentGeometry & a_adjacent,
                                              ModifiedPuck & a_puck) const
{
  a_adjacent.Clear();
  float radiusSq = a_puck.disk.Radius();
  radiusSq *= radiusSq;

  DirMask dm;

  for (size_t i = 0; i < a_cpData.points.size(); i++)
  {
    //Find distance to point before puck moves
    float diff = a_cpData.points[i].distSqToPoint - radiusSq;

    //If the puck is already intersecting
    if (diff < EPSILON_INTERSECTS)
    {
      //Add the vector from the puck to the point to the direction mask  
      //TODO check for zero vector, but should we even? Will this ever happen?
      vec3 v = a_cpData.points[i].vToPoint;
      v.Normalize();
      dm.Add(v);

      //Check the trajectory of the puck against the mask 
      if (dm.InMask(a_puck.v))
      {
        a_puck.speed = 0.0f;
        return;
      }

      //Puck can still move along the line perpendicular to the point
      bool towards = a_puck.v.Dot(v) > 0.0f;
      if (towards)
      {
        vec3 perpVec = v.Perpendicular();
        float ratio = perpVec.PerpDot(a_puck.v);
        if (ratio > 0.0f)
        {
          a_puck.v = perpVec;
          a_puck.speed *= ratio;
        }
        else
        {
          a_puck.v = -perpVec;
          a_puck.speed *= (-ratio);
        }
      }
    }
    else
    {
      //Not intersecting, but close enough that we'll check against this later
      a_adjacent.points.push_back(i);
    }
  }

  for (size_t i = 0; i < a_cpData.lines.size(); i++)
  {
    float diff = a_cpData.lines[i].distSqToLine - radiusSq;

    size_t index = a_cpData.lines[i].index;

    bool intersecting = (a_cpData.lines[i].u > 0.0f)
      && (a_cpData.lines[i].u < m_boundaryLines[index].length)
      && (diff < EPSILON_INTERSECTS);

    //Intersecting!
    if (intersecting)
    {
      //TODO check for zero vector, but should we even? Will this ever happen?
      vec3 v = a_cpData.lines[i].vToLine;
      v.Normalize();
      dm.Add(v);

      //We can't move anywhere 
      if (dm.InMask(a_puck.v))
      {
        a_puck.speed = 0.0f;
        return;
      }

      bool towards = a_puck.v.Dot(v) >= 0.0f;
      if (towards)
      {
        float ratio = m_boundaryLines[index].line.Direction().Dot(a_puck.v);
        a_puck.speed *= abs(ratio);

        if (abs(a_puck.speed) < EPSILON_SPEED)
        {
          a_puck.speed = 0.0f;
          return;
        }

        if (ratio > 0.0f)
        {
          a_puck.v = m_boundaryLines[index].line.Direction();
        }
        else
        {
          a_puck.v = -m_boundaryLines[index].line.Direction();
        }
      }
    }
    else
    {
      //Not intersecting, but close enough that we'll check against this later
      a_adjacent.lines.push_back(i);
    }
  }
}

/*
  Find the closest object the puck will strike (if any) and move the puck there.
*/
void CollisionApp::TestAndMovePuck(AllCPData const & a_cpData, 
                                   AdjacentGeometry const & a_adjGeom, 
                                   float & a_dt, 
                                   ModifiedPuck & a_puck) const
{
  enum
  {
    E_None,
    E_Point,
    E_Line,
    E_Disk
  };

  size_t index;
  float closestTime(a_dt);
  int colType = E_None;
  float tTemp(0.0f);

  for (size_t i = 0; i < a_adjGeom.lines.size(); i++)
  {
    size_t indLine = a_cpData.lines[a_adjGeom.lines[i]].index;
    bool hitThis = WillCollide(a_dt, 
                               a_puck.disk, 
                               a_puck.speed * a_puck.v, 
                               m_boundaryLines[indLine].line, 
                               m_boundaryLines[indLine].length, 
                               tTemp);
    if (hitThis && tTemp < closestTime)
    {
      closestTime = tTemp;
      index = indLine;
      colType = E_Line;
    }
  }

  for (size_t i = 0; i < a_adjGeom.points.size(); i++)
  {
    size_t indPoint = a_cpData.points[a_adjGeom.points[i]].index;
    bool hitThis = WillCollide(a_dt, 
                               a_puck.disk, 
                               a_puck.speed * a_puck.v, 
                               m_boundaryPoints[indPoint], 
                               tTemp);
    if (hitThis && tTemp < closestTime)
    {
      closestTime = tTemp;
      index = indPoint;
      colType = E_Point;
    }
  }

  if (colType == E_None)
  {
    MovePuck(closestTime, a_puck);
  }
  else
  {
    switch (colType)
    {
      case E_Line:
      {
        Disk larger(a_puck.disk.Center(), a_puck.disk.Radius() + EPSILON_RADIUS);

        Dg::R2::FPCDiskLine<float> fpc;
        Dg::R2::FPCDiskLine<float>::Result result_fpc = fpc(larger, a_puck.speed * a_puck.v, m_boundaryLines[index].line, vec3::ZeroVector());
        
        MovePuck(result_fpc.t, a_puck);

        //Find new velocity vector
        float ratio = m_boundaryLines[index].line.Direction().Dot(a_puck.v);
        a_puck.speed *= abs(ratio);

        if (abs(a_puck.speed) < EPSILON_SPEED)
        {
          a_puck.speed = 0.0f;
        }
        else
        {
          if (ratio > 0.0f)
          {
            a_puck.v = m_boundaryLines[index].line.Direction();
          }
          else
          {
            a_puck.v = -m_boundaryLines[index].line.Direction();
          }
        }

        break;
      }
      case E_Point:
      {
        Disk larger(a_puck.disk.Center(), a_puck.disk.Radius() + EPSILON_RADIUS);

        Dg::R2::FPCPointDisk<float> fpc;
        Dg::R2::FPCPointDisk<float>::Result result_fpc = fpc(larger, a_puck.speed * a_puck.v, m_boundaryPoints[index], vec3::ZeroVector());

        MovePuck(result_fpc.t, a_puck);

        vec3 perpVec = m_boundaryPoints[index] - a_puck.disk.Center();
        perpVec.Normalize();
        perpVec = perpVec.Perpendicular();
        float ratio = perpVec.PerpDot(a_puck.v);
        if (ratio > 0.0f)
        {
          a_puck.v = perpVec;
          a_puck.speed *= ratio;
        }
        else
        {
          a_puck.v = -perpVec;
          a_puck.speed *= (-ratio);
        }

        break;
      }
    }
  }

  a_dt -= closestTime;
}

vec3 CollisionApp::MovePuck(Puck const & a_puck, float a_dt) const
{
  //Objects further than this distance we can safely discard.
  //We'll give it a buffer of 1.2
  float buffer = 1.2f;
  float maxObjDist = a_puck.disk.Radius() + a_puck.speed * a_dt * buffer;
  float maxObjDistSq = maxObjDist * maxObjDist;

  ModifiedPuck newPuck{a_puck.speed, vec3(cos(a_puck.angle), sin(a_puck.angle), 0.0f), a_puck.disk};

  do
  {
    AllCPData cpData;
    AdjacentGeometry adjGeom;

    GetPCS(a_puck.disk, maxObjDistSq, cpData);
    if (cpData.Empty())
    {
      MovePuck(a_dt, newPuck);
      break;
    }

    RemoveIntersectingGeometry(cpData, adjGeom, newPuck);
    if (newPuck.speed == 0.0f) break;
    if (adjGeom.Empty())
    {
      MovePuck(a_dt, newPuck);
      break;
    }

    TestAndMovePuck(cpData, adjGeom, a_dt, newPuck);
    if (a_dt < EPSILON_TIME) 
    {
      break;
    }

    SetCPData(a_puck.disk, maxObjDistSq, cpData);
    RemoveIntersectingGeometry(cpData, adjGeom, newPuck);
    if (newPuck.speed == 0.0f) 
    {
      break;
    }
    if (adjGeom.Empty())
    {
      MovePuck(a_dt, newPuck);
      break;
    }
    //We could do another one after this, but two should rounds should be enough
    TestAndMovePuck(cpData, adjGeom, a_dt, newPuck);

  } while (false);

  return newPuck.disk.Center();
}

void CollisionApp::DoPhysics(double a_dt)
{
  float dt = float(a_dt);
  m_player.angle += (m_dTurn * dt);

  m_player.disk.SetCenter(MovePuck(m_player, dt));
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