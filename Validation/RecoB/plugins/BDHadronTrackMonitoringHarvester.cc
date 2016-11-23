#include "Validation/RecoB/plugins/BDHadronTrackMonitoringHarvester.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DQMOffline/RecoB/interface/Tools.h"
#include "DQMOffline/RecoB/interface/TagInfoPlotterFactory.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/MakerMacros.h"

using namespace edm;
using namespace std;
using namespace RecoBTag;

BDHadronTrackMonitoringHarvester::BDHadronTrackMonitoringHarvester(const edm::ParameterSet& pSet) 
{
}


BDHadronTrackMonitoringHarvester::~BDHadronTrackMonitoringHarvester()
{
}

void BDHadronTrackMonitoringHarvester::beginJob()
{
}

void BDHadronTrackMonitoringHarvester::dqmEndJob(DQMStore::IBooker & ibook, DQMStore::IGetter & iget)
{
  // Book all histograms.

  setTDRStyle();

}


//define this as a plug-in
DEFINE_FWK_MODULE(BDHadronTrackMonitoringHarvester);
