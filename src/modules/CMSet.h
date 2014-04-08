/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

#include <vector>

class CMLine;  // Forward-declare

class CMSet {
  private:

  public:
    CMSet();
    ~CMSet();

    std::vector<CMLine *> lines;
};
