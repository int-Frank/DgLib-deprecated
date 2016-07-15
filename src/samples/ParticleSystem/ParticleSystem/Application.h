#ifndef APPLICATION_H
#define APPLICATION_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

#include "particle_system/DgParticleSystem.h"
#include "Renderer.h"
#include "Types.h"
#include "DgIDManager.h"

struct GLFWwindow;

class Application
{
public:

  Application();

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

  static int const    s_nEmitters = 1;
  static int const    s_nAttractors = 1;

private:

  char * const m_configFileName = "config.ini";

  APPINFO		          m_info;
  GLFWwindow*         m_window;
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

  AttractorData       m_aData[s_nAttractors];
  AttractorData       m_aDataPrev[s_nAttractors];

  Dg::IDManager<int>  m_IDManager;

  //Main initializer function. All others are called through here.
  bool Init();

  void GetConfiguration();
  bool InitGL();
  void InitControls();
  void InitParticleSystem();

  void UpdateParSysAttr();

  void HandleInput();
  void DoLogic();
  void Render();

  void BuildMainUI();

  void Shutdown();
};

#endif