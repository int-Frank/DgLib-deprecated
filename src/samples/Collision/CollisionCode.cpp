
#include "CollisionApp.h"
#include "query\DgR2QueryDiskLine.h"
#include "query\DgR2QueryPointDisk.h"
#include "query\DgR2QueryDiskDisk.h"
#include "query\DgR2QueryPointLine.h"
#include "DirectionMask.h"

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
                 BoundaryPoint const & a_point,
                 float & a_outTime)
{
  if (!a_point.IsInArc(a_disk.Center()))
  {
    return false;
  }

  Dg::R2::FPCPointDisk<float> fpc;
  Dg::R2::FPCPointDisk<float>::Result result_fpc = fpc(a_disk, a_vel, a_point.origin, vec3::ZeroVector());

  bool willCollide = result_fpc.code == Dg::QC_Intersecting;
  willCollide = willCollide && (result_fpc.t <= a_dt);
  willCollide = willCollide && (result_fpc.t >= 0.0f);
  a_outTime = result_fpc.t;

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
  a_outTime = result_fpc.t;

  return willCollide;
}

bool BoundaryPoint::IsInArc(vec3 const & a_point) const
{
  vec3 v = a_point - origin;

  float loaPv = loa.PerpDot(v);
  float roaPv = roa.PerpDot(v);

  return (loaPv < 0.0f) && (roaPv > 0.0f);
}

bool AllCPData::Empty() const
{
  return (nLines == 0)
    && (nPoints == 0)
    && (nDisks == 0);
}

void AllCPData::SetFlags(bool a_val)
{
  for (int i = 0; i < nLines; i++)
    lines[i].active = a_val;
  for (int i = 0; i < nPoints; i++)
    points[i].active = a_val;
  for (int i = 0; i < nDisks; i++)
    disks[i].active = a_val;
}

//Develop a Potentially Collidable Set
void CollisionApp::GetPCS(Disk const & a_disk, AllCPData & a_out) const
{
  a_out.nLines = 0;
  a_out.nPoints = 0;
  a_out.nDisks = 0;

  float radSq = a_disk.Radius() * a_disk.Radius();

  for (size_t i = 0; i < m_boundaryPoints.size(); i++)
  {
    a_out.points[a_out.nPoints].vToPoint = m_boundaryPoints[i].origin - a_disk.Center();
    a_out.points[a_out.nPoints].distSqToPoint = a_out.points[a_out.nPoints].vToPoint.LengthSquared();

    if (a_out.points[a_out.nPoints].distSqToPoint < radSq)
    {
      a_out.points[a_out.nPoints].index = i;
      a_out.points[a_out.nPoints].active = true;
      a_out.nPoints++;
    }
  }

  for (size_t i = 0; i < m_boundaryDisks.size(); i++)
  {
    a_out.disks[a_out.nDisks].vToDisk = m_boundaryDisks[i].Center() - a_disk.Center();
    a_out.disks[a_out.nDisks].distSqToDisk = a_out.disks[a_out.nDisks].vToDisk.LengthSquared();

    float rSq = m_boundaryDisks[i].Radius() + a_disk.Radius();
    rSq *= rSq;

    if (a_out.disks[a_out.nDisks].distSqToDisk < rSq)
    {
      a_out.disks[a_out.nDisks].index = i;
      a_out.disks[a_out.nDisks].active = true;
      a_out.nDisks++;
    }
  }

  for (size_t i = 0; i < m_boundaryLines.size(); i++)
  {
    Dg::R2::CPPointLine<float> cp;
    Dg::R2::CPPointLine<float>::Result result = cp(a_disk.Center(), m_boundaryLines[i].line);

    a_out.lines[a_out.nLines].u = result.u;
    a_out.lines[a_out.nLines].vToLine = result.cp - a_disk.Center();
    a_out.lines[a_out.nLines].distSqToLine = a_out.lines[a_out.nLines].vToLine.LengthSquared();

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
      distSq = a_out.lines[a_out.nLines].distSqToLine;
    }

    if (distSq < radSq)
    {
      a_out.lines[a_out.nLines].index = i;
      a_out.lines[a_out.nLines].active = true;
      a_out.nLines++;
    }
  }
}

//Set the potentially collidable set for a new puck position
void CollisionApp::SetCPData(Disk const & a_disk, AllCPData & a_out) const
{
  for (size_t i = 0; i < a_out.nPoints; i++)
  {
    a_out.points[i].vToPoint = m_boundaryPoints[a_out.points[i].index].origin - a_disk.Center();
    a_out.points[i].distSqToPoint = a_out.points[i].vToPoint.LengthSquared();
  }

  for (size_t i = 0; i < a_out.nDisks; i++)
  {
    a_out.disks[i].vToDisk = m_boundaryDisks[a_out.disks[i].index].Center() - a_disk.Center();
    a_out.disks[i].distSqToDisk = a_out.disks[i].vToDisk.LengthSquared();
  }

  for (size_t i = 0; i < a_out.nLines; i++)
  {
    Dg::R2::CPPointLine<float> cp;
    Dg::R2::CPPointLine<float>::Result result = cp(a_disk.Center(), m_boundaryLines[a_out.lines[i].index].line);

    a_out.lines[i].u = result.u;
    a_out.lines[i].vToLine = result.cp - a_disk.Center();
    a_out.lines[i].distSqToLine = a_out.lines[i].vToLine.LengthSquared();
  }
}


void CollisionApp::FindIntersections(ModifiedPuck & a_puck, AllCPData & a_cpData) const
{
  float radiusSq = a_puck.disk.Radius();
  radiusSq *= radiusSq;

  DirMask dm;
  vec3 originalV = a_puck.v;

  for (size_t i = 0; i < a_cpData.nPoints; i++)
  {
    //Find distance to point before puck moves
    size_t index = a_cpData.points[i].index;
    float diff = a_cpData.points[i].distSqToPoint - radiusSq;

    bool intersecting = diff < EPSILON_INTERSECTS;
    intersecting = intersecting && m_boundaryPoints[index].IsInArc(a_puck.disk.Center());

    //If the puck is already intersecting
    if (intersecting)
    {
      a_cpData.points[i].active = false;
      //Add the vector from the puck to the point to the direction mask
      vec3 v = a_cpData.points[i].vToPoint;
      if (v.IsZero())
        v = vec3::xAxis();
      else
      {
        v.Normalize();
        dm.Add(v);
      }

      //Check the trajectory of the puck against the mask 
      if (dm.InMask(originalV))
      {
        a_puck.speed = 0.0f;
        return;
      }

      //Puck can still move along the line perpendicular to the point
      bool towards = a_puck.v.Dot(v) > 0.0f;
      if (towards)
      {
        vec3 perpVec = v.Perpendicular();
        float ratio = v.PerpDot(a_puck.v);
        a_puck.speed *= abs(ratio);
        if (ratio > 0.0f)
        {
          a_puck.v = perpVec;
        }
        else
        {
          a_puck.v = -perpVec;
        }
      }
    }
  }

  for (size_t i = 0; i < a_cpData.nDisks; i++)
  {
    //Find distance to point before puck moves
    size_t index = a_cpData.disks[i].index;
    float radSq = m_boundaryDisks[index].Radius() + a_puck.disk.Radius();
    radSq *= radSq;

    float diff = a_cpData.disks[i].distSqToDisk - radSq;

    bool intersecting = diff < EPSILON_INTERSECTS;

    //If the puck is already intersecting
    if (intersecting)
    {
      a_cpData.disks[i].active = false;
      //Add the vector from the puck to the point to the direction mask  
      vec3 v = a_cpData.disks[i].vToDisk;
      if (v.IsZero())
        v = vec3::xAxis();
      else
      {
        v.Normalize();
        dm.Add(v);
      }

      //Check the trajectory of the puck against the mask 
      if (dm.InMask(originalV))
      {
        a_puck.speed = 0.0f;
        return;
      }

      //Puck can still move along the line perpendicular to the point
      bool towards = a_puck.v.Dot(v) > 0.0f;
      if (towards)
      {
        vec3 perpVec = v.Perpendicular();
        float ratio = v.PerpDot(a_puck.v);
        a_puck.speed *= abs(ratio);
        if (ratio > 0.0f)
        {
          a_puck.v = perpVec;
        }
        else
        {
          a_puck.v = -perpVec;
        }
      }
    }
  }

  for (size_t i = 0; i < a_cpData.nLines; i++)
  {
    float diff = a_cpData.lines[i].distSqToLine - radiusSq;

    size_t index = a_cpData.lines[i].index;

    bool intersecting = (a_cpData.lines[i].u > 0.0f)
      && (a_cpData.lines[i].u < m_boundaryLines[index].length)
      && (diff < EPSILON_INTERSECTS);

    //Intersecting!
    if (intersecting)
    {
      a_cpData.lines[i].active = false;
      vec3 v = a_cpData.lines[i].vToLine;
      if (v.IsZero())
        v = vec3::xAxis();
      else
      {
        v.Normalize();
        dm.Add(v);
      }

      //We can't move anywhere 
      if (dm.InMask(originalV))
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
  }
}

float CollisionApp::StepToNextIntersection(ModifiedPuck & a_puck, float a_dt, AllCPData const & a_cpData) const
{
  float closestTime(a_dt);
  float tTemp(0.0f);

  for (size_t i = 0; i < a_cpData.nLines; i++)
  {
    if (!a_cpData.lines[i].active)
      continue;

    size_t indLine = a_cpData.lines[i].index;
    bool hitThis = WillCollide(a_dt, 
      a_puck.disk, 
      a_puck.speed * a_puck.v, 
      m_boundaryLines[indLine].line, 
      m_boundaryLines[indLine].length, 
      tTemp);
    if (hitThis && tTemp < closestTime)
      closestTime = tTemp;
  }

  for (size_t i = 0; i < a_cpData.nPoints; i++)
  {
    if (!a_cpData.points[i].active)
      continue;

    size_t indPoint = a_cpData.points[i].index;
    bool hitThis = WillCollide(a_dt, 
      a_puck.disk, 
      a_puck.speed * a_puck.v, 
      m_boundaryPoints[indPoint], 
      tTemp);
    if (hitThis && tTemp < closestTime)
      closestTime = tTemp;
  }

  for (size_t i = 0; i < a_cpData.nDisks; i++)
  {
    if (!a_cpData.disks[i].active)
      continue;

    size_t indPoint = a_cpData.disks[i].index;
    bool hitThis = WillCollide(a_dt, 
      a_puck.disk, 
      a_puck.speed * a_puck.v, 
      m_boundaryDisks[indPoint],
      vec3::ZeroVector(),
      tTemp);
    if (hitThis && tTemp < closestTime)
      closestTime = tTemp;
  }

  //Move the puck
  vec3 trajectory = closestTime * a_puck.speed * a_puck.v;
  a_puck.disk.SetCenter(a_puck.disk.Center() + trajectory);

  return a_dt - closestTime;
}

vec3 CollisionApp::MovePuck(Puck const & a_puck, float a_dt) const
{
  if (Dg::IsZero(a_puck.speed))
    return a_puck.disk.Center();

  ModifiedPuck newPuck{a_puck.speed, vec3(cos(a_puck.angle), sin(a_puck.angle), 0.0f), a_puck.disk};

  if (newPuck.speed < 0.0f)
  {
    newPuck.speed = - newPuck.speed;
    newPuck.v = -newPuck.v;
  }

  //Objects further than this distance we can safely discard.
  //Floating point  buffer
  float encDiskRad = (newPuck.disk.Radius() + newPuck.speed * a_dt) + EPSILON_RADIUS;

  AllCPData cpData;
  GetPCS(Disk(a_puck.disk.Center(), encDiskRad), cpData);

  if (cpData.Empty())
  {
    //Move the puck
    vec3 trajectory = a_dt * newPuck.speed * newPuck.v;
    newPuck.disk.SetCenter(newPuck.disk.Center() + trajectory);
  }
  else
  {
    float dt = a_dt;
    float radiusOrig = newPuck.disk.Radius();
    float radiusLarge = radiusOrig + EPSILON_RADIUS;

    //We can loop through as many times as we want, but 2 should be enough. 2 is
    //also the minimum for this algorithm to work
    int counter = 2;
    while (true)
    {
      counter--;

      //Test for intersections
      newPuck.disk.SetRadius(radiusLarge);

      FindIntersections(newPuck, cpData);
      if (newPuck.speed == 0.0f) 
        break;

      newPuck.disk.SetRadius(radiusOrig);

      dt = StepToNextIntersection(newPuck, dt, cpData);
      if (dt < EPSILON_TIME || counter == 0)
        break;

      SetCPData(newPuck.disk, cpData);
      cpData.SetFlags(true);
    }
  }
  return newPuck.disk.Center();
}

void CollisionApp::DoPhysics(double a_dt)
{
  float dt = float(a_dt);
  m_player.angle += (m_dTurn * dt);

  m_player.disk.SetCenter(MovePuck(m_player, dt));
}
