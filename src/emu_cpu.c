#include "emu_cpu.h"
#include "emu_debug.h"
#include "../test/test_emu_cpu.h"
#include <string.h>

static EMUCPU_Context cpuContext;

void EMUCPU_init()
{
  (void) memset(&cpuContext, 0, sizeof(cpuContext));
  cpuContext.sp = 255u;
}

void EMUCPU_run(char* prog)
{
  EMU_DEBUG_ASSERT_COND(prog);
}

void EMUCPU_getContext(struct EMUCPU_Context** context)
{
  *context = (struct EMUCPU_Context*)&cpuContext;
}
