#ifndef APPLICATION_H
#define APPLICATION_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

#include "particle_system/DgParticleSystem.h"
#include "Renderer.h"
#include "Types.h"
#include "DgIDManager.h"

struct GLFWwindow;
typedef std::pair<EmitterData, EmitterData> eDataItem;
typedef std::vector<eDataItem> eData;
typedef std::pair<AttractorData, AttractorData> aDataItem;
typedef std::vector<aDataItem> aData;

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

  struct ParSysOpts
  {
    bool  useUpdaterRelativeForce;
    bool  useUpdaterColor;
    bool  useUpdaterSize;
    bool  useUpdaterResetAccel;
    int   preset;
  };

private:

  static Application* s_app;

private:

  Dg::ParticleSystem<float> m_particleSystem;

private:

  char * const              m_configFileName = "config.ini";

  APPINFO		                m_info;
  GLFWwindow*               m_window;
  Renderer                  m_renderer;

  float                     m_dt;

  double                    m_mouseSpeed;
  double                    m_mouseCurrentX;
  double                    m_mouseCurrentY;
  double                    m_mousePrevX;
  double                    m_mousePrevY;

  double                    m_camRotX;
  double                    m_camRotZ;

  double                    m_camZoom;
  double                    m_camZoomTarget;

  eData                     m_eData;
  aData                     m_aData;
  int                       m_attrFocus;

  ParSysOpts                m_parSysOpts;
  ParSysOpts                m_parSysOptsPrev;

  Dg::IDManager<int>        m_IDManager;
  bool                      m_shouldQuit;

  char const *              m_projectPath = "./projects/";
  char const *              m_fileExt = "dgp";

  std::string               m_loadFile;
  std::string               m_saveFile;

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
  std::vector<std::string> GetProjects();
  bool LoadFile(std::string) { return true; }
  bool SaveFile(std::string) { return true; }

  void Shutdown();
};

#endif