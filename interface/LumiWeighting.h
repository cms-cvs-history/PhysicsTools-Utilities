#ifndef PhysicsTools_Utilities_interface_LumiWeighting_h
#define PhysicsTools_Utilities_interface_LumiWeighting_h


/**
  \class    LumiWeighting LumiWeighting.h "PhysicsTools/Utilities/interface/LumiWeighting.h"
  \brief    Class to provide lumi weighting for analyzers to weight "flat-to-N" MC samples to data

  This class will trivially take two histograms:
  1. The generated "flat-to-N" distributions from a given processing
  2. A histogram generated from the "estimatePileup" macro here:

  https://twiki.cern.ch/twiki/bin/view/CMS/LumiCalc#How_to_use_script_estimatePileup

  \author Salvatore Rappoccio
*/

#include "TH1.h"
#include "TFile.h"
#include <string>
#include <boost/shared_ptr.hpp>


namespace edm {
  class LumiWeighting {
  public:
    LumiWeighting( std::string generatedFile,
		   std::string dataFile,
		   std::string histName1 = "pileup",
		   std::string histName2 = "pileup" ) :
      generatedFileName_( generatedFile), 
      dataFileName_     ( dataFile ), 
      histName1_        ( histName1 ), 
      histName2_        ( histName2 )
      {
	generatedFile_ = boost::shared_ptr<TFile>( new TFile(generatedFileName_.c_str()) );
	dataFile_      = boost::shared_ptr<TFile>( new TFile(dataFileName_.c_str()) );
	weights_       = boost::shared_ptr<TH1F> ( new TH1F( *(static_cast<TH1F*>(dataFile_->Get( histName1_.c_str() )->Clone() ))));
	weights_->SetName("lumiWeights");
	TH1F const * den = static_cast<TH1F*>(generatedFile_->Get( histName2_.c_str() ));
	weights_->Divide( den );
	weights_->Scale( 1.0 / weights_->Integral() );
      }

    double weight( int npv ) const {
      int bin = weights_->GetXaxis()->FindBin( npv );
      return weights_->GetBinContent( bin );
    }
    
  protected:

    std::string generatedFileName_;
    std::string dataFileName_;
    std::string histName1_;
    std::string histName2_;
    boost::shared_ptr<TFile>     generatedFile_;
    boost::shared_ptr<TFile>     dataFile_;
    boost::shared_ptr<TH1F>      weights_;
  };
}



#endif
