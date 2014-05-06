/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

#include <vector>
#include <cstring>
#include <queue>
#include "CMCache.h"
#include "CMBusShout.h"

class CMAddr;
class CMCache;
class CMJob;
class CMBusShout;

class CMProc {
  private:
    CMAddr *_updatePendingRequest(CMAddr *newReq,
                                  bool &makeShout,
                                  shout_t &shoutType,
                                  res_t &rytpe);

  public:
    CMProc(size_t _pid);
    ~CMProc();

    void tick();
    void respondToBusShout(CMBusShout *shout, bool &shared, bool &dirty);

    void writeToFile(res_t rtype);

    void postShoutingProcess(bool shared);

    size_t pid;
    CMCache *cache;
    CMBusShout *pendingShout;
    std::queue<CMAddr*> requests;
    bool isDone;
    CMJob *currentJob;
};
