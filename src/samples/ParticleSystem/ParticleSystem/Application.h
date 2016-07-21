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
  void NewProject();
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

  //The GUI touches only this struct. After which, we can 
  //go through and check if anything has changed and update the
  //app accordingly. Exceptions are:
  //  - Opening a project (creates event)
  //  - Saving a project (creates event)
  //  - Creating Emitters / Attractors (creates them directly)
  //  - Killing Emitters / Attractors (kills them directly)
  //  - 'Quit' menu item modifies m_shouldQuit direcly
  //  - anything from the UI namespace
  ProjectData               m_projData;

  EventManager              m_eventManager;
  Dg::ParticleSystem<float> m_particleSystem;

  char * const              m_configFileName = "config.ini";

  APPINFO		                m_info;
  GLFWwindow*               m_window;
  Renderer                  m_renderer;

  double                    m_mouseSpeed;
  double                    m_mouseCurrentX;
  double                    m_mouseCurrentY;
  double                    m_mousePrevX;
  double                    m_mousePrevY;

  double                    m_camRotX;
  double                    m_camRotZ;

  double                    m_camZoom;
  double                    m_camZoomTarget;

  int                       m_focusAttr;
  int                       m_focusEmitter;

  bool                      m_camCanRotate;

  Dg::IDManager<int>        m_IDManager;
  bool                      m_shouldQuit;

  std::string const         m_projectPath = "./projects/";
  std::string const         m_fileExt = "dgp";

  //Main initializer function. All others are called through here.
  bool Init();

  void GetConfiguration();
  bool InitGL();
  void ResetCamera();
  void InitParticleSystem();
  int  AddEmitter();
  int  AddAttractor();
  void CreateFileList(std::vector<std::string> const &
                    , char const *
                    , int *);

  void UpdateParSysAttr();

  void UI_NewFrame();
  void HandleEvents();
  void DoLogic(double dt);
  void Render();

  std::vector<std::string> GetProjects();

  void Shutdown();

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

  void ShowMainGUIWindow();
};

#endif