/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

#include <map>
#include <vector>

class CMBusShout;
class CMAddr;

class CMSharing {
  private:
    bool isInMap(long long unsigned addr);

  public:
    CMSharing();
    ~CMSharing();

    void record(CMBusShout *shout);
    void print();
    void reportContension();

    std::map<long long unsigned,
             long long unsigned> sharingMap;

    std::map<long long unsigned,
             std::vector<CMAddr*> > addrAccessHistory;
};
