#include "Validation/RecoB/plugins/BDHadronTrackMonitoringAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h" 


using namespace reco;
using namespace edm;
using namespace std;
//using namespace RecoBTag;
//typedef edm::View<reco::Jet> JetCollection;

BDHadronTrackMonitoringAnalyzer::BDHadronTrackMonitoringAnalyzer(const edm::ParameterSet& pSet) :
	JetSrc_ ( pSet.getParameter<InputTag>("JetSource") )
	//PatJetSrc_ ( pSet.getParameter<InputTag>("PatJetSource") )
{
	JetCollectionTag_ = consumes<reco::PFJetCollection>(JetSrc_);
}

void BDHadronTrackMonitoringAnalyzer::bookHistograms(DQMStore::IBooker & ibook, edm::Run const & run, edm::EventSetup const & es)
{
  //
  // Book all histograms.
  //
  nJets = ibook.book1D("nJets","Number of jets;number of jets;Entries",16,-0.5,15.5);
  
}


BDHadronTrackMonitoringAnalyzer::~BDHadronTrackMonitoringAnalyzer()
{  
}


void BDHadronTrackMonitoringAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  edm::Handle<reco::PFJetCollection> jetsColl;
  iEvent.getByToken(JetCollectionTag_, jetsColl);
  
  int njets = 0;
  for ( reco::PFJetCollection::const_iterator jet = jetsColl->begin(); jet != jetsColl->end(); ++jet ) {
    if ( ( jet->pt() < 20 || std::fabs( jet->eta() ) > 2.4 ) ) continue;
    njets += 1;
  }
  //std::cout << njets << std::endl;
  nJets->Fill(njets);
  
}


//define this as a plug-in
DEFINE_FWK_MODULE(BDHadronTrackMonitoringAnalyzer);
