/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

#include <vector>

enum res_t {
  RTYPE_HIT,
  RTYPE_MISS,
  RTYPE_EVICT
};

class CMSet;
class CMAddr;

class CMCache {
  private:

  public:
    CMCache();
    ~CMCache();

    res_t accessCache(CMAddr *addr);
    bool isInCache(CMAddr *addr);
    bool bringLineIntoCache(CMAddr *addr);
    void printRType(res_t rtype);
    long long unsigned cacheAge;

    std::vector<CMSet*> sets;
};
