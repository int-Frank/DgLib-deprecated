#ifndef RENDERTESTAPP_H
#define RENDERTESTAPP_H

#include <vector>
#include <map>

#include "DgApp.h"
#include "DgR2Vector.h"
#include "DgR3Vector.h"
#include "DgR2Disk.h"
#include "DgR2Line.h"
#include "DgR3Matrix.h"
#include "Mod_Context_Line.h"
#include "Mod_Window.h"

typedef Dg::R2::Vector<float> vec3;
typedef Dg::R3::Vector<float> vec4;
typedef Dg::R3::Matrix<float> mat4;
typedef Dg::R2::Disk<float>   Disk;
typedef Dg::R2::Line<float>   Line;

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

private:

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
    bool    isHit;
  };

  struct BoundaryPoint
  {
    vec3    p;
  };

  struct BoundaryPDisk
  {
    Disk disk;
  };

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