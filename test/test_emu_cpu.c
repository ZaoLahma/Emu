#include "test_emu_cpu.h"
#include "emu_cpu.h"
#include "test_emu_fwk.h"
#include <string.h>

static EMUCPU_Context* cpuContext;

static void EMUCPU_testInit(void);
static void EMUCPU_testIllegalInstruction(void);
static void EMUCPU_testLdSp(void);
static void EMUCPU_testNop(void);
static void EMUCPU_testXorA(void);
static void EMUCPU_testLdHL(void);
static void EMUCPU_testCbBit7H(void);
static void EMUCPU_testJRNZ(void);

static void EMUCPU_testInit(void)
{
  uint8_t nop[] = {0x0};

  EMUCPU_init(nop, sizeof(nop));

  TEST_ASSERT_INT_EQ(cpuContext->sp, 0xFFFEu);
  TEST_ASSERT_INT_EQ(cpuContext->pc, 0x0);
  TEST_ASSERT_INT_EQ(cpuContext->stateOk, true);
}

static void EMUCPU_testIllegalInstruction(void)
{
  uint8_t illegalInstruction[] = {0xFE};

  EMUCPU_init(illegalInstruction, sizeof(illegalInstruction));

  EMUCPU_run();

  TEST_ASSERT_UINT_EQ(cpuContext->stateOk, false);
}

static void EMUCPU_testLdSp(void)
{
  uint8_t ldsp[] = {0x31, 0xFE, 0xFF};

  EMUCPU_init(ldsp, sizeof(ldsp));

  EMUCPU_run();

  TEST_ASSERT_UINT_EQ(cpuContext->pc, 3u);
  TEST_ASSERT_UINT_EQ(cpuContext->sp, 0XFFFE);
}

static void EMUCPU_testNop(void)
{
  uint8_t nop[] = {0x0};

  EMUCPU_init(nop, sizeof(nop));

  EMUCPU_run();

  TEST_ASSERT_UINT_EQ(cpuContext->pc, 1u);
}

static void EMUCPU_testXorA(void)
{
  uint8_t xorA[] = {0xAF};

  EMUCPU_init(xorA, sizeof(xorA));

  uint8_t expectedRegVal = 0u;
  uint8_t expectedFlagVal = 1u;

  cpuContext->a = 0u;

  EMUCPU_run();

  TEST_ASSERT_UINT_EQ(cpuContext->a, expectedRegVal);
  TEST_ASSERT_UINT_EQ(cpuContext->flags[EMUCPU_ZERO_FLAG], expectedFlagVal);

  cpuContext->pc = 0u;

  expectedRegVal = 0u;
  expectedFlagVal = 1u;

  cpuContext->a = 1u;

  EMUCPU_run(xorA);

  TEST_ASSERT_UINT_EQ(cpuContext->a, expectedRegVal);
  TEST_ASSERT_UINT_EQ(cpuContext->flags[EMUCPU_ZERO_FLAG], expectedFlagVal);
}

static void EMUCPU_testLdHL(void)
{
  uint8_t ldHL[] = {0x21, 0x30, 0x50};

  EMUCPU_init(ldHL, sizeof(ldHL));

  uint8_t expectedH = 0x50;
  uint8_t expectedL = 0x30;

  EMUCPU_run();

  TEST_ASSERT_UINT_EQ(cpuContext->h, expectedH);
  TEST_ASSERT_UINT_EQ(cpuContext->l, expectedL);
}

static void EMUCPU_testCbBit7H(void)
{
  uint8_t cbBit7H[] = {0xCB, 0x7C};

  EMUCPU_init(cbBit7H, sizeof(cbBit7H));

  cpuContext->h = 0x40u;

  EMUCPU_run();

  TEST_ASSERT_UINT_EQ(cpuContext->flags[EMUCPU_ZERO_FLAG], 1u);

  cpuContext->pc = 0u;
  cpuContext->h = 0x39u;

  EMUCPU_run();

  TEST_ASSERT_UINT_EQ(cpuContext->flags[EMUCPU_ZERO_FLAG], 0u);
}

static void EMUCPU_testJRNZ(void)
{
  uint8_t jRNZ[] = {0x20, 0x1, 0x0, 0x0};

  EMUCPU_init(jRNZ, sizeof(jRNZ));

  cpuContext->flags[EMUCPU_ZERO_FLAG] = 0u;

  EMUCPU_run();

  TEST_ASSERT_UINT_EQ(cpuContext->pc, 3u);

  cpuContext->pc = 0u;
  cpuContext->flags[EMUCPU_ZERO_FLAG] = 1u;

  EMUCPU_run();

  TEST_ASSERT_UINT_EQ(cpuContext->pc, 2u);  
}

void EMUCPU_test(void)
{
  EMUCPU_getContext((struct EMUCPU_Context**)&cpuContext);
  TEST_CASE(EMUCPU_testInit);
  TEST_CASE(EMUCPU_testIllegalInstruction);
  TEST_CASE(EMUCPU_testLdSp);
  TEST_CASE(EMUCPU_testNop);
  TEST_CASE(EMUCPU_testXorA);
  TEST_CASE(EMUCPU_testLdHL);
  TEST_CASE(EMUCPU_testCbBit7H);
  TEST_CASE(EMUCPU_testJRNZ);
}
