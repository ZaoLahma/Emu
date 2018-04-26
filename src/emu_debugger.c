#include "emu_debugger.h"
#include "emu_debug.h"
#include "emu_cpu.h"
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
#define DUMP_REGS_CMD       "d"

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
  (void) printf("------- EMU DEBUGGER -------" NEW_LINE);
  (void) printf("Set breakpoint: bp <address> (ex: bp 0x20)" NEW_LINE);
  (void) printf("Dump CPU state: d" NEW_LINE);
  (void) printf("Run           : r" NEW_LINE);
  (void) printf("Step          : s" NEW_LINE);
  (void) printf("Quit          : q" NEW_LINE);
  (void) printf("$ ");
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

  if(0u == memcmp(context.command, DUMP_REGS_CMD, strlen(DUMP_REGS_CMD)))
  {
    context.stop = true;
    context.step = false;
    validCommand = true;

    (void) printf("------- CPU DUMP -------" NEW_LINE);
    (void) printf("a:               0x%X" NEW_LINE, context.cpuContext->a);
    (void) printf("zero flag:       0x%X" NEW_LINE, context.cpuContext->flags[EMUCPU_ZERO_FLAG]);
    (void) printf("carry flag:      0x%X" NEW_LINE, context.cpuContext->flags[EMUCPU_CARRY_FLAG]);
    (void) printf("half carry flag: 0x%X" NEW_LINE, context.cpuContext->flags[EMUCPU_HALF_CARRY_FLAG]);
    (void) printf("subtract flag:   0x%X" NEW_LINE, context.cpuContext->flags[EMUCPU_SUBTRACT_FLAG]);
    (void) printf("bc:              0x%X" NEW_LINE, context.cpuContext->bc.regValue);
    (void) printf("de:              0x%X" NEW_LINE, context.cpuContext->de.regValue);
    (void) printf("hl:              0x%X" NEW_LINE, context.cpuContext->hl.regValue);
    (void) printf("sp:              0x%X" NEW_LINE, context.cpuContext->sp);
    (void) printf("pc:              0x%X" NEW_LINE, context.cpuContext->pc);
  }

  if(!validCommand)
  {
    (void) printf("Command %s not valid" NEW_LINE, context.command);
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
    uint8_t op = context.cpuContext->ram[context.cpuContext->pc];
    (void) printf("Next op: 0x%X (%s) at address 0x%X" NEW_LINE, op, EMUCPU_getOpName(op), context.cpuContext->pc);
    if(context.cpuContext->pc == context.breakpoint)
    {
      (void) printf("Breakpoint reached at: 0x%X" NEW_LINE, context.cpuContext->pc);
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
    (void) printf("------- EXECUTE -------" NEW_LINE);
  }
}
