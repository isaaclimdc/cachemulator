/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMBusCtrlr.h"
#include "CMGlobals.h"
#include "CMJob.h"
#include "debug.h"
#include "CMBusShout.h"
#include <fstream>

CMBusCtrlr::CMBusCtrlr() {
  roundRobin = 0;
  currentJob = new CMJob();
  currentShoutChar = 'O';
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


    bool found = false;
    for (int i=roundRobin; i<roundRobin + P; ++i) {
      arb = i % P;
      if (BUSRequests[arb]) {
        found = true;
        break;
      }
    }

    if (found) {
      roundRobin = (roundRobin + 1) % P;
      memset(BUSRequests, 0, sizeof(bool) * P);
      // the processors are responsible to set the BusCtrlr's currentJob
      // Because right now we don't have access to the pointer

      return arb;
    } else {
      // indicate that nothing is chosen in this round
      return P;
    }
  } else {
    currentJob->tick();
    // indicate that nothing is chosen in this round
    return P;
  }
}

void CMBusCtrlr::setJob(CMJob *respondToJob) {
  currentJob->update(JTYPE_WAIT_UNTIL, -1, respondToJob);
}

void CMBusCtrlr::trackBusTraffic(CMBusShout *shoutingJob) {
  if (shoutingJob != NULL) {
    // new job type came in
    currentShoutChar = shoutingJob->toChar();
    switch (shoutingJob->shoutType) {
    case (BusRd):
      totalBusRd++;
      break;
    case (BusRdX):
      totalBusRdX++;
      break;
    case (BusWr):
      totalBusWr++;
      break;
    case (BusUpg):
      totalBusUpg++;
      break;
    }
  } else if (currentJob->jobDone) {
    // old job done, no new job
    currentShoutChar = 'I'; // stand for idle
  }

  (*busTrafficFile) << currentShoutChar;
}
