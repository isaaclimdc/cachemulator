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
    CMSet(int _setIdx);
    ~CMSet();

    // If addr is in the set, return the line it is in,
    // otherwise return NULL.
    CMLine *getLineUpdateAge(CMAddr *addr, long long unsigned cacheAge);
    CMLine *getLine(CMAddr *addr);
    CMAddr *probeLine(CMAddr *addr);
    void bringLineIntoSet(CMAddr *addr, bool shared,
                          long long unsigned cacheAge);

    std::vector<CMLine*> lines;
};
