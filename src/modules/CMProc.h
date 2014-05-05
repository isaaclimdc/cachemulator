/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

#include <vector>
#include <queue>
#include "CMCache.h"
#include "CMBusShout.h"

class CMAddr;
class CMCache;
class CMJob;
class CMBusShout;

class CMProc {
  private:
    CMCache *cache;
    int pid;

  public:
    CMProc(int _pid);
    ~CMProc();

    void tick(std::vector<res_t> &verif);
    void respondToBusShout(CMBusShout *shout, bool &shared, bool &dirty);

    void bringShoutedLineIntoCache(bool shared);
    CMBusShout *pendingShout;
    std::queue<CMAddr*> requests;
    bool isDone;
    CMJob *currentJob;
};
