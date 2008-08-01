#include <cppunit/extensions/HelperMacros.h>
#include "PhysicsTools/Utilities/interface/cutParser.h"
#include "PhysicsTools/Utilities/interface/StringCutObjectSelector.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include <iostream>
#include <Reflex/Object.h>
#include <Reflex/Type.h>
#include <typeinfo>

class testCutParser : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(testCutParser);
  CPPUNIT_TEST(checkAll);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp() {}
  void tearDown() {}
  void checkAll(); 
  void check(const std::string &, bool);
  reco::Track trk;
  ROOT::Reflex::Object o;
  reco::parser::SelectorPtr sel;
};

CPPUNIT_TEST_SUITE_REGISTRATION(testCutParser);

void testCutParser::check(const std::string & cut, bool res) {
  std::cerr << "parsing cut: \"" << cut << "\"" << std::endl;
  sel.reset();
  CPPUNIT_ASSERT(reco::parser::cutParser<reco::Track>(cut, sel));
  CPPUNIT_ASSERT((*sel)(o) == res);
  StringCutObjectSelector<reco::Track> select(cut);
  CPPUNIT_ASSERT(select(trk) == res);
}

void testCutParser::checkAll() {
  using namespace reco;
  using namespace ROOT::Reflex;
  const double chi2 = 20.0;
  const int ndof = 10;
  reco::Track::Point v(1, 2, 3);
  reco::Track::Vector p(0, 3, 10);
  double e[] = { 1.1,
                 1.2, 2.2,
                 1.3, 2.3, 3.3,
                 1.4, 2.4, 3.4, 4.4,
                 1.5, 2.5, 3.5, 4.5, 5.5 };
  reco::TrackBase::CovarianceMatrix cov(e, e + 15);
  trk = reco::Track(chi2, ndof, v, p, -1, cov);

  ROOT::Reflex::Type t = ROOT::Reflex::Type::ByTypeInfo(typeid(reco::Track));
  o = ROOT::Reflex::Object(t, & trk);

  std::cerr << "Track pt: " << trk.pt() << std::endl;
  check( "", true );
  check( "  ", true );
  check("pt", true);
  check("px", false);
  check( "pt > 2", true );
  check( "charge < 0", true );
  check( "pt < 2", false );
  check( "pt >= 2", true );
  check( "pt <= 2", false );
  check( "pt = 3", true );
  check( "pt == 3", true );
  check( "pt != 3", false );
  check( "2.9 < pt < 3.1", true );
  check( "pt > 2 & charge < 0", true );
  check( "pt > 2 && charge < 0", true );
  check( "pt < 2 & charge < 0", false );
  check( "pt > 2 & charge > 0", false );
  check( "pt < 2 & charge > 0", false );
  check( "pt > 2 || charge > 0", true );
  check( "pt > 2 | charge > 0", true );
  check( "pt > 2 | charge < 0", true );
  check( "pt < 2 | charge < 0", true );
  check( "pt < 2 | charge > 0", false );
  check( "(pt) > 2", true );
  check( "-pt < -2", true );
  check( "3.9 < pt + 1 < 4.1", true );
  check( "1.9 < pt - 1 < 2.1", true );
  check( "5.9 < 2 * pt < 6.1", true );
  check( "0.9 < pt / 3 < 1.1", true );
  check( "8.9 < pt ^ 2 < 9.1", true );
  check( "26.9 < 3 * pt ^ 2 < 27.1", true );
  check( "27.9 < 3 * pt ^ 2 + 1 < 28.1", true );
  check( " 0.99 < sin( phi ) < 1.01", true );
  check( " -0.01 < cos( phi ) < 0.01", true );
  check( " 8.9 < pow( pt, 2 ) < 9.1", true );
  check( "( 0.99 < sin( phi ) < 1.01 ) & ( -0.01 < cos( phi ) < 0.01 )", true );
  check( "( 3.9 < pt + 1 < 4.1 ) | ( pt < 2 )", true );
  check( " pt = 3 &  pt > 2 | pt < 2", true );
  check( "( ( pt = 3 &  pt > 2 ) | pt < 2 ) & 26.9 < 3 * pt ^ 2 < 27.1", true );
  check( "! pt > 2", false );
  check( "! pt < 2", true );
  check( "! (( 0.99 < sin( phi ) < 1.01 ) & ( -0.01 < cos( phi ) < 0.01 ))", false );
  // check trailing space
  check( "pt > 2 ", true );
  // check handling of errors 
  sel.reset();
  CPPUNIT_ASSERT(!reco::parser::cutParser<reco::Track>("1abc",sel));
  CPPUNIT_ASSERT_THROW(reco::parser::cutParser<reco::Track>("doesNotExist < 1",sel), edm::Exception);
}
