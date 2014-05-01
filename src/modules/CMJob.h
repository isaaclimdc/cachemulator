/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

#define JOB_TYPE_DELAY 0
#define JOB_TYPE_WAIT_UNTIL 1

class CMJob {
  private:

  public:
    CMJob();
    ~CMJob();

    void newJob(int _jobType, int _delayTime, CMJob *_requestingJob);
    void tick();
    void signalDone();

    bool jobDone;
    int jobType;
    int remainingTicks;
    CMJob *requestingJob;
};
