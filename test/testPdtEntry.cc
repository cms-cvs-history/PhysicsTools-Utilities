#include <cppunit/extensions/HelperMacros.h>
#include "PhysicsTools/Utilities/interface/PdtEntry.h"
#include "FWCore/Utilities/interface/EDMException.h" 
#include <iostream>

class testPdtEntry : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(testPdtEntry);
  CPPUNIT_TEST(checkAll);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp() {}
  void tearDown() {}
  void checkAll(); 
};

CPPUNIT_TEST_SUITE_REGISTRATION( testPdtEntry );

void testPdtEntry::checkAll() {
  PdtEntry e( 13 );
  CPPUNIT_ASSERT_THROW( e.name(), cms::Exception ); 
  PdtEntry n( "mu-" );
  CPPUNIT_ASSERT_THROW( n.pdgId(), cms::Exception ); 
}
