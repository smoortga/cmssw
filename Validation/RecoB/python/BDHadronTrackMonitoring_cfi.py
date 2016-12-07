import FWCore.ParameterSet.Config as cms

from SimTracker.TrackHistory.TrackClassifier_cff import *

BDHadronTrackMonitoringAnalyze = cms.EDAnalyzer("BDHadronTrackMonitoringAnalyzer",
								trackClassifier,
								distJetAxisCut = cms.double(0.07),
								decayLengthCut = cms.double(5.0),
								PatJetSource = cms.InputTag('selectedPatJets'),
								ipTagInfos = cms.string('pfImpactParameter'),
								TrackSource = cms.InputTag('generalTracks'),
								PrimaryVertexSource = cms.InputTag('offlinePrimaryVertices'),
								clusterTPMap = cms.InputTag("tpClusterProducer"),
                                )


BDHadronTrackMonitoringHarvest = cms.EDAnalyzer("BDHadronTrackMonitoringHarvester"
								)
