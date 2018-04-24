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

#define BIT_7                 (6u)

#define UNUSED_ARG(arg) \
{\
  arg = arg; \
} \

typedef void (*EMUCPU_InstructionHandler)(EMUCPU_Context* cpu);

typedef struct
{
  EMUCPU_InstructionHandler handle;
} EMUCPU_Instruction;

static uint16_t read16BitWord(uint8_t* buf);

static void handleXor(EMUCPU_Context* cpu);

static void illegalInstruction(EMUCPU_Context* cpu);
static void handleNop(EMUCPU_Context* cpu);
static void handleLdSp(EMUCPU_Context* cpu);
static void handleXorA(EMUCPU_Context* cpu);
static void handleLdHL(EMUCPU_Context* cpu);
static void handleLdDHLA(EMUCPU_Context* cpu);
static void handleJRNZ(EMUCPU_Context* cpu);
static void handleCb(EMUCPU_Context* cpu);

static void handleCbBit7H(EMUCPU_Context* cpu);

static uint8_t stack[STACK_SIZE];
static EMUCPU_Context cpu;
static EMUCPU_Instruction instructionTable[NUM_INSTRUCTIONS];
static EMUCPU_Instruction cbInstructionTable[NUM_INSTRUCTIONS];

static uint16_t read16BitWord(uint8_t* buf)
{
  uint16_t retVal = buf[1u] << 8u | buf[0u];
  DEBUG_LOG("retVal: 0x%X", retVal);
  return (retVal);
}

static void illegalInstruction(EMUCPU_Context* cpu)
{
  uint8_t op = cpu->ram[cpu->pc];
  (void) printf("illegal instruction 0x%X at 0x%X\n", op, cpu->pc);
  UNUSED_ARG(cpu);
  cpu->stateOk = false;
}

static void handleNop(EMUCPU_Context* cpu)
{
  cpu->pc += 1u;
}

static void handleLdSp(EMUCPU_Context* cpu)
{
  cpu->sp = read16BitWord(&cpu->ram[cpu->pc + 1u]);
  cpu->pc += 3u;
}

static void handleXor(EMUCPU_Context* cpu)
{
  uint8_t op = cpu->ram[cpu->pc];
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

static void handleXorA(EMUCPU_Context* cpu)
{
  handleXor(cpu);
  cpu->flags[EMUCPU_ZERO_FLAG] = (cpu->a == 0u);
}

static void handleLdHL(EMUCPU_Context* cpu)
{
  uint16_t val = read16BitWord(&cpu->ram[cpu->pc + 1u]);
  cpu->h = (uint8_t)((val & UINT16_HIGH_BYTE_MASK) >> BITS_IN_BYTE);
  cpu->l = (uint8_t)(val & UINT16_LOW_BYTE_MASK);
  cpu->pc += 3u;
}

static void handleLdDHLA(EMUCPU_Context* cpu)
{
  uint16_t ramAddress = ((cpu->h << BITS_IN_BYTE) | cpu->l);
  DEBUG_LOG("ramAddress: 0x%X", ramAddress);
  cpu->ram[ramAddress] = cpu->a;
  ramAddress -= 1u;
  cpu->h = (uint8_t)((ramAddress & UINT16_HIGH_BYTE_MASK) >> BITS_IN_BYTE);
  cpu->l = (uint8_t)(ramAddress & UINT16_LOW_BYTE_MASK);
  cpu->pc += 1u;
}

static void handleJRNZ(EMUCPU_Context* cpu)
{
  if(0u == cpu->flags[EMUCPU_ZERO_FLAG])
  {
    int8_t jumpOffset = cpu->ram[cpu->pc + 1u];
    cpu->pc += jumpOffset;
  }
  cpu->pc += 2u;
}

static void handleCb(EMUCPU_Context* cpu)
{
  cpu->pc += 1u;
  uint8_t cbOp = cpu->ram[cpu->pc];
  cbInstructionTable[cbOp].handle(cpu);
}

static void handleCbBit7H(EMUCPU_Context* cpu)
{
  uint8_t bit7 = ((cpu->h >> BIT_7) & 1u);
  cpu->flags[EMUCPU_ZERO_FLAG] = bit7;
  cpu->pc += 1u;
}

void EMUCPU_init(uint8_t* prog, uint16_t size)
{
  EMU_DEBUG_ASSERT_COND(prog);
  (void) memset(&cpu, 0, sizeof(cpu));
  (void) memcpy(&cpu.ram[0u], prog, size);

  cpu.sp = STACK_SIZE - 1u;
  cpu.pc = 0u;
  cpu.stateOk = true;

  for(uint32_t i = 0; i < NUM_INSTRUCTIONS; ++i)
  {
    instructionTable[i].handle = illegalInstruction;
    cbInstructionTable[i].handle = illegalInstruction;
  }

  instructionTable[0x0].handle = handleNop;
  instructionTable[0x21].handle = handleLdHL;
  instructionTable[0x20].handle = handleJRNZ;
  instructionTable[0x31].handle = handleLdSp;
  instructionTable[0x32].handle = handleLdDHLA;
  instructionTable[0xAF].handle = handleXorA;
  instructionTable[0xCB].handle = handleCb;

  cbInstructionTable[0x7C].handle = handleCbBit7H;
}

void EMUCPU_run()
{
  uint8_t op = cpu.ram[cpu.pc];
  DEBUG_LOG("CPU handling op: 0x%X at address: 0x%x", op, cpu.pc);
  instructionTable[op].handle(&cpu);
  EMU_DEBUG_ASSERT_COND(cpu.stateOk);
}

void EMUCPU_getContext(struct EMUCPU_Context** context)
{
  *context = (struct EMUCPU_Context*)&cpu;
}
