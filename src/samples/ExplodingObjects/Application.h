#ifndef APPLICATION_H
#define APPLICATION_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <stack>

#include "Renderer.h"
#include "EventManager.h"


class TimeManager
{
public:
  virtual ~TimeManager() {}
  virtual float GetTime(float dt) { return 0.f; }
};

class TimeAccumulator : public TimeManager
{
public:

  TimeAccumulator() : m_totalTime(0.f) {}
  float GetTime(float dt) 
  { 
    m_totalTime += dt;
    return m_totalTime; 
  }

private:
  float m_totalTime;
};

class Application
{
public:
  Application();

  ~Application() { delete m_ptimeManager; }

  Application(const Application&);
  Application& operator= (const Application&);

  void Run(Application*);

  void PushEvent(Event const &);

  bool LoadProject(std::string const &);
  void UpdateScroll(double);
  void KeyEvent(int key, int action);
  void ClearProject();

  void Explode();

  static Application * GetInstance() { return s_app; }
private:

  static Application* s_app;

private:

  //UI only touches this data
  AppData                   m_appData;

  EventManager              m_eventManager;

  std::string const         m_configFileName = "config.ini";
  std::string const         m_projectPath = "./objects/";
  std::string const         m_fileExt = "obj";

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

  TimeManager *             m_ptimeManager;
  std::vector<vec4>         m_centroids;

private:
  //Main initializer function. All others are called through here.
  bool Init();

  void ResetCamera();
  void GetConfiguration();
  bool InitGL();
  void CreateFileList(std::vector<std::string> const &
                    , char const *
                    , int *);
  void UpdateProjectTitle(std::string const &);

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
    OpenWindow,
    ViewHelp,
    ViewAbout,
    None
  };

  std::stack<int>          m_windowStack;

  void ShowMainGUIWindow();
};

#endif