/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMProc.h"
#include "debug.h"
#include "CMAddr.h"
#include "CMCache.h"

CMProc::CMProc() {
  cache = new CMCache();
  isDone = false;
}

CMProc::~CMProc() {
}

void CMProc::tick(std::vector<state_t> &verif) {
  // TODO: Need to take into account idle cycles
  if (jobs.size() == 0) {
    isDone = true;
    return;
  }

  // If no currently-executing job
  CMAddr *nextJob = jobs.front();
  nextJob->printAddr();
  state_t stype = cache->accessCache(nextJob);
  jobs.pop();
  verif.push_back(stype);
  cache->printSType(stype);
}
