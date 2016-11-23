#ifndef BDHadronTrackMonitoringHarvester_H
#define BDHadronTrackMonitoringHarvester_H

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "DQMServices/Core/interface/DQMEDHarvester.h"

/** \class BDHadronTrackMonitoringHarvester
 *
 *  Top level steering routine for b tag performance harvesting.
 *
 */

class BDHadronTrackMonitoringHarvester : public DQMEDHarvester {
   public:
      explicit BDHadronTrackMonitoringHarvester(const edm::ParameterSet& pSet);
      ~BDHadronTrackMonitoringHarvester();

   private:
      void dqmEndJob(DQMStore::IBooker &, DQMStore::IGetter &) override;
      void beginJob();

};

#endif
