#ifndef BREAKERBASE_H
#define BREAKERBASE_H

#include <vector>

#include "Model.h"

class BreakerBase
{
public:

  virtual ~BreakerBase() {}

  virtual std::vector<Model> operator() {}
};

#endif