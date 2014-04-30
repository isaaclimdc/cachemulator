/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

#include <vector>
#include <queue>
#include "CMCache.h"

class CMAddr;
class CMCache;

class CMProc {
  private:
    CMCache *cache;

  public:
    CMProc();
    ~CMProc();

    void tick(std::vector<state_t> &verif);

    std::queue<CMAddr*> jobs;
    bool isDone;
};
