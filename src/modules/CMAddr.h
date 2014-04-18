/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

class CMAddr {
  private:

  public:
    CMAddr(long long unsigned raw_addr, int s, int b);
    ~CMAddr();

    void printAddr();

    unsigned tag;
    unsigned set_index;
    unsigned block_offset;
    long long unsigned raw;
};
