/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

enum state_t {
  STYPE_SHARED,
  STYPE_MODIFIED,
  STYPE_INVALID
};

class CMAddr;

class CMLine {
  private:

  public:
    CMLine();
    ~CMLine();

    bool isHit(CMAddr *addr, long long unsigned cacheAge);
    void update(CMAddr *addr);

    unsigned tag;
    bool valid;
    bool dirty;
    int age;
    state_t stype;
};
