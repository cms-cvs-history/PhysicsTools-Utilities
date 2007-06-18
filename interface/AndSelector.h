#ifndef Utilities_AndSelector_h
#define Utilities_AndSelector_h
/* \class AndSelector
 *
 * \author Luca Lista, INFN
 *
 * $Id: AndSelector.h,v 1.4 2007/05/15 16:07:53 llista Exp $
 */

namespace helpers {
  struct NullAndOperand { };
}

template<typename S1, typename S2, 
	 typename S3 = helpers::NullAndOperand, typename S4 = helpers::NullAndOperand,
	 typename S5 = helpers::NullAndOperand>
struct AndSelector {
  AndSelector( const S1 & s1, const S2 & s2, const S3 & s3, const S4 & s4, const S5 & s5 ) :
    s1_( s1 ), s2_( s2 ), s3_( s3 ), s4_( s4 ), s5_( s5 ) { }
  template<typename T>
  bool operator()( const T & t ) const { 
    return s1_( t ) && s2_( t ) && s3_( t ) && s4_( t ) && s5_( t );
  }

private:
  S1 s1_;
  S2 s2_;
  S3 s3_;
  S4 s4_;
  S5 s5_;
};


template<typename S1, typename S2>
struct AndSelector<S1, S2, helpers::NullAndOperand, helpers::NullAndOperand, helpers::NullAndOperand> {
  AndSelector( const S1 & s1, const S2 & s2 ) :
    s1_( s1 ), s2_( s2 ) { }
  template<typename T>
  bool operator()( const T & t ) const { 
    return s1_( t ) && s2_( t );
  }
private:
  S1 s1_;
  S2 s2_;
};

template<typename S1, typename S2, typename S3>
struct AndSelector<S1, S2, S3, helpers::NullAndOperand, helpers::NullAndOperand> {
  AndSelector( const S1 & s1, const S2 & s2, const S3 & s3 ) :
    s1_( s1 ), s2_( s2 ), s3_( s3 ) { }
  template<typename T>
  bool operator()( const T & t ) const { 
    return s1_( t ) && s2_( t ) && s3_( t );
  }
private:
  S1 s1_;
  S2 s2_;
  S3 s3_;
};

template<typename S1, typename S2, typename S3, typename S4>
struct AndSelector<S1, S2, S3, S4, helpers::NullAndOperand> {
  AndSelector( const S1 & s1, const S2 & s2, const S3 & s3, const S4 & s4 ) :
    s1_( s1 ), s2_( s2 ), s3_( s3 ), s4_( s4 ) { }
  template<typename T>
  bool operator()( const T & t ) const { 
    return s1_( t ) && s2_( t ) && s3_( t ) && s4_( t );
  }
private:
  S1 s1_;
  S2 s2_;
  S3 s3_;
  S4 s4_;
};

#endif
