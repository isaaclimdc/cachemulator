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

CMProc::CMProc(int _procId) {
  cache = new CMCache();
  isDone = false;
  currentJob = new CMJob();
  pendingBusShout = NULL;
  procId = _procId;
}

CMProc::~CMProc() {
  delete cache;
  delete currentJob;
}

void CMProc::tick(std::vector<res_t> &verif) {
  // TODO: Need to take into account idle cycles
  if (requests.size() == 0) {
    isDone = true;
    return;
  }

  if (currentJob->jobDone) {
    CMAddr *newReq = requests.front();
    newReq->printAddr();
    res_t rtype = cache->accessCache(newReq);

    // create current job or busRequest based on cache access result
    switch (rtype) {
      case RTYPE_HIT:
        currentJob->update(JTYPE_DELAY, CONFIG->cache_hit_delay, NULL);
        break;

      // miss and evict both need bus shouts
      case RTYPE_MISS:
      case RTYPE_EVICT:
        BUSRequests[procId] = true;  // flag the request vector
        currentJob->update(JTYPE_WAIT_UNTIL, -1, NULL);

        // decide shout type based on R/W
        shout_t shoutType;
        if (newReq->itype == ITYPE_READ) {
          shoutType = BusRd;
        } else if (newReq->itype == ITYPE_WRITE) {
          shoutType = BusRdX;
        } else {
          throw std::string("Unsupported ITYPE!!!");
        }

        pendingBusShout = new CMBusShout(newReq, shoutType, currentJob);
        break;
      default:
        break;
    }

    requests.pop();

    // for testing
    #ifdef DEBUG
    verif.push_back(rtype);
    cache->printRType(rtype);
    #endif
  }
  else {
    currentJob->tick();
  }
}
