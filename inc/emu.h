#ifndef HEADER_EMU
#define HEADER_EMU

#include <stdbool.h>

void EMU_init(char* romPath, bool debuggerActive);
void EMU_run(void);
void EMU_abort(void);

#endif
