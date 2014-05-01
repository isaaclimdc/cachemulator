/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

class CMBusCtrlr {
  private:
    int round_robin;

  public:
    CMBusCtrlr();
    ~CMBusCtrlr();

    int tick();
};
