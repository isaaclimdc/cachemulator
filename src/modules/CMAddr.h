/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

#include <cstring>

enum inst_t {
  ITYPE_READ,
  ITYPE_WRITE
};

class CMAddr {
  private:

  public:
    CMAddr(long long unsigned raw_addr, inst_t _itype, size_t _pid);
    ~CMAddr();

    void print();
    CMAddr *copy();

    unsigned tag;
    unsigned setIndex;
    unsigned blockOffset;
    long long unsigned raw;

    inst_t itype;
    size_t pid;
};
