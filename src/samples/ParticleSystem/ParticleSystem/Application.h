#ifndef APPLICATION_H
#define APPLICATION_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <stack>

#include "particle_system/DgParticleSystem.h"
#include "Renderer.h"
#include "Types.h"
#include "DgIDManager.h"
#include "EventManager.h"

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

  void PushEvent(Event const &);

  static Application * GetInstance() { return s_app; }

  bool LoadProject(std::string);
  bool SaveProject(std::string);
  void UpdateScroll(double);
  void KeyEvent(int, int);

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

  EventManager              m_eventManager;
  Dg::ParticleSystem<float> m_particleSystem;

  char * const              m_configFileName = "config.ini";

  APPINFO		                m_info;
  GLFWwindow*               m_window;
  Renderer                  m_renderer;

  //float                     m_dt;

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
  bool                      m_camCanRotate;

  ParSysOpts                m_parSysOpts[2];

  Dg::IDManager<int>        m_IDManager;
  bool                      m_shouldQuit;

  char const *              m_projectPath = "./projects/";
  char const *              m_fileExt = "dgp";

  //Main initializer function. All others are called through here.
  bool Init();

  void GetConfiguration();
  bool InitGL();
  void InitControls();
  void InitParticleSystem();

  void UpdateParSysAttr();

  void HandleEvents();
  void DoLogic(double dt);
  void Render();

  std::vector<std::string> GetProjects();

private:

  //UI
  enum Modal
  {
    SavePrompt,
    OverwriteWindow,
    SaveAsWindow,
    OpenWindow,
    None
  };

  std::stack<int>          m_windowStack;

  void BuildMainUI();
  void BuildMenu();
  void HandleSavePrompt();
  void HandleOverwrite();
  void HandleSaving();
  void HandleOpen();
  

  void Shutdown();
};

#endif