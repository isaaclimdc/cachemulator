/* CACHEMULATOR
 * Yuyang Guo (yuyangg) and Isaac Lim (idl)
 */

#pragma once

#include "CMConfig.h"
#include <string>

#define FILE_HITSMISSES "hitsmisses.out"
#define FILE_BUSSHOUTS "busshouts.out"

extern CMConfig *CONFIG;
extern bool *BUSRequests;
extern std::ofstream *busShoutsFile;
extern std::ofstream *hitsMissesFile;

std::string MAKE_TMP_FILEPATH(size_t pid);
