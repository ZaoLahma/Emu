#include "emu_cpu.h"
#include "test_emu_fwk.h"
#include "test_emu_rom.h"
#include "test_emu_gameboy_cpu.h"
#include <stdio.h>

int main(void)
{
  TEST_SUITE(EMUROM_test);
  TEST_SUITE(EMUCPU_test);

  if(TEST_OK == EMUTEST_getOverallResult())
  {
    (void) printf("TEST EXECUTION SUCCEEDED\n");
  }
  else
  {
    (void) printf("TEST EXECUTION FAILED\n");
  }
  return 0;
}
