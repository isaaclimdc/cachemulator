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

void CMComp::tick() {
  // Tick bus

  // Tick each processor
  std::vector<CMProc*>::iterator it;
  std::vector<CMProc*> addrs = test->addrs;
  for (it = addrs.begin(); it != addrs.end(); ++it) {
    CMProc *proc = *it;
    proc.tick();
  }
}

void CMComp::distrbTrace(CMTest *test) {
  std::vector<CMAddr*>::iterator it;
  std::vector<CMAddr*> addrs = test->addrs;
  for (it = addrs.begin(); it != addrs.end(); ++it) {
    CMAddr *addr = *it;
    CMProc *proc = procs.at(addr->pid);
    proc->jobs.push_back(addr);
  }
}
