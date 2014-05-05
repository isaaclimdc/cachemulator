/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

#include <vector>

class CMLine;
class CMAddr;

class CMSet {
  private:

  public:
    CMSet();
    ~CMSet();

    // If addr is in the set, return the line it is in,
    // otherwise return NULL.
    CMLine *getLine(CMAddr *addr, long long unsigned cacheAge);
    bool bringLineIntoSet(CMAddr *addr);

    std::vector<CMLine*> lines;
};
