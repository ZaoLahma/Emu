#include "emu_cpu.h"
#include "emu_debug.h"
#include "../test/test_emu_cpu.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#define NUM_INSTRUCTIONS (0xFF)
#define STACK_SIZE       (0xFFFFu)

#define UNUSED_ARG(arg) \
{\
  arg = arg; \
} \

typedef void (*EMUCPU_InstructionHandler)(EMUCPU_Context* cpu, uint8_t* rom);

typedef struct
{
  EMUCPU_InstructionHandler handle;
} EMUCPU_Instruction;

static void illegalInstruction(EMUCPU_Context* cpu, uint8_t* rom);
static void handleNop(EMUCPU_Context* cpu, uint8_t* rom);
static void handleLdSp(EMUCPU_Context* cpu, uint8_t* rom);

static uint8_t stack[STACK_SIZE];
static EMUCPU_Context cpu;
static EMUCPU_Instruction instructionTable[NUM_INSTRUCTIONS];

static void illegalInstruction(EMUCPU_Context* cpu, uint8_t* rom)
{
  uint8_t op = rom[cpu->pc];
  (void) printf("illegal instruction 0x%x at 0x%x\n", op, cpu->pc);
  UNUSED_ARG(cpu);
  UNUSED_ARG(rom);
  cpu->stateOk = false;
}

static void handleNop(EMUCPU_Context* cpu, uint8_t* rom)
{
  UNUSED_ARG(rom);
  cpu->pc += 1u;
}

static void handleLdSp(EMUCPU_Context* cpu, uint8_t* rom)
{
  cpu->sp = rom[cpu->pc + 2u] << 8u | rom[cpu->pc + 1u];
  cpu->pc += 3u;
}

void EMUCPU_init()
{
  (void) memset(&cpu, 0, sizeof(cpu));
  cpu.sp = STACK_SIZE;
  cpu.pc = 0u;
  cpu.stateOk = true;

  for(uint32_t i = 0; i < NUM_INSTRUCTIONS; ++i)
  {
    instructionTable[i].handle = illegalInstruction;
  }

  instructionTable[0x0].handle = handleNop;
  instructionTable[0x31].handle = handleLdSp;
}

void EMUCPU_run(uint8_t* prog)
{
  EMU_DEBUG_ASSERT_COND(prog);
  uint8_t op = prog[cpu.pc];
  (void) printf("op: %u\n", op);
  instructionTable[op].handle(&cpu, prog);
  EMU_DEBUG_ASSERT_COND(cpu.stateOk);
}

void EMUCPU_getContext(struct EMUCPU_Context** context)
{
  *context = (struct EMUCPU_Context*)&cpu;
}
