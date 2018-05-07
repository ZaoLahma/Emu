#include "emu_cpu_cfg.h"
#include "emu_debug.h"

#define UINT16_HIGH_BYTE_MASK (0xFF00u)
#define UINT16_LOW_BYTE_MASK  (0x00FFu)
#define BITS_IN_BYTE          (8u)

#define BIT_7                 (6u)

#define UNUSED_ARG(arg) \
{\
  arg = arg; \
} \

static uint16_t read16BitWord(uint8_t* buf);

static void handleXor(EMUCPU_Context* cpu);
static void handleLd(uint8_t* reg, EMUCPU_Context* cpu);

static void illegalInstruction(EMUCPU_Context* cpu);
static void handleNop(EMUCPU_Context* cpu);
static void handleLdSp(EMUCPU_Context* cpu);
static void handleXorA(EMUCPU_Context* cpu);
static void handleLdHL(EMUCPU_Context* cpu);
static void handleLdDHLA(EMUCPU_Context* cpu);
static void handleLdHLA(EMUCPU_Context* cpu);
static void handleJRNZ(EMUCPU_Context* cpu);
static void handleCb(EMUCPU_Context* cpu);
static void handleLdC(EMUCPU_Context* cpu);
static void handleLdA(EMUCPU_Context* cpu);
static void handleLdCA(EMUCPU_Context* cpu);

static void handleCbBit7H(EMUCPU_Context* cpu);

EMUCPU_Instruction instructionTable[EMUCPU_NUM_INSTRUCTIONS];
static EMUCPU_Instruction cbInstructionTable[EMUCPU_NUM_INSTRUCTIONS];

static uint16_t read16BitWord(uint8_t* buf)
{
  uint16_t retVal = buf[1u] << 8u | buf[0u];
  DEBUG_LOG_PRINTF("retVal: 0x%X", retVal);
  return (retVal);
}

static void handleXor(EMUCPU_Context* cpu)
{
  uint8_t op = cpu->ram[cpu->pc];
  uint8_t data = 0u;
  switch(op)
  {
    case 0xAF:
      data = cpu->a;
      DEBUG_LOG_PRINTF("cpu->a: %u", cpu->a);
      cpu->flags[EMUCPU_SUBTRACT_FLAG]   = 0u;
      cpu->flags[EMUCPU_CARRY_FLAG]      = 0u;
      cpu->flags[EMUCPU_HALF_CARRY_FLAG] = 0u;
      break;
    default:
      DEBUG_LOG_PRINTF("Xor with 0x%X not implemented", op);
      break;
  }
  cpu->a ^= data;
  DEBUG_LOG_PRINTF("Data: %u, cpu->a: %u", data, cpu->a);
  cpu->pc += 1u;
}

static void handleLd(uint8_t* reg, EMUCPU_Context* cpu)
{
  *reg = cpu->ram[cpu->pc + 1u];
  cpu->pc += 2u;
}

static void illegalInstruction(EMUCPU_Context* cpu)
{
  uint8_t op = cpu->ram[cpu->pc];
  UNUSED_ARG(op);
  DEBUG_LOG_PRINTF("illegal instruction 0x%X at 0x%X\n", op, cpu->pc);
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

static void handleXorA(EMUCPU_Context* cpu)
{
  handleXor(cpu);
  cpu->flags[EMUCPU_ZERO_FLAG] = (cpu->a == 0u);
}

static void handleLdHL(EMUCPU_Context* cpu)
{
  uint16_t val = read16BitWord(&cpu->ram[cpu->pc + 1u]);
  cpu->hl.regValue = val;
  cpu->pc += 3u;
}

static void handleLdDHLA(EMUCPU_Context* cpu)
{
  uint16_t ramAddress = cpu->hl.regValue;
  DEBUG_LOG_PRINTF("ramAddress: 0x%X", ramAddress);
  cpu->ram[ramAddress] = cpu->a;
  ramAddress -= 1u;
  cpu->hl.regValue = ramAddress;
  cpu->pc += 1u;
}

static void handleLdHLA(EMUCPU_Context* cpu)
{
  uint16_t ramAddress = cpu->hl.regValue;
  DEBUG_LOG_PRINTF("LdHLA ramAddress: 0x%X", ramAddress);
  cpu->ram[ramAddress] = cpu->a;
  cpu->cycles += 8u;
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

static void handleLdC(EMUCPU_Context* cpu)
{
  handleLd(&cpu->bc.low, cpu);
}

static void handleLdA(EMUCPU_Context* cpu)
{
  handleLd(&cpu->a, cpu);
}

static void handleLdCA(EMUCPU_Context* cpu)
{
  uint16_t ramAddress = cpu->bc.high;
  DEBUG_LOG_PRINTF("LdCA: ramAddress: 0x%X", ramAddress);
  cpu->ram[ramAddress] = cpu->a;
  cpu->pc += 2u;
}

static void handleCbBit7H(EMUCPU_Context* cpu)
{
  uint8_t bit7 = ((cpu->hl.high >> BIT_7) & 1u);
  cpu->flags[EMUCPU_ZERO_FLAG] = bit7;
  cpu->pc += 1u;
}

void EMUCPUCFG_init(void)
{

    for(uint32_t i = 0; i < EMUCPU_NUM_INSTRUCTIONS; ++i)
    {
      instructionTable[i] = (EMUCPU_Instruction){.handle = illegalInstruction, .name = "IllegalInstruction"};
      cbInstructionTable[i] = (EMUCPU_Instruction){.handle = illegalInstruction, .name = "IllegalInstruction"};
    }

    instructionTable[0x00] = (EMUCPU_Instruction){.handle = handleNop,          .name = "NOP"};
    instructionTable[0x01] = (EMUCPU_Instruction){.handle = illegalInstruction, .name = "LdBC"};
    instructionTable[0x02] = (EMUCPU_Instruction){.handle = illegalInstruction, .name = "LdBCA"};
    instructionTable[0x03] = (EMUCPU_Instruction){.handle = illegalInstruction, .name = "IncBC"};
    instructionTable[0x04] = (EMUCPU_Instruction){.handle = illegalInstruction, .name = "IncB"};
    instructionTable[0x05] = (EMUCPU_Instruction){.handle = illegalInstruction, .name = "DecB"};
    instructionTable[0x06] = (EMUCPU_Instruction){.handle = illegalInstruction, .name = "LdB"};
    instructionTable[0x07] = (EMUCPU_Instruction){.handle = illegalInstruction, .name = "RLCA"};
    instructionTable[0x08] = (EMUCPU_Instruction){.handle = illegalInstruction, .name = "LdNNSp"};
    instructionTable[0x09] = (EMUCPU_Instruction){.handle = illegalInstruction, .name = "AddHLBC"};
    instructionTable[0x0A] = (EMUCPU_Instruction){.handle = illegalInstruction, .name = "LdABC"};
    instructionTable[0x0B] = (EMUCPU_Instruction){.handle = illegalInstruction, .name = "DecBC"};
    instructionTable[0x0C] = (EMUCPU_Instruction){.handle = illegalInstruction, .name = "IncC"};
    instructionTable[0x0D] = (EMUCPU_Instruction){.handle = illegalInstruction, .name = "DecC"};
    instructionTable[0x0E] = (EMUCPU_Instruction){.handle = handleLdC,          .name = "RrCA"};
    instructionTable[0x0F] = (EMUCPU_Instruction){.handle = illegalInstruction, .name = "LdB"};
    instructionTable[0x20] = (EMUCPU_Instruction){.handle = handleJRNZ,         .name = "JRNZ"};
    instructionTable[0x21] = (EMUCPU_Instruction){.handle = handleLdHL,         .name = "LdHL"};
    instructionTable[0x31] = (EMUCPU_Instruction){.handle = handleLdSp,         .name = "LdSP"};
    instructionTable[0x32] = (EMUCPU_Instruction){.handle = handleLdDHLA,       .name = "LdDHLA"};
    instructionTable[0x77] = (EMUCPU_Instruction){.handle = handleLdHLA,        .name = "LdHLA"};
    instructionTable[0x3E] = (EMUCPU_Instruction){.handle = handleLdA,          .name = "LdA"};
    instructionTable[0xAF] = (EMUCPU_Instruction){.handle = handleXorA,         .name = "XorA"};
    instructionTable[0xCB] = (EMUCPU_Instruction){.handle = handleCb,           .name = "Cb"};
    instructionTable[0xE2] = (EMUCPU_Instruction){.handle = handleLdCA,         .name = "LdCA"};

    cbInstructionTable[0x7C] = (EMUCPU_Instruction){.handle = handleCbBit7H, .name = "CbBit7H"};
}
