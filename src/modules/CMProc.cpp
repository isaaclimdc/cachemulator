/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMProc.h"
#include "debug.h"
#include <stdio.h>
#include <string>

#include "CMAddr.h"
#include "CMCache.h"
#include "CMLine.h"
#include "CMJob.h"
#include "CMGlobals.h"
#include "CMBusShout.h"

CMProc::CMProc(int _pid) {
  cache = new CMCache();
  isDone = false;
  currentJob = new CMJob();
  pendingShout = NULL;
  pid = _pid;
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
    int data;
    res_t rtype = cache->accessCache(newReq, data);

    // create current job or busRequest based on cache access result
    switch (rtype) {
      case RTYPE_HIT:
        currentJob->update(JTYPE_DELAY, CONFIG->cacheHitDelay, NULL);
        break;

      // miss and evict both need bus shouts
      case RTYPE_MISS:
      case RTYPE_EVICT: {
        BUSRequests[pid] = true;  // flag the request vector

        // decide shout type based on R/W
        shout_t shoutType = BusRd;
        if (newReq->itype == ITYPE_READ) {
          shoutType = BusRd;
        } else if (newReq->itype == ITYPE_WRITE) {
          shoutType = BusRdX;
        } else {
          dassert(false, "Unsupported itype!");
        }

        // Save this shout until request granted by bus arbiter
        // Copy newReq because pop destroys it
        CMAddr *newReqCopy = newReq->copy();
        pendingShout = new CMBusShout(newReqCopy, shoutType, currentJob);
        break;
      }
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

void CMProc::bringShoutedLineIntoCache(bool shared) {
  cache->bringLineIntoCache(pendingShout->addr, shared);
}

void CMProc::respondToBusShout(CMBusShout *shout,
                               bool *shared,
                               bool *dirty) {
  // A request is pending on this processor; don't respond
  *shared = false;
  *dirty = false;
  if (BUSRequests[pid]) {
    currentJob->update(JTYPE_WAIT_UNTIL, -1, NULL);
    return;
  }

  state_t stype = cache->getLineState(shout->addr);
  switch (stype) {
    case STYPE_SHARED:
      switch (shout->shoutType) {
        case BusRd:
          // Other proc just reading. Stay in SHARED state.
          dprintf("Staying in SHARED state, addr");
          shout->addr->printAddr();
          *shared = true;
          break;
        case BusRdX:
          // Other proc has intention to write. INVALIDATE, but don't flush.
          cache->invalidate(shout->addr);
          dprintf("Move to INVALID state, addr");
          shout->addr->printAddr();
          break;
        default:
          dassert(false, "Cache shared, but not busRd or busRdx not implemented");
          break;
      }
      break;

    case STYPE_MODIFIED:
      *dirty = true;
      switch (shout->shoutType) {
        case BusRd:
          // Other proc reading. Move to SHARED state and FLUSH.
          dprintf("Move to SHARED state and FLUSH, addr %p\n", shout->addr);
          break;
        case BusRdX:
          // Other proc has intention to write. INVALIDATE and FLUSH.
          dprintf("Move to INVALID state and FLUSH, addr %p\n", shout->addr);
          break;
        default:
          break;
      }
    break;

    case STYPE_INVALID:
    default:
      break;
  }
}
