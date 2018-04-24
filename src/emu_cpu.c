#include "emu_cpu.h"
#include "emu_debug.h"
#include "../test/test_emu_cpu.h"
#include <string.h>
#include <stdio.h>

#define NUM_INSTRUCTIONS      (0xFF)
#define STACK_SIZE            (0xFFFFu)
#define UINT16_HIGH_BYTE_MASK (0xFF00u)
#define UINT16_LOW_BYTE_MASK  (0x00FFu)
#define BITS_IN_BYTE          (8u)

#define UNUSED_ARG(arg) \
{\
  arg = arg; \
} \

typedef void (*EMUCPU_InstructionHandler)(EMUCPU_Context* cpu, uint8_t* rom);

typedef struct
{
  EMUCPU_InstructionHandler handle;
} EMUCPU_Instruction;

static uint16_t read16BitWord(uint8_t* buf);

static void handleXor(EMUCPU_Context* cpu, uint8_t* rom);

static void illegalInstruction(EMUCPU_Context* cpu, uint8_t* rom);
static void handleNop(EMUCPU_Context* cpu, uint8_t* rom);
static void handleLdSp(EMUCPU_Context* cpu, uint8_t* rom);
static void handleXorA(EMUCPU_Context* cpu, uint8_t* rom);
static void handleLdHL(EMUCPU_Context* cpu, uint8_t* rom);

static uint8_t stack[STACK_SIZE];
static EMUCPU_Context cpu;
static EMUCPU_Instruction instructionTable[NUM_INSTRUCTIONS];

static uint16_t read16BitWord(uint8_t* buf)
{
  uint16_t retVal = buf[1u] << 8u | buf[0u];
  DEBUG_LOG("retVal: 0x%X", retVal);
  return (retVal);
}

static void illegalInstruction(EMUCPU_Context* cpu, uint8_t* rom)
{
  uint8_t op = rom[cpu->pc];
  (void) printf("illegal instruction 0x%X at 0x%X\n", op, cpu->pc);
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
  cpu->sp = read16BitWord(&rom[cpu->pc + 1u]);
  cpu->pc += 3u;
}

static void handleXor(EMUCPU_Context* cpu, uint8_t* rom)
{
  uint8_t op = rom[cpu->pc];
  uint8_t data = 0u;
  switch(op)
  {
    case 0xAF:
      data = cpu->a;
      DEBUG_LOG("cpu->a: %u", cpu->a);
      cpu->flags[EMUCPU_SUBTRACT_FLAG]   = 0u;
      cpu->flags[EMUCPU_CARRY_FLAG]      = 0u;
      cpu->flags[EMUCPU_HALF_CARRY_FLAG] = 0u;
      break;
    default:
      DEBUG_LOG("Xor with 0x%X not implemented", op);
      break;
  }
  cpu->a ^= data;
  DEBUG_LOG("Data: %u, cpu->a: %u", data, cpu->a);
  cpu->pc += 1u;
}

static void handleXorA(EMUCPU_Context* cpu, uint8_t* rom)
{
  handleXor(cpu, rom);
  cpu->flags[EMUCPU_ZERO_FLAG] = (cpu->a == 0u);
}

static void handleLdHL(EMUCPU_Context* cpu, uint8_t* rom)
{
  uint16_t val = read16BitWord(&rom[cpu->pc + 1u]);
  cpu->h = (uint8_t)((val & UINT16_HIGH_BYTE_MASK) >> BITS_IN_BYTE);
  cpu->l = (uint8_t)(val & UINT16_LOW_BYTE_MASK);
}

void EMUCPU_init()
{
  (void) memset(&cpu, 0, sizeof(cpu));
  cpu.sp = STACK_SIZE - 1u;
  cpu.pc = 0u;
  cpu.stateOk = true;

  for(uint32_t i = 0; i < NUM_INSTRUCTIONS; ++i)
  {
    instructionTable[i].handle = illegalInstruction;
  }

  instructionTable[0x0].handle = handleNop;
  instructionTable[0x21].handle = handleLdHL;
  instructionTable[0x31].handle = handleLdSp;
  instructionTable[0xAF].handle = handleXorA;
}

void EMUCPU_run(uint8_t* prog)
{
  EMU_DEBUG_ASSERT_COND(prog);
  uint8_t op = prog[cpu.pc];
  DEBUG_LOG("CPU handling op: 0x%X", op);
  instructionTable[op].handle(&cpu, prog);
  EMU_DEBUG_ASSERT_COND(cpu.stateOk);
}

void EMUCPU_getContext(struct EMUCPU_Context** context)
{
  *context = (struct EMUCPU_Context*)&cpu;
}
