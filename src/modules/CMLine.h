/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

enum state_t {
  STYPE_INVALID,  // Not in this cache
  STYPE_SHARED,
  STYPE_MODIFIED,
  STYPE_EXCLUSIVE,
  STYPE_FORWARD
};

class CMAddr;

class CMLine {
  private:

  public:
    CMLine(unsigned _setIdx);
    ~CMLine();

    bool isHitUpdateAge(CMAddr *addr, long long unsigned cacheAge);
    bool isHit(CMAddr *addr);
    void update(CMAddr *addr, bool shared);
    CMAddr *getBaseAddr();

    unsigned tag;
    unsigned setIdx; // record the set number to reconstruct addr
    bool dirty;
    int age;
    state_t stype;
};
