#ifndef RENDERTESTAPP_H
#define RENDERTESTAPP_H

#include <vector>
#include <map>

#include "DgApp.h"
#include "Types.h"
#include "Mod_Context_Line.h"
#include "Mod_Window.h"
#include "DirectionMask.h"
#include "DgDynamicArray.h"

struct ScreenObject
{
  mat4    transform;
  bool    draw;
};

struct Puck
{
  Disk    disk;
  float   angle;
  float   speed;
};

struct BoundaryLine
{
  Line    line;
  float   length;
};

class BoundaryPoint
{
public:

  bool IsInArc(vec3 const &) const;

  vec3  origin;
  vec3  loa;
  vec3  roa;
};

struct CPDataPoint
{
  size_t index;
  vec3   vToPoint;
  float  distSqToPoint;
};

struct CPDataLine
{
  size_t        index;
  float         u;
  vec3          vToLine;
  float         distSqToLine;
};

struct CPDataDisk
{
  size_t index;
  vec3   vToDisk;
  float  distSqToDisk;
};

class AllCPData
{
public:

  bool Empty() const;

  Dg::DynamicArray<CPDataLine>  lines;
  Dg::DynamicArray<CPDataPoint> points;
  Dg::DynamicArray<CPDataDisk>  disks;
};

struct ModifiedPuck
{
  float speed;
  vec3  v;
  Disk  disk;
};
class CollisionApp : public DgApp
{

public:

  CollisionApp();
  ~CollisionApp(){}

protected:

  void BuildUI(){}
  void DoFrame(double dt);
  virtual void KeyEvent(int key, int action);

private:

  void DoPhysics(double dt);
  void Render();

  vec3 MovePuck(Puck const &, float dt) const;
  void DoIntersections(ModifiedPuck &,
                       AllCPData const &) const;
  void GetPCS(Disk const &, float, AllCPData &) const;
  void SetCPData(Disk const &, AllCPData &) const;

private:

  typedef std::map<Context::ObjectHandle, ScreenObject> dMap;
  typedef std::pair<Context::ObjectHandle, ScreenObject> dItem;

  mat4                        m_transform;
  Puck                        m_player;

  Context::ObjectHandle       m_circleHandle;
  Context::ObjectHandle       m_arrowHandle;
  Context::ObjectHandle       m_circleHandleInt;
  Context::ObjectHandle       m_arrowHandleInt;

  std::vector<Puck>           m_pucks;
  std::vector<BoundaryLine>   m_boundaryLines;
  std::vector<BoundaryPoint>  m_boundaryPoints;
  std::vector<Disk>           m_boundaryDisks;
  float                       m_canvasDim;

  bool                        m_playerMoving;
  float                       m_dTurn;
  float                       m_turnRate;
  float                       m_moveRate;

  Context::ContextLine        m_contextLine;
  Window::Window *            m_pViewport;
  dMap                        m_drawables;
};

#endif