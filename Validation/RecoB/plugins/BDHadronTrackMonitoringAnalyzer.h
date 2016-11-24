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

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/BTauReco/interface/TrackIPTagInfo.h"





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
   
    // strings
    std::string ipTagInfos_;
   
	// InputTags
  	edm::InputTag PatJetSrc_;
  	edm::InputTag TrackSrc_;
	
	// Tokens
    edm::EDGetTokenT<pat::JetCollection> PatJetCollectionTag_;
    edm::EDGetTokenT<reco::TrackCollection> TrackCollectionTag_;
  
    // Histograms
    // b jets
    MonitorElement *nSelectedTracks_bJet; // total number of selected tracks



};


#endif
