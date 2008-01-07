#include <cppunit/extensions/HelperMacros.h>
#include "PhysicsTools/Utilities/interface/expressionParser.h"
#include "PhysicsTools/Utilities/interface/StringObjectFunction.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include <iostream>
#include <Reflex/Object.h>
#include <Reflex/Type.h>
#include <typeinfo>
#include "DataFormats/Common/test/TestHandle.h"

class testExpressionParser : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(testExpressionParser);
  CPPUNIT_TEST(checkAll);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp() {}
  void tearDown() {}
  void checkAll(); 
  void checkTrack(const std::string &, double);
  reco::Track trk;
  ROOT::Reflex::Object o;
  reco::parser::ExpressionPtr expr;
};

CPPUNIT_TEST_SUITE_REGISTRATION(testExpressionParser);

void testExpressionParser::checkTrack(const std::string & expression, double x) {
  std::cerr << "parsing expression: \"" << expression << "\"" << std::endl;
  expr.reset();
  CPPUNIT_ASSERT(reco::parser::expressionParser<reco::Track>(expression, expr));
  CPPUNIT_ASSERT(expr.get() != 0);
  double res = expr->value(o);
  std::cerr << expression << " = " << res << std::endl;
  StringObjectFunction<reco::Track> f( expression );
  CPPUNIT_ASSERT(fabs(f(trk) - res) < 1.e-6);
  CPPUNIT_ASSERT(fabs(f(trk) - x) < 1.e-6);
}

void testExpressionParser::checkAll() {


  using namespace reco;
  using namespace ROOT::Reflex;
  const double chi2 = 20.0;
  const int ndof = 10;
  reco::Track::Point v(1, 2, 3);
  reco::Track::Vector p(5, 3, 10);
  double e[] = { 1.1,
                 1.2, 2.2,
                 1.3, 2.3, 3.3,
                 1.4, 2.4, 3.4, 4.4,
                 1.5, 2.5, 3.5, 4.5, 5.5 };
  reco::TrackBase::CovarianceMatrix cov(e, e + 15);
  trk = reco::Track(chi2, ndof, v, p, -1, cov);

  edm::ProductID const pid(1);
  reco::TrackExtraCollection trkExtras;
  reco::Track::Point outerV(100, 200, 300), innerV(v);
  reco::Track::Vector outerP(0.5, 3.5, 10.5), innerP(p);
  reco::Track::CovarianceMatrix  outerC, innerC;
  unsigned int outerId = 123, innerId = 456;
  reco::TrackExtra trkExtra(outerV, outerP, true, innerV, innerP, true,
			    outerC, outerId, innerC, innerId,
			    anyDirection);
  trkExtras.push_back(trkExtra);
  edm::TestHandle<reco::TrackExtraCollection> h(&trkExtras, pid);
  reco::TrackExtraRef trkExtraRef(h, 0);
  trk.setExtra(trkExtraRef);
  ROOT::Reflex::Type t = ROOT::Reflex::Type::ByTypeInfo(typeid(reco::Track));
  o = ROOT::Reflex::Object(t, & trk);

  checkTrack( "pt", trk.pt() );
  checkTrack( "charge", trk.charge() );
  checkTrack( "pt/3", trk.pt()/3 );
  checkTrack( "momentum.x", trk.momentum().x() );
  checkTrack( "hitPattern.numberOfValidHits", trk.hitPattern().numberOfValidHits() );
  checkTrack( "extra.outerPhi", trk.extra()->outerPhi() );

}
