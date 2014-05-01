/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMJob.h"

CMJob::CMJob() {
  jobDone = true;
}

CMJob::~CMJob() {
}

void CMJob::newJob(int _jobType, int _delayTime, CMJob *_requestingJob) {
  jobType = _jobType;
  remainingTicks = _delayTime;
  jobDone = (remainingTicks == 0);
  requestingJob = _requestingJob;
}

void CMJob::tick() {
  switch (jobType) {

  case JOB_TYPE_DELAY:
    remainingTicks--;
    jobDone = (remainingTicks == 0);
    break;

  case JOB_TYPE_WAIT_UNTIL:
    // do nothing
    break;

  default:
    break;
  }
}

void CMJob::signalDone() {
  jobDone = true;
}
