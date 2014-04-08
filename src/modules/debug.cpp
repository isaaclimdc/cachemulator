/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#include "debug.h"
#include <stdarg.h>
#include <stdio.h>

void dprintf(const char *format, ...) {
#ifndef DEBUG
  return;
#endif
  va_list arg;
  int done;
  va_start(arg, format);
  done = vfprintf(stdout, format, arg);
  va_end(arg);
}
