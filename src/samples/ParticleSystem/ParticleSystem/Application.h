#ifndef APPLICATION_H
#define APPLICATION_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

#include "particle_system/DgParticleSystem.h"
#include "Renderer.h"

struct GLFWwindow;

enum
{
  //Updaters 
  E_UpdaterLife,
  E_UpdaterZeroAccel,
  E_UpdaterAttractor_1,
  E_UpdaterAttractor_2,
  E_UpdaterEuler,
  E_UpdaterColor,
  E_UpdaterSize,

  //Generators
  E_GenPosPoint,
  E_GenPosBox,
  E_GenPosSphere,
  E_GenVelCone,
  E_GenVelOutwards,
  E_GenColor,
  E_GenLife,
  E_GenSize,
  E_GenForce,

  //Emitter types
  E_Emitter_Linear,
  E_Emitter_Poisson,

  //Emitters
  E_Emitter_1,
  E_Emitter_2,
  E_Emitter_3,

  E_NULL
};

class AttractorData
{
public:
  int ID;
  int strength;           int prev_strength;
  int maxAccelMag;        int prev_maxAccelMag;
  float transform[6];     float prev_transform[6]; //[x, y, z, rz, rx, scale]
};

class EmitterData
{
public:

  EmitterData()
    : ID(0)
    , emitterType(E_Emitter_Linear)
    , on(true)
    , posGenMethod(E_GenPosPoint)
    , velGenMethod(E_GenVelCone)
    , transform{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f}
    , boxDim{0.5f, 0.5f, 0.5f}
    , velCone{0.0f, 0.0f, Dg::PI_f / 8.0f }
    , velocity(1.0f)
    , sphereRadius(1.0f)
    , colors{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f}
    , rate(10.0f)
    , life(5.0f)
    , force(1.0f)
    , sizes{0.1f, 0.1f}
  {}

  int ID;
  int emitterType;       
  bool on;               
  int posGenMethod; // point, box, sphere
  int velGenMethod; // direction, repel from center     
  float transform[7]; // [x, y, z, rx, ry, rz, scale]         
  float boxDim[3];       
  float velCone[3]; // [rz, rx, angle]    
  float velocity;        
  float sphereRadius;    
  float colors[8]; //[sr, sg, sb, sa, er, eg, eb, ea]
  float rate;            
  float life;            
  float force;           
  float sizes[2]; //[start, end]
};

class Application
{
public:

  Application() : m_window(nullptr)
                , m_particleSystem(65536){}
  ~Application() {}

  Application(const Application&);
  Application& operator= (const Application&);

  void Run(Application*);

  static double s_scrollOffset;

public:
  struct APPINFO
  {
    char title[128];
    int windowWidth;
    int windowHeight;
    int majorVersion;
    int minorVersion;
    int samples;
    union
    {
      struct
      {
        unsigned int    fullscreen : 1;
        unsigned int    vsync : 1;
        unsigned int    debug : 1;
      };
      unsigned int        all;
    } flags;
  };
private:

  static Application* s_app;

private:

  Dg::ParticleSystem<float>   m_particleSystem;

  static int const    s_nEmitters = 3;
  static int const    s_nAttractors = 2;

private:

  char * const m_configFileName = "config.ini";

  APPINFO		          m_info;
  GLFWwindow*         m_window;
  GLuint              m_renderingProgram;
  GLuint              m_vao;
  Renderer            m_renderer;

  float               m_dt;

  double              m_mouseSpeed;
  double              m_mouseCurrentX;
  double              m_mouseCurrentY;
  double              m_mousePrevX;
  double              m_mousePrevY;

  double              m_camRotX;
  double              m_camRotZ;

  double              m_camZoom;
  double              m_camZoomTarget;

  EmitterData         m_eData[s_nEmitters];
  EmitterData         m_eDataPrev[s_nEmitters];

  //Main initializer function. All others are called through here.
  bool Init();

  void UpdateParSysAttr();

  void GetConfiguration();
  bool InitGL();
  void InitControls();
  void InitParticleSystem();

  void HandleInput();
  void DoLogic();
  void Render();

  void Shutdown();
};

#endif