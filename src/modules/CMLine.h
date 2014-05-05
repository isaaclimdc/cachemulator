/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

enum state_t {
  STYPE_INVALID,  // Not in this cache
  STYPE_SHARED,
  STYPE_MODIFIED
};

class CMAddr;

class CMLine {
  private:

  public:
    CMLine();
    ~CMLine();

    bool isHit(CMAddr *addr, long long unsigned cacheAge);
    void update(CMAddr *addr, bool shared);

    unsigned tag;
    bool dirty;
    int age;
    state_t stype;
};
