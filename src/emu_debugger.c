#include "emu_debugger.h"
#include "emu_debug.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define NO_BREAKPOINT (0xFFFFu)

typedef struct
{
  uint16_t breakpoint;
  bool active;
} EMUDEBUGGER_context;

static EMUDEBUGGER_context context;

void EMUDEBUGGER_init(void)
{
  (void) memset(&context, 0u, sizeof(context));
  context.breakpoint = NO_BREAKPOINT;
}

void EMUDEBUGGER_run(EMUCPU_Context* cpu)
{
  DEBUG_LOG("cpu->pc: 0x%X", cpu->pc);
  if(context.active)
  {
    if(cpu->pc == context.breakpoint)
    {

    }
  }
}
