/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "CMJob.h"
#include <string>
#include "debug.h"

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
  if (!jobDone) {
    switch (jobType) {
      case JTYPE_DELAY:
        remainingTicks--;
        if (remainingTicks == 0) {
          setDone();
        }
        break;

      case JTYPE_WAIT_UNTIL:
        break;

      default:
        break;
    }
  }
}

void CMJob::signalDone() {
  setDone();
}

void CMJob::setDone() {
  if (requestingJob != NULL) {
    requestingJob->signalDone();
  }
  jobDone = true;
}
