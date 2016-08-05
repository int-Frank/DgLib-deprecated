#ifndef APPLICATION_H
#define APPLICATION_H

class Application
{
public:
  Application() {}

  ~Application() {}

  Application(const Application&);
  Application& operator= (const Application&);
  void Run(Application*);

private:

  static Application* s_app;
};

#endif