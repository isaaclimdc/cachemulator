/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

#include <vector>
#include "CMAddr.h"

class CMLine;  // Forward-declare

class CMSet {
  private:
    int _E;

  public:
    CMSet(int E);
    ~CMSet();

    bool isInSet(CMAddr *addr);
    std::vector<CMLine *> lines;
};
