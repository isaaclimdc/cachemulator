/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMMemCtrlr.h"
#include "CMJob.h"
#include "CMGlobals.h"
#include "debug.h"

CMMemCtrlr::CMMemCtrlr() {
}

CMMemCtrlr::~CMMemCtrlr() {
  std::vector<CMJob*>::iterator it;
  for (it = memReqs.begin(); it != memReqs.end(); ++it) {
    CMJob *job= *it;
    delete job;
  }
}

void CMMemCtrlr::tick() {
  std::vector<CMJob*>::iterator it;
  for (it = memReqs.begin(); it != memReqs.end(); ++it) {
    CMJob *job= *it;
    job->tick();
  }
}

void CMMemCtrlr::addJob(CMJob *requestingJob, int delay) {
  // should loop through and replace the done jobs
  CMJob *newJob = new CMJob();
  newJob->update(JTYPE_DELAY, delay, requestingJob);
  memReqs.push_back(newJob);
}
