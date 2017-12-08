#include "test_emu_cpu.h"
#include "emu_cpu.h"
#include "test_emu_fwk.h"
#include <string.h>

static EMUCPU_Context* cpuContext;

static void EMUCPU_testInit(void);
static void EMUCPU_testLdSp(void);
static void EMUCPU_testNop(void);

static void EMUCPU_testInit(void)
{
  EMUCPU_init();
  TEST_ASSERT_INT_EQ(cpuContext->sp, 0xFFFFu);
  TEST_ASSERT_INT_EQ(cpuContext->pc, 0x0);
  TEST_ASSERT_INT_EQ(cpuContext->stateOk, true);
}

static void EMUCPU_testLdSp(void)
{
  EMUCPU_init();
  uint8_t ldsp[] = {0x31, 0xFE, 0xFF};

  EMUCPU_run(ldsp);

  TEST_ASSERT_UINT_EQ(cpuContext->pc, 3u);
  TEST_ASSERT_UINT_EQ(cpuContext->sp, 0XFFFE);
}

static void EMUCPU_testNop(void)
{
  EMUCPU_init();
  uint8_t nop[] = {0x0};

  EMUCPU_run(nop);

  TEST_ASSERT_UINT_EQ(cpuContext->pc, 1u);
}

void EMUCPU_test(void)
{
  EMUCPU_getContext((struct EMUCPU_Context**)&cpuContext);
  EMUCPU_testInit();
  EMUCPU_testLdSp();
  EMUCPU_testNop();
}
