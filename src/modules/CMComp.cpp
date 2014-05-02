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
  // Get new processors, i is their pid
  for (int i=0; i<P; i++) {
    CMProc *proc = new CMProc(i);
    procs.push_back(proc);
  }

  // Get a bus controller
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

  // Tick bus, returns pid of the 'winning' processor
  size_t shoutPid = busCtrlr->tick();

  dprintf("Proc %d wins access to the bus!\n", shoutPid);

  // The winning processor makes its request
  CMBusShout *outstandingShout = procs.at(shoutPid)->pendingShout;
  if (outstandingShout == NULL) {
    dprintf("Granted access to a non-requesting proc!!!\n");
    throw;
  }

  // Other processors respond
  for (it = procs.begin(); it != procs.end(); ++it) {
    CMProc *proc = *it;
    proc->respondToBusShout(outstandingShout);
  }

  // TODO: Tick memory
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
