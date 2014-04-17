/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include <stdlib.h>
#include <stdio.h>

#include "CMCache.h"
#include "modules/CMSet.h"
#include "modules/CMAddr.h"
#include "modules/CMLine.h"

int main(int argc, char **argv) {
    CMCache *c = new CMCache(5, 6);
    delete c;
    return 0;
}
