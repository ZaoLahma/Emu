#include "test_emu_rom.h"
#include "emu_rom.h"
#include "test_emu_fwk.h"

static char romBuf[50000];

void EMUROM_test(void)
{
  uint32_t romSize;

  TEST_ASSERT_INT_EQ(EMUROM_OK, EMUROM_read("TestRom.nes", romBuf, &romSize));
  TEST_ASSERT_INT_EQ(25u, romSize);
}
