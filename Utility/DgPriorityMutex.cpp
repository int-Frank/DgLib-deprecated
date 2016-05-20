//! @file priority_mutex.cpp
//!
//! @author Frank Hart
//! @date 4/10/2015
//!
//! Class definitions: priority_mutex

#include "DgPriorityMutex.h"

namespace Dg
{
  //--------------------------------------------------------------------------------
  //	@	priority_mutex::high_lock()
  //--------------------------------------------------------------------------------
  void priority_mutex::high_lock()
  {
    N.lock();
    M.lock();
    N.unlock();
  } //End: priority_mutex::high_lock()

  //--------------------------------------------------------------------------------
  //	@	priority_mutex::high_try_lock()
  //--------------------------------------------------------------------------------
  bool priority_mutex::high_try_lock()
  {
    if (!N.try_lock())
    {
      return false;
    }
    if (!M.try_lock())
    {
      N.unlock();
      return false;
    }
    N.unlock();
    return true;
  } //End: priority_mutex::high_try_lock()

  //--------------------------------------------------------------------------------
  //	@	priority_mutex::high_unlock()
  //--------------------------------------------------------------------------------
  void priority_mutex::high_unlock()
  {
    M.unlock();
  } //End: priority_mutex::high_unlock()

  //--------------------------------------------------------------------------------
  //	@	priority_mutex::low_lock()
  //--------------------------------------------------------------------------------
  void priority_mutex::low_lock()
  {
    L.lock();
    N.lock();
    M.lock();
    N.unlock();
  } //End: priority_mutex::low_lock()

  //--------------------------------------------------------------------------------
  //	@	priority_mutex::low_try_lock()
  //--------------------------------------------------------------------------------
  bool priority_mutex::low_try_lock()
  {
    if (!L.try_lock())
    {
      return false;
    }
    if (!N.try_lock())
    {
      L.unlock();
      return false;
    }
    if (!M.try_lock())
    {
      N.unlock();
      L.unlock();
      return false;
    }
    N.unlock();
    return true;
  } //End: priority_mutex::low_try_lock()

  //--------------------------------------------------------------------------------
  //	@	priority_mutex::low_unlock()
  //--------------------------------------------------------------------------------
  void priority_mutex::low_unlock()
  {
    M.unlock();
    L.unlock();
  } //End: priority_mutex::low_unlock()
}