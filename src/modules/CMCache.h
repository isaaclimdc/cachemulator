/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

#include <vector>
#include <map>
#include "CMLine.h"

enum res_t {
  RTYPE_HIT,
  RTYPE_MISS,
  RTYPE_EVICT
};

class CMSet;
class CMLine;
class CMAddr;

class CMCache {
  private:

  public:
    CMCache();
    ~CMCache();

    res_t accessCache(CMAddr *addr);

    // If addr is in the cache, return the line it is in,
    // otherwise return NULL.
    CMLine *getLine(CMAddr *addr);

    bool probeLine(CMAddr *addr);
    void bringLineIntoCache(CMAddr *addr, bool shared);

    // If addr is in the cache, return the state of the line,
    // otherwise return NULL.
    state_t getLineState(CMAddr *addr);

    void setLineState(CMAddr *addr, state_t setToState);

    void invalidate(CMAddr *addr);

    char rTypeToChar(res_t rtype);
    void printRType(res_t rtype);

    long long unsigned cacheAge;
    std::vector<CMSet*> sets;
};
