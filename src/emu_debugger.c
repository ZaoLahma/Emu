#include "emu_debugger.h"
#include "emu_debug.h"
#include "../test/test_emu_cpu.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define NO_BREAKPOINT       (0xFFFFu)
#define MAX_COMMAND_LENGTH  (50u)
#define BREAKPOINT_CMD      "bp"
#define RUN_CMD             "r"

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
static void parseCommand(void);

static void showMenu(void)
{
  DEBUG_LOG("EMU DEBUGGER");
  DEBUG_LOG("Set breakpoint: bp <address> (ex: bp 0x20)");
  DEBUG_LOG("Run           : r")
}

void parseCommand(void)
{
  if(0u == memcmp(context.command, BREAKPOINT_CMD, strlen(BREAKPOINT_CMD)))
  {
    DEBUG_LOG("bp found");
    context.stop = false;
  }
}

void EMUDEBUGGER_init(void)
{
  (void) memset(&context, 0u, sizeof(context));
  context.breakpoint = NO_BREAKPOINT;
  context.stop = true;
  context.active = false;
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
      (void) memset(context.command, 0u, sizeof(context.command));

      showMenu();

      int32_t command;

      while((command = getchar()) != '\n' &&
             context.commandIndex < MAX_COMMAND_LENGTH - 1u)
      {
        context.command[context.commandIndex] = (uint8_t)command;
        context.commandIndex += 1u;
      }
      context.command[context.commandIndex] = '\0';
      context.commandIndex += 1u;

      parseCommand();
    }
  }
}
