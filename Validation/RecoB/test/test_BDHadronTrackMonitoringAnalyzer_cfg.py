import FWCore.ParameterSet.Config as cms

process = cms.Process('BDHadronTrackMonitorAnalyzerDQM')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.EDMtoMEAtRunEnd_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

# load DQM
process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.DQMEnvironment_cfi")

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring()
)

process.PoolSource.fileNames = ['root://xrootd-cms.infn.it//store/mc/RunIISpring16DR80/TT_TuneCUETP8M1_13TeV-powheg-pythia8/GEN-SIM-RECODEBUG/PUSpring16_RECODEBUG_80X_mcRun2_asymptotic_2016_v3_ext3-v1/20000/00C31A90-2237-E611-9C70-002590D0AFD0.root']

from PhysicsTools.PatAlgos.patEventContent_cff import patEventContent, patEventContentNoCleaning
process.DQMoutput = cms.OutputModule("DQMRootOutputModule",
                                     fileName = cms.untracked.string("OUT.root"),
                                     outputCommands = cms.untracked.vstring('drop *', *patEventContent)
                                     )

"""
bTagDiscriminators = [
    'pfJetBProbabilityBJetTags'
   ,'pfJetProbabilityBJetTags'
   ,'pfPositiveOnlyJetBProbabilityBJetTags'
   ,'pfPositiveOnlyJetProbabilityBJetTags'
   ,'pfNegativeOnlyJetBProbabilityBJetTags'
   ,'pfNegativeOnlyJetProbabilityBJetTags'
   ,'pfTrackCountingHighPurBJetTags'
   ,'pfTrackCountingHighEffBJetTags'
   ,'pfNegativeTrackCountingHighPurBJetTags'
   ,'pfNegativeTrackCountingHighEffBJetTags'
   ,'pfSimpleSecondaryVertexHighEffBJetTags'
   ,'pfSimpleSecondaryVertexHighPurBJetTags'
   ,'pfNegativeSimpleSecondaryVertexHighEffBJetTags'
   ,'pfNegativeSimpleSecondaryVertexHighPurBJetTags'
   ,'pfCombinedSecondaryVertexV2BJetTags'
   ,'pfPositiveCombinedSecondaryVertexV2BJetTags'
   ,'pfNegativeCombinedSecondaryVertexV2BJetTags'
   ,'pfCombinedInclusiveSecondaryVertexV2BJetTags'
   ,'pfPositiveCombinedInclusiveSecondaryVertexV2BJetTags'
   ,'pfNegativeCombinedInclusiveSecondaryVertexV2BJetTags'
   ,'softPFMuonBJetTags'
   ,'positiveSoftPFMuonBJetTags'
   ,'negativeSoftPFMuonBJetTags'
   ,'softPFElectronBJetTags'
   ,'positiveSoftPFElectronBJetTags'
   ,'negativeSoftPFElectronBJetTags'
   ,'pfCombinedMVAV2BJetTags'
   ,'pfNegativeCombinedMVAV2BJetTags'
   ,'pfPositiveCombinedMVAV2BJetTags'
   ,'pfCombinedCvsBJetTags'
   ,'pfNegativeCombinedCvsBJetTags'
   ,'pfPositiveCombinedCvsBJetTags'
   ,'pfCombinedCvsLJetTags'
   ,'pfNegativeCombinedCvsLJetTags'
   ,'pfPositiveCombinedCvsLJetTags'
]

postfix = "PFlow"

process.load("PhysicsTools.PatAlgos.patSequences_cff")
from PhysicsTools.PatAlgos.tools.pfTools import *
#from PhysicsTools.PatAlgos.patTemplate_cfg import *
#from PhysicsTools.PatAlgos.tools.coreTools import *
usePF2PAT(
			process,
			runPF2PAT=True,
			jetAlgo="AK4",
			runOnMC=True,
			postfix=postfix,
			jetCorrections=('AK4PF', ['L1FastJet', 'L2Relative', 'L3Absolute'], 'None'),
			pvCollection=cms.InputTag('offlinePrimaryVertices')
)



process.load("PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff")
process.load("PhysicsTools.PatAlgos.selectionLayer1.selectedPatCandidates_cff")
# switch jet collection to make PAT collection
from PhysicsTools.PatAlgos.tools.jetTools import *
switchJetCollection(
        process,
        jetSource = cms.InputTag('ak4PFJetsCHS'),
        #'ak4PFJets'
        pfCandidates = cms.InputTag('particleFlow'),
        pvSource = cms.InputTag('offlinePrimaryVertices'),
        svSource = cms.InputTag('inclusiveCandidateSecondaryVertices'),
        muSource = cms.InputTag('muons'),
        elSource = cms.InputTag('gedGsfElectrons'),
        btagInfos = [
    				'pfImpactParameterTagInfos'
   					,'pfSecondaryVertexTagInfos'
   					,'pfInclusiveSecondaryVertexFinderTagInfos'
   					,'pfSecondaryVertexNegativeTagInfos'
   					,'pfInclusiveSecondaryVertexFinderNegativeTagInfos'
   					,'softPFMuonsTagInfos'
   					,'softPFElectronsTagInfos'
   					,'pfInclusiveSecondaryVertexFinderCvsLTagInfos'
   					,'pfInclusiveSecondaryVertexFinderNegativeCvsLTagInfos'
		],
        btagDiscriminators = bTagDiscriminators,
        jetCorrections = ('AK4PF', ['L1FastJet', 'L2Relative', 'L3Absolute'], 'None'),
        genJetCollection = cms.InputTag('ak4GenJetsNoNu'),
        genParticles = cms.InputTag('genParticles'),
        postfix=postfix
)

#process.content = cms.EDAnalyzer("EventContentAnalyzer")
"""

# my analyzer
process.load('Validation.RecoB.BDHadronTrackMonitoring_cfi')
process.BDHadronTrackMonitoringAnalyze.JetSource = cms.InputTag('ak4PFJetsCHS')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(500)
)

process.MessageLogger.cerr.FwkReport.reportEvery = 10
#process.MessageLogger = cms.Service("MessageLogger",
#	detailedInfo    = cms.untracked.PSet(
#        eventNumber = cms.untracked.PSet(
#                reportEvery = cms.untracked.int32(10)
#				),
#)


# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:mc', '')  #for MC


# Path and EndPath definitions
#process.BDHadronTrackMonitoringAnalyzer = cms.Path(process.content * process.BDHadronTrackMonitoringAnalyze)
process.BDHadronTrackMonitoringAnalyzer = cms.Path(process.BDHadronTrackMonitoringAnalyze)
#process.dqmsave_step = cms.Path(process.DQMSaver)
process.DQMoutput_step = cms.EndPath(process.DQMoutput)

#process.selPatJets = cms.Path(getattr(process,'selectedPatJets'+postfix))

#process.pftopat = cms.Path(getattr(process,'patPF2PATSequence'+postfix))

# Schedule definition
process.schedule = cms.Schedule(
	#process.pftopat,
    process.BDHadronTrackMonitoringAnalyzer,
    process.DQMoutput_step
#    process.dqmsave_step
    )
