#include "Validation/RecoB/plugins/BDHadronTrackMonitoringAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h" 

#include "DQMOffline/RecoB/interface/Tools.h"



using namespace reco;
using namespace edm;
using namespace std;


typedef TrackIPTagInfo::input_container Tracks;


// ---------- Constructor -----------

BDHadronTrackMonitoringAnalyzer::BDHadronTrackMonitoringAnalyzer(const edm::ParameterSet& pSet) :
	ipTagInfos_ ( pSet.getParameter<std::string>("ipTagInfos") ),
	PatJetSrc_ ( pSet.getParameter<InputTag>("PatJetSource") ),
	TrackSrc_ ( pSet.getParameter<InputTag>("TrackSource") )
	
{
	PatJetCollectionTag_ = consumes<pat::JetCollection>(PatJetSrc_);
	TrackCollectionTag_ = consumes<reco::TrackCollection>(TrackSrc_);
}



// ---------- BookHistograms -----------

void BDHadronTrackMonitoringAnalyzer::bookHistograms(DQMStore::IBooker & ibook, edm::Run const & run, edm::EventSetup const & es)
{
  //
  // Book all histograms.
  //
  RecoBTag::setTDRStyle();
  
  nSelectedTracks_bJet = ibook.book1D("nSelectedTracks_bJet","Number of selected tracks in b jets;number of selected tracks;jets",11,-0.5,10.5);
  
}


// ---------- Destructor -----------

BDHadronTrackMonitoringAnalyzer::~BDHadronTrackMonitoringAnalyzer()
{  
}


// ---------- Analyze -----------

void BDHadronTrackMonitoringAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  
  edm::Handle<pat::JetCollection> patJetsColl;
  iEvent.getByToken(PatJetCollectionTag_, patJetsColl);
  
  edm::Handle<reco::TrackCollection> tracksHandle;
  iEvent.getByToken(TrackCollectionTag_,tracksHandle);
  
  for ( pat::JetCollection::const_iterator jet = patJetsColl->begin(); jet != patJetsColl->end(); ++jet ) {
    if ( ( jet->pt() < 20 || std::fabs( jet->eta() ) > 2.4 ) ) continue;
    
    unsigned int flav = abs(jet->hadronFlavour());
    
    
    //std::cout << "is there a taginfo? " << jet->hasTagInfo(ipTagInfos_.c_str()) << std::endl;
    const CandIPTagInfo *trackIpTagInfo = jet->tagInfoCandIP(ipTagInfos_.c_str());
    const std::vector<edm::Ptr<reco::Candidate> > & selectedTracks( trackIpTagInfo->selectedTracks() );

    int nseltracks = 0;
    nseltracks = selectedTracks.size();
    if (flav == 5){ nSelectedTracks_bJet->Fill(nseltracks); }
    
  }

  
}





//define this as a plug-in
DEFINE_FWK_MODULE(BDHadronTrackMonitoringAnalyzer);
