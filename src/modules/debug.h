/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

#include <assert.h>

#define DEBUG

#define dassert(exp, str)                        \
do {                                             \
  if (!(exp))                                    \
    dprintf("ASSERTION FAILED: %s\n", (str));    \
  assert((exp));                                 \
} while (0)

void dprintf(const char *format, ...);
void printBUSRequests();
