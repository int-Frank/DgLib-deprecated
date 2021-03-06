#ifndef ATTRACTORFACTORY_H
#define ATTRACTORFACTORY_H

class AttractorData;

namespace Dg
{
  template<typename Real>
  class Attractor;
}

class AttractorFactory
{
public:

  Dg::Attractor<float> * operator()(AttractorData const &) const;

};

#endif