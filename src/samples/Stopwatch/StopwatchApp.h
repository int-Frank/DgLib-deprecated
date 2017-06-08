#ifndef STOPWATCHAPP_H
#define STOPWATCHAPP_H

#include "DgApp.h"
#include "DgTimer.h"

class StopwatchApp : public DgApp
{
public:

  StopwatchApp();

private:

  void BuildUI();
  void DoFrame();

private:

  Dg::Timer m_timer;
};

#endif