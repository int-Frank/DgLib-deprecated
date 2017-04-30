#ifndef DGAPP_H
#define DGAPP_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <stack>

#include "EventManager.h"

class Renderer
{
public:
  virtual ~Renderer() {}
  virtual bool Init() {};
  virtual void Shutdown() {};
};

class AppInfo
{
public:

  AppInfo()
    : title("New Dg Application")
    , windowWidth(800)
    , windowHeight(600)
    , majorVersion(4)
    , minorVersion(3)
    , samples(0)
    , fullscreen(false)
  {}

  char title[128];
  int windowWidth;
  int windowHeight;
  int majorVersion;
  int minorVersion;
  int samples;
  bool fullscreen;
};

class DgApp
{
public:
  DgApp();

  virtual ~DgApp() {}

  DgApp(const DgApp&);
  DgApp& operator= (const DgApp&);

  void Run(DgApp*);

  void PushEvent(Event const &);

  void UpdateScroll(double);
  void KeyEvent(int key, int action);

  static DgApp * GetInstance() { return s_app; }

protected:

  //These need to be overridden
  virtual bool _Init() { return true; }
  virtual void _Shutdown() {}
  virtual void _KeyEvent(int, int) {}
  virtual void _UpdateScroll(double) {}
  virtual void _HandleEvents() {}
  virtual void _DoLogic(double dt) {}
  virtual void _Render() {}
  virtual void _BuildUI() {}

  //Some useful methods
  std::vector<std::string> GetFiles(std::string const & a_dirPath, 
                                    std::string a_ext = std::string("*")) const;
  bool FileExists(std::string const &) const;

private:

  static DgApp* s_app;

private:

  std::string const         m_configFileName = "config.ini";

  EventManager              m_eventManager;

  AppInfo		                m_info;
  GLFWwindow*               m_window;
  Renderer *                m_pRenderer;

  double                    m_camZoom;
  bool                      m_shouldQuit;

private:
  //Main initializer function. All others are called through here.
  bool Init();

  void GetConfiguration();
  bool InitGL();

  void UI_NewFrame();
  void HandleEvents();
  void DoLogic(double dt);
  void Render();

  void Shutdown();

  //Some useful functions


private:

  //The windows stack allows us to work around ImGui's issue of stacking modals
  //called from menu items.
  enum Modal
  {
    OpenWindow,
    ViewAbout,
    None
  };

  std::stack<int>          m_windowStack;

};

#endif