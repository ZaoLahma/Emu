#include "emu_cpu.h"
#include "test_emu_fwk.h"
#include "test_emu_rom.h"
#include <stdio.h>

int main(void)
{
  EMUCPU_init();

  EMUCPU_run(0u);

  EMUROM_test();

  if(TEST_OK != EMUTEST_getResult())
  {
    (void) printf("TEST FAILED\n");
  }

  return 0;
}
