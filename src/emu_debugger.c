#include "emu_debugger.h"
#include "emu_debug.h"
#include "../test/test_emu_cpu.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define NO_BREAKPOINT       (0xFFFFu)
#define MAX_COMMAND_LENGTH  (50u)
#define BREAKPOINT_CMD      "bp"
#define RUN_CMD             "r"
#define QUIT_CMD            "q"
#define STEP_CMD            "s"

typedef struct
{
  uint8_t command[MAX_COMMAND_LENGTH];
  uint8_t commandIndex;
  uint16_t breakpoint;
  EMUCPU_Context* cpuContext;
  bool active;
  bool stop;
  bool step;
} EMUDEBUGGER_context;

static EMUDEBUGGER_context context;

static void showMenu(void);
static void parseCommand(void);

static void showMenu(void)
{
  DEBUG_LOG("EMU DEBUGGER");
  DEBUG_LOG("Set breakpoint: bp <address> (ex: bp 0x20)");
  DEBUG_LOG("Run           : r")
  DEBUG_LOG("Step          : s")
  DEBUG_LOG("Quit          : q")
}

void parseCommand(void)
{
  bool validCommand = false;

  context.step = false;

  if(0u == memcmp(context.command, BREAKPOINT_CMD, strlen(BREAKPOINT_CMD)))
  {
    uint32_t address = strtoul((char*)&context.command[strlen(BREAKPOINT_CMD)], NULL, 16);
    context.breakpoint = address;
    context.stop = false;
    validCommand = true;
  }

  if(0u == memcmp(context.command, RUN_CMD, strlen(RUN_CMD)))
  {
    context.breakpoint = NO_BREAKPOINT;
    context.stop = false;
    validCommand = true;
  }

  if(0u == memcmp(context.command, QUIT_CMD, strlen(QUIT_CMD)))
  {
    context.breakpoint = NO_BREAKPOINT;
    context.stop = false;
    EMU_abort();
    validCommand = true;
  }

  if(0u == memcmp(context.command, STEP_CMD, strlen(STEP_CMD)))
  {
    context.breakpoint = NO_BREAKPOINT;
    context.stop = false;
    context.step = true;
    validCommand = true;
  }

  if(!validCommand)
  {
    DEBUG_LOG_PRINTF("Command %s not valid", context.command);
  }

  (void) memset(context.command, 0u, sizeof(context.command));
  context.commandIndex = 0u;
}

void EMUDEBUGGER_init(bool debuggerActive)
{
  (void) memset(&context, 0u, sizeof(context));
  context.breakpoint = NO_BREAKPOINT;
  context.stop = true;
  context.active = debuggerActive;
  EMUCPU_getContext((struct EMUCPU_Context**)&context.cpuContext);
}

void EMUDEBUGGER_run()
{
  if(context.active)
  {
    DEBUG_LOG_PRINTF("Next op: 0x%x at address 0x%x", context.cpuContext->ram[context.cpuContext->pc], context.cpuContext->pc);
    if(context.cpuContext->pc == context.breakpoint)
    {
      DEBUG_LOG_PRINTF("Breakpoint reached at: 0x%X", context.cpuContext->pc);
      context.stop = true;
    }

    if(context.step)
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
