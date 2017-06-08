//! @file DgTimer.h
//!
//! @author Frank Hart
//! @date
//!
//! Class declaration: DgTimer

#ifndef DGTIMER_H
#define DGTIMER_H

#include <stdint.h>

namespace Dg
{
  //! @ingroup DgUtility_types
  //!
  //! @class Timer
  //!
  //! @brief Timer
  //!
  //! @author Frank Hart
  //! @date
  class Timer
  {
  public:

    Timer();
    ~Timer();

    Timer(Timer const &);
    Timer & operator=(Timer const &);

    bool IsStarted() const;
    bool IsStopped() const;
    bool IsPaused() const;

    void Start();
    void Stop();
    void Pause();
    void Resume();

    //In seconds...
    double GetTime() const;

  private:

    class PIMPL;
    PIMPL * m_pimpl;
  };
}

#endif