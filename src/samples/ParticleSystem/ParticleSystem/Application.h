#ifndef APPLICATION_H
#define APPLICATION_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

#include "particle_system/DgParticleSystem.h"
#include "Renderer.h"

struct GLFWwindow;

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
    E_GenVelCone,
    E_GenColor,
    E_GenLife,
    E_GenSize,

    //Emitters
    E_Emitter_1,
    E_Emitter_2,
    E_Emitter_3,

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

  //The system will contain the following entities
  int                 m_att_1;
  int                 m_att_2;
  int                 m_emitter_1;
  int                 m_emitter_2;
  int                 m_emitter_3;

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

  //Main initializer function. All others are called through here.
  bool Init();

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