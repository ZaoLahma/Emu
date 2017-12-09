#include "test_emu_fwk.h"

static EMUTEST_Result caseResult = TEST_OK;
static EMUTEST_Result overallResult = TEST_OK;

EMUTEST_Result EMUTEST_getResult(void)
{
  EMUTEST_Result retVal = caseResult;
  if(TEST_OK != caseResult)
  {
    overallResult = caseResult;
  }
  caseResult = TEST_OK;
  return retVal;
}

EMUTEST_Result EMUTEST_getOverallResult(void)
{
  return overallResult;
}

void EMUTEST_fail(void)
{
  caseResult = TEST_NOK;
}
