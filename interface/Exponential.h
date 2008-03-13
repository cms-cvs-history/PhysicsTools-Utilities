#ifndef PhysicsTools_Utilities_Exponential_h
#define PhysicsTools_Utilities_Exponential_h
#include <boost/shared_ptr.hpp>
#include <cmath>

namespace function {

  struct Exponential {
    static const unsigned int arguments = 1;
    Exponential(boost::shared_ptr<double> l) : lambda(l) { }
    Exponential(double l) : lambda(new double(l)) { }
    double operator()(double x) const { return exp((*lambda)*x); }
    boost::shared_ptr<double> lambda;
  };

}

#endif
