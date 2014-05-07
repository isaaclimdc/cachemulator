/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMProc.h"
#include "debug.h"
#include <stdio.h>
#include <fstream>

#include "CMAddr.h"
#include "CMCache.h"
#include "CMLine.h"
#include "CMJob.h"
#include "CMGlobals.h"
#include "CMBusShout.h"
#include "CMComp.h"

#define REQS_REFILL_SIZE 100
#define REQS_MIN_QUEUE_SIZE 10

CMProc::CMProc(size_t _pid) {
  cache = new CMCache();
  isDone = false;
  currentJob = new CMJob();
  pendingShout = new CMBusShout();
  pid = _pid;

  reqFile.open(MAKE_TMP_FILEPATH(_pid).c_str(), std::ios_base::app);
  refillReqsIfNeeded();
}

CMProc::~CMProc() {
  delete cache;
  delete currentJob;
  delete pendingShout;
  reqFile.close();
}

void CMProc::tick() {
  // TODO: Need to take into account idle cycles
  if (reqs.size() == 0 && currentJob->jobDone) {
    isDone = true;
    return;
  }

  refillReqsIfNeeded();

  bool makeShout = true;
  shout_t shoutType = BusRd;
  res_t rtype = RTYPE_HIT;
  CMAddr *evictingAddr;
  if (currentJob->jobDone) {
    CMAddr *newReq = reqs.front();
    newReq->print();
    evictingAddr = _updatePendingRequest(newReq, makeShout, shoutType, rtype);

    if (shoutType == BusWr) {
      // if BusWr then need to evict that line first
        BUSRequests[pid] = true;  // flag the request vector
        CMAddr *newReqCopy = evictingAddr->copy();
        newReqCopy->pid = pid;
        pendingShout->update(newReqCopy, shoutType, currentJob);
        currentJob->update(JTYPE_WAIT_UNTIL, -1, NULL);
    } else {
      if (makeShout) {
        // Save this shout until request granted by bus arbiter
        // Copy newReq because pop destroys it
        BUSRequests[pid] = true;  // flag the request vector
        CMAddr *newReqCopy = newReq->copy();
        pendingShout->update(newReqCopy, shoutType, currentJob);
        currentJob->update(JTYPE_WAIT_UNTIL, -1, NULL);
      } else {
        currentJob->update(JTYPE_DELAY, CONFIG->cacheHitDelay, NULL);
      }
      reqs.pop();
    }

    #ifdef DEBUG
    cache->printRType(rtype);
    writeToFile(rtype);
    #endif
  }

  else if (!pendingShout->isDone) {
    // tried to make the request to shout, but wasn't granted, so try again
    evictingAddr = _updatePendingRequest(pendingShout->addr, makeShout,
                                         shoutType, rtype);
    if (makeShout) {
      pendingShout->update(NULL, shoutType, NULL);
      BUSRequests[pid] = true;  // flag the request vector
    } else {
      dassert(false, "Unimplemented! no make shout");
    }
  }

  else {
    currentJob->tick();
  }
}

inline void CMProc::refillReqsIfNeeded() {
  if (reqs.size() < REQS_MIN_QUEUE_SIZE) {
    // Grab the next REQS_REFILL_SIZE from the file
    int numCnt = 0;
    std::string s;
    while (std::getline(reqFile, s)) {
      char op;
      long long unsigned rawAddr;
      size_t pid;

      sscanf(s.c_str(), "%c %llx %zu", &op, &rawAddr, &pid);

      inst_t itype = op == 'R' ? ITYPE_READ : ITYPE_WRITE;
      CMAddr *addr = new CMAddr(rawAddr, itype, pid);
      addr->print();
      reqs.push(addr);

      numCnt++;
      if (numCnt> REQS_REFILL_SIZE) {
        return;
      }
    }
  }
}

CMAddr *CMProc::_updatePendingRequest(CMAddr *newReq, bool &makeShout,
                                      shout_t &shoutType, res_t &rtype) {
  CMAddr *evictingAddr = NULL;
  rtype = cache->accessCache(newReq, &evictingAddr);
  state_t stype = cache->getLineState(newReq);

  // create current job or busRequest based on cache access result
  shoutType = BusRd;
  makeShout = true;
  inst_t itype = newReq->itype;

  switch(stype) {
  case STYPE_INVALID:
    if (rtype == RTYPE_EVICT && evictingAddr != NULL &&
            cache->getLine(evictingAddr)->stype == STYPE_MODIFIED) {
          // We have the line dirty!!! Need an extra bus write to flush to memory!
          makeShout = true;
          shoutType = BusWr;
    } else {
      if (itype == ITYPE_READ) {
        makeShout = true;
        shoutType = BusRd;
      } else {
        makeShout = true;
        shoutType = BusRdX;
      }
    }
    break;

  case STYPE_FORWARD:
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

  case STYPE_EXCLUSIVE:
    if (CONFIG->hasExclusiveState()) {
      makeShout = false;
      if (itype == ITYPE_WRITE) {
        // Secretly upgrade to Modified without shouting
        dprintf("Secretly upgrading to modified state!!!\n");
        cache->setLineState(newReq, STYPE_MODIFIED);
      }
    }
    break;

  default:
    dassert(false, "UNIMPLEMENTED state type");
    break;
  }
  return evictingAddr;
}

void CMProc::writeToFile(res_t rtype) {
  std::ofstream file;
  file.open(FILE_HITSMISSES, std::ios_base::app);
  file << cache->rTypeToChar(rtype) << "\n";
  file.close();
}

void CMProc::postShoutingProcess(bool shared) {
  switch (pendingShout->shoutType) {
  case BusWr:
    dprintf("BUS WRING\n");
    cache->invalidate(pendingShout->addr);
    break;

  default:
    cache->bringLineIntoCache(pendingShout->addr, shared);
    break;
  }
}

void CMProc::respondToBusShout(CMBusShout *shout, bool &shared, bool &dirty, bool &forward) {
  shared = false;
  dirty = false;
  forward = false;

  state_t stype = cache->getLineState(shout->addr);
  switch (stype) {

    case STYPE_SHARED:
      shared = true;
      switch (shout->shoutType) {
        case BusRd:
          // Other proc just reading. Stay in SHARED state.
          dprintf("Proc %d responds: 0x%llx staying in SHARED state\n",
                  pid, shout->addr->raw);
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

    case STYPE_FORWARD:
    case STYPE_EXCLUSIVE:
      if (CONFIG->hasExclusiveState()) {
        shared = true;
        forward = true;
        switch (shout->shoutType) {
          case BusRd:
            // Other proc just reading. Move to Shared
            cache->setLineState(shout->addr, STYPE_SHARED);
            dprintf("Proc %d responds: 0x%llx moving from Exclusive to SHARED state\n",
                    pid, shout->addr->raw);
            break;
          case BusRdX:
            // Other proc has intention to write. INVALIDATE, but don't flush.
            cache->invalidate(shout->addr);
            dprintf("Proc %d responds: Move 0x%llx from Exclusive to INVALID state\n",
                    pid, shout->addr->raw);
            break;
          case BusUpg:
            if (CONFIG->hasExclusiveState()) {
              cache->invalidate(shout->addr);
              dprintf("Proc %d responds: Move 0x%llx from Exclusive/Forward to INVALID state\n",
                        pid, shout->addr->raw);
            } else {
              dassert(false, "Cache Exclusive but received BusUpg");
            }
            break;
          default:
            dassert(false, "Cache Exclusive but received BusUpg");
            break;
        }
        //dprintf("Processor %d forwarding line 0x%llx\n", pid, shout->addr->raw);
      } else {
        dassert(false, "In Exclusive state in wrong protocol");
      }
      break;

    case STYPE_MODIFIED:
      dirty = true;
      switch (shout->shoutType) {
        case BusRd:
          // Other proc reading. Move to SHARED state and FLUSH.
          shared = true;
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
