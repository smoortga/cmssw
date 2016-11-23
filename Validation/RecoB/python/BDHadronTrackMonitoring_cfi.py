import FWCore.ParameterSet.Config as cms

#from DQMOffline.RecoB.bTagCommon_cff import *

BDHadronTrackMonitoringAnalyze = cms.EDAnalyzer("BDHadronTrackMonitoringAnalyzer",
								JetSource = cms.InputTag('ak4PFJets')
                                )


BDHadronTrackMonitoringHarvest = cms.EDAnalyzer("BDHadronTrackMonitoringHarvester"
								)
