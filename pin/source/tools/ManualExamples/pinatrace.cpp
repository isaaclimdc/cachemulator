#include <stdio.h>
#include <cstring>
#include "pin.H"

FILE *trace;
PIN_LOCK lock;

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
  std::string name = RTN_Name(rtn);

  if (strcmp(name.c_str(), "meowmeow") == 0) {
    RTN_Open(rtn);
    // For each instruction of the routine
    for (INS ins = RTN_InsHead(rtn); INS_Valid(ins); ins = INS_Next(ins)) {
      UINT32 memOperands = INS_MemoryOperandCount(ins);

      // Iterate over each memory operand of the instruction.
      for (UINT32 memOp = 0; memOp < memOperands; memOp++) {
        if (INS_MemoryOperandIsRead(ins, memOp)) {
          if (INS_IsStackRead(ins)) continue;
          INS_InsertPredicatedCall(
            ins, IPOINT_BEFORE, (AFUNPTR)RecordMemRead,
            IARG_INST_PTR,
            IARG_MEMORYOP_EA, memOp, IARG_THREAD_ID,
            IARG_END);
        }
        if (INS_MemoryOperandIsWritten(ins, memOp)) {
          if (INS_IsStackWrite(ins)) continue;
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
  trace = fopen("pinatrace.out", "w");

  PIN_InitSymbols();

  RTN_AddInstrumentFunction(Routine, 0);

  PIN_AddFiniFunction(Fini, 0);

  PIN_StartProgram();

  return 0;
}
