/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMJob.h"

CMJob::CMJob() {
  jobDone = true;
}

CMJob::~CMJob() {
}

void CMJob::update(job_t _jobType, int _delayTime, CMJob *_requestingJob) {
  jobType = _jobType;
  remainingTicks = _delayTime;
  jobDone = (remainingTicks == 0);
  requestingJob = _requestingJob;
}

void CMJob::tick() {
  switch (jobType) {
    case JTYPE_DELAY:
      remainingTicks--;
      jobDone = (remainingTicks == 0);
      break;

    case JTYPE_WAIT_UNTIL:
      // do nothing
      break;

    default:
      break;
  }
}

void CMJob::signalDone() {
  jobDone = true;
}
