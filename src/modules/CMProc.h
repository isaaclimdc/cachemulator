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
    int pid;

  public:
    CMProc(int _pid);
    ~CMProc();

    void tick(std::vector<res_t> &verif);
    void respondToBusShout(CMBusShout *shout);

    CMBusShout *pendingShout;
    std::queue<CMAddr*> requests;
    bool isDone;
};
