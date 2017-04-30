#ifndef EVENT_H
#define EVENT_H

#include "DgObject.h"

class Event : public Dg::Object<Event>
{
public:

  Event(){}

  virtual ~Event() {}

  Event(Event const & a_other){}

  Event & operator=(Event const & a_other)
  {
    return *this;
  }

  virtual void DoEvent() {}

  Event * Clone() const { return new Event(*this); }
};

#endif