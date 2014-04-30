/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMComp.h"
#include "debug.h"
#include "CMAddr.h"
#include "CMProc.h"
#include "CMTest.h"

CMComp::CMComp(int P) {
  for (int i=0; i<P; i++) {
    CMProc *proc = new CMProc();
    procs.push_back(proc);
  }
}

CMComp::~CMComp() {
}

void CMComp::tick(std::vector<state_t> &verif) {
  // Tick bus

  // Tick each processor
  std::vector<CMProc*>::iterator it;
  for (it = procs.begin(); it != procs.end(); ++it) {
    CMProc *proc = *it;
    proc->tick(verif);
  }
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
