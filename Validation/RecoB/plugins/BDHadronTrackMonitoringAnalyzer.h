#ifndef BDHadronTrackMonitoringAnalyzer_H
#define BDHadronTrackMonitoringAnalyzer_H

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Utilities/interface/EDGetToken.h"

#include "DQMServices/Core/interface/DQMEDAnalyzer.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/BTauReco/interface/TrackIPTagInfo.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"

#include "SimTracker/TrackHistory/interface/TrackCategories.h"
#include "SimTracker/TrackHistory/interface/TrackClassifier.h"
#include "SimTracker/TrackerHitAssociation/interface/ClusterTPAssociation.h"





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
  	edm::InputTag PVSrc_;
  	edm::InputTag ClusterTPMapSrc_;
	
	// Tokens
    edm::EDGetTokenT<pat::JetCollection> PatJetCollectionTag_;
    edm::EDGetTokenT<reco::TrackCollection> TrackCollectionTag_;
    edm::EDGetTokenT<reco::VertexCollection> PrimaryVertexColl_;
    edm::EDGetTokenT<ClusterTPAssociation> clusterTPMapToken_;
    
    // TrackClassifier
    TrackClassifier classifier_;
    
  
    // Histograms
    // b jets
    MonitorElement *nTrkAll_bjet, *nTrkTruthAll_bjet; // total number of selected tracks (or TrackingParticles)
    MonitorElement *nTrk_bjet[6], *nTrkTruth_bjet[6]; // total number of selected tracks (or TrackingParticles) in each TrackHistory category
	MonitorElement *TrkPt_bjet[6], *TrkTruthPt_bjet[6]; // Pt of selected tracks (or TrackingParticles)
	MonitorElement *TrkEta_bjet[6], *TrkTruthEta_bjet[6]; // Eta of selected tracks (or TrackingParticles)
	MonitorElement *TrkPhi_bjet[6], *TrkTruthPhi_bjet[6]; // Phi of selected tracks (or TrackingParticles)
	MonitorElement *TrkDxy_bjet[6], *TrkTruthDxy_bjet[6]; // Transverse IP of selected tracks (or TrackingParticles)
	MonitorElement *TrkDz_bjet[6], *TrkTruthDz_bjet[6]; // Longitudinal IP of selected tracks (or TrackingParticles)
	MonitorElement *TrkHitAll_bjet[6], *TrkTruthHitAll_bjet[6]; // total number Tracker hits of selected tracks (or TrackingParticles)
	MonitorElement *TrkHitStrip_bjet[6], *TrkTruthHitStrip_bjet[6]; // number of strip hits of of selected tracks (or TrackingParticles)
	MonitorElement *TrkHitPixel_bjet[6], *TrkTruthHitPixel_bjet[6]; // number of pixel hits of selected tracks (or TrackingParticles)


	// c jets
  	MonitorElement *nTrkAll_cjet, *nTrkTruthAll_cjet; // total number of selected tracks (or TrackingParticles)
    MonitorElement *nTrk_cjet[6], *nTrkTruth_cjet[6]; // total number of selected tracks (or TrackingParticles) in each TrackHistory category
	MonitorElement *TrkPt_cjet[6], *TrkTruthPt_cjet[6]; // Pt of selected tracks (or TrackingParticles)
	MonitorElement *TrkEta_cjet[6], *TrkTruthEta_cjet[6]; // Eta of selected tracks (or TrackingParticles)
	MonitorElement *TrkPhi_cjet[6], *TrkTruthPhi_cjet[6]; // Phi of selected tracks (or TrackingParticles)
	MonitorElement *TrkDxy_cjet[6], *TrkTruthDxy_cjet[6]; // Transverse IP of selected tracks (or TrackingParticles)
	MonitorElement *TrkDz_cjet[6], *TrkTruthDz_cjet[6]; // Longitudinal IP of selected tracks (or TrackingParticles)
	MonitorElement *TrkHitAll_cjet[6], *TrkTruthHitAll_cjet[6]; // total number Tracker hits of selected tracks (or TrackingParticles)
	MonitorElement *TrkHitStrip_cjet[6], *TrkTruthHitStrip_cjet[6]; // number of strip hits of of selected tracks (or TrackingParticles)
	MonitorElement *TrkHitPixel_cjet[6], *TrkTruthHitPixel_cjet[6]; // number of pixel hits of selected tracks (or TrackingParticles)


	// dusg jets
	MonitorElement *nTrkAll_dusgjet, *nTrkTruthAll_dusgjet; // total number of selected tracks (or TrackingParticles)
    MonitorElement *nTrk_dusgjet[6], *nTrkTruth_dusgjet[6]; // total number of selected tracks (or TrackingParticles) in each TrackHistory category
	MonitorElement *TrkPt_dusgjet[6], *TrkTruthPt_dusgjet[6]; // Pt of selected tracks (or TrackingParticles)
	MonitorElement *TrkEta_dusgjet[6], *TrkTruthEta_dusgjet[6]; // Eta of selected tracks (or TrackingParticles)
	MonitorElement *TrkPhi_dusgjet[6], *TrkTruthPhi_dusgjet[6]; // Phi of selected tracks (or TrackingParticles)
	MonitorElement *TrkDxy_dusgjet[6], *TrkTruthDxy_dusgjet[6]; // Transverse IP of selected tracks (or TrackingParticles)
	MonitorElement *TrkDz_dusgjet[6], *TrkTruthDz_dusgjet[6]; // Longitudinal IP of selected tracks (or TrackingParticles)
	MonitorElement *TrkHitAll_dusgjet[6], *TrkTruthHitAll_dusgjet[6]; // total number Tracker hits of selected tracks (or TrackingParticles)
	MonitorElement *TrkHitStrip_dusgjet[6], *TrkTruthHitStrip_dusgjet[6]; // number of strip hits of of selected tracks (or TrackingParticles)
	MonitorElement *TrkHitPixel_dusgjet[6], *TrkTruthHitPixel_dusgjet[6]; // number of pixel hits of selected tracks (or TrackingParticles)


};


#endif
