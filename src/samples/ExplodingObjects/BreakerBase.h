#ifndef BREAKERBASE_H
#define BREAKERBASE_H

#include <list>

#include "Mesh.h"

class BreakerBase
{
public:

  virtual ~BreakerBase() {}

  virtual std::list<Mesh> operator()(Mesh const &) {}
};

#endif