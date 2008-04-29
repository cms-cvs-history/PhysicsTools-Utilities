#include <cppunit/extensions/HelperMacros.h>
#include "PhysicsTools/Utilities/interface/DeltaPhi.h"
#include "PhysicsTools/Utilities/interface/DeltaR.h"
#include <iostream>

class testUtilitiess : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(testUtilitiess);
  CPPUNIT_TEST(checkAll);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp() {}
  void tearDown() {}
  void checkAll(); 
};

CPPUNIT_TEST_SUITE_REGISTRATION( testUtilitiess );

namespace test {
  struct Phi {
    Phi(double phi) : phi_(phi) { }
    double phi() const { return phi_; }
  private:
    double phi_;
  };


  struct PhiEta {
    PhiEta(double phi, double eta) : phi_(phi), eta_(eta) { }
    double phi() const { return phi_; }
    double eta() const { return eta_; }
  private:
    double phi_, eta_;
 };
}

void testUtilitiess::checkAll() {
  using namespace test;
  Phi phi1(1);
  Phi phi2(2);
  DeltaPhi<Phi> deltaPhi;
  double d = deltaPhi(phi1, phi2);
  CPPUNIT_ASSERT(fabs(d - (phi2.phi() - phi1.phi()) < 1.e-6));

  PhiEta pe1(1, 1);
  PhiEta pe2(2, 2);
  DeltaR<PhiEta> deltaR;
  double dr = deltaR(pe1, pe2);
  double dp = reco::deltaPhi(pe1, pe2);
  double de = pe2.eta() - pe1.eta();
  CPPUNIT_ASSERT(fabs(dr*dr - (de*de + dp*dp)) < 1.e-6);

}
