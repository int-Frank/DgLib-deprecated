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

  void Run(DgApp *);
  static DgApp * GetInstance();

  void PushEvent(Event const &);

  std::string CurrentFile() const;
  virtual void NewProject() {}
  virtual bool LoadProject(const std::string & a_fileName) { return true; }
  virtual bool SaveProject(const std::string & a_filePath) { return true; }

  virtual void KeyEvent(int, int) {}
  virtual void UpdateScroll(double) {}

  void SetDirty(bool);

protected:

  virtual bool Init() { return true; }
  virtual void Shutdown() {}

  virtual void BuildUI() {}
  virtual void DoFrame(double dt) {}

  GLFWwindow * GetWindow();

  void AddFileHandlingMenuItems();
  void AddFileHandlingWindows();

private:

  class PIMPL;
  PIMPL * m_pimpl;
};

#endif