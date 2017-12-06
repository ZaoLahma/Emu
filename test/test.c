#include "emu_cpu.h"
#include "test_emu_fwk.h"
#include "test_emu_rom.h"
#include "test_emu_cpu.h"
#include <stdio.h>

int main(void)
{

  EMUROM_test();
  EMUCPU_test();

  if(TEST_OK == EMUTEST_getResult())
  {
    (void) printf("TEST SUCCEEDED\n");
  }
  else
  {
    (void) printf("TEST FAILED\n");
  }

  return 0;
}
