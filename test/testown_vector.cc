// $Id: testown_vector.cc,v 1.3 2005/12/13 01:45:02 llista Exp $
#include <cppunit/extensions/HelperMacros.h>
#include "PhysicsTools/Utilities/interface/own_vector.h"

class testown_vector : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(testown_vector);
  CPPUNIT_TEST(checkAll);
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp() {}
  void tearDown() {}
  void checkAll(); 
};

CPPUNIT_TEST_SUITE_REGISTRATION(testown_vector);

namespace test {
  struct Dummy {
    Dummy( int n, bool * r ) : value( n ), ref( r ) { }
    ~Dummy() { * ref = true; }
    int value;
  private:
    bool * ref;
  };
}

void testown_vector::checkAll() {
  own_vector<test::Dummy> v;
  CPPUNIT_ASSERT( v.size() == 0 );
  CPPUNIT_ASSERT( v.empty() );
  bool deleted[ 3 ] = { false, false, false };
  v.push_back( new test::Dummy( 0, deleted     ) );
  v.push_back( new test::Dummy( 1, deleted + 1 ) );
  v.push_back( new test::Dummy( 2, deleted + 2 ) );
  CPPUNIT_ASSERT( v.size() == 3 );
  CPPUNIT_ASSERT( ! v.empty() );
  CPPUNIT_ASSERT( v[ 0 ].value == 0 );
  CPPUNIT_ASSERT( v[ 1 ].value == 1 );
  CPPUNIT_ASSERT( v[ 2 ].value == 2 );
  v.clearAndDestroy();
  CPPUNIT_ASSERT( v.size() == 0 );
  CPPUNIT_ASSERT( v.empty() );
  CPPUNIT_ASSERT( deleted[ 0 ] );
  CPPUNIT_ASSERT( deleted[ 1 ] );
  CPPUNIT_ASSERT( deleted[ 2 ] );
}
