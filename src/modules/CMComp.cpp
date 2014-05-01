/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMComp.h"
#include "debug.h"
#include "CMAddr.h"
#include "CMProc.h"
#include "CMTest.h"
#include "CMBusCtrlr.h"
#include <string>

CMComp::CMComp(int P) {

  // get new processors
  for (int i=0; i<P; i++) {
    CMProc *proc = new CMProc(i);
    procs.push_back(proc);
  }

  // get a bus controller
  busCtrlr = new CMBusCtrlr();
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
  // Tick bus
  int shoutingProc = busCtrlr->tick();

  dprintf("Proc %d got access to Bus!!!", shoutingProc);
  // The processor Granted access make request
  CMBusShout *outstandingReq = procs.at(shoutingProc)->pendingBusShout;
  if (outstandingReq == NULL) {
    throw std::string("Granted access to a non-requesting proc???");
  }

  // TODO: tick Memory
}

void CMComp::distrbTrace(CMTest *test) {
  std::vector<CMAddr*>::iterator it;
  std::vector<CMAddr*> addrs = test->addrs;
  for (it = addrs.begin(); it != addrs.end(); ++it) {
    CMAddr *addr = *it;
    CMProc *proc = procs.at(addr->pid);
    proc->jobs.push(addr);
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
