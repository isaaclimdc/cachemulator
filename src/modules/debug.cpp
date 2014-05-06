/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "debug.h"
#include <stdarg.h>
#include <stdio.h>
#include "CMGlobals.h"

void dprintf(const char *format, ...) {
#ifndef DEBUG
  return;
#endif

  va_list arg;
  va_start(arg, format);
  vfprintf(stdout, format, arg);
  va_end(arg);
}

void printBUSRequests() {
#ifndef DEBUG
  return;
#endif

  dprintf("<");
  for (int i=0; i<CONFIG->numProcs; i++) {
    dprintf("%d", BUSRequests[i] ? 1 : 0);
  }
  dprintf(">\n");
}
