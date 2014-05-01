/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

#include <vector>
#include "CMCache.h"

class CMProc;
class CMBusCtrlr;
class CMTest;

class CMComp {
  private:

  public:
    CMComp(int P);
    ~CMComp();

    void tick(std::vector<state_t> &verif);
    void distrbTrace(CMTest *test);
    bool hasOutstandingJobs();

    std::vector<CMProc*> procs;
    CMBusCtrlr *busCtrlr;
};
