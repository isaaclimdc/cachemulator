/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

#include <cstring>
#include <vector>

class CMJob;

class CMMemCtrlr {
  private:
    std::vector<CMJob*> memReqs;

  public:
    CMMemCtrlr();
    ~CMMemCtrlr();

    void tick();
    void addJob(CMJob *requestingJob);
};
