/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

class CMAddr;

class CMLine {
  private:

  public:
    CMLine();
    ~CMLine();

    bool isHit(CMAddr *addr);
    void update(CMAddr *addr);

    unsigned tag;
    bool valid;
    bool dirty;
    int age;
};
