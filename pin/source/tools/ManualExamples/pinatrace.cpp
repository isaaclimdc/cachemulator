#include <stdio.h>
#include "pin.H"
#include <fstream>
#include <vector>
#include <cstring>
#include <string>

FILE *trace;
PIN_LOCK lock;
std::vector<std::string> userFuncs;

// Print a memory read record
VOID RecordMemRead(VOID *ip, VOID *addr, THREADID tid) {
  PIN_GetLock(&lock, tid);
  fprintf(trace,"R %p %d\n", addr, tid);
  PIN_ReleaseLock(&lock);
}

// Print a memory write record
VOID RecordMemWrite(VOID *ip, VOID *addr, THREADID tid) {
  PIN_GetLock(&lock, tid);
  fprintf(trace,"W %p %d\n", addr, tid);
  PIN_ReleaseLock(&lock);
}

VOID Routine(RTN rtn, VOID *v) {
  std::string name = PIN_UndecorateSymbolName(RTN_Name(rtn).c_str(),
                       UNDECORATION_NAME_ONLY);

  std::vector<std::string>::iterator it;
  for (it = userFuncs.begin(); it != userFuncs.end(); ++it) {
    std::string userFunc = *it;
    if (name.find(userFunc) == std::string::npos) continue;

    RTN_Open(rtn);

    // For each instruction of the routine
    for (INS ins = RTN_InsHead(rtn); INS_Valid(ins); ins = INS_Next(ins)) {
      UINT32 memOperands = INS_MemoryOperandCount(ins);

      // Iterate over each memory operand of the instruction.
      for (UINT32 memOp = 0; memOp < memOperands; memOp++) {
        if (INS_IsStackRead(ins) || INS_IsStackWrite(ins))
          break;

        if (INS_MemoryOperandIsRead(ins, memOp)) {
          INS_InsertPredicatedCall(
            ins, IPOINT_BEFORE, (AFUNPTR)RecordMemRead,
            IARG_INST_PTR,
            IARG_MEMORYOP_EA, memOp, IARG_THREAD_ID,
            IARG_END);
        }
        if (INS_MemoryOperandIsWritten(ins, memOp)) {
          INS_InsertPredicatedCall(
            ins, IPOINT_BEFORE, (AFUNPTR)RecordMemWrite,
            IARG_INST_PTR,
            IARG_MEMORYOP_EA, memOp, IARG_THREAD_ID,
            IARG_END);
        }
      }
    }

    RTN_Close(rtn);
  }
}

VOID Fini(INT32 code, VOID *v) {
  fclose(trace);
}

int main(int argc, char *argv[]) {
  std::ifstream configFile;
  configFile.open("userfuncs.in");

  std::string s;
  while (std::getline(configFile, s)) {
    userFuncs.push_back(s);
  }
  configFile.close();

  // std::vector<std::string>::iterator it;
  // for (it = userFuncs.begin(); it != userFuncs.end(); ++it) {
  //   std::string str = *it;
  //   printf("func: %s\n", str.c_str());
  // }

  trace = fopen("user.trace", "w");

  PIN_Init(argc, argv);
  PIN_InitSymbols();

  RTN_AddInstrumentFunction(Routine, 0);

  PIN_AddFiniFunction(Fini, 0);

  PIN_StartProgram();

  return 0;
}
