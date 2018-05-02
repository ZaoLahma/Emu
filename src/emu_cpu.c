#include "emu_cpu.h"
#include "emu_debug.h"
#include "../test/test_emu_cpu.h"
#include <string.h>
#include <stdio.h>

#include "emu_cpu_cfg.h"

static EMUCPU_Context cpu;

void EMUCPU_init(uint8_t* prog, uint16_t size)
{
  EMU_DEBUG_ASSERT_COND(prog);
  (void) memset(&cpu, 0, sizeof(cpu));
  (void) memcpy(&cpu.ram[0u], prog, size);

  cpu.sp = EMUCPU_STACK_SIZE - 1u;
  cpu.pc = 0u;
  cpu.stateOk = true;

  EMUCPUCFG_init();
}

void EMUCPU_run()
{
  uint8_t op = cpu.ram[cpu.pc];
  DEBUG_LOG_PRINTF("CPU handling op: 0x%X (%s) at address: 0x%x", op, instructionTable[op].name, cpu.pc);
  instructionTable[op].handle(&cpu);
  EMU_DEBUG_ASSERT_COND(cpu.stateOk);
}

void EMUCPU_getContext(struct EMUCPU_Context** context)
{
  *context = (struct EMUCPU_Context*)&cpu;
}

char* EMUCPU_getOpName(uint8_t op)
{
  return instructionTable[op].name;
}
