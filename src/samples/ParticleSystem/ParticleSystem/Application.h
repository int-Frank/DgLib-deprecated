#ifndef APPLICATION_H
#define APPLICATION_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

#include "particle_system/DgParticleSystem.h"

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

    //Generators
    E_GenPosPoint,
    E_GenVelCone,
    E_GenColor,

    //Emitters
    E_Emitter_1,
    E_Emitter_2,
    E_Emitter_3,

  };
public:

  Application() : m_window(nullptr), m_particleSystem(4096) {}
  ~Application() {}

  Application(const Application&);
  Application& operator= (const Application&);

  void Run(Application*);
  void Render(double currentTime);
  void OnResize(int w, int h);
  void OnMouseButton(int button, int action);
  void OnMouseWheel(int pos);

  static void GetMousePosition(int& x, int& y);
  static void OnMouseMove(GLFWwindow*, double x, double y);
  static void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods);

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
        unsigned int    cursor : 1;
        unsigned int    stereo : 1;
        unsigned int    debug : 1;
      };
      unsigned int        all;
    } flags;
  };
private:

  static double       s_mouseX;
  static double       s_mouseY;
  static double       s_prevX;
  static double       s_prevY;

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

  APPINFO		          m_info;
  GLFWwindow*         m_window;
  GLuint              m_renderingProgram;
  GLuint              m_vao;
  GLuint              m_posBuffer;
  GLuint              m_indexBuffer;

  void Init();
  void InitParticleSystem();
  void Startup();
  void Shutdown();
  GLuint CompileShaders();
  GLuint LoadShaderFromFile(std::string, GLenum shaderType);
};

#endif