#include "emu_debugger.h"
#include "emu_debug.h"
#include "../test/test_emu_cpu.h"
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
  EMUCPU_Context* cpuContext;
  bool active;
  bool stop;
} EMUDEBUGGER_context;

static EMUDEBUGGER_context context;

static void showMenu(void);

static void showMenu(void)
{
  DEBUG_LOG("EMU DEBUGGER");
  DEBUG_LOG("Set breakpoint: bp <address> (ex: bp 0x20)");
  DEBUG_LOG("Run           : r")
}

void EMUDEBUGGER_init(void)
{
  (void) memset(&context, 0u, sizeof(context));
  context.breakpoint = NO_BREAKPOINT;
  context.stop = true;
  EMUCPU_getContext((struct EMUCPU_Context**)&context.cpuContext);
}

void EMUDEBUGGER_run()
{
  DEBUG_LOG_PRINTF("context.cpuContext->pc: 0x%X", context.cpuContext->pc);
  if(context.active)
  {
    DEBUG_LOG_PRINTF("handling op 0x%x at address 0x%x", context.cpuContext->ram[context.cpuContext->pc], context.cpuContext->pc);
    if(context.cpuContext->pc == context.breakpoint)
    {
      context.stop = true;
    }

    while(context.stop)
    {
      showMenu();

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
