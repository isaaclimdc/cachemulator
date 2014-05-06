/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

#include <vector>
#include <string>
#include "CMCache.h"

class CMProc;
class CMBusCtrlr;
class CMTest;
class CMMemCtrlr;
class CMBusShout;
class CMSharing;

class CMComp {
  private:

  public:
    CMComp(int P);
    ~CMComp();

    void tick();
    bool hasOutstandingJobs();

    void writeToFile(CMBusShout *shout);

    std::vector<CMProc*> procs;
    CMBusCtrlr *busCtrlr;
    CMMemCtrlr *memCtrlr;
    long long unsigned totalTicks;
    CMSharing *sharing;
};
