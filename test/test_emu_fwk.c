#include "test_emu_fwk.h"

static EMUTEST_Result result = TEST_OK;

EMUTEST_Result EMUTEST_getResult(void)
{
  return result;
}

void EMUTEST_fail(void)
{
  result = TEST_NOK;
}
