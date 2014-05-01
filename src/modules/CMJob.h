/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

#define JOB_TYPE_DELAY 0
#define JOB_TYPE_WAIT_UNTIL 1

class CMJob {
  private:
    int remainingTicks;
    int jobType;
    CMJob *requestingJob;

  public:
    CMJob(int jobType, int delayTicks, CMJob *requestingJob);
    ~CMJob();

    void tick();
    void signalDone();

    bool jobDone;
};
