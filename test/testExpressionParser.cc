#include <cppunit/extensions/HelperMacros.h>
#include "PhysicsTools/Utilities/interface/expressionParser.h"
#include "PhysicsTools/Utilities/interface/StringObjectFunction.h"
#include "DataFormats/TrackReco/interface/Track.h"
//#include <iostream>
#include <Reflex/Object.h>
#include <Reflex/Type.h>
#include <typeinfo>

class testExpressionParser : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(testExpressionParser);
  CPPUNIT_TEST(checkAll);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp() {}
  void tearDown() {}
  void checkAll(); 
  void check(const std::string &);
  reco::Track trk;
  ROOT::Reflex::Object o;
  reco::parser::ExpressionPtr expr;
};

CPPUNIT_TEST_SUITE_REGISTRATION(testExpressionParser);

void testExpressionParser::check(const std::string & expression) {
  //  std::cerr << "parsing expression: \"" << expression << "\"" << std::endl;
  expr.reset();
  CPPUNIT_ASSERT(reco::parser::expressionParser<reco::Track>(expression, expr));
  CPPUNIT_ASSERT(expr.get() != 0);
  double res = expr->value(o);
  //  std::cerr << expression << " = " << res << std::endl;
  StringObjectFunction<reco::Track> f( expression );
  CPPUNIT_ASSERT(fabs(f(trk) - res) < 1.e-6);
}

void testExpressionParser::checkAll() {
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

  check( "pt" );
  check( "charge" );
  check( "pt/3" );
}
