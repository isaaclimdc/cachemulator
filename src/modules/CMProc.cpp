/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMProc.h"
#include "debug.h"
#include <stdio.h>
#include "CMAddr.h"
#include "CMCache.h"
#include "CMJob.h"
#include "CMGlobals.h"
#include "CMBusShout.h"
#include <string>

CMProc::CMProc(int procId) {
  cache = new CMCache();
  isDone = false;
  currentJob = NULL;
  pendingBusShout = NULL;
  procId = procId;
}

CMProc::~CMProc() {
}

void CMProc::tick(std::vector<state_t> &verif) {
  // TODO: Need to take into account idle cycles
  if (jobs.size() == 0) {
    isDone = true;
    return;
  }

  if (currentJob == NULL || currentJob->jobDone) {

    // free old job
    if (currentJob != NULL) {
      delete currentJob;
    }

    // fetch next memory access
    CMAddr *nextJob = jobs.front();
    nextJob->printAddr();
    state_t stype = cache->accessCache(nextJob);

    // create current job or busRequest based on cache access result
    switch (stype) {
    case STYPE_HIT:
      currentJob = new CMJob(JOB_TYPE_DELAY, CONFIG->cache_hit_delay, NULL);
      break;

    // miss and evict both need bus shouts
    case STYPE_MISS:
    case STYPE_EVICT:
      BUSRequests[procId] = true;
      currentJob = new CMJob(JOB_TYPE_WAIT_UNTIL, -1, NULL);

      // decide shout type based on R/W
      shout_t shoutType;
      if (nextJob->itype == ITYPE_READ) {
        shoutType = BusRd;
      } else if (nextJob->itype == ITYPE_WRITE) {
        shoutType = BusRdX;
      } else {
        throw std::string("Unsupported ITYPE!!!");
      }

      pendingBusShout = new CMBusShout(nextJob, shoutType, currentJob);
      break;
    default:
      break;
    }
    jobs.pop();

    // for testing
    #ifdef DEBUG
    verif.push_back(stype);
    cache->printSType(stype);
    #endif

  } else {
      currentJob->tick();
  }
}
