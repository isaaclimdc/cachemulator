/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

enum shout_t {
  BusRd,   // R
  BusRdX,  // X
  BusWr,   // W
  BusUpg   // U
};

class CMAddr;
class CMJob;

class CMBusShout {
  private:
    CMJob *requestingJob;

  public:
    CMBusShout(CMAddr *_addr, shout_t _shoutType, CMJob *_requestingJob);
    ~CMBusShout();

    CMAddr *addr;
    shout_t shoutType;
    bool isDone;

    char toChar();
    void print();
};
