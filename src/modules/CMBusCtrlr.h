/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

#include <cstring>

class CMJob;

class CMBusCtrlr {
  private:
    int roundRobin;

  public:
    CMBusCtrlr();
    ~CMBusCtrlr();

    size_t tick();
    CMJob *currentJob;
    void setJob(CMJob *respondToJob);
};
