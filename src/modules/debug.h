/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

#include <assert.h>

#define DEBUG 1

void dprintf(const char *format, ...);
void dassert(bool exp, const char *str);
void printBUSRequests();
