#ifndef HEADER_EMU_DEBUGGER
#define HEADER_EMU_DEBUGGER

#include "emu_cpu.h"
#include <stdbool.h>

void EMUDEBUGGER_init(bool debuggerActive);
void EMUDEBUGGER_run(void);

#endif
