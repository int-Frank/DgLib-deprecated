
#include <Windows.h>
#include <stdint.h>

#include "DgTimer.h"

typedef  LONGLONG timer_int;

namespace Dg
{
  static class TimerState 
  {
  public:

    virtual ~TimerState() {}

    virtual bool IsStarted() const { return false; }
    virtual bool IsStopped() const { return false; }
    virtual bool IsPaused() const  { return false; }

    virtual TimerState * Start()   { return nullptr; }
    virtual TimerState * Stop()    { return nullptr; }
    virtual TimerState * Pause()   { return nullptr; }
    virtual TimerState * Resume()  { return nullptr; }

    virtual double GetTime() const { return 0.0; }

    virtual TimerState * Clone() const 
    {
      return new TimerState(*this);
    }
  };

  //We need some factory functions to prevent circular dependices...
  static TimerState * GetTimerState_Off();
  static TimerState * GetTimerState_Paused(timer_int a_startTicks, timer_int a_totalTicks);

  static class TimerState_On : public TimerState
  {
  public:

    TimerState_On(timer_int a_totalTime = 0)
    {
      LARGE_INTEGER startTime;
      QueryPerformanceCounter(&startTime);
      m_startTime = startTime.QuadPart;
      m_totalTime = a_totalTime;
    }

    bool IsStarted() const { return true; }
    bool IsStopped() const { return false; }
    bool IsPaused() const { return false; }

    TimerState * Start() { return new TimerState_On(); }
    TimerState * Stop() { return GetTimerState_Off(); }
    TimerState * Pause() { return GetTimerState_Paused(m_startTime, m_totalTime); }
    TimerState * Resume() { return new TimerState_On(*this); }

    double GetTime() const
    {
      LARGE_INTEGER currentTime, frequency;

      QueryPerformanceCounter(&currentTime);
      QueryPerformanceFrequency(&frequency);

      return static_cast<double>(m_totalTime + (currentTime.QuadPart - m_startTime))
        / static_cast<double>(frequency.QuadPart);
    }

    TimerState_On * Clone() const
    {
      return new TimerState_On(*this);
    }

  private:

    timer_int m_startTime;
    timer_int m_totalTime;
  };

  static class TimerState_Off : public TimerState
  {
  public:

    TimerState_Off() {}

    bool IsStarted() const { return false; }
    bool IsStopped() const { return true; }
    bool IsPaused() const { return false; }

    TimerState * Start() { return new TimerState_On(); }
    TimerState * Stop() { return new TimerState_Off(); }
    TimerState * Pause() { return new TimerState_Off(); }
    TimerState * Resume() { return new TimerState_Off(); }

    double GetTime() const { return 0.0; }

    TimerState_Off * Clone() const
    {
      return new TimerState_Off(*this);
    }
  };

  static class TimerState_Paused : public TimerState
  {
  public:

    TimerState_Paused(timer_int a_startTime, timer_int a_totalTime)
    {
      LARGE_INTEGER pauseTime;
      QueryPerformanceCounter(&pauseTime);
      m_totalTime = a_totalTime + (pauseTime.QuadPart - a_startTime);
    }

    bool IsStarted() const { return false; }
    bool IsStopped() const { return false; }
    bool IsPaused() const { return true; }

    TimerState * Start() { return new TimerState_On(); }
    TimerState * Stop() { return new TimerState_Off(); }
    TimerState * Pause() { return new TimerState_On(m_totalTime); }
    TimerState * Resume() { return new TimerState_On(m_totalTime); }

    double GetTime() const
    {
      LARGE_INTEGER frequency;
      QueryPerformanceFrequency(&frequency);

      return static_cast<double>(m_totalTime) / static_cast<double>(frequency.QuadPart);
    }

    TimerState_Paused * Clone() const
    {
      return new TimerState_Paused(*this);
    }

  private:
    timer_int m_totalTime;
  };


  static TimerState * GetTimerState_Off()
  {
    return new TimerState_Off();
  }

  static TimerState * GetTimerState_Paused(timer_int a_startTime, timer_int a_totalTime)
  {
    return new TimerState_Paused(a_startTime, a_totalTime);
  }

  class Timer::PIMPL
  {
  public:

    PIMPL()
      : pState(new TimerState_Off())
    {}

    ~PIMPL()
    {
      delete pState;
    }

    PIMPL(PIMPL const & a_other)
      : pState(a_other.pState->Clone())
    {}

    TimerState * pState;
  };


  Timer::Timer()
    : m_pimpl(new PIMPL()) 
  {

  }

  Timer::~Timer()
  {
    delete m_pimpl;
  }

  Timer::Timer(Timer const & a_other)
    : m_pimpl(new PIMPL(*a_other.m_pimpl))
  {

  }

  Timer & Timer::operator=(Timer const & a_other)
  {
    if (this != &a_other)
    {
      delete m_pimpl;
      m_pimpl = new PIMPL(*a_other.m_pimpl);
    }
    return *this;
  }

  bool Timer::IsStarted() const
  {
    return m_pimpl->pState->IsStarted();
  }

  bool Timer::IsStopped() const
  {
    return m_pimpl->pState->IsStopped();
  }

  bool Timer::IsPaused() const
  {
    return m_pimpl->pState->IsPaused();
  }

  void Timer::Start()
  {
    TimerState * newState = m_pimpl->pState->Start();
    delete m_pimpl->pState;
    m_pimpl->pState = newState;
  }

  void Timer::Stop()
  {
    TimerState * newState = m_pimpl->pState->Stop();
    delete m_pimpl->pState;
    m_pimpl->pState = newState;
  }

  void Timer::Pause()
  {
    TimerState * newState = m_pimpl->pState->Pause();
    delete m_pimpl->pState;
    m_pimpl->pState = newState;
  }

  void Timer::Resume()
  {
    TimerState * newState = m_pimpl->pState->Resume();
    delete m_pimpl->pState;
    m_pimpl->pState = newState;
  }

  double Timer::GetTime() const
  {
    return m_pimpl->pState->GetTime();
  }
}