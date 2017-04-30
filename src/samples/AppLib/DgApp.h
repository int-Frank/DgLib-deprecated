#ifndef DGAPP_H
#define DGAPP_H

#include <string>
#include <vector>

class Event;
struct GLFWwindow;

class DgApp
{
public:

  DgApp();

  virtual ~DgApp();

  DgApp(const DgApp&);
  DgApp& operator= (const DgApp&);

  void RUN(DgApp *);
  static DgApp * GetInstance();

  //Internal...
  void PushEvent(Event const &);

public:

  virtual void KeyEvent(int, int) {}
  virtual void UpdateScroll(double) {}

protected:

  virtual bool Init() { return true; }
  virtual void Shutdown() {}

  virtual void BuildUI() {}
  virtual void DoFrame(double dt) {}

protected:
  //Some handy methods...
  bool FileExists(std::string const &) const;

  std::vector<std::string> GetFiles(std::string const & a_dirPath,
                                    std::string a_ext) const;
protected:

  GLFWwindow * GetWindow();

private:

  class PIMPL;
  PIMPL * m_pimpl;
};

#endif