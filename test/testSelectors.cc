#include <cppunit/extensions/HelperMacros.h>
#include "PhysicsTools/Utilities/interface/MinSelector.h"
#include "PhysicsTools/Utilities/interface/MaxSelector.h"
#include "PhysicsTools/Utilities/interface/RangeSelector.h"
#include "PhysicsTools/Utilities/interface/MinObjectPairSelector.h"
#include "PhysicsTools/Utilities/interface/MaxObjectPairSelector.h"
#include "PhysicsTools/Utilities/interface/RangeObjectPairSelector.h"
#include "PhysicsTools/Utilities/interface/PtMinSelector.h"
#include "PhysicsTools/Utilities/interface/EtMinSelector.h"
#include <iostream>

class testSelectors : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(testSelectors);
  CPPUNIT_TEST(checkAll);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp() {}
  void tearDown() {}
  void checkAll(); 
};

CPPUNIT_TEST_SUITE_REGISTRATION( testSelectors );

namespace test {
  struct A {
    explicit A( double x ) : x_( x ) { }
    double x() const { return x_; } 
    double pt() const { return x_; }
    double et() const { return x_; }
  private:
    double x_;
  };

  struct Add {
    double operator()( const A & a1, const A& a2 ) const {
      return a1.x() + a2.x();
    }
  };
}

void testSelectors::checkAll() {
  using namespace test;
  {
    A a( 1.0 );
    MinSelector<A, & A::x> minSel( 0.9 );
    MaxSelector<A, & A::x> maxSel( 1.1 );
    RangeSelector<A, & A::x> rangeSel( 0.9, 1.1 );
    PtMinSelector<A> ptMinSel( 0.9 );
    EtMinSelector<A> etMinSel( 0.9 );
    
    CPPUNIT_ASSERT( minSel( a ) );
    CPPUNIT_ASSERT( maxSel( a ) );
    CPPUNIT_ASSERT( rangeSel( a ) );
    CPPUNIT_ASSERT( ptMinSel( a ) );
    CPPUNIT_ASSERT( etMinSel( a ) );
  }
  {
    A a1( 3.0 ), a2( 5.0 );
    MinObjectPairSelector<A, Add> minSel( 7.9 );
    MaxObjectPairSelector<A, Add> maxSel( 8.1 );
    RangeObjectPairSelector<A, Add> rangeSel( 7.9, 8.1 );
    CPPUNIT_ASSERT( minSel( a1, a2 ) );
    CPPUNIT_ASSERT( maxSel( a1, a2 ) );
    CPPUNIT_ASSERT( rangeSel( a1, a2 ) );
  }
}
