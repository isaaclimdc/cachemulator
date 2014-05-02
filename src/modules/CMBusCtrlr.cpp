/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMBusCtrlr.h"
#include "CMGlobals.h"
#include "CMJob.h"
#include "debug.h"

CMBusCtrlr::CMBusCtrlr() {
  roundRobin = 0;
  currentJob = new CMJob();
}

CMBusCtrlr::~CMBusCtrlr() {
  delete currentJob;
}

// Use round robin arbitration to grant access. Returns pid
// of the 'winning' processor.
size_t CMBusCtrlr::tick() {
  int P = CONFIG->numProcs;

  if (currentJob->jobDone) {
    size_t arb = 0;


    for (int i=roundRobin; i<roundRobin + P; ++i) {
      arb = i % P;
      if (BUSRequests[arb]) {
        break;
      }
    }

    roundRobin = (roundRobin + 1) % P;
    memset(BUSRequests, 0, sizeof(bool) * P);
    // the processors are responsible to set the BusCtrlr's currentJob
    // Because right now we don't have access to the pointer

    return arb;
  } else {
    currentJob->tick();
    // indicate that nothing is chosen in this round
    return P;
  }
}

void CMBusCtrlr::setJob(CMJob *respondToJob) {
  currentJob->update(JTYPE_WAIT_UNTIL, -1, respondToJob);
}
