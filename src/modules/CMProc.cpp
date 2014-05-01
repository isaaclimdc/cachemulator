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

CMProc::CMProc(int procId) {
  cache = new CMCache();
  isDone = false;
  currentJob = NULL;
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
    if (currentJob != NULL) {
      delete currentJob;
    }
    CMAddr *nextJob = jobs.front();
    nextJob->printAddr();
    state_t stype = cache->accessCache(nextJob);

    switch (stype) {
    case STYPE_HIT:
      currentJob = new CMJob(JOB_TYPE_DELAY, CONFIG->cache_hit_delay, NULL);
      break;
    case STYPE_MISS:
    case STYPE_EVICT:
      BUSRequests[procId] = true;
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
