/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

#include <map>

class CMBusShout;

class CMSharing {
  private:
    bool isInMap(long long unsigned addr);

  public:
    CMSharing();
    ~CMSharing();

    void record(CMBusShout *shout);
    void print();

    std::map<long long unsigned,
             long long unsigned> sharingMap;
};
