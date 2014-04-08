/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

class CMLine {
  private:

  public:
    CMLine();
    ~CMLine();

    int tag;
    bool valid;
    bool dirty;
    int age;
};
