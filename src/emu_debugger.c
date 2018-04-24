#include "emu_debugger.h"
#include "emu_debug.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define NO_BREAKPOINT       (0xFFFFu)
#define MAX_COMMAND_LENGTH  (50u)

typedef struct
{
  uint8_t command[MAX_COMMAND_LENGTH];
  uint8_t commandIndex;
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

    DEBUG_LOG("handling op 0x%x at address 0x%x", cpu->ram[cpu->pc], cpu->pc);
    if(cpu->pc == context.breakpoint)
    {
      int32_t command;

      while((command = getchar()) != EOF &&
             context.commandIndex < MAX_COMMAND_LENGTH)
      {
        context.command[context.commandIndex] = (uint8_t)command;
        context.commandIndex += 1u;
      }
    }
  }
}
