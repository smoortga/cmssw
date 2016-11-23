#ifndef BDHadronTrackMonitoringAnalyzer_H
#define BDHadronTrackMonitoringAnalyzer_H

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DQMServices/Core/interface/DQMEDAnalyzer.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "FWCore/Utilities/interface/EDGetToken.h"

#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
//#include "SimDataFormats/JetMatching/interface/JetFlavourInfoMatching.h"


/** \class BDHadronTrackMonitoringAnalyzer
 *
 *  Top level steering routine for B + D hadron track monitoring tool from RECODEBUG samples.
 *
 */

class BDHadronTrackMonitoringAnalyzer : public DQMEDAnalyzer {
   public:
      explicit BDHadronTrackMonitoringAnalyzer(const edm::ParameterSet& pSet);

      ~BDHadronTrackMonitoringAnalyzer();

      virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) override;
      virtual void bookHistograms(DQMStore::IBooker &, edm::Run const &, edm::EventSetup const &) override;     


   private:
	// InputTags
  	edm::InputTag JetSrc_;
	
	// Tokens
    edm::EDGetTokenT<reco::PFJetCollection> JetCollectionTag_;
  
    // Histograms
    MonitorElement *nJets;


};


#endif
