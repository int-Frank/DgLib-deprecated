#ifndef EVENT_H
#define EVENT_H

#include "DgObject.h"
#include "DgApp.h"

class Event
{
public:

  Event(){}

  virtual ~Event() {}

  Event(Event const & a_other){}

  Event & operator=(Event const & a_other)
  {
    return *this;
  }

  virtual void DoEvent(DgApp * ) {}

  virtual Event * Clone() const { return new Event(*this); }
};

#endif