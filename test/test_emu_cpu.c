#include "test_emu_cpu.h"
#include "emu_cpu.h"
#include "test_emu_fwk.h"
#include <string.h>

static EMUCPU_Context* cpuContext;

static void EMUCPU_testInit(void);

static void EMUCPU_testInit(void)
{
  EMUCPU_init();
  TEST_ASSERT_INT_EQ(cpuContext->sp, 0xFFFFu);
}

void EMUCPU_test(void)
{
  EMUCPU_getContext((struct EMUCPU_Context**)&cpuContext);
  EMUCPU_testInit();
}
