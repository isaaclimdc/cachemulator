/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

enum job_t {
  JTYPE_DELAY,
  JTYPE_WAIT_UNTIL
};

class CMJob {
  private:

  public:
    CMJob();
    ~CMJob();

    void update(job_t _jobType, int _delayTime, CMJob *_requestingJob);
    void tick();
    void signalDone();

    bool jobDone;
    job_t jobType;
    int remainingTicks;
    CMJob *requestingJob;
};
