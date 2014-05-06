/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMProc.h"
#include "debug.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include "CMAddr.h"
#include "CMCache.h"
#include "CMLine.h"
#include "CMJob.h"
#include "CMGlobals.h"
#include "CMBusShout.h"

CMProc::CMProc(size_t _pid) {
  cache = new CMCache();
  isDone = false;
  currentJob = new CMJob();
  pendingShout = new CMBusShout();
  pid = _pid;
}

CMProc::~CMProc() {
  delete cache;
  delete currentJob;
  delete pendingShout;
}

void CMProc::tick() {
  // TODO: Need to take into account idle cycles
  if (requests.size() == 0 && currentJob->jobDone) {
    isDone = true;
    return;
  }

  bool makeShout = true;
  shout_t shoutType = BusRd;
  res_t rtype = RTYPE_HIT;
  if (currentJob->jobDone) {
    CMAddr *newReq = requests.front();
    newReq->print();
    _updatePendingRequest(newReq, makeShout, shoutType, rtype);
    // Save this shout until request granted by bus arbiter
    // Copy newReq because pop destroys it
    if (makeShout) {
      BUSRequests[pid] = true;  // flag the request vector
      CMAddr *newReqCopy = newReq->copy();
      pendingShout->update(newReqCopy, shoutType, currentJob);
      currentJob->update(JTYPE_WAIT_UNTIL, -1, NULL);
    }

    requests.pop();

    cache->printRType(rtype);

#ifdef DEBUG
    writeToFile(rtype);
#endif
  }
  else if (!pendingShout->isDone) {
    // tried to make the request to shout, but wasn't granted, so try again
    _updatePendingRequest(pendingShout->addr, makeShout, shoutType, rtype);
    if (makeShout) {
      pendingShout->update(NULL, shoutType, NULL);
      BUSRequests[pid] = true;  // flag the request vector
    } else {
      dassert(false, "update shout decides no longer need to make a shout, unimplemented!");
    }
  }
  else {
    currentJob->tick();
  }
}

void CMProc::_updatePendingRequest(CMAddr *newReq,
    bool &makeShout, shout_t &shoutType, res_t &rtype) {
  int data;
  rtype = cache->accessCache(newReq, data);
  state_t stype = cache->getLineState(newReq);

  // create current job or busRequest based on cache access result
  shoutType = BusRd;
  makeShout = true;
  inst_t itype = newReq->itype;

  switch(stype) {
  case STYPE_INVALID:
    if (itype == ITYPE_READ) {
      makeShout = true;
      shoutType = BusRd;
    } else {
      makeShout = true;
      shoutType = BusRdX;
    }
    break;

  case STYPE_SHARED:
    if (itype == ITYPE_READ) {
      makeShout = false;
    } else {
      makeShout = true;
      shoutType = BusUpg;
    }
    break;

  case STYPE_MODIFIED:
    makeShout = false;
    break;

  default:
    dassert(false, "UNIMPLEMENTED state type");
    break;
  }
}

void CMProc::writeToFile(res_t rtype) {
  std::ofstream file;
  file.open(FILE_HITSMISSES, std::ios_base::app);
  file << cache->rTypeToChar(rtype) << "\n";
  file.close();
}

void CMProc::bringShoutedLineIntoCache(bool shared) {
  cache->bringLineIntoCache(pendingShout->addr, shared);
}

void CMProc::respondToBusShout(CMBusShout *shout, bool &shared, bool &dirty) {
  shared = false;
  dirty = false;

  state_t stype = cache->getLineState(shout->addr);
  switch (stype) {
    case STYPE_SHARED:
      switch (shout->shoutType) {
        case BusRd:
          // Other proc just reading. Stay in SHARED state.
          dprintf("Proc %d responds: 0x%llx staying in SHARED state\n",
                  pid, shout->addr->raw);
          shared = true;
          break;
        case BusRdX:
        case BusUpg:
          // Other proc has intention to write. INVALIDATE, but don't flush.
          cache->invalidate(shout->addr);
          dprintf("Proc %d responds: Move 0x%llx to INVALID state\n",
                  pid, shout->addr->raw);
          break;
        default:
          dassert(false, "Cache shared, but not BusRd or BusRdX");
          break;
      }
      break;

    case STYPE_MODIFIED:
      dirty = true;
      switch (shout->shoutType) {
        case BusRd:
          // Other proc reading. Move to SHARED state and FLUSH.
          cache->setLineState(shout->addr, STYPE_SHARED);
          dprintf("Proc %d responds: Move 0x%llx to SHARED state and FLUSH\n",
                  pid, shout->addr->raw);
          break;
        case BusUpg:
        case BusRdX:
          // Other proc has intention to write. INVALIDATE and FLUSH.
          cache->invalidate(shout->addr);
          dprintf("Proc %d responds: Move 0x%llx to INVALID state and FLUSH\n",
                  pid, shout->addr->raw);
          break;
        default:
          dassert(false, "Cache shared, but not BusRd or BusRdX");
          break;
      }
    break;

    case STYPE_INVALID:
    default:
      break;
  }
}
