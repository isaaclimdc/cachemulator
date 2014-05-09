/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMComp.h"
#include "debug.h"
#include "CMAddr.h"
#include "CMProc.h"
#include "CMBusCtrlr.h"
#include "CMGlobals.h"
#include "CMMemCtrlr.h"
#include "CMSharing.h"
#include <string>
#include <iostream>
#include <fstream>

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

  totalTicks = 0;

  sharing = new CMSharing();
}

CMComp::~CMComp() {
  std::vector<CMProc*>::iterator it;
  for (it = procs.begin(); it != procs.end(); ++it) {
    CMProc *proc = *it;
    delete proc;
  }

  delete busCtrlr;
  delete sharing;
}

void CMComp::tick() {
  totalTicks++;

  // Tick each processor
  std::vector<CMProc*>::iterator it;
  for (it = procs.begin(); it != procs.end(); ++it) {
    CMProc *proc = *it;
    proc->tick();
  }

  // Tick bus, returns pid of the 'winning' processor
  size_t shoutPid = busCtrlr->tick();

  if (shoutPid < (size_t)CONFIG->numProcs) {
    // Granted access on this cycle
    // The winning processor makes its request
    dprintf("Proc %lu wins access to the bus!\n", shoutPid);

    CMProc *grantedProc = procs.at(shoutPid);
    CMBusShout *outstandingShout = grantedProc->pendingShout;
    dassert(outstandingShout != NULL,
            "Granted access to a non-requesting proc!");
    outstandingShout->print();
    busCtrlr->trackBusTraffic(outstandingShout);

    // Save this shout in the sharing object
    sharing->record(outstandingShout);

#ifdef DEBUG
    writeToFile(outstandingShout);
#endif

    // Other processors respond
    bool hasShare = false;
    bool hasDirty = false;
    bool hasForward = false;
    for (it = procs.begin(); it != procs.end(); ++it) {
      CMProc *proc = *it;
      if (proc->pid != shoutPid) {
        bool shared;
        bool dirty;
        bool forward;
        proc->respondToBusShout(outstandingShout, shared, dirty, forward);
        hasShare = shared || hasShare;
        hasDirty = dirty || hasDirty;
        hasForward = forward || hasForward;
      }
    }

    // set the bus to wait until our transaction is done
    busCtrlr->setJob(grantedProc->currentJob);
    // TODO: This only works if there is only one
    // outstanding request in the system!!!
    // If more, busCtrlr have to create different jobs

    // then look at the response and react accordingly
    if (hasDirty) {
      memCtrlr->addJob(busCtrlr->currentJob, CONFIG->flushAndLoadDelay);
    }
    else if (outstandingShout->shoutType == BusUpg) {
      memCtrlr->addJob(busCtrlr->currentJob, CONFIG->upgDelay);
    }
    else if (hasForward) {
      memCtrlr->addJob(busCtrlr->currentJob, CONFIG->c2cDelay);
    }
    else {
      memCtrlr->addJob(busCtrlr->currentJob, CONFIG->memDelay);
    }

    // the granted processor updates its access tag
    grantedProc->postShoutingProcess(hasShare);
    grantedProc->pendingShout->isDone = true;
  } else {
    busCtrlr->trackBusTraffic(NULL); // c for continue
  }
  memCtrlr->tick();
}

void CMComp::writeToFile(CMBusShout *shout) {
  (*busShoutsFile) << shout->toChar() << "\n";
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
