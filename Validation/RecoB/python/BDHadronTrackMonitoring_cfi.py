import FWCore.ParameterSet.Config as cms

#from DQMOffline.RecoB.bTagCommon_cff import *

BDHadronTrackMonitoringAnalyze = cms.EDAnalyzer("BDHadronTrackMonitoringAnalyzer",
								PatJetSource = cms.InputTag('selectedPatJets'),
								ipTagInfos = cms.string('pfImpactParameter'),
								TrackSource = cms.InputTag('generalTracks')
                                )


BDHadronTrackMonitoringHarvest = cms.EDAnalyzer("BDHadronTrackMonitoringHarvester"
								)
