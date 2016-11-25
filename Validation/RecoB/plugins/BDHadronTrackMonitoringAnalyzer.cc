#include "Validation/RecoB/plugins/BDHadronTrackMonitoringAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h" 

#include "DQMOffline/RecoB/interface/Tools.h"



using namespace reco;
using namespace edm;
using namespace std;


std::map<unsigned int, std::string> TrkHistCat{
	{0, "BCWeakDecay"},
	{1, "BWeakDecay"},
	{2, "CWeakDecay"},
	{3, "Fake"},
	{4, "PU"},
	{5, "Other"}	
};

const reco::TrackBaseRef toTrackRef(const edm::Ptr<reco::Candidate> & cnd)
{
  	const reco::PFCandidate * pfcand = dynamic_cast<const reco::PFCandidate *>(cnd.get());

    if ( (std::abs(pfcand->pdgId()) == 11 || pfcand->pdgId() == 22) && pfcand->gsfTrackRef().isNonnull() && pfcand->gsfTrackRef().isAvailable() )
      return reco::TrackBaseRef(pfcand->gsfTrackRef());
    else if ( pfcand->trackRef().isNonnull() && pfcand->trackRef().isAvailable() )
      return reco::TrackBaseRef(pfcand->trackRef());
    else
      return reco::TrackBaseRef();
}


// ---------- Constructor -----------

BDHadronTrackMonitoringAnalyzer::BDHadronTrackMonitoringAnalyzer(const edm::ParameterSet& pSet) :
	ipTagInfos_ ( pSet.getParameter<std::string>("ipTagInfos") ),
	PatJetSrc_ ( pSet.getParameter<InputTag>("PatJetSource") ),
	TrackSrc_ ( pSet.getParameter<InputTag>("TrackSource") ),
	PVSrc_ ( pSet.getParameter<InputTag>("PrimaryVertexSource") ),
	ClusterTPMapSrc_ ( pSet.getParameter<InputTag>("clusterTPMap") ),
	classifier_(pSet, consumesCollector())
	
{
	PatJetCollectionTag_ = consumes<pat::JetCollection>(PatJetSrc_);
	TrackCollectionTag_ = consumes<reco::TrackCollection>(TrackSrc_);
	PrimaryVertexColl_ = consumes<reco::VertexCollection>(PVSrc_);
	clusterTPMapToken_ = consumes<ClusterTPAssociation>(ClusterTPMapSrc_);
}



// ---------- BookHistograms -----------

void BDHadronTrackMonitoringAnalyzer::bookHistograms(DQMStore::IBooker & ibook, edm::Run const & run, edm::EventSetup const & es)
{
  //
  // Book all histograms.
  //
  RecoBTag::setTDRStyle();
  
  
  nTrkAll_bjet = ibook.book1D("nTrkAll_bJet","Number of selected tracks in b jets;number of selected tracks;jets",16,-0.5,15.5);
  nTrkTruthAll_bjet = ibook.book1D("nTrkTruthAll_bJet","Number of selected TrackingParticles in b jets;number of selected truth tracks;jets",16,-0.5,15.5);
  
  nTrkAll_cjet = ibook.book1D("nTrkAll_cjet","Number of selected tracks in c jets;number of selected tracks;jets",16,-0.5,15.5);
  nTrkTruthAll_cjet = ibook.book1D("nTrkTruthAll_cjet","Number of selected TrackingParticles in c jets;number of selected truth tracks;jets",16,-0.5,15.5);

  nTrkAll_dusgjet = ibook.book1D("nTrkAll_dusgjet","Number of selected tracks in dusg jets;number of selected tracks;jets",16,-0.5,15.5);
  nTrkTruthAll_dusgjet = ibook.book1D("nTrkTruthAll_dusgjet","Number of selected TrackingParticles in dusg jets;number of selected truth tracks;jets",16,-0.5,15.5);

  // Loop over different Track History Categories
  for (unsigned int i = 0; i < TrkHistCat.size(); i++){
  	// b jets
  	nTrk_bjet[i] = ibook.book1D("nTrk_bjet_"+TrkHistCat[i],"Number of selected tracks in b jets ("+TrkHistCat[i]+");number of selected tracks ("+TrkHistCat[i]+");jets",16,-0.5,15.5);
  	TrkPt_bjet[i] = ibook.book1D("TrkPt_bjet_"+TrkHistCat[i],"Track pT in b jets ("+TrkHistCat[i]+");track p_{T} ("+TrkHistCat[i]+");tracks",30,0,150);
  	TrkEta_bjet[i] = ibook.book1D("TrkEta_bjet_"+TrkHistCat[i],"Track #eta in b jets ("+TrkHistCat[i]+");track #eta ("+TrkHistCat[i]+");tracks",30,-2.5,2.5);
  	TrkPhi_bjet[i] = ibook.book1D("TrkPhi_bjet_"+TrkHistCat[i],"Track #phi in b jets ("+TrkHistCat[i]+");track #phi ("+TrkHistCat[i]+");tracks",30,-3.15,3.15);
  	TrkDxy_bjet[i] = ibook.book1D("TrkDxy_bjet_"+TrkHistCat[i],"Track dxy in b jets ("+TrkHistCat[i]+");track dxy ("+TrkHistCat[i]+");tracks",30,-0.1,0.1);
  	TrkDz_bjet[i] = ibook.book1D("TrkDz_bjet_"+TrkHistCat[i],"Track dz in b jets ("+TrkHistCat[i]+");track dz ("+TrkHistCat[i]+");tracks",30,-0.1,0.1);
  	TrkHitAll_bjet[i] = ibook.book1D("TrkHitAll_bjet_"+TrkHistCat[i],"Number of tracker hits in b jets ("+TrkHistCat[i]+");track number of all hits ("+TrkHistCat[i]+");tracks",31,-0.5,30.5);
  	TrkHitStrip_bjet[i] = ibook.book1D("TrkHitStrip_bjet_"+TrkHistCat[i],"Number of strip hits in b jets ("+TrkHistCat[i]+");track number of strip hits ("+TrkHistCat[i]+");tracks",31,-0.5,30.5);
  	TrkHitPixel_bjet[i] = ibook.book1D("TrkHitPixel_bjet_"+TrkHistCat[i],"Number of pixel hits in b jets ("+TrkHistCat[i]+");track number of pixel hits ("+TrkHistCat[i]+");tracks",9,-0.5,8.5);
  	nTrkTruth_bjet[i] = ibook.book1D("nTrkTruth_bjet_"+TrkHistCat[i],"Number of selected tracks in b jets ("+TrkHistCat[i]+" Truth);number of selected tracks ("+TrkHistCat[i]+" Truth);jets",16,-0.5,15.5);
  	TrkTruthPt_bjet[i] = ibook.book1D("TrkTruthPt_bjet_"+TrkHistCat[i],"Track pT in b jets ("+TrkHistCat[i]+" Truth);track p_{T} ("+TrkHistCat[i]+" Truth);tracks",30,0,150);
  	TrkTruthEta_bjet[i] = ibook.book1D("TrkTruthEta_bjet_"+TrkHistCat[i],"Track #eta in b jets ("+TrkHistCat[i]+" Truth);track #eta ("+TrkHistCat[i]+" Truth);tracks",30,-2.5,2.5);
  	TrkTruthPhi_bjet[i] = ibook.book1D("TrkTruthPhi_bjet_"+TrkHistCat[i],"Track #phi in b jets ("+TrkHistCat[i]+" Truth);track #phi ("+TrkHistCat[i]+" Truth);tracks",30,-3.15,3.15);
  	TrkTruthDxy_bjet[i] = ibook.book1D("TrkTruthDxy_bjet_"+TrkHistCat[i],"Track dxy in b jets ("+TrkHistCat[i]+" Truth);track dxy ("+TrkHistCat[i]+" Truth);tracks",30,-0.1,0.1);
  	TrkTruthDz_bjet[i] = ibook.book1D("TrkTruthDz_bjet_"+TrkHistCat[i],"Track dz in b jets ("+TrkHistCat[i]+" Truth);track dz ("+TrkHistCat[i]+" Truth);tracks",30,-0.1,0.1);
  	TrkTruthHitAll_bjet[i] = ibook.book1D("TrkTruthHitAll_bjet_"+TrkHistCat[i],"Number of tracker hits in b jets ("+TrkHistCat[i]+" Truth);track number of all hits ("+TrkHistCat[i]+" Truth);tracks",31,-0.5,30.5);
  	TrkTruthHitStrip_bjet[i] = ibook.book1D("TrkTruthHitStrip_bjet_"+TrkHistCat[i],"Number of strip hits in b jets ("+TrkHistCat[i]+" Truth);track number of strip hits ("+TrkHistCat[i]+" Truth);tracks",31,-0.5,30.5);
  	TrkTruthHitPixel_bjet[i] = ibook.book1D("TrkTruthHitPixel_bjet_"+TrkHistCat[i],"Number of pixel hits in b jets ("+TrkHistCat[i]+" Truth);track number of pixel hits ("+TrkHistCat[i]+" Truth);tracks",9,-0.5,8.5);
  	
  	// c jets
  	nTrk_cjet[i] = ibook.book1D("nTrk_cjet_"+TrkHistCat[i],"Number of selected tracks in c jets ("+TrkHistCat[i]+");number of selected tracks ("+TrkHistCat[i]+");jets",16,-0.5,15.5);
  	TrkPt_cjet[i] = ibook.book1D("TrkPt_cjet_"+TrkHistCat[i],"Track pT in c jets ("+TrkHistCat[i]+");track p_{T} ("+TrkHistCat[i]+");tracks",30,0,150);
  	TrkEta_cjet[i] = ibook.book1D("TrkEta_cjet_"+TrkHistCat[i],"Track #eta in c jets ("+TrkHistCat[i]+");track #eta ("+TrkHistCat[i]+");tracks",30,-2.5,2.5);
  	TrkPhi_cjet[i] = ibook.book1D("TrkPhi_cjet_"+TrkHistCat[i],"Track #phi in c jets ("+TrkHistCat[i]+");track #phi ("+TrkHistCat[i]+");tracks",30,-3.15,3.15);
  	TrkDxy_cjet[i] = ibook.book1D("TrkDxy_cjet_"+TrkHistCat[i],"Track dxy in c jets ("+TrkHistCat[i]+");track dxy ("+TrkHistCat[i]+");tracks",30,-0.1,0.1);
  	TrkDz_cjet[i] = ibook.book1D("TrkDz_cjet_"+TrkHistCat[i],"Track dz in c jets ("+TrkHistCat[i]+");track dz ("+TrkHistCat[i]+");tracks",30,-0.1,0.1);
  	TrkHitAll_cjet[i] = ibook.book1D("TrkHitAll_cjet_"+TrkHistCat[i],"Number of tracker hits in c jets ("+TrkHistCat[i]+");track number of all hits ("+TrkHistCat[i]+");tracks",31,-0.5,30.5);
  	TrkHitStrip_cjet[i] = ibook.book1D("TrkHitStrip_cjet_"+TrkHistCat[i],"Number of strip hits in c jets ("+TrkHistCat[i]+");track number of strip hits ("+TrkHistCat[i]+");tracks",31,-0.5,30.5);
  	TrkHitPixel_cjet[i] = ibook.book1D("TrkHitPixel_cjet_"+TrkHistCat[i],"Number of pixel hits in c jets ("+TrkHistCat[i]+");track number of pixel hits ("+TrkHistCat[i]+");tracks",9,-0.5,8.5);
  	nTrkTruth_cjet[i] = ibook.book1D("nTrkTruth_cjet_"+TrkHistCat[i],"Number of selected tracks in c jets ("+TrkHistCat[i]+" Truth);number of selected tracks ("+TrkHistCat[i]+" Truth);jets",16,-0.5,15.5);
  	TrkTruthPt_cjet[i] = ibook.book1D("TrkTruthPt_cjet_"+TrkHistCat[i],"Track pT in c jets ("+TrkHistCat[i]+" Truth);track p_{T} ("+TrkHistCat[i]+" Truth);tracks",30,0,150);
  	TrkTruthEta_cjet[i] = ibook.book1D("TrkTruthEta_cjet_"+TrkHistCat[i],"Track #eta in c jets ("+TrkHistCat[i]+" Truth);track #eta ("+TrkHistCat[i]+" Truth);tracks",30,-2.5,2.5);
  	TrkTruthPhi_cjet[i] = ibook.book1D("TrkTruthPhi_cjet_"+TrkHistCat[i],"Track #phi in c jets ("+TrkHistCat[i]+" Truth);track #phi ("+TrkHistCat[i]+" Truth);tracks",30,-3.15,3.15);
  	TrkTruthDxy_cjet[i] = ibook.book1D("TrkTruthDxy_cjet_"+TrkHistCat[i],"Track dxy in c jets ("+TrkHistCat[i]+" Truth);track dxy ("+TrkHistCat[i]+" Truth);tracks",30,-0.1,0.1);
  	TrkTruthDz_cjet[i] = ibook.book1D("TrkTruthDz_cjet_"+TrkHistCat[i],"Track dz in c jets ("+TrkHistCat[i]+" Truth);track dz ("+TrkHistCat[i]+" Truth);tracks",30,-0.1,0.1);
  	TrkTruthHitAll_cjet[i] = ibook.book1D("TrkTruthHitAll_cjet_"+TrkHistCat[i],"Number of tracker hits in c jets ("+TrkHistCat[i]+" Truth);track number of all hits ("+TrkHistCat[i]+" Truth);tracks",31,-0.5,30.5);
  	TrkTruthHitStrip_cjet[i] = ibook.book1D("TrkTruthHitStrip_cjet_"+TrkHistCat[i],"Number of strip hits in c jets ("+TrkHistCat[i]+" Truth);track number of strip hits ("+TrkHistCat[i]+" Truth);tracks",31,-0.5,30.5);
  	TrkTruthHitPixel_cjet[i] = ibook.book1D("TrkTruthHitPixel_cjet_"+TrkHistCat[i],"Number of pixel hits in c jets ("+TrkHistCat[i]+" Truth);track number of pixel hits ("+TrkHistCat[i]+" Truth);tracks",9,-0.5,8.5);
  	
  	// dusg jets
  	nTrk_dusgjet[i] = ibook.book1D("nTrk_dusgjet_"+TrkHistCat[i],"Number of selected tracks in dusg jets ("+TrkHistCat[i]+");number of selected tracks ("+TrkHistCat[i]+");jets",16,-0.5,15.5);
  	TrkPt_dusgjet[i] = ibook.book1D("TrkPt_dusgjet_"+TrkHistCat[i],"Track pT in dusg jets ("+TrkHistCat[i]+");track p_{T} ("+TrkHistCat[i]+");tracks",30,0,150);
  	TrkEta_dusgjet[i] = ibook.book1D("TrkEta_dusgjet_"+TrkHistCat[i],"Track #eta in dusg jets ("+TrkHistCat[i]+");track #eta ("+TrkHistCat[i]+");tracks",30,-2.5,2.5);
  	TrkPhi_dusgjet[i] = ibook.book1D("TrkPhi_dusgjet_"+TrkHistCat[i],"Track #phi in dusg jets ("+TrkHistCat[i]+");track #phi ("+TrkHistCat[i]+");tracks",30,-3.15,3.15);
  	TrkDxy_dusgjet[i] = ibook.book1D("TrkDxy_dusgjet_"+TrkHistCat[i],"Track dxy in dusg jets ("+TrkHistCat[i]+");track dxy ("+TrkHistCat[i]+");tracks",30,-0.1,0.1);
  	TrkDz_dusgjet[i] = ibook.book1D("TrkDz_dusgjet_"+TrkHistCat[i],"Track dz in dusg jets ("+TrkHistCat[i]+");track dz ("+TrkHistCat[i]+");tracks",30,-0.1,0.1);
  	TrkHitAll_dusgjet[i] = ibook.book1D("TrkHitAll_dusgjet_"+TrkHistCat[i],"Number of tracker hits in dusg jets ("+TrkHistCat[i]+");track number of all hits ("+TrkHistCat[i]+");tracks",31,-0.5,30.5);
  	TrkHitStrip_dusgjet[i] = ibook.book1D("TrkHitStrip_dusgjet_"+TrkHistCat[i],"Number of strip hits in dusg jets ("+TrkHistCat[i]+");track number of strip hits ("+TrkHistCat[i]+");tracks",31,-0.5,30.5);
  	TrkHitPixel_dusgjet[i] = ibook.book1D("TrkHitPixel_dusgjet_"+TrkHistCat[i],"Number of pixel hits in dusg jets ("+TrkHistCat[i]+");track number of pixel hits ("+TrkHistCat[i]+");tracks",9,-0.5,8.5);
  	nTrkTruth_dusgjet[i] = ibook.book1D("nTrkTruth_dusgjet_"+TrkHistCat[i],"Number of selected tracks in dusg jets ("+TrkHistCat[i]+" Truth);number of selected tracks ("+TrkHistCat[i]+" Truth);jets",16,-0.5,15.5);
  	TrkTruthPt_dusgjet[i] = ibook.book1D("TrkTruthPt_dusgjet_"+TrkHistCat[i],"Track pT in dusg jets ("+TrkHistCat[i]+" Truth);track p_{T} ("+TrkHistCat[i]+" Truth);tracks",30,0,150);
  	TrkTruthEta_dusgjet[i] = ibook.book1D("TrkTruthEta_dusgjet_"+TrkHistCat[i],"Track #eta in dusg jets ("+TrkHistCat[i]+" Truth);track #eta ("+TrkHistCat[i]+" Truth);tracks",30,-2.5,2.5);
  	TrkTruthPhi_dusgjet[i] = ibook.book1D("TrkTruthPhi_dusgjet_"+TrkHistCat[i],"Track #phi in dusg jets ("+TrkHistCat[i]+" Truth);track #phi ("+TrkHistCat[i]+" Truth);tracks",30,-3.15,3.15);
  	TrkTruthDxy_dusgjet[i] = ibook.book1D("TrkTruthDxy_dusgjet_"+TrkHistCat[i],"Track dxy in dusg jets ("+TrkHistCat[i]+" Truth);track dxy ("+TrkHistCat[i]+" Truth);tracks",30,-0.1,0.1);
  	TrkTruthDz_dusgjet[i] = ibook.book1D("TrkTruthDz_dusgjet_"+TrkHistCat[i],"Track dz in dusg jets ("+TrkHistCat[i]+" Truth);track dz ("+TrkHistCat[i]+" Truth);tracks",30,-0.1,0.1);
  	TrkTruthHitAll_dusgjet[i] = ibook.book1D("TrkTruthHitAll_dusgjet_"+TrkHistCat[i],"Number of tracker hits in dusg jets ("+TrkHistCat[i]+" Truth);track number of all hits ("+TrkHistCat[i]+" Truth);tracks",31,-0.5,30.5);
  	TrkTruthHitStrip_dusgjet[i] = ibook.book1D("TrkTruthHitStrip_dusgjet_"+TrkHistCat[i],"Number of strip hits in dusg jets ("+TrkHistCat[i]+" Truth);track number of strip hits ("+TrkHistCat[i]+" Truth);tracks",31,-0.5,30.5);
  	TrkTruthHitPixel_dusgjet[i] = ibook.book1D("TrkTruthHitPixel_dusgjet_"+TrkHistCat[i],"Number of pixel hits in dusg jets ("+TrkHistCat[i]+" Truth);track number of pixel hits ("+TrkHistCat[i]+" Truth);tracks",9,-0.5,8.5);
  
  }
}


// ---------- Destructor -----------

BDHadronTrackMonitoringAnalyzer::~BDHadronTrackMonitoringAnalyzer()
{  
}


// ---------- Analyze -----------
// This is needed to get a TrackingParticle --> Cluster match (instead of Cluster-->TP) (only needed in processJets)
using P = std::pair<OmniClusterRef, TrackingParticleRef>;
bool compare(const P& i, const P& j) {
  	return i.second.index() > j.second.index();
}

void BDHadronTrackMonitoringAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  
  edm::Handle<pat::JetCollection> patJetsColl;
  iEvent.getByToken(PatJetCollectionTag_, patJetsColl);
  
  edm::Handle<reco::TrackCollection> tracksHandle;
  iEvent.getByToken(TrackCollectionTag_,tracksHandle);
  
  edm::Handle<ClusterTPAssociation> pCluster2TPListH;
  iEvent.getByToken(clusterTPMapToken_, pCluster2TPListH);
  const ClusterTPAssociation& clusterToTPMap = *pCluster2TPListH;
  
  classifier_.newEvent(iEvent, iSetup);
  
  // -----Primary Vertex-----
  const reco::Vertex *pv;
  
  edm::Handle<reco::VertexCollection> primaryVertex ;
  iEvent.getByToken(PrimaryVertexColl_,primaryVertex);

  bool pvFound = (primaryVertex->size() != 0);
  if ( pvFound ) {
    pv = &(*primaryVertex->begin());
  }
  else {
    reco::Vertex::Error e;
    e(0,0)=0.0015*0.0015;
    e(1,1)=0.0015*0.0015;
    e(2,2)=15.*15.;
    reco::Vertex::Point p(0,0,0);
    pv=  new reco::Vertex(p,e,1,1,1);
  }
  // -----------------------
  
  
  // -------- Loop Over Jets ----------
  for ( pat::JetCollection::const_iterator jet = patJetsColl->begin(); jet != patJetsColl->end(); ++jet ) {
    if ( ( jet->pt() < 20 || std::fabs( jet->eta() ) > 2.4 ) ) continue;
    
    unsigned int flav = abs(jet->hadronFlavour());
    
    
    //std::cout << "is there a taginfo? " << jet->hasTagInfo(ipTagInfos_.c_str()) << std::endl;
    const CandIPTagInfo *trackIpTagInfo = jet->tagInfoCandIP(ipTagInfos_.c_str());
    const std::vector<edm::Ptr<reco::Candidate> > & selectedTracks( trackIpTagInfo->selectedTracks() );
	
	
    unsigned int nseltracks = 0;
    nseltracks = selectedTracks.size();
    if (flav == 5){ nTrkAll_bjet->Fill(nseltracks); }
    else if (flav == 4){ nTrkAll_cjet->Fill(nseltracks); }
    else {nTrkAll_dusgjet->Fill(nseltracks);}
    
    int nseltracksCat[6] = {0,0,0,0,0,0}; // following the order of TrkHistCat
    int nseltracksTruthCat[6] = {0,0,0,0,0,0}; // following the order of TrkHistCat
    unsigned int nseltracksTruth = 0;
    
    // -------- Loop Over (selected) Tracks ----------
    for (unsigned int itt=0; itt < nseltracks; ++itt)
	{
		const TrackBaseRef ptrackRef = toTrackRef(selectedTracks[itt]);
        const reco::Track * ptrackPtr = reco::btag::toTrack(ptrackRef);
		const reco::Track & ptrack = *ptrackPtr;
		
		TrackCategories::Flags theFlag = classifier_.evaluate( toTrackRef(selectedTracks[itt]) ).flags();
		
		double TrkPt = ptrack.pt();
		double TrkEta = ptrack.eta();
		double TrkPhi = ptrack.phi();
		double TrkDxy = ptrack.dxy(pv->position());
		double TrkDz = ptrack.dz(pv->position());
		int TrknHitAll = ptrack.numberOfValidHits();
		int TrknHitPixel = ptrack.hitPattern().numberOfValidPixelHits();
		int TrknHitStrip = ptrack.hitPattern().numberOfValidStripHits();

		
		double TrkTruthPt=-99;
		double TrkTruthEta=-99;
		double TrkTruthPhi=-99;
		double TrkTruthDxy=-1;
		double TrkTruthDz=-1;
		int TrkTruthnHitAll=-1;
		int TrkTruthnHitPixel=-1;
		int TrkTruthnHitStrip=-1;
		
		// Get corresponding Trackingparticle
		std::pair<TrackingParticleRef, double> res = classifier_.history().getMatchedTrackingParticle();
		TrackingParticleRef tpr = res.first;
		double quality_tpr = res.second;
		
		// Match TP to hit-cluster (re-ordering according to TP rather than clusters and look for equal_range of a given tpr)
		auto clusterTPmap = clusterToTPMap.map();
		std::sort(clusterTPmap.begin(), clusterTPmap.end(), compare);
		auto clusterRange = std::equal_range(clusterTPmap.begin(), clusterTPmap.end(),std::make_pair(OmniClusterRef(), tpr), compare);
		if (quality_tpr != 0) {
			nseltracksTruth +=1;
			
			TrkTruthPt = tpr->pt();
			TrkTruthEta = tpr->eta();
			TrkTruthPhi = tpr->phi();
			
			TrackingParticle::Point vertex_pv = pv->position();
			TrackingParticle::Point vertex_tpr = tpr->vertex();
			TrackingParticle::Vector momentum_tpr = tpr->momentum();
			TrkTruthDxy = (-(vertex_tpr.x()-vertex_pv.x())*momentum_tpr.y()+(vertex_tpr.y()-vertex_pv.y())*momentum_tpr.x())/tpr->pt();
			TrkTruthDz = (vertex_tpr.z()-vertex_pv.z()) - ((vertex_tpr.x()-vertex_pv.x())*momentum_tpr.x()+(vertex_tpr.y()-vertex_pv.y())*momentum_tpr.y())/sqrt(momentum_tpr.perp2()) * momentum_tpr.z()/sqrt(momentum_tpr.perp2());

			TrkTruthnHitAll = 0;
			TrkTruthnHitPixel = 0;
			TrkTruthnHitStrip = 0;
			if( clusterRange.first != clusterRange.second ) {
				for( auto ip=clusterRange.first; ip != clusterRange.second; ++ip ) {
					const OmniClusterRef& cluster = ip->first;
					if (cluster.isPixel() && cluster.isValid()){ TrkTruthnHitPixel+=1;}
					if (cluster.isStrip() && cluster.isValid()){ TrkTruthnHitStrip+=1;}
				}
			}
			TrkTruthnHitAll = TrkTruthnHitPixel + TrkTruthnHitStrip;
			
			if ( theFlag[TrackCategories::SignalEvent] && theFlag[TrackCategories::BWeakDecay] && theFlag[TrackCategories::CWeakDecay] ) {nseltracksTruthCat[0] += 1;}
			else if ( theFlag[TrackCategories::SignalEvent] && theFlag[TrackCategories::BWeakDecay] && !theFlag[TrackCategories::CWeakDecay] ) {nseltracksTruthCat[1] += 1;}
			else if ( theFlag[TrackCategories::SignalEvent] && !theFlag[TrackCategories::BWeakDecay] && theFlag[TrackCategories::CWeakDecay] ) {nseltracksTruthCat[2] += 1;}
			else if ( !theFlag[TrackCategories::SignalEvent] && theFlag[TrackCategories::Fake] ) {nseltracksTruthCat[3] += 1;}
			else if ( !theFlag[TrackCategories::SignalEvent] && !theFlag[TrackCategories::Fake] ) {nseltracksTruthCat[4] += 1;}
			else{ nseltracksTruthCat[5] += 1; }
			
			
		}
		
		// fill nselectedtrackTruth for different flavour
		if (flav == 5){ nTrkTruthAll_bjet->Fill(nseltracksTruth); }
    	else if (flav == 4){ nTrkTruthAll_cjet->Fill(nseltracksTruth); }
    	else {nTrkTruthAll_dusgjet->Fill(nseltracksTruth);}
		
		
		// ----------- Filling the correct histograms based on jet flavour and Track history Category --------
		// b jet
		if (flav == 5){
			if ( theFlag[TrackCategories::SignalEvent] && theFlag[TrackCategories::BWeakDecay] && theFlag[TrackCategories::CWeakDecay] ) {
				nseltracksCat[0] += 1;
				TrkPt_bjet[0]->Fill(TrkPt);
				TrkEta_bjet[0]->Fill(TrkEta);
				TrkPhi_bjet[0]->Fill(TrkPhi);
				TrkDxy_bjet[0]->Fill(TrkDxy);
				TrkDz_bjet[0]->Fill(TrkDz);
				TrkHitAll_bjet[0]->Fill(TrknHitAll);
				TrkHitPixel_bjet[0]->Fill(TrknHitPixel);
				TrkHitStrip_bjet[0]->Fill(TrknHitStrip);
				TrkTruthPt_bjet[0]->Fill(TrkTruthPt);
				TrkTruthEta_bjet[0]->Fill(TrkTruthEta);
				TrkTruthPhi_bjet[0]->Fill(TrkTruthPhi);
				TrkTruthDxy_bjet[0]->Fill(TrkTruthDxy);
				TrkTruthDz_bjet[0]->Fill(TrkTruthDz);
				TrkTruthHitAll_bjet[0]->Fill(TrkTruthnHitAll);
				TrkTruthHitPixel_bjet[0]->Fill(TrkTruthnHitPixel);
				TrkTruthHitStrip_bjet[0]->Fill(TrkTruthnHitStrip);
			}
			else if ( theFlag[TrackCategories::SignalEvent] && theFlag[TrackCategories::BWeakDecay] && !theFlag[TrackCategories::CWeakDecay] ) {
				nseltracksCat[1] += 1;
				TrkPt_bjet[1]->Fill(TrkPt);
				TrkEta_bjet[1]->Fill(TrkEta);
				TrkPhi_bjet[1]->Fill(TrkPhi);
				TrkDxy_bjet[1]->Fill(TrkDxy);
				TrkDz_bjet[1]->Fill(TrkDz);
				TrkHitAll_bjet[1]->Fill(TrknHitAll);
				TrkHitPixel_bjet[1]->Fill(TrknHitPixel);
				TrkHitStrip_bjet[1]->Fill(TrknHitStrip);
				TrkTruthPt_bjet[1]->Fill(TrkTruthPt);
				TrkTruthEta_bjet[1]->Fill(TrkTruthEta);
				TrkTruthPhi_bjet[1]->Fill(TrkTruthPhi);
				TrkTruthDxy_bjet[1]->Fill(TrkTruthDxy);
				TrkTruthDz_bjet[1]->Fill(TrkTruthDz);
				TrkTruthHitAll_bjet[1]->Fill(TrkTruthnHitAll);
				TrkTruthHitPixel_bjet[1]->Fill(TrkTruthnHitPixel);
				TrkTruthHitStrip_bjet[1]->Fill(TrkTruthnHitStrip);
			}
			else if ( theFlag[TrackCategories::SignalEvent] && !theFlag[TrackCategories::BWeakDecay] && theFlag[TrackCategories::CWeakDecay] ) {
				nseltracksCat[2] += 1;
				TrkPt_bjet[2]->Fill(TrkPt);
				TrkEta_bjet[2]->Fill(TrkEta);
				TrkPhi_bjet[2]->Fill(TrkPhi);
				TrkDxy_bjet[2]->Fill(TrkDxy);
				TrkDz_bjet[2]->Fill(TrkDz);
				TrkHitAll_bjet[2]->Fill(TrknHitAll);
				TrkHitPixel_bjet[2]->Fill(TrknHitPixel);
				TrkHitStrip_bjet[2]->Fill(TrknHitStrip);
				TrkTruthPt_bjet[2]->Fill(TrkTruthPt);
				TrkTruthEta_bjet[2]->Fill(TrkTruthEta);
				TrkTruthPhi_bjet[2]->Fill(TrkTruthPhi);
				TrkTruthDxy_bjet[2]->Fill(TrkTruthDxy);
				TrkTruthDz_bjet[2]->Fill(TrkTruthDz);
				TrkTruthHitAll_bjet[2]->Fill(TrkTruthnHitAll);
				TrkTruthHitPixel_bjet[2]->Fill(TrkTruthnHitPixel);
				TrkTruthHitStrip_bjet[2]->Fill(TrkTruthnHitStrip);
			}
			else if ( !theFlag[TrackCategories::SignalEvent] && theFlag[TrackCategories::Fake] ) {
				nseltracksCat[3] += 1;
				TrkPt_bjet[3]->Fill(TrkPt);
				TrkEta_bjet[3]->Fill(TrkEta);
				TrkPhi_bjet[3]->Fill(TrkPhi);
				TrkDxy_bjet[3]->Fill(TrkDxy);
				TrkDz_bjet[3]->Fill(TrkDz);
				TrkHitAll_bjet[3]->Fill(TrknHitAll);
				TrkHitPixel_bjet[3]->Fill(TrknHitPixel);
				TrkHitStrip_bjet[3]->Fill(TrknHitStrip);
				TrkTruthPt_bjet[3]->Fill(TrkTruthPt);
				TrkTruthEta_bjet[3]->Fill(TrkTruthEta);
				TrkTruthPhi_bjet[3]->Fill(TrkTruthPhi);
				TrkTruthDxy_bjet[3]->Fill(TrkTruthDxy);
				TrkTruthDz_bjet[3]->Fill(TrkTruthDz);
				TrkTruthHitAll_bjet[3]->Fill(TrkTruthnHitAll);
				TrkTruthHitPixel_bjet[3]->Fill(TrkTruthnHitPixel);
				TrkTruthHitStrip_bjet[3]->Fill(TrkTruthnHitStrip);
			}
			else if ( !theFlag[TrackCategories::SignalEvent] && !theFlag[TrackCategories::Fake] ) {
				nseltracksCat[4] += 1;
				TrkPt_bjet[4]->Fill(TrkPt);
				TrkEta_bjet[4]->Fill(TrkEta);
				TrkPhi_bjet[4]->Fill(TrkPhi);
				TrkDxy_bjet[4]->Fill(TrkDxy);
				TrkDz_bjet[4]->Fill(TrkDz);
				TrkHitAll_bjet[4]->Fill(TrknHitAll);
				TrkHitPixel_bjet[4]->Fill(TrknHitPixel);
				TrkHitStrip_bjet[4]->Fill(TrknHitStrip);
				TrkTruthPt_bjet[4]->Fill(TrkTruthPt);
				TrkTruthEta_bjet[4]->Fill(TrkTruthEta);
				TrkTruthPhi_bjet[4]->Fill(TrkTruthPhi);
				TrkTruthDxy_bjet[4]->Fill(TrkTruthDxy);
				TrkTruthDz_bjet[4]->Fill(TrkTruthDz);
				TrkTruthHitAll_bjet[4]->Fill(TrkTruthnHitAll);
				TrkTruthHitPixel_bjet[4]->Fill(TrkTruthnHitPixel);
				TrkTruthHitStrip_bjet[4]->Fill(TrkTruthnHitStrip);
			}
			else {
				nseltracksCat[5] += 1;
				TrkPt_bjet[5]->Fill(TrkPt);
				TrkEta_bjet[5]->Fill(TrkEta);
				TrkPhi_bjet[5]->Fill(TrkPhi);
				TrkDxy_bjet[5]->Fill(TrkDxy);
				TrkDz_bjet[5]->Fill(TrkDz);
				TrkHitAll_bjet[5]->Fill(TrknHitAll);
				TrkHitPixel_bjet[5]->Fill(TrknHitPixel);
				TrkHitStrip_bjet[5]->Fill(TrknHitStrip);
				TrkTruthPt_bjet[5]->Fill(TrkTruthPt);
				TrkTruthEta_bjet[5]->Fill(TrkTruthEta);
				TrkTruthPhi_bjet[5]->Fill(TrkTruthPhi);
				TrkTruthDxy_bjet[5]->Fill(TrkTruthDxy);
				TrkTruthDz_bjet[5]->Fill(TrkTruthDz);
				TrkTruthHitAll_bjet[5]->Fill(TrkTruthnHitAll);
				TrkTruthHitPixel_bjet[5]->Fill(TrkTruthnHitPixel);
				TrkTruthHitStrip_bjet[5]->Fill(TrkTruthnHitStrip);
			}
		}
		
		// c jet
		else if (flav == 4){
			if ( theFlag[TrackCategories::SignalEvent] && theFlag[TrackCategories::BWeakDecay] && theFlag[TrackCategories::CWeakDecay] ) {
				nseltracksCat[0] += 1;
				TrkPt_cjet[0]->Fill(TrkPt);
				TrkEta_cjet[0]->Fill(TrkEta);
				TrkPhi_cjet[0]->Fill(TrkPhi);
				TrkDxy_cjet[0]->Fill(TrkDxy);
				TrkDz_cjet[0]->Fill(TrkDz);
				TrkHitAll_cjet[0]->Fill(TrknHitAll);
				TrkHitPixel_cjet[0]->Fill(TrknHitPixel);
				TrkHitStrip_cjet[0]->Fill(TrknHitStrip);
				TrkTruthPt_cjet[0]->Fill(TrkTruthPt);
				TrkTruthEta_cjet[0]->Fill(TrkTruthEta);
				TrkTruthPhi_cjet[0]->Fill(TrkTruthPhi);
				TrkTruthDxy_cjet[0]->Fill(TrkTruthDxy);
				TrkTruthDz_cjet[0]->Fill(TrkTruthDz);
				TrkTruthHitAll_cjet[0]->Fill(TrkTruthnHitAll);
				TrkTruthHitPixel_cjet[0]->Fill(TrkTruthnHitPixel);
				TrkTruthHitStrip_cjet[0]->Fill(TrkTruthnHitStrip);
			}
			else if ( theFlag[TrackCategories::SignalEvent] && theFlag[TrackCategories::BWeakDecay] && !theFlag[TrackCategories::CWeakDecay] ) {
				nseltracksCat[1] += 1;
				TrkPt_cjet[1]->Fill(TrkPt);
				TrkEta_cjet[1]->Fill(TrkEta);
				TrkPhi_cjet[1]->Fill(TrkPhi);
				TrkDxy_cjet[1]->Fill(TrkDxy);
				TrkDz_cjet[1]->Fill(TrkDz);
				TrkHitAll_cjet[1]->Fill(TrknHitAll);
				TrkHitPixel_cjet[1]->Fill(TrknHitPixel);
				TrkHitStrip_cjet[1]->Fill(TrknHitStrip);
				TrkTruthPt_cjet[1]->Fill(TrkTruthPt);
				TrkTruthEta_cjet[1]->Fill(TrkTruthEta);
				TrkTruthPhi_cjet[1]->Fill(TrkTruthPhi);
				TrkTruthDxy_cjet[1]->Fill(TrkTruthDxy);
				TrkTruthDz_cjet[1]->Fill(TrkTruthDz);
				TrkTruthHitAll_cjet[1]->Fill(TrkTruthnHitAll);
				TrkTruthHitPixel_cjet[1]->Fill(TrkTruthnHitPixel);
				TrkTruthHitStrip_cjet[1]->Fill(TrkTruthnHitStrip);
			}
			else if ( theFlag[TrackCategories::SignalEvent] && !theFlag[TrackCategories::BWeakDecay] && theFlag[TrackCategories::CWeakDecay] ) {
				nseltracksCat[2] += 1;
				TrkPt_cjet[2]->Fill(TrkPt);
				TrkEta_cjet[2]->Fill(TrkEta);
				TrkPhi_cjet[2]->Fill(TrkPhi);
				TrkDxy_cjet[2]->Fill(TrkDxy);
				TrkDz_cjet[2]->Fill(TrkDz);
				TrkHitAll_cjet[2]->Fill(TrknHitAll);
				TrkHitPixel_cjet[2]->Fill(TrknHitPixel);
				TrkHitStrip_cjet[2]->Fill(TrknHitStrip);
				TrkTruthPt_cjet[2]->Fill(TrkTruthPt);
				TrkTruthEta_cjet[2]->Fill(TrkTruthEta);
				TrkTruthPhi_cjet[2]->Fill(TrkTruthPhi);
				TrkTruthDxy_cjet[2]->Fill(TrkTruthDxy);
				TrkTruthDz_cjet[2]->Fill(TrkTruthDz);
				TrkTruthHitAll_cjet[2]->Fill(TrkTruthnHitAll);
				TrkTruthHitPixel_cjet[2]->Fill(TrkTruthnHitPixel);
				TrkTruthHitStrip_cjet[2]->Fill(TrkTruthnHitStrip);
			}
			else if ( !theFlag[TrackCategories::SignalEvent] && theFlag[TrackCategories::Fake] ) {
				nseltracksCat[3] += 1;
				TrkPt_cjet[3]->Fill(TrkPt);
				TrkEta_cjet[3]->Fill(TrkEta);
				TrkPhi_cjet[3]->Fill(TrkPhi);
				TrkDxy_cjet[3]->Fill(TrkDxy);
				TrkDz_cjet[3]->Fill(TrkDz);
				TrkHitAll_cjet[3]->Fill(TrknHitAll);
				TrkHitPixel_cjet[3]->Fill(TrknHitPixel);
				TrkHitStrip_cjet[3]->Fill(TrknHitStrip);
				TrkTruthPt_cjet[3]->Fill(TrkTruthPt);
				TrkTruthEta_cjet[3]->Fill(TrkTruthEta);
				TrkTruthPhi_cjet[3]->Fill(TrkTruthPhi);
				TrkTruthDxy_cjet[3]->Fill(TrkTruthDxy);
				TrkTruthDz_cjet[3]->Fill(TrkTruthDz);
				TrkTruthHitAll_cjet[3]->Fill(TrkTruthnHitAll);
				TrkTruthHitPixel_cjet[3]->Fill(TrkTruthnHitPixel);
				TrkTruthHitStrip_cjet[3]->Fill(TrkTruthnHitStrip);
			}
			else if ( !theFlag[TrackCategories::SignalEvent] && !theFlag[TrackCategories::Fake] ) {
				nseltracksCat[4] += 1;
				TrkPt_cjet[4]->Fill(TrkPt);
				TrkEta_cjet[4]->Fill(TrkEta);
				TrkPhi_cjet[4]->Fill(TrkPhi);
				TrkDxy_cjet[4]->Fill(TrkDxy);
				TrkDz_cjet[4]->Fill(TrkDz);
				TrkHitAll_cjet[4]->Fill(TrknHitAll);
				TrkHitPixel_cjet[4]->Fill(TrknHitPixel);
				TrkHitStrip_cjet[4]->Fill(TrknHitStrip);
				TrkTruthPt_cjet[4]->Fill(TrkTruthPt);
				TrkTruthEta_cjet[4]->Fill(TrkTruthEta);
				TrkTruthPhi_cjet[4]->Fill(TrkTruthPhi);
				TrkTruthDxy_cjet[4]->Fill(TrkTruthDxy);
				TrkTruthDz_cjet[4]->Fill(TrkTruthDz);
				TrkTruthHitAll_cjet[4]->Fill(TrkTruthnHitAll);
				TrkTruthHitPixel_cjet[4]->Fill(TrkTruthnHitPixel);
				TrkTruthHitStrip_cjet[4]->Fill(TrkTruthnHitStrip);
			}
			else {
				nseltracksCat[5] += 1;
				TrkPt_cjet[5]->Fill(TrkPt);
				TrkEta_cjet[5]->Fill(TrkEta);
				TrkPhi_cjet[5]->Fill(TrkPhi);
				TrkDxy_cjet[5]->Fill(TrkDxy);
				TrkDz_cjet[5]->Fill(TrkDz);
				TrkHitAll_cjet[5]->Fill(TrknHitAll);
				TrkHitPixel_cjet[5]->Fill(TrknHitPixel);
				TrkHitStrip_cjet[5]->Fill(TrknHitStrip);
				TrkTruthPt_cjet[5]->Fill(TrkTruthPt);
				TrkTruthEta_cjet[5]->Fill(TrkTruthEta);
				TrkTruthPhi_cjet[5]->Fill(TrkTruthPhi);
				TrkTruthDxy_cjet[5]->Fill(TrkTruthDxy);
				TrkTruthDz_cjet[5]->Fill(TrkTruthDz);
				TrkTruthHitAll_cjet[5]->Fill(TrkTruthnHitAll);
				TrkTruthHitPixel_cjet[5]->Fill(TrkTruthnHitPixel);
				TrkTruthHitStrip_cjet[5]->Fill(TrkTruthnHitStrip);
			}
		}
		
		// dusg jet
		else{
			if ( theFlag[TrackCategories::SignalEvent] && theFlag[TrackCategories::BWeakDecay] && theFlag[TrackCategories::CWeakDecay] ) {
				nseltracksCat[0] += 1;
				TrkPt_dusgjet[0]->Fill(TrkPt);
				TrkEta_dusgjet[0]->Fill(TrkEta);
				TrkPhi_dusgjet[0]->Fill(TrkPhi);
				TrkDxy_dusgjet[0]->Fill(TrkDxy);
				TrkDz_dusgjet[0]->Fill(TrkDz);
				TrkHitAll_dusgjet[0]->Fill(TrknHitAll);
				TrkHitPixel_dusgjet[0]->Fill(TrknHitPixel);
				TrkHitStrip_dusgjet[0]->Fill(TrknHitStrip);
				TrkTruthPt_dusgjet[0]->Fill(TrkTruthPt);
				TrkTruthEta_dusgjet[0]->Fill(TrkTruthEta);
				TrkTruthPhi_dusgjet[0]->Fill(TrkTruthPhi);
				TrkTruthDxy_dusgjet[0]->Fill(TrkTruthDxy);
				TrkTruthDz_dusgjet[0]->Fill(TrkTruthDz);
				TrkTruthHitAll_dusgjet[0]->Fill(TrkTruthnHitAll);
				TrkTruthHitPixel_dusgjet[0]->Fill(TrkTruthnHitPixel);
				TrkTruthHitStrip_dusgjet[0]->Fill(TrkTruthnHitStrip);
			}
			else if ( theFlag[TrackCategories::SignalEvent] && theFlag[TrackCategories::BWeakDecay] && !theFlag[TrackCategories::CWeakDecay] ) {
				nseltracksCat[1] += 1;
				TrkPt_dusgjet[1]->Fill(TrkPt);
				TrkEta_dusgjet[1]->Fill(TrkEta);
				TrkPhi_dusgjet[1]->Fill(TrkPhi);
				TrkDxy_dusgjet[1]->Fill(TrkDxy);
				TrkDz_dusgjet[1]->Fill(TrkDz);
				TrkHitAll_dusgjet[1]->Fill(TrknHitAll);
				TrkHitPixel_dusgjet[1]->Fill(TrknHitPixel);
				TrkHitStrip_dusgjet[1]->Fill(TrknHitStrip);
				TrkTruthPt_dusgjet[1]->Fill(TrkTruthPt);
				TrkTruthEta_dusgjet[1]->Fill(TrkTruthEta);
				TrkTruthPhi_dusgjet[1]->Fill(TrkTruthPhi);
				TrkTruthDxy_dusgjet[1]->Fill(TrkTruthDxy);
				TrkTruthDz_dusgjet[1]->Fill(TrkTruthDz);
				TrkTruthHitAll_dusgjet[1]->Fill(TrkTruthnHitAll);
				TrkTruthHitPixel_dusgjet[1]->Fill(TrkTruthnHitPixel);
				TrkTruthHitStrip_dusgjet[1]->Fill(TrkTruthnHitStrip);
			}
			else if ( theFlag[TrackCategories::SignalEvent] && !theFlag[TrackCategories::BWeakDecay] && theFlag[TrackCategories::CWeakDecay] ) {
				nseltracksCat[2] += 1;
				TrkPt_dusgjet[2]->Fill(TrkPt);
				TrkEta_dusgjet[2]->Fill(TrkEta);
				TrkPhi_dusgjet[2]->Fill(TrkPhi);
				TrkDxy_dusgjet[2]->Fill(TrkDxy);
				TrkDz_dusgjet[2]->Fill(TrkDz);
				TrkHitAll_dusgjet[2]->Fill(TrknHitAll);
				TrkHitPixel_dusgjet[2]->Fill(TrknHitPixel);
				TrkHitStrip_dusgjet[2]->Fill(TrknHitStrip);
				TrkTruthPt_dusgjet[2]->Fill(TrkTruthPt);
				TrkTruthEta_dusgjet[2]->Fill(TrkTruthEta);
				TrkTruthPhi_dusgjet[2]->Fill(TrkTruthPhi);
				TrkTruthDxy_dusgjet[2]->Fill(TrkTruthDxy);
				TrkTruthDz_dusgjet[2]->Fill(TrkTruthDz);
				TrkTruthHitAll_dusgjet[2]->Fill(TrkTruthnHitAll);
				TrkTruthHitPixel_dusgjet[2]->Fill(TrkTruthnHitPixel);
				TrkTruthHitStrip_dusgjet[2]->Fill(TrkTruthnHitStrip);
			}
			else if ( !theFlag[TrackCategories::SignalEvent] && theFlag[TrackCategories::Fake] ) {
				nseltracksCat[3] += 1;
				TrkPt_dusgjet[3]->Fill(TrkPt);
				TrkEta_dusgjet[3]->Fill(TrkEta);
				TrkPhi_dusgjet[3]->Fill(TrkPhi);
				TrkDxy_dusgjet[3]->Fill(TrkDxy);
				TrkDz_dusgjet[3]->Fill(TrkDz);
				TrkHitAll_dusgjet[3]->Fill(TrknHitAll);
				TrkHitPixel_dusgjet[3]->Fill(TrknHitPixel);
				TrkHitStrip_dusgjet[3]->Fill(TrknHitStrip);
				TrkTruthPt_dusgjet[3]->Fill(TrkTruthPt);
				TrkTruthEta_dusgjet[3]->Fill(TrkTruthEta);
				TrkTruthPhi_dusgjet[3]->Fill(TrkTruthPhi);
				TrkTruthDxy_dusgjet[3]->Fill(TrkTruthDxy);
				TrkTruthDz_dusgjet[3]->Fill(TrkTruthDz);
				TrkTruthHitAll_dusgjet[3]->Fill(TrkTruthnHitAll);
				TrkTruthHitPixel_dusgjet[3]->Fill(TrkTruthnHitPixel);
				TrkTruthHitStrip_dusgjet[3]->Fill(TrkTruthnHitStrip);
			}
			else if ( !theFlag[TrackCategories::SignalEvent] && !theFlag[TrackCategories::Fake] ) {
				nseltracksCat[4] += 1;
				TrkPt_dusgjet[4]->Fill(TrkPt);
				TrkEta_dusgjet[4]->Fill(TrkEta);
				TrkPhi_dusgjet[4]->Fill(TrkPhi);
				TrkDxy_dusgjet[4]->Fill(TrkDxy);
				TrkDz_dusgjet[4]->Fill(TrkDz);
				TrkHitAll_dusgjet[4]->Fill(TrknHitAll);
				TrkHitPixel_dusgjet[4]->Fill(TrknHitPixel);
				TrkHitStrip_dusgjet[4]->Fill(TrknHitStrip);
				TrkTruthPt_dusgjet[4]->Fill(TrkTruthPt);
				TrkTruthEta_dusgjet[4]->Fill(TrkTruthEta);
				TrkTruthPhi_dusgjet[4]->Fill(TrkTruthPhi);
				TrkTruthDxy_dusgjet[4]->Fill(TrkTruthDxy);
				TrkTruthDz_dusgjet[4]->Fill(TrkTruthDz);
				TrkTruthHitAll_dusgjet[4]->Fill(TrkTruthnHitAll);
				TrkTruthHitPixel_dusgjet[4]->Fill(TrkTruthnHitPixel);
				TrkTruthHitStrip_dusgjet[4]->Fill(TrkTruthnHitStrip);
			}
			else {
				nseltracksCat[5] += 1;
				TrkPt_dusgjet[5]->Fill(TrkPt);
				TrkEta_dusgjet[5]->Fill(TrkEta);
				TrkPhi_dusgjet[5]->Fill(TrkPhi);
				TrkDxy_dusgjet[5]->Fill(TrkDxy);
				TrkDz_dusgjet[5]->Fill(TrkDz);
				TrkHitAll_dusgjet[5]->Fill(TrknHitAll);
				TrkHitPixel_dusgjet[5]->Fill(TrknHitPixel);
				TrkHitStrip_dusgjet[5]->Fill(TrknHitStrip);
				TrkTruthPt_dusgjet[5]->Fill(TrkTruthPt);
				TrkTruthEta_dusgjet[5]->Fill(TrkTruthEta);
				TrkTruthPhi_dusgjet[5]->Fill(TrkTruthPhi);
				TrkTruthDxy_dusgjet[5]->Fill(TrkTruthDxy);
				TrkTruthDz_dusgjet[5]->Fill(TrkTruthDz);
				TrkTruthHitAll_dusgjet[5]->Fill(TrkTruthnHitAll);
				TrkTruthHitPixel_dusgjet[5]->Fill(TrkTruthnHitPixel);
				TrkTruthHitStrip_dusgjet[5]->Fill(TrkTruthnHitStrip);
			}
		}
		
	
	}
	// -------- END Loop Over (selected Tracks ----------
	
	if (flav == 5){
		for (unsigned int i = 0; i < TrkHistCat.size(); i++){
			nTrk_bjet[i]->Fill(nseltracksCat[i]);
			nTrkTruth_bjet[i]->Fill(nseltracksTruthCat[i]);
		}
	}
    else if (flav == 4){
    	for (unsigned int i = 0; i < TrkHistCat.size(); i++){
			nTrk_cjet[i]->Fill(nseltracksCat[i]);
			nTrkTruth_cjet[i]->Fill(nseltracksTruthCat[i]);
		}
    }
    else {
    	for (unsigned int i = 0; i < TrkHistCat.size(); i++){
			nTrk_dusgjet[i]->Fill(nseltracksCat[i]);
			nTrkTruth_dusgjet[i]->Fill(nseltracksTruthCat[i]);
		}
    }
    
  }
  // -------- END Loop Over Jets ----------

  
}





//define this as a plug-in
DEFINE_FWK_MODULE(BDHadronTrackMonitoringAnalyzer);
