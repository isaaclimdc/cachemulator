/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

#include <vector>

enum state_t {
  STYPE_HIT,
  STYPE_MISS,
  STYPE_EVICT
};

class CMSet;
class CMAddr;

class CMCache {
  private:

  public:
    CMCache(int s, int E);
    ~CMCache();

    state_t accessCache(CMAddr *addr);
    bool isInCache(CMAddr *addr);
    bool bringLineIntoCache(CMAddr *addr);
    void printSType(state_t stype);
    long long unsigned cacheAge;

    std::vector<CMSet*> sets;
};
