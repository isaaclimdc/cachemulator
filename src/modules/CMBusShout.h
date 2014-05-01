/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

class CMAddr;
class CMJob;

enum shout_t {
  BusRd,
  BusRdX,
  BusWr,
  BusUpg
};

class CMBusShout {
  private:
    CMAddr *addr;
    shout_t shoutType;
    CMJob *requestingJob;

  public:
    CMBusShout(CMAddr *addr, shout_t shoutType, CMJob *requestingJob);
    ~CMBusShout();

};
