/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMJob.h"

CMJob::CMJob(int jobType, int delayTime, CMJob *requestingJob) {
  jobType = jobType;
  remainingTicks = delayTime;
  jobDone = (remainingTicks == 0);
  requestingJob = requestingJob;
}

CMJob::~CMJob() {
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
