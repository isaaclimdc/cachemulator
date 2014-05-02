/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

#include <cstring>

class CMBusCtrlr {
  private:
    int roundRobin;

  public:
    CMBusCtrlr();
    ~CMBusCtrlr();

    size_t tick();
};
