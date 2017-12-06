#include "test_emu_rom.h"
#include "emu_rom.h"
#include "test_emu_fwk.h"
#include <string.h>

#define TEST_ROM_CONTENTS "hejhejhejhejhejhejhejhej\n";

static void EMUROM_testFileReadOk(void);
static void EMUROM_testFileReadNok(void);

static void EMUROM_testFileReadOk(void)
{
  char testRomContents[] = TEST_ROM_CONTENTS;

  char romBuf[30];
  uint32_t romSize;

  TEST_ASSERT_INT_EQ(EMUROM_OK, EMUROM_read("TestRom.nes", romBuf, &romSize));
  TEST_ASSERT_INT_EQ(25u, romSize);
  TEST_ASSERT_INT_EQ(0u, memcmp(romBuf, testRomContents, romSize));
}

static void EMUROM_testFileReadNok(void)
{
  char romBuf[1];
  uint32_t romSize;

  TEST_ASSERT_INT_EQ(EMUROM_NOK, EMUROM_read("ThisShouldNotWork.nes", romBuf, &romSize));
}

void EMUROM_test(void)
{
  EMUROM_testFileReadOk();
  EMUROM_testFileReadNok();
}
