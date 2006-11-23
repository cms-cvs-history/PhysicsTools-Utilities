#ifndef Utilities_DeltaR_h
#define Utilities_DeltaR_h
#include "DataFormats/Math/interface/LorentzVector.h"
#include "FWCore/ParameterSet/interface/ParameterSetfwd.h"
#include <Math/VectorUtil.h>

template<typename T>
struct DeltaR {
  DeltaR() { }
  explicit DeltaR( const edm::ParameterSet & ) { }
  double operator()( const T & t1, const T & t2 ) const {
    return ROOT::Math::VectorUtil::DeltaR( t1.p4(), t2.p4() );
  }
};

#endif
