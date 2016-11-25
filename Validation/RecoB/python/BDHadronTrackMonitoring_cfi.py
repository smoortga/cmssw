import FWCore.ParameterSet.Config as cms

from SimTracker.TrackHistory.TrackClassifier_cff import *

BDHadronTrackMonitoringAnalyze = cms.EDAnalyzer("BDHadronTrackMonitoringAnalyzer",
								trackClassifier,
								PatJetSource = cms.InputTag('selectedPatJets'),
								ipTagInfos = cms.string('pfImpactParameter'),
								TrackSource = cms.InputTag('generalTracks'),
								PrimaryVertexSource = cms.InputTag('offlinePrimaryVertices'),
								clusterTPMap = cms.InputTag("tpClusterProducer"),
                                )


BDHadronTrackMonitoringHarvest = cms.EDAnalyzer("BDHadronTrackMonitoringHarvester"
								)
