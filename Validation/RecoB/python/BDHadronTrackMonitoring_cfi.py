import FWCore.ParameterSet.Config as cms

#from DQMOffline.RecoB.bTagCommon_cff import *

BDHadronTrackMonitoringAnalyze = cms.EDAnalyzer("BDHadronTrackMonitoringAnalyzer",
								JetSource = cms.InputTag('ak4PFJets')
								#PatJetSource = cms.InputTag('selectedPatJets')
								#pfCandidates = cms.InputTag('particleFlow'),
        						#pvSource = cms.InputTag('offlinePrimaryVertices'),
        						#svSource = cms.InputTag('inclusiveCandidateSecondaryVertices'),
        						#muSource = cms.InputTag('muons'),
        						#elSource = cms.InputTag('gedGsfElectrons'),
        						#btagInfos = bTagInfos = [
    							#			'pfImpactParameterTagInfos'
   								#			,'pfSecondaryVertexTagInfos'
   								#			,'pfInclusiveSecondaryVertexFinderTagInfos'
   								#			,'pfSecondaryVertexNegativeTagInfos'
   								#			,'pfInclusiveSecondaryVertexFinderNegativeTagInfos'
   								#			,'softPFMuonsTagInfos'
   								#			,'softPFElectronsTagInfos'
   								#			,'pfInclusiveSecondaryVertexFinderCvsLTagInfos'
   								#			,'pfInclusiveSecondaryVertexFinderNegativeCvsLTagInfos'
								#],
        						#btagDiscriminators = []
        						#jetCorrections = ('AK4PF', ['L1FastJet', 'L2Relative', 'L3Absolute'], 'None'),
        						#genJetCollection = cms.InputTag('ak4GenJetsNoNu'),
        						#genParticles = cms.InputTag('genParticles'),
        						#trackSource = cms.InputTag('generalTracks')
                                )


BDHadronTrackMonitoringHarvest = cms.EDAnalyzer("BDHadronTrackMonitoringHarvester"
								)
