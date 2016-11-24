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

#process.PoolSource.fileNames = ['root://xrootd-cms.infn.it//store/mc/RunIISpring16DR80/TT_TuneCUETP8M1_13TeV-powheg-pythia8/GEN-SIM-RECODEBUG/PUSpring16_RECODEBUG_80X_mcRun2_asymptotic_2016_v3_ext3-v1/20000/00C31A90-2237-E611-9C70-002590D0AFD0.root']
process.PoolSource.fileNames = ['file:mytest.root']

from PhysicsTools.PatAlgos.patEventContent_cff import patEventContent, patEventContentNoCleaning
process.DQMoutput = cms.OutputModule("DQMRootOutputModule",
                                     fileName = cms.untracked.string("OUT.root"),
                                     outputCommands = cms.untracked.vstring('drop *', *patEventContent)
                                     )
                                
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(50)
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(False),
    allowUnscheduled = cms.untracked.bool(True)
)

process.MessageLogger.cerr.FwkReport.reportEvery = 1


# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:mc', '')  #for MC



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

]


postfix = "PFlow"

process.load("PhysicsTools.PatAlgos.patSequences_cff")
process.load("PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff")
process.load("PhysicsTools.PatAlgos.selectionLayer1.selectedPatCandidates_cff")
process.load("PhysicsTools.PatAlgos.cleaningLayer1.cleanPatCandidates_cff")
process.load("PhysicsTools.PatAlgos.selectionLayer1.countPatCandidates_cff")
from PhysicsTools.PatAlgos.tools.pfTools import *

usePF2PAT(
			process,
			runPF2PAT=True,
			jetAlgo="AK4",
			runOnMC=True,
			postfix=postfix,
			jetCorrections=('AK4PF', ['L1FastJet', 'L2Relative', 'L3Absolute'], 'None'),
			pvCollection=cms.InputTag('offlinePrimaryVertices')
)

 ## Top projections in PF2PAT
getattr(process,"pfPileUpJME"+postfix).checkClosestZVertex = False
getattr(process,"pfNoPileUpJME"+postfix).enable = True
getattr(process,"pfNoMuonJMEPFBRECO"+postfix).enable = False
getattr(process,"pfNoElectronJMEPFBRECO"+postfix).enable = False

#process.load("PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff")
#process.load("PhysicsTools.PatAlgos.selectionLayer1.selectedPatCandidates_cff")
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
        #explicitJTA = False,
        postfix=postfix
)


#process.content = cms.EDAnalyzer("EventContentAnalyzer")
## Add TagInfos to PAT jets
for m in ['patJets'+postfix]:
    if hasattr(process,m) and getattr( getattr(process,m), 'addBTagInfo' ):
        print "Switching 'addTagInfos' for " + m + " to 'True'"
        setattr( getattr(process,m), 'addTagInfos', cms.bool(True) )

# my analyzer
process.load('Validation.RecoB.BDHadronTrackMonitoring_cfi')
process.BDHadronTrackMonitoringAnalyze.PatJetSource = cms.InputTag('selectedPatJets'+postfix)

	
process.BDHadronTrackMonitoringAnalyzer = cms.Path(process.BDHadronTrackMonitoringAnalyze)
#process.dqmsave_step = cms.Path(process.DQMSaver)
process.DQMoutput_step = cms.EndPath(process.DQMoutput)



# Schedule definition
process.schedule = cms.Schedule(
	#process.patSeq,
    process.BDHadronTrackMonitoringAnalyzer,
    process.DQMoutput_step
#    process.dqmsave_step
    )
