#ifndef HEADER_EMU_DEBUGGER
#define HEADER_EMU_DEBUGGER

#include "emu_cpu.h"

void EMUDEBUGGER_init(void);
void EMUDEBUGGER_run(EMUCPU_Context* cpu);

#endif
