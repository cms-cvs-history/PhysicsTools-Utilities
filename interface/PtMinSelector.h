#ifndef RecoAlgos_PtMinSelector_h
#define RecoAlgos_PtMinSelector_h
/* \class PtMinSelector
 *
 * \author Luca Lista, INFN
 *
 * $Id: PtMinSelector.h,v 1.2 2006/07/25 17:21:31 llista Exp $
 */
#include "FWCore/ParameterSet/interface/ParameterSet.h"

template<typename T>
struct PtMinSelector {
  typedef T value_type;
  PtMinSelector( double ptMin ) : 
    ptMin_( ptMin ) { }
  PtMinSelector( const edm::ParameterSet & cfg ) : 
    ptMin_( cfg.template getParameter<double>( "ptMin" ) ) { }
  bool operator()( const value_type & t ) const { return t.pt() > ptMin_; }
private:
  double ptMin_;
};

#endif
