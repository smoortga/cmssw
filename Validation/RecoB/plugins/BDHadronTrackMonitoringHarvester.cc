#include "Validation/RecoB/plugins/BDHadronTrackMonitoringHarvester.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DQMOffline/RecoB/interface/Tools.h"
#include "DQMOffline/RecoB/interface/TagInfoPlotterFactory.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/MakerMacros.h"

using namespace edm;
using namespace std;
using namespace RecoBTag;

std::map<unsigned int, std::string> TrkHistCategories{
	{0, "BCWeakDecay"},
	{1, "BWeakDecay"},
	{2, "CWeakDecay"},
	{3, "Fake"},
	{4, "PU"},
	{5, "Other"}	
};

typedef std::map<unsigned int, std::string>::iterator it_type;

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
	// ***********************
	//
	// Book all new histograms.
	//
	// ***********************

	// b jets
	nTrk_absolute_bjet = ibook.book1D("nTrk_absolute_bjet","absolute average number of tracks in b jets",6,-0.5,5.5);
	for(it_type iterator = TrkHistCategories.begin(); iterator != TrkHistCategories.end(); iterator++) {
		nTrk_absolute_bjet->setBinLabel(iterator->first+1,iterator->second,1);
	}
	nTrk_absolute_bjet->setAxisRange(0, 5, 2);
	nTrk_absolute_bjet->setAxisTitle("average number of tracks",2);

	nTrk_relative_bjet = ibook.book1D("nTrk_relative_bjet","relative average number of tracks in b jets",6,-0.5,5.5);
	for(it_type iterator = TrkHistCategories.begin(); iterator != TrkHistCategories.end(); iterator++) {
		nTrk_relative_bjet->setBinLabel(iterator->first+1,iterator->second,1);
	}
	nTrk_relative_bjet->setAxisRange(0, 1, 2);
	nTrk_relative_bjet->setAxisTitle("average fraction of tracks",2);


	// c jets
	nTrk_absolute_cjet = ibook.book1D("nTrk_absolute_cjet","absolute average number of tracks in b jets",6,-0.5,5.5);
	for(it_type iterator = TrkHistCategories.begin(); iterator != TrkHistCategories.end(); iterator++) {
		nTrk_absolute_cjet->setBinLabel(iterator->first+1,iterator->second,1);
	}
	nTrk_absolute_cjet->setAxisRange(0, 5, 2);
	nTrk_absolute_cjet->setAxisTitle("average number of tracks",2);

	nTrk_relative_cjet = ibook.book1D("nTrk_relative_cjet","relative average number of tracks in b jets",6,-0.5,5.5);
	for(it_type iterator = TrkHistCategories.begin(); iterator != TrkHistCategories.end(); iterator++) {
		nTrk_relative_cjet->setBinLabel(iterator->first+1,iterator->second,1);
	}
	nTrk_relative_cjet->setAxisRange(0, 1, 2);
	nTrk_relative_cjet->setAxisTitle("average fraction of tracks",2);


	// b jets
	nTrk_absolute_dusgjet = ibook.book1D("nTrk_absolute_dusgjet","absolute average number of tracks in b jets",6,-0.5,5.5);
	for(it_type iterator = TrkHistCategories.begin(); iterator != TrkHistCategories.end(); iterator++) {
		nTrk_absolute_dusgjet->setBinLabel(iterator->first+1,iterator->second,1);
	}
	nTrk_absolute_dusgjet->setAxisRange(0, 5, 2);
	nTrk_absolute_dusgjet->setAxisTitle("average number of tracks",2);

	nTrk_relative_dusgjet = ibook.book1D("nTrk_relative_dusgjet","relative average number of tracks in b jets",6,-0.5,5.5);
	for(it_type iterator = TrkHistCategories.begin(); iterator != TrkHistCategories.end(); iterator++) {
		nTrk_relative_dusgjet->setBinLabel(iterator->first+1,iterator->second,1);
	}
	nTrk_relative_dusgjet->setAxisRange(0, 1, 2);
	nTrk_relative_dusgjet->setAxisTitle("average fraction of tracks",2);

	setTDRStyle();


	// ***********************
	//
	// get all the old histograms
	//
	// ***********************
	
	// b jets
	MonitorElement *nTrk_bjet[6];
	for(it_type iterator = TrkHistCategories.begin(); iterator != TrkHistCategories.end(); iterator++) {
		nTrk_bjet[iterator->first] = iget.get("nTrk_bjet_"+iterator->second);
	}
	MonitorElement *nTrkAll_bjet = iget.get("nTrkAll_bJet");
	
	// c jets
	MonitorElement *nTrk_cjet[6];
	for(it_type iterator = TrkHistCategories.begin(); iterator != TrkHistCategories.end(); iterator++) {
		nTrk_cjet[iterator->first] = iget.get("nTrk_cjet_"+iterator->second);
	}
	MonitorElement *nTrkAll_cjet = iget.get("nTrkAll_cjet");
	
	// dusg jets
	MonitorElement *nTrk_dusgjet[6];
	for(it_type iterator = TrkHistCategories.begin(); iterator != TrkHistCategories.end(); iterator++) {
		nTrk_dusgjet[iterator->first] = iget.get("nTrk_dusgjet_"+iterator->second);
	}
	MonitorElement *nTrkAll_dusgjet = iget.get("nTrkAll_dusgjet");
	
	
	
	// ***********************
	//
	// Calculate contents of new histograms
	//
	// ***********************
	
	// b jets
	float mean_bjets[6];
	float meanAll_bjets;
	meanAll_bjets = nTrkAll_bjet->getMean(1);
	for (unsigned int i = 0; i< TrkHistCategories.size(); i++){
		mean_bjets[i] = nTrk_bjet[i]->getMean(1); // mean number of tracks per category
		nTrk_absolute_bjet->setBinContent(i+1,mean_bjets[i]);
		nTrk_relative_bjet->setBinContent(i+1,mean_bjets[i]/meanAll_bjets);
	}
	
	// c jets
	float mean_cjets[6];
	float meanAll_cjets;
	meanAll_cjets = nTrkAll_cjet->getMean(1);
	for (unsigned int i = 0; i< TrkHistCategories.size(); i++){
		mean_cjets[i] = nTrk_cjet[i]->getMean(1); // mean number of tracks per category
		nTrk_absolute_cjet->setBinContent(i+1,mean_cjets[i]);
		nTrk_relative_cjet->setBinContent(i+1,mean_cjets[i]/meanAll_cjets);
	}
	
	
	// dusg jets
	float mean_dusgjets[6];
	float meanAll_dusgjets;
	meanAll_dusgjets = nTrkAll_dusgjet->getMean(1);
	for (unsigned int i = 0; i< TrkHistCategories.size(); i++){
		mean_dusgjets[i] = nTrk_dusgjet[i]->getMean(1); // mean number of tracks per category
		nTrk_absolute_dusgjet->setBinContent(i+1,mean_dusgjets[i]);
		nTrk_relative_dusgjet->setBinContent(i+1,mean_dusgjets[i]/meanAll_dusgjets);
	}
	
	
	//std::cout << mean_bjets << meanAll_bjets << mean_cjets << meanAll_cjets << mean_dusgjets << meanAll_dusgjets << std::endl;
	
}


//define this as a plug-in
DEFINE_FWK_MODULE(BDHadronTrackMonitoringHarvester);
