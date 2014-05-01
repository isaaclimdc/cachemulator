/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

#include <vector>
#include <queue>
#include "CMCache.h"

class CMAddr;
class CMCache;
class CMJob;
class CMBusShout;

class CMProc {
  private:
    CMCache *cache;
    CMJob *currentJob;
    int procId;

  public:
    CMProc(int procId);
    ~CMProc();

    void tick(std::vector<res_t> &verif);

    CMBusShout *pendingBusShout;
    std::queue<CMAddr*> requests;
    bool isDone;
};
