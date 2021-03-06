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
  void ClearProject();
  void NewProject();
  void UpdateScroll(double);
  void KeyEvent(int, int);

private:

  static Application* s_app;

private:

  //The GUI touches only this struct. After which, we can 
  //go through and check if anything has changed and update the
  //app accordingly. Exceptions are:
  //  - Opening a project (creates event)
  //  - Saving a project (creates event)
  //  - 'Quit' menu item modifies m_shouldQuit directly
  //  - anything from the UI namespace
  ProjectData               m_projData;

  EventManager              m_eventManager;
  Dg::ParticleSystem<float> m_particleSystem;

  std::string const         m_configFileName = "config.ini";
  std::string const         m_projectPath = "./projects/";
  std::string const         m_fileExt = "dgp";

  AppInfo		                m_info;
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
  double                    m_maxCamZoomOut;

  bool                      m_camCanRotate;
  bool                      m_shouldQuit;

  Dg::IDManager<int>        m_IDManager;

private:
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
  void BuildLineRenderData(std::vector<LineRenderData> & a_out);
  void GetRenderTransforms(Dg::R3::Matrix<float> & a_mv
                         , Dg::R3::Matrix<float> & a_proj
                         , float & a_parScale);
  void UpdateProjectTitle(std::string const &);

  void UpdateParSysAttr();

  void UI_NewFrame();
  void HandleEvents();
  void DoLogic(double dt);
  void Render();

  std::vector<std::string> GetProjects();

  void Shutdown();

private:

  //The windows stack allows us to work around ImGui's issue of stacking modals
  //called from menu items.
  enum Modal
  {
    SavePrompt,
    OverwriteWindow,
    SaveAsWindow,
    OpenWindow,
    NewProjectRequest,
    ViewHelp,
    ViewAbout,
    None
  };

  std::stack<int>          m_windowStack;

  void ShowMainGUIWindow();
};

#endif