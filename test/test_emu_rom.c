#include "test_emu_rom.h"
#include "emu_rom.h"
#include "test_emu_fwk.h"
#include <string.h>

static char romBuf[50000];
#define TEST_ROM_CONTENTS "hejhejhejhejhejhejhejhej\n";

static char testRomContents[] = TEST_ROM_CONTENTS;

void EMUROM_test(void)
{
  uint32_t romSize;

  TEST_ASSERT_INT_EQ(EMUROM_OK, EMUROM_read("TestRom.nes", romBuf, &romSize));
  TEST_ASSERT_INT_EQ(25u, romSize);
  TEST_ASSERT_INT_EQ(0u, memcmp(romBuf, testRomContents, romSize));
}
