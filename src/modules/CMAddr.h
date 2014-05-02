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

    void printAddr();
    CMAddr *copy();

    unsigned tag;
    unsigned set_index;
    unsigned block_offset;
    long long unsigned raw;

    inst_t itype; //TODO: Should these be here or in CMInst?
    size_t pid;
};
