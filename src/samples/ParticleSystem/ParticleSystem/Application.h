#ifndef APPLICATION_H
#define APPLICATION_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

#include "particle_system/DgParticleSystem.h"
#include "Renderer.h"

struct GLFWwindow;

class EmitterData
{
public:

  void Init()
  {
    ID = 0;
    emitterType = prev_emitterType = 0;
    on = prev_on = true;
    posGenMethod = prev_posGenMethod = 0;
    velGenMethod = prev_velGenMethod = 0;
    spread = prev_spread = 0.4f;
    repelFromCenter = prev_repelFromCenter = false;
    sphereRadius = prev_sphereRadius = 0.2f;
    rate = prev_rate = 10.0f;
    velocity = prev_velocity = 1.0f;
    life = prev_life = 5.0f;
    force = prev_force = 1.0f;
    startSize = prev_startSize = 0.02f;
    endSize = prev_endSize = 0.02f;
    
    for (int i = 0; i < 4; ++i)
    {
      startColor[i] = prev_startColor[i] = 1.0f;
      endColor[i] = prev_endColor[i] = 1.0f;
    }
    for (int i = 0; i < 3; ++i)
    {
      pos[i] = prev_pos[i] = 0.0f;
      boxDim[i] = prev_boxDim[i] = 0.5f;
    }
    for (int i = 0; i < 2; ++i)
    {
      rot[i] = prev_rot[i] = 0.0f;
      velRot[i] = prev_velRot[i] = 0.0f;
    }
  }

  int ID;

  int emitterType;        int prev_emitterType;
  bool on;                bool prev_on;
  int posGenMethod;       int prev_posGenMethod;
  int velGenMethod;       int prev_velGenMethod;
  float pos[3];           float prev_pos[3]; //[x, y, z]
  float rot[2];           float prev_rot[2]; //[rz, rx]
  float spread;           float prev_spread;
  float boxDim[3];        float prev_boxDim[3]; //[l, w, h]
  bool repelFromCenter;   bool prev_repelFromCenter;
  float velRot[2];        float prev_velRot[2]; //[rz, rx]
  float sphereRadius;     float prev_sphereRadius;
  float startColor[4];    float prev_startColor[4];
  float endColor[4];      float prev_endColor[4];
  float rate;             float prev_rate;
  float velocity;         float prev_velocity;
  float life;             float prev_life;
  float force;            float prev_force;
  float startSize;        float prev_startSize;
  float endSize;          float prev_endSize;
};

class Application
{
public:
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

    //Emitters
    E_Emitter_1,
    E_Emitter_2,
    E_Emitter_3,

    E_NULL
  };
public:

  Application() : m_window(nullptr)
                , m_particleSystem(4096)
                , m_canRotate(false){}
  ~Application() {}

  Application(const Application&);
  Application& operator= (const Application&);

  void Run(Application*);

  static void OnMouseScroll(GLFWwindow* window, double xOffset, double yOffset);
  static double s_dZoom;

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
  GLuint              m_posBuffer;
  GLuint              m_indexBuffer;

  float               m_dt;

  Renderer            m_renderer;

  bool                m_canRotate;
  double              m_mouseSpeed;
  double              m_mouseCurentX;
  double              m_mouseCurentY;
  double              m_mousePrevX;
  double              m_mousePrevY;

  double              m_camRotX;
  double              m_camRotZ;

  double              m_camZoom;

  EmitterData         m_eData[s_nEmitters];

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