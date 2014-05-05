/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMComp.h"
#include "debug.h"
#include "CMAddr.h"
#include "CMProc.h"
#include "CMTest.h"
#include "CMBusCtrlr.h"
#include "CMGlobals.h"
#include "CMMemCtrlr.h"
#include <string>

CMComp::CMComp(int P) {
  // Get new processors, i is their pid
  for (int i=0; i<P; i++) {
    CMProc *proc = new CMProc(i);
    procs.push_back(proc);
  }

  // Get a bus controller
  busCtrlr = new CMBusCtrlr();

  // Get a mem controller
  memCtrlr = new CMMemCtrlr();
}

CMComp::~CMComp() {
  std::vector<CMProc*>::iterator it;
  for (it = procs.begin(); it != procs.end(); ++it) {
    CMProc *proc = *it;
    delete proc;
  }

  delete busCtrlr;
}

void CMComp::tick(std::vector<res_t> &verif) {
  // Tick each processor
  std::vector<CMProc*>::iterator it;
  for (it = procs.begin(); it != procs.end(); ++it) {
    CMProc *proc = *it;
    proc->tick(verif);
  }

  // Tick bus, returns pid of the 'winning' processor
  size_t shoutPid = busCtrlr->tick();

  if (shoutPid < (size_t)CONFIG->numProcs) {
    // Granted access on this cycle
    // The winning processor makes its request
    dprintf("Proc %lu wins access to the bus!\n", shoutPid);

    CMProc *grantedProc = procs.at(shoutPid);
    CMBusShout *outstandingShout = grantedProc->pendingShout;
    dassert(outstandingShout != NULL, "Granted access to a non-requesting proc!");

    // Other processors respond
    bool hasShare = false;
    bool hasDirty = false;
    for (it = procs.begin(); it != procs.end(); ++it) {
      bool shared;
      bool dirty;
      CMProc *proc = *it;
      proc->respondToBusShout(outstandingShout, shared, dirty);
      hasShare = shared || hasShare;
      hasDirty = dirty || hasDirty;
    }

    // set the bus to wait until our transaction is done
    busCtrlr->setJob(grantedProc->currentJob);
    // TODO: This only works if there is only one
    // outstanding request in the system!!!
    // If more, busCtrlr have to create different jobs

    // then look at the response and react accordingly
    // TODO: mem need to get correct value from processor too...
    if (hasDirty) {
      memCtrlr->addJob(busCtrlr->currentJob, CONFIG->flushAndLoadDelay);
    } else {
      memCtrlr->addJob(busCtrlr->currentJob, CONFIG->memDelay);
    }

    // the granted processor updates its access tag
    grantedProc->bringShoutedLineIntoCache(hasShare);
  }

  memCtrlr->tick();
}

void CMComp::distrbTrace(CMTest *test) {
  std::vector<CMAddr*>::iterator it;
  std::vector<CMAddr*> addrs = test->addrs;
  for (it = addrs.begin(); it != addrs.end(); ++it) {
    CMAddr *addr = *it;
    CMProc *proc = procs.at(addr->pid);
    proc->requests.push(addr);
  }
}

bool CMComp::hasOutstandingJobs() {
  std::vector<CMProc*>::iterator it;
  for (it = procs.begin(); it != procs.end(); ++it) {
    CMProc *proc = *it;
    if (!proc->isDone) {
      return true;
    }
  }

  return false;
}
