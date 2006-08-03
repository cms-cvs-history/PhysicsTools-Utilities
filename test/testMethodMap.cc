#include <cppunit/extensions/HelperMacros.h>
#include "PhysicsTools/Utilities/interface/MethodMap.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include <iostream>

class testMethodMap : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(testMethodMap);
  CPPUNIT_TEST(checkAll);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp() {}
  void tearDown() {}
  void checkAll(); 
};

CPPUNIT_TEST_SUITE_REGISTRATION( testMethodMap );

void testMethodMap::checkAll() {
  using namespace reco;
  using namespace ROOT::Reflex;
  const double chi2 = 20.0;
  const int ndof = 10;
  double p[] = { 1.0, 2.0, 3.0, 4.0, 5.0 };
  double e[] = { 1.1, 1.2, 1.3, 1.4, 1.5,
                      2.2, 2.3, 2.4, 2.5,
                           3.3, 3.4, 3.5,
		                4.4, 4.5,
		                     5.5 };
  reco::TrackBase::ParameterVector par( p[0], p[1], p[2], p[3], p[4] );
  reco::TrackBase::CovarianceMatrix cov( e, e + 15 );
  double pt = 3.14;
  reco::Track trk( chi2, ndof, par, pt, cov );

  const MethodMap & map = MethodMap::methods<Track>();
  std::cerr << map;
  const Type & type = map.type();
  MethodMap::const_iterator i;
  i = map.find( "pt" );
  CPPUNIT_ASSERT( i != map.end() );
  CPPUNIT_ASSERT( * static_cast<double *>( i->second.first.Invoke( Object( type, & trk ) ).Address() ) == pt );
  i = map.find( "charge" );
  CPPUNIT_ASSERT( i != map.end() );
  CPPUNIT_ASSERT( * static_cast<int *>( i->second.first.Invoke( Object( type, & trk ) ).Address() ) == - 1 );
}
