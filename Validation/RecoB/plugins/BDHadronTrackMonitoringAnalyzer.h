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

//typedef edm::View<reco::Jet> JetCollection;

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


  // Get histogram plotting options from configuration.
  //virtual void bookHistograms(DQMStore::IBooker &, edm::Run const &, edm::EventSetup const &) override;     


  	edm::InputTag JetSrc_;
  //edm::InputTag PatJetSrc_;
  //edm::InputTag TrackSrc;
  //edm::InputTag TrackingParticleSrc;
  
  

  
    //add consumes 
    //edm::EDGetTokenT<PatJetCollection> JetCollectionTag_;
    edm::EDGetTokenT<reco::PFJetCollection> JetCollectionTag_;
  
  
  
    // The Histograms
    MonitorElement *nJets;
    //MonitorElement *Track_pt_b_BWeak

};


#endif
